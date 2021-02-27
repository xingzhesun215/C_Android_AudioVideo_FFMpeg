package com.sun.eg9;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;
import android.widget.Toast;


public class MainActivity extends AppCompatActivity implements JniCallbackDemo.Listener {
    private TextView tv_result;
    /**
     * jni回调用java计时的实例
     */
    private JniCallbackDemo jniCallbackDemo;
    /**
     * 计时是否已经运行
     */
    private boolean running;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv_result = this.findViewById(R.id.tv_result);
        jniCallbackRun(true);
    }

    /**
     * 测试jni调用java
     */
    private void jniCallbackRun(boolean run) {
        if (running) {
            return;
        }
        if (jniCallbackDemo == null) {
            jniCallbackDemo = new JniCallbackDemo();
        }
        jniCallbackDemo.setListener(this);
        if (run) {
            running = true;
            Toast.makeText(this, "开始计时，请查看控制台日志输出", Toast.LENGTH_SHORT).show();
            jniCallbackDemo.startTiming();
        } else {
            jniCallbackDemo.stopTiming();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        jniCallbackRun(false);
    }

    @Override
    public void counter(int count) {
        tv_result.setText("count=" + count);
    }
}
