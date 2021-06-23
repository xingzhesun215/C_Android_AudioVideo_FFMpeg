package com.sun.ffmpegdemo;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.sun.ffmpeglib.FFmpeg0Version;
import com.sun.ffmpeglib.FFmpeg5ExtractAudio;


public class FFmpeg5ExtractAudioActivity extends AppCompatActivity {


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg5_extractaudio);
    }

    public void extractAudioClick(View view) {
        FFmpeg5ExtractAudio.extractAudio("/sdcard/test_av/input.mp4");
        //此份ffmpeg不支持对http协议
//        FFmpeg5ExtractAudio.extractAudio("http://vfx.mtime.cn/Video/2019/03/17/mp4/190317150237409904.mp4");
    }

    public static void startActivity(Context context) {
        context.startActivity(new Intent(context, FFmpeg5ExtractAudioActivity.class));
    }
}
