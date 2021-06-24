package com.sun.ffmpegdemo;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.sun.ffmpeglib.FFmpeg8Mp4Cut;

import java.io.File;


public class FFmpeg11VideoCutActivity extends AppCompatActivity {

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg11_videocut);
    }

    public void mp4CutClick(View view) {
        File file = new File("/sdcard/test_av/result_cut.mp4");
        if (file != null && file.exists()) {
            file.delete();
        }
        FFmpeg8Mp4Cut.mp4Cut("/sdcard/test_av/Amao.mp4", "/sdcard/test_av/result_cut.mp4", 5, 20);
    }


    public static void startActivity(Context context) {
        context.startActivity(new Intent(context, FFmpeg11VideoCutActivity.class));
    }
}
