package com.sun.ffmpeglib;

public class FFmpeg3File {

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

    public static native void renameFile(String oldName, String newName);

    public static native void deleteFile(String filePath);

    public static native void dirList(String filePath);
}
