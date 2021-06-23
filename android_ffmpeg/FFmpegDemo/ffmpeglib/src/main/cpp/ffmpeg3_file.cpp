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

//FFmpeg 删除文件：avpriv_io_delete()
//FFmpeg 重命名文件：avpriv_io_move()
//FFmpeg 打开目录：avio_open_dir()
//FFmpeg 读取目录：avio_read_dir();
//FFmpeg 关闭目录：avio_close_dir()

// FFmpeg 删除文件操作
int ffmpegDelFile(const char *filePath) {
    int ret;
    ret = avpriv_io_delete(filePath);  // 在项目目录下创建的文件（测试时需要创建好）
    printf("Del File Code : %d \n", ret);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Failed to delete file \n");
    } else {
        av_log(NULL, AV_LOG_INFO, "Delete File Success！\n ");
    }
    return ret;
}

// FFmpeg 重命名或移动文件
int ffmpegMoveFile(const char *src,const char *dst) {
    int ret;
    ret = avpriv_io_move(src, dst);
    printf("Move File Code : %d \n", ret);
    // 重命名时，如果文件不存在，ret也会0
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "Failed to Move File %s!\n ", src);
    } else {
        av_log(NULL, AV_LOG_INFO, "Success Move File %s!\n", src);
    }
    return ret;
}


// FFmpeg 目录操作
void ffmpegDir(char *dir) {

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

extern "C"
JNIEXPORT void JNICALL
Java_com_sun_ffmpeglib_FFmpeg3File_renameFile(JNIEnv *env, jclass clazz, jstring old_name,
                                              jstring new_name) {

    const char *old_nameP = env->GetStringUTFChars(old_name, nullptr);
    const char *new_nameP = env->GetStringUTFChars(new_name, nullptr);
    LOGE("老文件名 %s", old_nameP);
    LOGE("新文件名 %s", new_nameP);

    LOGE("the result of rename is %d",
         ffmpegMoveFile(old_nameP, new_nameP));
}

extern "C"

JNICALL extern "C"
JNIEXPORT void JNICALL
Java_com_sun_ffmpeglib_FFmpeg3File_deleteFile(JNIEnv *env, jclass clazz, jstring file_path) {

    const char *path = env->GetStringUTFChars(file_path, nullptr);
    LOGE("delete path = %s", path);
    LOGE("delete result= %d", ffmpegDelFile(path));
}


extern "C"

JNICALL extern "C"
JNIEXPORT void JNICALL
Java_com_sun_ffmpeglib_FFmpeg3File_dirList(JNIEnv *env, jclass clazz, jstring file_path) {

    const char *filePath = env->GetStringUTFChars(file_path, nullptr);
    LOGE("遍历文件夹名称=%s", filePath);
    ffmpegDir("/sdcard/test_av");
}

