package com.sun.ffmpegdemo;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;


import com.sun.ffmpeglib.FFmpeg9YuvToH264;

import java.io.File;


public class FFmpeg9YuvToH264Activity extends AppCompatActivity {

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg9_yuvtoh264);
    }

    public void yuvToH264Click(View view) {
        File file = new File("/sdcard/test_av/640_360.h264");
        if (file != null && file.exists()) {
            file.delete();
        }
        FFmpeg9YuvToH264.yuvFileToH264("/sdcard/test_av/640_360.yuv", "/sdcard/test_av/640_360.h264", 640, 360,10);
    }


    public static void startActivity(Context context) {
        context.startActivity(new Intent(context, FFmpeg9YuvToH264Activity.class));
    }
}
