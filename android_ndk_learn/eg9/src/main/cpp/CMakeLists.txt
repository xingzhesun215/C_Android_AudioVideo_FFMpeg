cmake_minimum_required(VERSION 3.4.1)


find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

add_library(
        jni_callback
        SHARED
        jni_callback.cpp)

target_link_libraries(
        jni_callback
        ${log-lib})


