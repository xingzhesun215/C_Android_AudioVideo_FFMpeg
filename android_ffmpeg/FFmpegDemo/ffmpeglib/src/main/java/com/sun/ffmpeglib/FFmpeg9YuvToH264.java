package com.sun.ffmpeglib;

public class FFmpeg9YuvToH264 {

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

    public static native void yuvFileToH264(String mp4Path, String cutPath, int wid, int hei,int fps);

}
