package com.sun.ffmpegdemo;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.sun.ffmpeglib.FFmpeg7Mp4toFlv;

import java.io.File;


public class FFmpeg7mp4toflvActivity extends AppCompatActivity {


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg7_mp4toflv);

    }

    public void mp4toFlvClick(View view) {
        File file = new File("/sdcard/test_av/input_result.flv");
        if (file != null && file.exists()) {
            file.delete();
        }
        FFmpeg7Mp4toFlv.mp4toFlv("/sdcard/test_av/input.mp4", "/sdcard/test_av/input_result.flv");
    }


    public static void startActivity(Context context) {
        context.startActivity(new Intent(context, FFmpeg7mp4toflvActivity.class));
    }
}
