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


extern "C"
JNIEXPORT void JNICALL
Java_com_sun_ffmpeglib_FFmpeg3File_renameFile(JNIEnv *env, jclass clazz, jstring old_name,
                                              jstring new_name) {


    const char *old_nameP = env->GetStringUTFChars(old_name, nullptr);
    const char *new_nameP = env->GetStringUTFChars(new_name, nullptr);
    LOGE("老文件名 %s", old_nameP);
    LOGE("新文件名 %s", new_nameP);


}

extern "C"

JNICALL extern "C"
JNIEXPORT void JNICALL
Java_com_sun_ffmpeglib_FFmpeg3File_deltenFile(JNIEnv *env, jclass clazz, jstring file_path) {


}

