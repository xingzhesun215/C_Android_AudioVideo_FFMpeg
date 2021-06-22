package com.sun.ffmpegdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.sun.ffmpeglib.FFmpeg1BaseInfo;

public class FFmpeg1BaseInfoActivity extends AppCompatActivity implements View.OnClickListener {

    private Button mAvcodecBtn;
    private Button mAvformatBtn;
    private Button mAvfilterBtn;
    private Button mProtocolBtn;
    private TextView mInfoText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ffmpeg1_baseinfo);

        mInfoText = findViewById(R.id.sample_text);
        mAvcodecBtn = findViewById(R.id.avcodec_btn);
        mAvformatBtn = findViewById(R.id.avformat_btn);
        mAvfilterBtn = findViewById(R.id.avfilter_btn);
        mProtocolBtn = findViewById(R.id.protocol_btn);

        mInfoText.setText(FFmpeg1BaseInfo.avcodecInfo());
        mInfoText.setMovementMethod(ScrollingMovementMethod.getInstance());

        mAvcodecBtn.setOnClickListener(this);
        mAvformatBtn.setOnClickListener(this);
        mAvfilterBtn.setOnClickListener(this);
        mProtocolBtn.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        if (v == mAvcodecBtn) {
            mInfoText.setText(FFmpeg1BaseInfo.avcodecInfo());
        } else if (v == mAvformatBtn) {
            mInfoText.setText(FFmpeg1BaseInfo.avformatInfo());
        } else if (v == mAvfilterBtn) {
            mInfoText.setText(FFmpeg1BaseInfo.avfilterInfo());
        } else if (v == mProtocolBtn) {
            mInfoText.setText(FFmpeg1BaseInfo.protocolInfo());
        }
    }

    public static void startActivity(Context context){
        context.startActivity(new Intent(context,FFmpeg1BaseInfoActivity.class));
    }
}
