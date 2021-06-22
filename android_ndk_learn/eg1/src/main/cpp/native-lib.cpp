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

extern "C" JNIEXPORT jstring JNICALL
Java_com_sun_eg1_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT void JNICALL
Java_com_sun_eg1_MainActivity_setStuInfo(JNIEnv *env, jobject thiz, jobject stu_info) {

    StuInfo stuInfo;
    LOGE("====setStu===1====");
    //获取jclass的实例
    jclass jcs = env->FindClass("com/sun/eg1/StuInfo");

    //获取StuInfo的字段ID
    jfieldID fileId = env->GetFieldID(jcs, "stuId", "I");
    jfieldID nameId = env->GetFieldID(jcs, "stuName", "Ljava/lang/String;");
    jfieldID ageId = env->GetFieldID(jcs, "stuAge", "I");
    jfieldID classId = env->GetFieldID(jcs, "className", "Ljava/lang/String;");

    //把字段Id设置到结构体中
    stuInfo.stuId = env->GetIntField(stu_info, fileId);
    jstring nameStr = (jstring) env->GetObjectField(stu_info, nameId);
    const char *locstr = env->GetStringUTFChars(nameStr, 0);
    strcpy(stuInfo.stuName, locstr);
    stuInfo.stuAge =  env->GetIntField(stu_info, ageId);;
    jstring classStr = (jstring) env->GetObjectField(stu_info, classId);
    const char *cstr = env->GetStringUTFChars(classStr, 0);
    strcpy(stuInfo.className, cstr);
    LOGE("====setStu===4====");
    LOGE("传递到C的StuInfo：stuId：%d;stuName：%s;stuAge：%d;className：%s",
         stuInfo.stuId, stuInfo.stuName, stuInfo.stuAge, stuInfo.className);
}