#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_sun_eg4_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jintArray JNICALL
Java_com_sun_eg4_MainActivity_getIntArray(JNIEnv *env, jobject thiz, jint length) {
    //创建一个指定大小的数组
    jintArray array = env->NewIntArray(length);


    //数组的起始地址 得到一个指向原始数据类型内容的指针

    jint *elementsP = env->GetIntArrayElements(array, nullptr);


    //设置0-100的随机元素
    jint *startP = elementsP;
    for (; startP < elementsP + length; startP++) {
        *startP = static_cast<jint >(random() % 100);
    }

    //0          复制回内容并释放elementsP缓冲区
    //JNI_COMMIT 复制回内容但不释放elems缓冲区
    //JNI_ABORT	 释放缓冲区但不复制回变化
    env->ReleaseIntArrayElements(array, elementsP, 0);
    return array;
}

int compare(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sun_eg4_MainActivity_sortIntArray(JNIEnv *env, jobject thiz, jintArray array) {

    //获取数组起始元素的指针
    jint *arr = env->GetIntArrayElements(array, nullptr);
    //获取数组的长度
    jint len=env->GetArrayLength(array);
    //排序
    qsort(arr,len, sizeof(jint),compare);
    //第三个参数 同步
    //0:java数组进行更新,并且释放c/c++数组
    //JNI_ABORT java数组不进行更新,但释放c/c++数组
    //JNI_COMMIT: java数组进行更新,不释放c/c++数组(函数执行完数组会释放)
    env->ReleaseIntArrayElements(array,arr,0);


}