package com.sun.ffmpegdemo;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.sun.ffmpeglib.FFmpeg0Version;


public class FFmpeg7VersionActivity extends AppCompatActivity {

    TextView tv_content;


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg7_version);

        tv_content = findViewById(R.id.tv_content);

        tv_content.setText("version=" + FFmpeg0Version.ffmpegversion());
    }

    public static void startActivity(Context context) {
        context.startActivity(new Intent(context, FFmpeg7VersionActivity.class));
    }
}
