package com.sun.ffmpegdemo;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.sun.ffmpeglib.FFmpeg0Version;
import com.sun.ffmpeglib.FFmpeg6ExtractVideo;

import java.io.File;


public class FFmpeg6ExtractVideoActivity extends AppCompatActivity {


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg6_extractvideo);


    }

    public void extractVideoClick(View view) {
        File file = new File("/sdcard/test_av/333.h264");
        if (file != null && file.exists()) {
            file.delete();
        }
        //部分视频能成功,不够健壮,后续修改
        FFmpeg6ExtractVideo.extractVideo("/sdcard/test_av/Amao.mp4");

    }

    public static void startActivity(Context context) {
        context.startActivity(new Intent(context, FFmpeg6ExtractVideoActivity.class));
    }
}
