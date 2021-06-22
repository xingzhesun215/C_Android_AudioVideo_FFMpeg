package com.sun.eg1;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private TextView tv_result;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv_result = this.findViewById(R.id.tv_result);
        tv_result.setText(stringFromJNI());

        StuInfo stuInfo = new StuInfo(1000, "孙观勇", 29, "信息安全1班");
        setStuInfo(stuInfo);
    }

    static {
        System.loadLibrary("native-lib");
    }
    public native String stringFromJNI();

    /**
     * 设置学生信息
     *
     * @param stuInfo
     * @return
     */
    public native void setStuInfo(StuInfo stuInfo);
}
