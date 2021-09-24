#include <jni.h>
#include <string>
#include <android/log.h>
#define LOG_TAG "SUN_JNI_Log"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG ,__VA_ARGS__) // 定义LOGE类型



typedef struct {
    int stuId;
    char stuName[50];
    int stuAge;
    char className[50];
} StuInfo;




