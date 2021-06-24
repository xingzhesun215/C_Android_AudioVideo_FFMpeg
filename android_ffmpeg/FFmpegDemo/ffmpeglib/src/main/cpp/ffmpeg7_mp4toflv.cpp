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

    printf("%s: pts:%s pts_time:%s dts:%s dts_time:%s duration:%s duration_time:%s stream_index:%d\n",
           tag,
           av_ts2str(pkt->pts), av_ts2timestr(pkt->pts, time_base),
           av_ts2str(pkt->dts), av_ts2timestr(pkt->dts, time_base),
           av_ts2str(pkt->duration), av_ts2timestr(pkt->duration, time_base),
           pkt->stream_index);
}

int mp4ToFlv(const char *in_filename, const char *out_filename) {
    AVOutputFormat *ofmt = NULL;
    AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;
    AVPacket pkt;
    int ret, i;
    int stream_index = 0;
    int *stream_mapping = NULL;
    int stream_mapping_size = 0;


    av_register_all();

    if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
        fprintf(stderr, "Could not open input file '%s'", in_filename);
        LOGE("Could not open input file %s", av_err2str(ret));
        goto end;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
        fprintf(stderr, "Failed to retrieve input stream information");
        LOGE("Failed to retrieve input stream information %s", av_err2str(ret));
        goto end;
    }

    av_dump_format(ifmt_ctx, 0, in_filename, 0);

    //根据输入文件名字创建输出上下文
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);
    if (!ofmt_ctx) {
        fprintf(stderr, "Could not create output context\n");
        LOGE("Could not create output context");
        ret = AVERROR_UNKNOWN;
        goto end;
    }

    stream_mapping_size = ifmt_ctx->nb_streams;//流的数量
    stream_mapping = (int *) av_mallocz_array(stream_mapping_size, sizeof(*stream_mapping));

    if (!stream_mapping) {
        ret = AVERROR(ENOMEM);
        LOGE("stream_mapping null %d %s", stream_mapping_size, av_err2str(ret));
        goto end;
    }

    LOGE("到这里了");
    ofmt = ofmt_ctx->oformat;

    for (i = 0; i < ifmt_ctx->nb_streams; i++) {
        AVStream *out_stream;
        AVStream *in_stream = ifmt_ctx->streams[i];
        AVCodecParameters *in_codecpar = in_stream->codecpar;

        if (in_codecpar->codec_type != AVMEDIA_TYPE_AUDIO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_VIDEO &&
            in_codecpar->codec_type != AVMEDIA_TYPE_SUBTITLE) {
            stream_mapping[i] = -1;
            continue;
        }
        //统计流的数量stream_index
        stream_mapping[i] = stream_index++;
        //创建一个流
        out_stream = avformat_new_stream(ofmt_ctx, NULL);
        if (!out_stream) {
            fprintf(stderr, "Failed allocating output stream\n");
            LOGE("Failed allocating output stream\n");
            ret = AVERROR_UNKNOWN;
            goto end;
        }
        //从输入流拷贝参数到输出流
        ret = avcodec_parameters_copy(out_stream->codecpar, in_codecpar);
        if (ret < 0) {
            fprintf(stderr, "Failed to copy codec parameters\n");
            LOGE("Failed to copy codec parameters\n");
            goto end;
        }
        out_stream->codecpar->codec_tag = 0;
    }

    av_dump_format(ofmt_ctx, 0, out_filename, 1);

    if (!(ofmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open output file '%s'", out_filename);
            LOGE("Could not open output file '%s'", out_filename);
            goto end;
        } else {
            LOGE(" open output file '%s'", out_filename);

        }
    }
    //因为出现 -22 所以加入下面的代码 有了声音,但还是没有视频
    ofmt_ctx->streams[0]->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;
    ofmt_ctx->streams[0]->codecpar->codec_id = AV_CODEC_ID_H264;
    ofmt_ctx->bit_rate = 2000000;
    ofmt_ctx->streams[0]->codecpar->width = 968;
    ofmt_ctx->streams[0]->codecpar->height = 544;
    ofmt_ctx->streams[0]->time_base.num = 1;
    ofmt_ctx->streams[0]->time_base.den = 30;
    //----------------------------------------------------

    //写头数据
    ret = avformat_write_header(ofmt_ctx, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file\n");
        LOGE("Error occurred when opening output file %s   %d", av_err2str(ret), ret);
        goto end;
    }

    while (1) {
        AVStream *in_stream, *out_stream;

        ret = av_read_frame(ifmt_ctx, &pkt);
        if (ret < 0)
            break;

        in_stream = ifmt_ctx->streams[pkt.stream_index];
        if (pkt.stream_index >= stream_mapping_size ||
            stream_mapping[pkt.stream_index] < 0) {
            av_packet_unref(&pkt);
            continue;
        }
        //拷贝流通道信息
        pkt.stream_index = stream_mapping[pkt.stream_index];
        //拷贝流数据
        out_stream = ofmt_ctx->streams[pkt.stream_index];

        log_packet(ifmt_ctx, &pkt, "in");

        /* copy packet */ //转换时间基 音频是44100 默认是1000； pts用于显示 dts用于解码 duration时长
        pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base,
                                   static_cast<AVRounding>(AV_ROUND_NEAR_INF |
                                                           AV_ROUND_PASS_MINMAX));
        pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base,
                                   static_cast<AVRounding>(AV_ROUND_NEAR_INF |
                                                           AV_ROUND_PASS_MINMAX));
        pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
        pkt.pos = -1;

        log_packet(ofmt_ctx, &pkt, "out");
        //将数据直接写入到输出上下文
        ret = av_interleaved_write_frame(ofmt_ctx, &pkt);
        if (ret < 0) {
            fprintf(stderr, "Error muxing packet\n");
            break;
        }
        av_packet_unref(&pkt);
    }

    av_write_trailer(ofmt_ctx);
    LOGE("走到这里基本是成功了");

    end:
    LOGE("end Error go to end");
    avformat_close_input(&ifmt_ctx);

    /* close output */
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
        avio_closep(&ofmt_ctx->pb);
    avformat_free_context(ofmt_ctx);

    av_freep(&stream_mapping);

    if (ret < 0 && ret != AVERROR_EOF) {
        fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
        LOGE("end Error occurred: %s\n", av_err2str(ret));
        return 1;
    }

    return 0;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_sun_ffmpeglib_FFmpeg7Mp4toFlv_mp4toFlv(JNIEnv *env, jclass clazz, jstring mp4_path,
                                                jstring flv_path) {


    const char *mp4Path = env->GetStringUTFChars(mp4_path, nullptr);
    const char *flvPath = env->GetStringUTFChars(flv_path, nullptr);
    LOGE("mp4文件地址%s", mp4Path);
    LOGE("flv文件地址%s", flvPath);
    mp4ToFlv(mp4Path, flvPath);

}

