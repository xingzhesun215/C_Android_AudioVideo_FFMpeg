package com.sun.eg10;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;

public class MainActivity extends Activity {

    private NDKFFmpeg ndkfFmpeg;
    private String inFilePath;
    private String outFilePath;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ndkfFmpeg = new NDKFFmpeg();
        String rootPath = Environment.getExternalStorageDirectory()
                .getAbsolutePath();
        inFilePath = rootPath.concat("/FFmpeg-Test/Test.mov");
        outFilePath = rootPath.concat("/FFmpeg-Test/Test.yuv");
    }

    public void clickFFmpegTestConfig(View v){
        ndkfFmpeg.callFFmpegTestConfig();
    }

    public void clickFFmpegOldReadInfo(View v){
        ndkfFmpeg.callFFmpegOldReadInfo(inFilePath);
    }

    public void clickFFmpegNewReadInfo(View v){
        ndkfFmpeg.callFFmpegNewReadInfo(inFilePath);
    }

    public void clickFFmpegOldDecode(View v){
        ndkfFmpeg.callFFmpegOldDecode(inFilePath,outFilePath);
    }

    public void clickFFmpegNewDecode(View v){
        ndkfFmpeg.callFFmpegNewDecode(inFilePath,outFilePath);
    }
}
