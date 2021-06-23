package com.sun.ffmpegdemo;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.sun.ffmpeglib.FFmpeg0Version;
import com.sun.ffmpeglib.FFmpeg4AvMeta;


public class FFmpeg4avMetaActivity extends AppCompatActivity {


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg4_avmeta);

    }

    public void oneFileMetaClick(View view){
        FFmpeg4AvMeta.fileMeta("/sdcard/test_av/test.avi");
    }

    public static void startActivity(Context context) {
        context.startActivity(new Intent(context, FFmpeg4avMetaActivity.class));
    }
}
