package com.sun.eg7;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    private TextView tv_result;

    private final String BASE_URL = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator+"DCIM";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv_result = this.findViewById(R.id.tv_result);
        tv_result.setText(stringFromJNI());
        Log.e("ME",BASE_URL);

    }

    public void testClick(View view){
        if(hasFilePermission()){
            Log.e("ME","开始遍历 "+BASE_URL);
            listDirAllFile(BASE_URL);
            Log.e("ME","遍历完了 "+BASE_URL);
        }
    }

    static {
        System.loadLibrary("native-lib");
    }
    public native String stringFromJNI();
    /**
     * 输出文件夹下得所有文件
     *
     * @param dirPath 文件夹路径
     */
    public native void listDirAllFile(String dirPath);

    /**
     * 申请权限
     */
    private boolean hasFilePermission() {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 0x1024);
                return false;
            }
        }
        return true;
    }
}
