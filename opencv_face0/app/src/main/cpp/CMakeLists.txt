cmake_minimum_required(VERSION 3.10.2)


project("opencv_face")


#把CMAKE_SOURCE_DIR目录的下的所有 .cpp 和 .c 文件赋值给 SRC_LIST
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR} SRC_LIST)

# 输出SRC_LIST
MESSAGE("SRC_LIST= " ${SRC_LIST})
# 输出打印构建目录
MESSAGE("This is HELLO_BINARY_DIR " ${HELLO_BINARY_DIR})
# 输出打印资源目录
MESSAGE("This is HELLO_SOURCE_DIR " ${HELLO_SOURCE_DIR})
# 输出打印资源目录，与HELLO_SOURCE_DIR 一样
MESSAGE("This is PROJECT_SOURCE_DIR " ${PROJECT_SOURCE_DIR})
# 输出打印 CMake 资源目录，与 PROJECT_SOURCE_DIR 一样
MESSAGE(STATUS "This is CMAKE_SOURCE_DIR " ${CMAKE_SOURCE_DIR})


#导入头文件
include_directories(include)

#导入main/jniLibs下的so库文件
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}")


add_library(
        # Sets the name of the library.
        native-lib
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        native-lib.cpp
)

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )


target_link_libraries(
        native-lib
        android
        opencv_java4
        jnigraphics
        ${log-lib} )