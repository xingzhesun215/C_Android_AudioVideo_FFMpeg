package com.sun.eg6;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private TextView tv_result;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv_result = this.findViewById(R.id.tv_result);
        tv_result.setText(stringFromJNI());
    }

    static {
        System.loadLibrary("native-lib");
    }
    public native String stringFromJNI();

    public void testClick(View view){
        new JniBitmapDemo().test();
    }
}
