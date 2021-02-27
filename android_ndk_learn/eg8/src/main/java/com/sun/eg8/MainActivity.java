package com.sun.eg8;

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
        new JniFileOperation().test();
    }

    static {
        System.loadLibrary("native-lib");
    }
    public native String stringFromJNI();
}
