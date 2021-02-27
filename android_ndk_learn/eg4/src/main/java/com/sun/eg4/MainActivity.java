package com.sun.eg4;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private TextView tv_result;
    private TextView tv_result0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv_result = this.findViewById(R.id.tv_result);
        tv_result0 = this.findViewById(R.id.tv_result0);
        tv_result.setText(stringFromJNI());
    }

    public void runClick(View view) {
        tv_result0.setText("排序前\n");
        //随机获取20个数的数组
        int[] array = getIntArray(20);
        for (int i : array) {
            tv_result0.append(i + " ");
        }
        tv_result.append("\n");
        tv_result.setText("排序后\n");
        sortIntArray(array);
        for (int i : array) {
            tv_result.append(i + " ");
        }

    }

    static {
        System.loadLibrary("native-lib");
    }

    public native String stringFromJNI();

    public native int[] getIntArray(int length);

    public native void sortIntArray(int[] arr);
}
