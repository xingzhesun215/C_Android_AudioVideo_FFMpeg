package com.sun.ffmpegdemo;

import android.os.Bundle;
import android.view.View;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;


public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void ffmpeg0VersionClick(View view) {
        FFmpeg0VersionActivity.startActivity(this);
    }


    public void ffmpeg1BaseInfoClick(View view) {
        FFmpeg1BaseInfoActivity.startActivity(this);
    }

    public void ffmpeg2LogClick(View view) {
        FFmpeg2LogActivity.startActivity(this);
    }

    public void ffmpeg3FileClick(View view) {
        FFmpeg3FileActivity.startActivity(this);
    }
}
