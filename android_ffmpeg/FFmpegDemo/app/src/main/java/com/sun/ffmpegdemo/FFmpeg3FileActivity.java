package com.sun.ffmpegdemo;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.sun.ffmpeglib.FFmpeg0Version;
import com.sun.ffmpeglib.FFmpeg3File;


public class FFmpeg3FileActivity extends AppCompatActivity {


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg3_file);

    }

    public void renameClick(View view) {
        FFmpeg3File.renameFile("123.png","456.jpg");
    }

    public void deleteClick(View view) {

    }

    public static void startActivity(Context context) {
        context.startActivity(new Intent(context, FFmpeg3FileActivity.class));
    }
}
