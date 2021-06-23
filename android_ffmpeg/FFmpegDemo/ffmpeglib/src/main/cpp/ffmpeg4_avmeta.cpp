#include <jni.h>
#include <string>

extern "C" {
#include "include/libavcodec/avcodec.h"
#include "include/libavformat/avformat.h"
#include "include/libavfilter/avfilter.h"
#include "include/libavutil/avutil.h"
#include "android_log.h"

#include <jni.h>
}


// FFmpeg 目录操作
void ffmpegMetaDir(char *dir) {

    int ret;

    // 上下文
    AVIODirContext *dirCtx = NULL;
    AVIODirEntry *dirEntry = NULL;

    LOGE("list 1");

    // 注意Windows下会返回-40，也就是Function not implement，方法未实现，也就是说windows下不支持此方法
    ret = avio_open_dir(&dirCtx, dir, NULL);
    LOGE("list 2 result= %d", ret);
    if (ret < 0) {
        LOGE(" list 3 cant open dir，msg = %s", av_err2str(ret));
        // 输出错误日志
        printf("cant open dir，msg = %s", av_err2str(ret));
        return;
    }
    LOGE("list 4");
    av_log(NULL, AV_LOG_INFO, "Open Dir Success!");


    while (1) {
        ret = avio_read_dir(dirCtx, &dirEntry);
        if (ret < 0) {
            printf("cant read dir : %s", av_err2str(ret));
            // 防止内存泄漏
            goto __failed;
        }
        av_log(NULL, AV_LOG_INFO, "read dir success");
        if (!dirEntry) {
            break;
        }
        printf("Entry Name = %s 类型=%d", dirEntry->name, dirEntry->type);
        LOGE("Entry Name = %s 类型=%d", dirEntry->name, dirEntry->type);//type3文件夹  type7文件

        // 释放资源
        avio_free_directory_entry(&dirEntry);
    }
    LOGE("list 5");
// 释放资源
    __failed:
    avio_close_dir(&dirCtx);
}

int ffmpegAvMeta(const char *filePath) {
    av_log_set_level(AV_LOG_INFO);
    AVFormatContext *fmt_ctx = NULL;
    int ret;
    //参数为 AVFormatContext上下文 文件名 指定格式(一般为NULL,由ffmpeg自行解析),附加参数(一般为NULL)
    ret = avformat_open_input(&fmt_ctx, filePath, NULL, NULL);
    if (ret < 0) {
        LOGE("cant open file :%s", av_err2str(ret));
    } else {
        LOGE("cant open file succ");
        ret = avformat_find_stream_info(fmt_ctx, 0);
        //获取音频视频流信息 ,h264 flv
        if (ret != 0)
        {
            LOGE("cant find_stream :%s", av_err2str(ret));
        }else{
            LOGE(" find_stream succ");
            //参数为AVFormatContext上下文,流索引值(一般不关心,为0) 文件名 是否是输入出文件1是0否
            av_dump_format(fmt_ctx, 0, filePath, 0);
        }

    }

    avformat_close_input(&fmt_ctx);
    return ret;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sun_ffmpeglib_FFmpeg4AvMeta_fileMeta(JNIEnv *env, jclass clazz, jstring file_path) {
    const char *path = env->GetStringUTFChars(file_path, nullptr);
    LOGE("the file path is %s", path);
    LOGE("the result is %d", ffmpegAvMeta(path));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sun_ffmpeglib_FFmpeg4AvMeta_dirFilesMeta(JNIEnv *env, jclass clazz, jstring file_path) {
    const char *path = env->GetStringUTFChars(file_path, nullptr);
    LOGE("the dir is %s", path);
}