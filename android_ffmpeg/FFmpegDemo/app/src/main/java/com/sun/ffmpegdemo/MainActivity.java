package com.sun.ffmpegdemo;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.sun.ffmpeglib.FFmpegVersion;


public class MainActivity extends AppCompatActivity implements View.OnClickListener {

  private Button mFFmpegInfoBtn;
  private Button mRemuxBtn;
  private TextView tv_version;

  @Override
  protected void onCreate(@Nullable Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);

    mFFmpegInfoBtn = findViewById(R.id.ffmpeg_info_btn);
    mRemuxBtn = findViewById(R.id.remux_btn);
    tv_version = findViewById(R.id.tv_version);

    mFFmpegInfoBtn.setOnClickListener(this);
    mRemuxBtn.setOnClickListener(this);
    tv_version.setText(FFmpegVersion.ffmpegversion());
  }

  @Override
  public void onClick(View v) {
    if (v == mFFmpegInfoBtn) {
      Intent intent = new Intent(this, FFmpegInfoActivity.class);
      startActivity(intent);
    } else if (v == mRemuxBtn) {
      Intent intent = new Intent(this, FFmpegRemuxActivity.class);
      startActivity(intent);
    }
  }
}
