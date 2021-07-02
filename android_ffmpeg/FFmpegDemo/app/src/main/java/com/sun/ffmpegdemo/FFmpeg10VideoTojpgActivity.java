package com.sun.ffmpegdemo;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.sun.ffmpeglib.FFmpeg10VideotoJpg;
import com.sun.ffmpeglib.FFmpeg8Mp4Cut;

import java.io.File;


public class FFmpeg10VideoTojpgActivity extends AppCompatActivity {

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg10_videotojpg);
    }

    public void videotojpgClick(View view) {
        File file = new File("/sdcard/test_av/videotojpg.jpg");
        if (file != null && file.exists()) {
            file.delete();
        }
        FFmpeg10VideotoJpg.videoToJpg("/sdcard/test_av/Amao.mp4", "/sdcard/test_av/videotojpg.jpg");
    }


    public static void startActivity(Context context) {
        context.startActivity(new Intent(context, FFmpeg10VideoTojpgActivity.class));
    }
}
