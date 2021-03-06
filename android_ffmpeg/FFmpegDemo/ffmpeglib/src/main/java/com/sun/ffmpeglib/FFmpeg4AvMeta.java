package com.sun.ffmpeglib;

public class FFmpeg4AvMeta {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("sun");
        System.loadLibrary("avcodec");
        System.loadLibrary("avfilter");
        System.loadLibrary("avformat");
        System.loadLibrary("avutil");
        System.loadLibrary("postproc");
        System.loadLibrary("swresample");
        System.loadLibrary("swscale");
    }


    public static native void fileMeta(String filePath);

    public static native void dirFilesMeta(String filePath);
}
