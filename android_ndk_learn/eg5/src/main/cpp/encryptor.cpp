#include <jni.h>
#include <string>
#include <cstdio>

#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR,"Encryptor_jni",FORMAT,##__VA_ARGS__);

char password[] = "xingzhesun215";

extern "C"
JNIEXPORT void JNICALL
Java_com_sun_eg5_Encryptor_createFile(JNIEnv *env, jobject thiz, jstring normalPath_) {
    //获取字符串存在JVM内存中
    const char *normalPath = env->GetStringUTFChars(normalPath_, nullptr);
    //打开 normalPath wb:只写打开或者新建一个二进制文件,只允许写数据
    FILE *fp=fopen(normalPath,"wb");

    //把字符串写到指定的流 stream 中,但不包括空字符
    fputs("Hi ,this file is created by JNI, and My name is xingzhesun215.",fp);
    //关闭流fp,刷新所有的缓冲区
    fclose(fp);
    //释放JVM保存的字符串的内存
    env->ReleaseStringUTFChars(normalPath_, normalPath);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_sun_eg5_Encryptor_encryption(JNIEnv *env, jclass type, jstring normalPath_,
                                      jstring encryptPath_) {
    //获取字符串保存在JVM中内存中
    const char *normalPath = env->GetStringUTFChars(normalPath_, nullptr);
    const char *encryptPath = env->GetStringUTFChars(encryptPath_, nullptr);

    //rb:只读打开一个二进制文件，允许读数据。
    //wb:只写打开或新建一个二进制文件；只允许写数据
    FILE *normal_fp = fopen(normalPath, "rb");
    FILE *encrypt_fp = fopen(encryptPath, "wb");

    if (normal_fp == nullptr) {
        return;
    }

    //一次读取一个字符
    int ch = 0;
    int i = 0;
    size_t pwd_length = strlen(password);
    while ((ch = fgetc(normal_fp)) != EOF) { //End of File
        //写入(异或运算)
        fputc(ch ^ password[i % pwd_length], encrypt_fp);
        i++;
    }

    //关闭流 normal_fp和encrypt_fp。刷新所有的缓冲区
    fclose(normal_fp);
    fclose(encrypt_fp);

    //释放JVM保存的字符串的内存
    env->ReleaseStringUTFChars(normalPath_, normalPath);
    env->ReleaseStringUTFChars(encryptPath_, encryptPath);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sun_eg5_Encryptor_decryption(JNIEnv *env, jclass type, jstring encryptPath_,
                                      jstring decryptPath_) {
//    获取字符串保存在JVM中内存中
    const char *encryptPath = env->GetStringUTFChars(encryptPath_, nullptr);
    const char *decryptPath = env->GetStringUTFChars(decryptPath_, nullptr);


    //rb:只读打开一个二进制文件，允许读数据。
    //wb:只写打开或新建一个二进制文件；只允许写数据
    FILE *encrypt_fp = fopen(encryptPath, "rb");
    FILE *decrypt_fp = fopen(decryptPath, "wb");

    if (encrypt_fp == nullptr) {
        return;
    }

    int ch;
    int i = 0;
    size_t pwd_length = strlen(password);
    while ((ch = fgetc(encrypt_fp)) != EOF) {
        fputc(ch ^ password[i % pwd_length], decrypt_fp);
        i++;
    }

    //关闭流 encrypt_fp 和 decrypt_fp。刷新所有的缓冲区
    fclose(encrypt_fp);
    fclose(decrypt_fp);

    //释放JVM保存的字符串的内存
    env->ReleaseStringUTFChars(encryptPath_, encryptPath);
    env->ReleaseStringUTFChars(decryptPath_, decryptPath);
}