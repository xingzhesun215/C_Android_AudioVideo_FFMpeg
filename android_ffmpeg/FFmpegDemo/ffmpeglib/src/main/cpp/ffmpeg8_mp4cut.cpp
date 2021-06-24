#include <jni.h>
#include <string>

extern "C" {
#include "include/libavcodec/avcodec.h"
#include "include/libavformat/avformat.h"
#include "include/libavfilter/avfilter.h"
#include "include/libavutil/timestamp.h"
#include "include/libavutil/avutil.h"
#include "include/libavutil/mem.h"
#include "include/libavutil/mathematics.h"

#include "libavformat/avformat.h"
#include "android_log.h"

#include <jni.h>
}


static void log_packet(const AVFormatContext *fmt_ctx, const AVPacket *pkt, const char *tag) {
    AVRational *time_base = &fmt_ctx->streams[pkt->stream_index]->time_base;

    LOGE("%s: pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           tag,
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);
}

int cut_video(double from_seconds, double end_seconds, const char *in_filename,
              const char *out_filename) {
    AVOutputFormat *ofmt = NULL;
    AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;
    AVPacket pkt;
    int ret, i;

    av_register_all();

    if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
        fprintf(stderr, "Could not open input file '%s'", in_filename);
        return -1;
    } else {
        LOGE("open input file '%s'", in_filename);
    }


    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
        fprintf(stderr, "Failed to retrieve input stream information");
        return -1;
    } else {
        LOGE("retrieve input stream information");
    }

    av_dump_format(ifmt_ctx, 0, in_filename, 0);
    //输出上下文
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);
    if (!ofmt_ctx) {
        fprintf(stderr, "Could not create output context\n");
        ret = AVERROR_UNKNOWN;
        return -1;
    }

    ofmt = ofmt_ctx->oformat;

    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        AVStream *in_stream = ifmt_ctx->streams[i];
        AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);
        if (!out_stream) {
            LOGE("Failed allocating output stream");
            ret = AVERROR_UNKNOWN;
            return -1;
        }
        //拷贝流信息
        ret = avcodec_copy_context(out_stream->codec, in_stream->codec);
        if (ret < 0) {
            fprintf(stderr, "Failed to copy context from input to output stream codec context\n");
            LOGE("Failed to copy context from input to output stream codec context");
            return -1;
        }
        out_stream->codec->codec_tag = 0;
        if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
            out_stream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }
    av_dump_format(ofmt_ctx, 0, out_filename, 1);

    if (!(ofmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open output file '%s'", out_filename);
            LOGE("Could not open output file '%s'", out_filename);
            return -1;
        }
    }
    //写多媒体信息头
    ret = avformat_write_header(ofmt_ctx, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file\n");
        LOGE("Error occurred when opening output file");
        return -1;
    }

    //定位跳去到指定流位置 秒数*时间基=开始剪切的位置
    ret = av_seek_frame(ifmt_ctx, -1, from_seconds * AV_TIME_BASE, AVSEEK_FLAG_ANY);
    if (ret < 0) {
        fprintf(stderr, "Error seek\n");
        LOGE("Error see");
        return -1;
    }

    int64_t *dts_start_from = (int64_t *) malloc(sizeof(int64_t) * ifmt_ctx->nb_streams);
    memset(dts_start_from, 0, sizeof(int64_t) * ifmt_ctx->nb_streams);
    int64_t *pts_start_from = (int64_t *) malloc(sizeof(int64_t) * ifmt_ctx->nb_streams);
    memset(pts_start_from, 0, sizeof(int64_t) * ifmt_ctx->nb_streams);

    while (1) {
        AVStream *in_stream, *out_stream;

        ret = av_read_frame(ifmt_ctx, &pkt);
        if (ret < 0)
            break;

        in_stream = ifmt_ctx->streams[pkt.stream_index];
        out_stream = ofmt_ctx->streams[pkt.stream_index];

        log_packet(ifmt_ctx, &pkt, "in");
        //和结束位置比较
        if (av_q2d(in_stream->time_base) * pkt.pts > end_seconds) {
            av_free_packet(&pkt);
            break;
        }

        if (dts_start_from[pkt.stream_index] == 0) {
            dts_start_from[pkt.stream_index] = pkt.dts;
            LOGE("dts_start_from: %s\n", av_ts2str(dts_start_from[pkt.stream_index]));
        }
        if (pts_start_from[pkt.stream_index] == 0) {
            pts_start_from[pkt.stream_index] = pkt.pts;
            LOGE("pts_start_from: %s\n", av_ts2str(pts_start_from[pkt.stream_index]));
        }

        /* copy packet *///时间基转换  pts 就是每一帧数据的播放时间
        pkt.pts = av_rescale_q_rnd(pkt.pts - pts_start_from[pkt.stream_index], in_stream->time_base,
                                   out_stream->time_base,
                                   static_cast<AVRounding>(AV_ROUND_NEAR_INF |
                                                           AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts - dts_start_from[pkt.stream_index], in_stream->time_base,
                                   out_stream->time_base,
                                   static_cast<AVRounding>(AV_ROUND_NEAR_INF |
                                                           AV_ROUND_PASS_MINMAX));

        LOGE("pts =%d dts=%d",pkt.pts,pkt.dts);
        if (pkt.pts < 0) {
            pkt.pts = 0;
        }
        if (pkt.dts < 0) {
            pkt.dts = 0;
        }
        pkt.duration = (int) av_rescale_q((int64_t) pkt.duration, in_stream->time_base,
                                          out_stream->time_base);
        pkt.pos = -1;
        log_packet(ofmt_ctx, &pkt, "out");
        printf("\n");

        ret = av_interleaved_write_frame(ofmt_ctx, &pkt);
        //当ret=-22时需要抛弃此次pkt
        //正常情况下，每一帧或者每一包过来的数据，dts和pts是累增的，也就是说，当前的帧或者pkt的dts和pts要比上一帧或者pkt的dts和pts要大的，
        // 当获取到的当前pkt的dts小于上一pkt的pts时，回报av_write_frame() error -22的错误。
        if (ret < 0) {
            fprintf(stderr, "Error muxing packet\n");
            LOGE("muxing packet %d", ret);
            continue;//之前这里是return,如果因为-22直接略过此pkt即可
        }
        av_free_packet(&pkt);
    }
    free(dts_start_from);
    free(pts_start_from);

    av_write_trailer(ofmt_ctx);
    end:

    avformat_close_input(&ifmt_ctx);

    /* close output */
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
        avio_closep(&ofmt_ctx->pb);
    avformat_free_context(ofmt_ctx);

    if (ret < 0 && ret != AVERROR_EOF) {
        fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
        LOGE("Error occurred: %s\n", av_err2str(ret));
        return 1;
    }
    return 0;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_sun_ffmpeglib_FFmpeg8Mp4Cut_mp4Cut(JNIEnv *env, jclass clazz, jstring mp4_path,
                                            jstring cut_path, jint start_time, jint end_time) {
    const char *mp4Path = env->GetStringUTFChars(mp4_path, nullptr);
    const char *cutPath = env->GetStringUTFChars(cut_path, nullptr);
    LOGE("源文件 %s 目标文件%s 开始时间=%d 结束时间=%d", mp4Path, cutPath, start_time, end_time);
    cut_video(start_time, end_time, mp4Path, cutPath);
}

