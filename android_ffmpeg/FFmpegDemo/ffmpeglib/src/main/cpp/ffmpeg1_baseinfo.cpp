#include <jni.h>
#include <string>

extern "C" {
#include "include/libavcodec/avcodec.h"
#include "include/libavformat/avformat.h"
#include "include/libavfilter/avfilter.h"
#include "include/libavutil/avutil.h"
#include "android_log.h"
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_sun_ffmpeglib_FFmpeg1BaseInfo_stringFromJNI(JNIEnv *env, jclass clazz) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

/**
 * avcodec结构体重要属性说明
 * const char *name：编解码器的名字，比较短
 * const char *long_name：编解码器的名字，全称，比较长
 * enum AVMediaType type：指明了类型，是视频，音频，还是字幕
 * enum AVCodecID id：ID，不重复
 * const AVRational *supported_framerates：支持的帧率（仅视频）
 * const enum AVPixelFormat *pix_fmts：支持的像素格式（仅视频）
 * const int *supported_samplerates：支持的采样率（仅音频）
 * const enum AVSampleFormat *sample_fmts：支持的采样格式（仅音频）
 * const uint64_t *channel_layouts：支持的声道数（仅音频）
 * int priv_data_size：私有数据的大小
 */


extern "C"
JNIEXPORT jstring JNICALL
Java_com_sun_ffmpeglib_FFmpeg1BaseInfo_avcodecInfo(JNIEnv *env, jclass clazz) {
    char info[40000] = {0};
    //初始化所有组件(注册所有编解码器)
    av_register_all();
    //获得指向链表下一个解码器的指针，循环往复可以获得所有解码器的信息。
    // 注意，如果想要获得指向第一个解码器的指针，则需要将该函数的参数设置为NULL
    AVCodec *temp = av_codec_next(NULL);

    while (temp != NULL) {
        if (temp->decode != NULL) {
            sprintf(info, "%s decode:", info);
        } else {
            sprintf(info, "%s encode:", info);
        }
        switch (temp->type) {
            case AVMEDIA_TYPE_VIDEO:
                sprintf(info, "%s(video):", info);
                break;
            case AVMEDIA_TYPE_AUDIO:
                sprintf(info, "%s(audio):", info);
                break;
            default:
                sprintf(info, "%s(other):", info);
                break;
        }
        sprintf(info, "%s[%10s]\n", info, temp->long_name);
        temp = temp->next;
    }



    return env->NewStringUTF(info);
}

/**
 * const char * name；//过滤器名称。
 * const char * description；//过滤器说明。
 * const AVFilterPad * inputs；//输入列表，由零元素终止。
 * const AVFilterPad * outputs；//输出列表，由零元素终止。
 * const AVClass * priv_class；//私有数据类，用于声明过滤器私有AVOptions。
 * int flags；  //AVFILTER_FLAG_ *的组合。
 * int(* init )(AVFilterContext *ctx)；//过滤初始化函数。
 * int(* init_dict )(AVFilterContext *ctx, AVDictionary **options)；//应该通过想要将AVOptions的字典传递给在init中分配的嵌套上下文的过滤器来设置而不是init。
 * void(* uninit )(AVFilterContext *ctx)；//过滤器在其输入和输出上支持的查询格式。
 * int priv_size；//要为过滤器分配的私有数据的大小
 * int flags_internal；//avfilter的附加标志仅供内部使用。
 * struct AVFilter * next；// 由过滤器注册系统使用。
 * int(* process_command )(AVFilterContext *, const char *cmd, const char *arg, char *res, int res_len, int flags)；// 使过滤器实例处理一个命令。
 * int(* init_opaque )(AVFilterContext *ctx, void *opaque)；//过滤初始化函数，替代init（）回调。
 * int(* activate )(AVFilterContext *ctx);//过滤器激活函数。
 */
extern "C"
JNIEXPORT jstring JNICALL
Java_com_sun_ffmpeglib_FFmpeg1BaseInfo_avfilterInfo(JNIEnv *env, jclass clazz) {
    char info[40000] = {0};
    //注册所有AVFilter
    //ffmpeg过滤器 滤镜
    avfilter_register_all();

    AVFilter *temp = (AVFilter *)avfilter_next(NULL);
    while(temp != NULL) {
        sprintf(info, "%s%s\n", info, temp->name);
        temp = temp->next;
    }
    return env->NewStringUTF(info);
}
/**
 * AVInputFormat结构体常见属性及方法定义
 * const char *name;//格式名列表.也可以分配一个新名字。
 * const char *long_name;//格式的描述性名称，意味着比名称更易于阅读。
 * int flags;
 * //可用的flag有: AVFMT_NOFILE, AVFMT_NEEDNUMBER, AVFMT_SHOW_IDS,
 * AVFMT_GENERIC_INDEX, AVFMT_TS_DISCONT, AVFMT_NOBINSEARCH,
 *  AVFMT_NOGENSEARCH, AVFMT_NO_BYTE_SEEK, AVFMT_SEEK_TO_PTS.
 * const char *extensions;//文件扩展名
 * const AVClass *priv_class; //一个模拟类型列表.用来在probe的时候check匹配的类型。
 * struct AVInputFormat *next;//用于把所有支持的输入文件容器格式连接成链表，便于遍历查找。
 * int priv_data_size;//标示具体的文件容器格式对应的Context 的大小。
 * int (*read_probe)(AVProbeData *);//判断一个给定的文件是否有可能被解析为这种格式。 给定的buf足够大，所以你没有必要去检查它，除非你需要更多 。
 * int (*read_header)(struct AVFormatContext *);//读取format头并初始化AVFormatContext结构体，如果成功，返回0。创建新的流需要调用avformat_new_stream。
 * int (*read_header2)(struct AVFormatContext *, AVDictionary **options);//新加的函数指针，用于打开进一步嵌套输入的格式。
 * int (*read_packet)(struct AVFormatContext *, AVPacket *pkt);//读取一个数据包并将其放在“pkt”中。 pts和flag也被设置。
 * int (*read_close)(struct AVFormatContext *);//关闭流。 AVFormatContext和Streams不会被此函数释放。
 * int (*read_seek)(struct AVFormatContext *, int stream_index, int64_t timestamp, int flags);
 * int64_t (*read_timestamp)(struct AVFormatContext *s, int stream_index, int64_t *pos, int64_t pos_limit);//获取stream [stream_index] .time_base单位中的下一个时间戳。
 * int (*read_play)(struct AVFormatContext *);//开始/继续播放 - 仅当使用基于网络的（RTSP）格式时才有意义。
 * int (*read_pause)(struct AVFormatContext *);//暂停播放 - 仅当使用基于网络的（RTSP）格式时才有意义。
 * int (*read_seek2)(struct AVFormatContext *s, int stream_index, int64_t min_ts, int64_t ts, int64_t max_ts, int flags);//寻求时间戳ts。
 * int (*get_device_list)(struct AVFormatContext *s, struct AVDeviceInfoList *device_list);返回设备列表及其属性。
 * int (*create_device_capabilities)(struct AVFormatContext *s, struct AVDeviceCapabilitiesQuery *caps);//初始化设备能力子模块。
 * int (*free_device_capabilities)(struct AVFormatContext *s, struct AVDeviceCapabilitiesQuery *caps);//释放设备能力子模块。
 */



/**
 * AVOutputFormat结构体属性及方法
 * const char *name;
 * const char *long_name;//格式的描述性名称，易于阅读。
 * enum AVCodecID audio_codec; //默认的音频编解码器
 * enum AVCodecID video_codec; //默认的视频编解码器
 * enum AVCodecID subtitle_codec; //默认的字幕编解码器
 * struct AVOutputFormat *next;
 * int (*write_header)(struct AVFormatContext *);
 * int (*write_packet)(struct AVFormatContext *, AVPacket *pkt);//写一个数据包。 如果在标志中设置AVFMT_ALLOW_FLUSH，则pkt可以为NULL。
 * int (*write_trailer)(struct AVFormatContext *);
 * int (*interleave_packet)(struct AVFormatContext *, AVPacket *out, AVPacket *in, int flush);
 * int (*control_message)(struct AVFormatContext *s, int type, void *data, size_t data_size);//允许从应用程序向设备发送消息。
 * int (*write_uncoded_frame)(struct AVFormatContext *, int stream_index,   AVFrame **frame, unsigned flags);//写一个未编码的AVFrame。
 * int (*init)(struct AVFormatContext *);//初始化格式。 可以在此处分配数据，并设置在发送数据包之前需要设置的任何AVFormatContext或AVStream参数。
 * void (*deinit)(struct AVFormatContext *);//取消初始化格式。
 * int (*check_bitstream)(struct AVFormatContext *, const AVPacket *pkt);//设置任何必要的比特流过滤，并提取全局头部所需的任何额外数据。
 */
extern "C"
JNIEXPORT jstring JNICALL
Java_com_sun_ffmpeglib_FFmpeg1BaseInfo_avformatInfo(JNIEnv *env, jclass clazz) {
    char info[40000] = {0};

    av_register_all();

    AVInputFormat *i_temp = av_iformat_next(NULL);
    AVOutputFormat *o_temp = av_oformat_next(NULL);
    while (i_temp != NULL) {
        sprintf(info, "%sInput: %s\n", info, i_temp->name);
        i_temp = i_temp->next;
    }
    while (o_temp != NULL) {
        sprintf(info, "%sOutput: %s\n", info, o_temp->name);
        o_temp = o_temp->next;
    }
    return env->NewStringUTF(info);
}
/**
 * URLProtocol是FFmepg操作文件的结构(包括文件，网络数据流等等)，包括open、close、read、write、seek等操作，位于url.h文件中。
 * URLProtocol为协议操作对象，针对每种协议，会有一个这样的对象，每个协议操作对象和一个协议对象关联，比如，文件操作对象为ff_file_protocol，它关联的结构体是FileContext。
 */
extern "C"
JNIEXPORT jstring JNICALL
Java_com_sun_ffmpeglib_FFmpeg1BaseInfo_protocolInfo(JNIEnv *env, jclass clazz) {
    char info[40000] = {0};
    av_register_all();

    struct URLProtocol *pup = NULL;

    struct URLProtocol **p_temp = &pup;
    avio_enum_protocols((void **) p_temp, 0);

    while ((*p_temp) != NULL) {
        sprintf(info, "%sInput: %s\n", info, avio_enum_protocols((void **) p_temp, 0));
    }
    pup = NULL;
    avio_enum_protocols((void **) p_temp, 1);
    while ((*p_temp) != NULL) {
        sprintf(info, "%sInput: %s\n", info, avio_enum_protocols((void **) p_temp, 1));
    }
    return env->NewStringUTF(info);
}
