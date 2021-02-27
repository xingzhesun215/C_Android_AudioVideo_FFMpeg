#include <jni.h>
#include <string>


/**
* 工具方法
* 作用: 把java中的string 转化成一个c语言中的char数组
* 接受的参数 envjni环境的指针
* jstr 代表的是要被转化的java的string 字符串
* 返回值 : 一个c语言中的char数组的首地址 (char 字符串)
*/

char *Jstring2CStr(JNIEnv *env, jstring jstr) {
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("UTF-8");   //这里填写是工作空间的编码，若是默认中文则是GB2312
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);  //字符串拼接函数...
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_sun_eg3_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_sun_eg3_MainActivity_accessField(JNIEnv *env, jobject instance) {
    //获取类
    jclass jcla = env->GetObjectClass(instance);
    //获取类的成员变量none_static_str的id
    jfieldID jfId = env->GetFieldID(jcla, "none_static_str", "Ljava/lang/String;");

    jstring none_static_str = (jstring) env->GetObjectField(instance, jfId);
//    return none_static_str;//获取到后可以原样返回
    jstring after = env->NewStringUTF("hello ,我是新的");
//    修改原属性的值
    env->SetObjectField(instance, jfId, after);
//    重新获取看下效果,实际变量已更改了
    jstring none_static_str_new = (jstring) env->GetObjectField(instance, jfId);
    return none_static_str_new;
//    char * new_str="hello ,我在原数据加了个头";
//    char * old_str=Jstring2CStr(env,none_static_str);;
//    strcat(new_str,old_str);//此方法有问题,待后续排除
//    return  env->NewStringUTF(new_str);//拼接后返回,有bug,暂停
}


extern "C"
JNIEXPORT jstring JNICALL
Java_com_sun_eg3_MainActivity_accessStaticField(JNIEnv *env, jobject instance) {

    //获取类
    jclass jcla = env->GetObjectClass(instance);
    //获取静态变量 static_str 的id
    jfieldID staticFid1 = env->GetStaticFieldID(jcla, "static_str", "Ljava/lang/String;");
    jfieldID staticFid = env->GetStaticFieldID(jcla, "age", "I");
    //获取静态变量
    int age = env->GetStaticIntField(jcla, staticFid);


    jstring static_str = (jstring) env->GetStaticObjectField(jcla, staticFid1);
    age = age + 100;
    //重新设置静态变量
    jstring after = env->NewStringUTF("新的静态属性值");
    env->SetStaticObjectField(jcla, staticFid1, after);
    jstring static_str_new = (jstring) env->GetStaticObjectField(jcla, staticFid1);
//    return age;
//    return static_str;
    return static_str_new;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_sun_eg3_MainActivity_accessMethod(JNIEnv *env, jobject thiz) {
//获取类
    jclass jCla = env->GetObjectClass(thiz);
//获取 getName 方法id 第二个参数 方法名 第三个参数(参数)返回值 的类型描述
    jmethodID jmethodId = env->GetMethodID(jCla, "getName",
                                           "(Ljava/lang/String;)Ljava/lang/String;");


    jstring res = env->NewStringUTF("xingzhesun215");
    jobject objRes = env->CallObjectMethod(thiz, jmethodId, res);
    return static_cast<jstring >(objRes);
}



extern "C"
JNIEXPORT jstring JNICALL
Java_com_sun_eg3_MainActivity_accessStaticMethod(JNIEnv *env, jobject thiz,jint no) {
    //获取类
    jclass jcla=env->GetObjectClass(thiz);
    //获取静态方法 getStaticName 的id
    jmethodID methodId=env->GetStaticMethodID(jcla,"getStaticName","(I)Ljava/lang/String;");
    //调用静态方法
    jstring res=(jstring)env->CallStaticObjectMethod(jcla,methodId,no);
    return res;

}

extern "C"
JNIEXPORT jobject JNICALL
Java_com_sun_eg3_MainActivity_accessConstructor(JNIEnv *env, jobject thiz) {

    //得到Test对应的class
    jclass jcla_test=env->FindClass("com/sun/eg3/Test");
    jclass jcla_date=env->FindClass("java/util/Date");

    //获取构造方法的method id 构造方法为void函数 对应的方法名为<init>
    jmethodID jmethodId=env->GetMethodID(jcla_date,"<init>","()V");
    jmethodID test_methodId=env->GetMethodID(jcla_test,"<init>","()V");

    jobject jdateClass=env->NewObject(jcla_date,jmethodId);
    jobject jTestClass=env->NewObject(jcla_test,test_methodId);

//    return jdateClass;
    return jTestClass;
}
