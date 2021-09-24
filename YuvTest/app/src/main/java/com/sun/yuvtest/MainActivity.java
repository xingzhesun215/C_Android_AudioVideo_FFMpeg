package com.sun.yuvtest;


import android.Manifest;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.sun.yuvtest.permission.PermissionUtils;

public class MainActivity extends Activity {
    public static final int PERMISSION_CODE = 10;
    private TextView tv_temssage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv_temssage = this.findViewById(R.id.tv_temssage);
        String[] perms = new String[]{Manifest.permission.CAMERA, Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE};
        if (!PermissionUtils.hasPermissions(this, perms)) {
            PermissionUtils.requestPermissions(this, PERMISSION_CODE, perms);
        } else {
        }
    }

    public void cameraClick(View ivew) {
        CameraActivity.startActivity(this);
    }
}