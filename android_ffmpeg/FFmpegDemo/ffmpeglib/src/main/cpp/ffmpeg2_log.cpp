#include <jni.h>
#include <string>

extern "C" {
#include "include/libavutil/avutil.h"
#include "android_log.h"
}


extern "C"
JNIEXPORT jstring

JNICALL Java_com_sun_ffmpeglib_FFmpeg2Log_log(JNIEnv *env, jclass clazz) {

    LOGE("hello ,this is LOGE");
    LOGI("hello ,this is LOGI");
    return env->NewStringUTF("我把日志放到logcat中了");
}

