package com.sun.ffmpegdemo;

import android.Manifest;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.sun.ffmpegdemo.permission.AfterPermissionGranted;
import com.sun.ffmpegdemo.permission.PermissionUtils;


public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        RequestPermission();
    }


    public static final int CAMERA_PERIMISSION_CODE = 11;

    public void RequestPermission() {
        String[] perms = new String[]{Manifest.permission.INTERNET, Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.CAMERA, Manifest.permission.RECORD_AUDIO};
        if (PermissionUtils.hasPermissions(this, perms)) {
            startPreview();
        } else {
            PermissionUtils.requestPermissions(this, CAMERA_PERIMISSION_CODE, perms);
        }

    }

    @AfterPermissionGranted(CAMERA_PERIMISSION_CODE)
    public void startPreview() {
        Log.e("ME", "已授权");
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

    public void ffmpeg4MetaClick(View view) {
        FFmpeg4avMetaActivity.startActivity(this);
    }

    public void ffmpeg5ExtractAudioClick(View view) {
        FFmpeg5ExtractAudioActivity.startActivity(this);
    }

    public void ffmpeg6ExtractVideoClick(View view) {
        FFmpeg6ExtractVideoActivity.startActivity(this);
    }
}
