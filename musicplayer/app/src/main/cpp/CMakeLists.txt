cmake_minimum_required(VERSION 3.4.1)

## libavcodec.so
add_library(libavcodec SHARED IMPORTED)
set_target_properties(libavcodec PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libavcodec.so)

## libavfilter.so
add_library(libavfilter SHARED IMPORTED)
set_target_properties(libavfilter PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libavfilter.so)

## libavformat.so
add_library(libavformat SHARED IMPORTED)
set_target_properties(libavformat PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libavformat.so)

## libavutil.so
add_library(libavutil SHARED IMPORTED)
set_target_properties(libavutil PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libavutil.so)

## libpostproc.so
add_library(libpostproc SHARED IMPORTED)
set_target_properties(libpostproc PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libpostproc.so)

## libswresample.so
add_library(libswresample SHARED IMPORTED)
set_target_properties(libswresample PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libswresample.so)

## libswscale.so
add_library(libswscale SHARED IMPORTED)
set_target_properties(libswscale PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libswscale.so)

include_directories(./include)

add_library(
        # Sets the name of the library.
        music-player
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).

        ffmpeg_voicer.c
)


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        # 链接额外的 ffmpeg 的编译
        music-player
        # 编解码(最重要的库)
        libavcodec
        # 滤镜特效处理库
        libavfilter
        # 封装格式处理库
        libavformat
        # 工具库(大部分库都需要这个库的支持)
        libavutil
        # 后期处理
        libpostproc
        # 音频采样数据格式转换库
        libswresample
        # 视频像素数据格式转换
        libswscale





        # 链接 android ndk 自带的一些库
        android
        # Links the target library to the log library
        # included in the NDK.
        log)