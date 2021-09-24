package com.sun.yuvutil;

public class YuvLib {
    static {
        System.loadLibrary("yuvlib");
    }

    public static native String stringFromJNI();
}
