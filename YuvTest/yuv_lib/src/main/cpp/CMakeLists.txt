cmake_minimum_required(VERSION 3.4.1)

add_library(libyuv SHARED IMPORTED)
set_target_properties(libyuv PROPERTIES IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libyuv.so)

include_directories(/libyuv)
add_library(
        yuvutil

        SHARED

        YuvJni.cpp)

find_library(
        log-lib
        log)


target_link_libraries(
        yuvutil

        libyuv
        ${log-lib}

       )