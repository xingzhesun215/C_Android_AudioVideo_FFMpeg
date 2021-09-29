package com.sun.eg15;


public class VideoPlayer {

    static {
        System.loadLibrary("music-player");
    }
    //音频播放
    public native void audioPlayer(String input);

}