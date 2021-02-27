package com.sun.eg3;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import java.util.Date;

public class MainActivity extends AppCompatActivity {
    private TextView tv_result;
    private TextView tv_content;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv_result = this.findViewById(R.id.tv_result);
        tv_content = this.findViewById(R.id.tv_content);
        tv_result.setText(stringFromJNI());
    }

    /**
     * jni访问java成员变量
     *
     * @param view
     */
    public void access_field_Click(View view) {
        tv_content.setText(accessField());
    }

    /**
     * jni访问java静态变量
     *
     * @param view
     */
    public void access_static_field_Click(View view) {
        tv_content.setText("今年年龄=" + accessStaticField());
    }

    /**
     * jni访问java非静态方法
     *
     * @param view
     */
    public void access_method_Click(View view) {
        tv_content.setText(accessMethod());
    }

    /**
     * jni访问java静态方法
     *
     * @param view
     */
    public void access_static_method_Click(View view) {
        tv_content.setText(accessStaticMethod(216));
    }

    /**
     * jni访问java构造方法
     *
     * @param view
     */
    public void access_constructor_Click(View view) {
        tv_content.setText(accessConstructor().toString());
    }

    public String none_static_str = "你好,我是对象字段";
    private static String static_str = "你好,我是类静态字段";
    private static int age = 10;

    public String getName(String name) {
        return "我的姓名是" + name;
    }

    public static String getStaticName(int no) {
        return "我的静态姓名是xingzhesun" + no;
    }

    static {
        System.loadLibrary("native-lib");
    }

    public native String stringFromJNI();

    public native String accessField();

    public native String accessStaticField();

    public native String accessMethod();

    public native String accessStaticMethod(int no);

    public native Test accessConstructor();
}
