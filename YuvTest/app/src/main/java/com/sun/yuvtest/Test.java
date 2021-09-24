package com.sun.yuvtest;

import android.util.Log;

import java.io.File;

public class Test {
    public static void main(String[] args) {


        File file = new File("F:\\BaiduNetdiskDownload\\Android进阶之旅（NDK实战篇）");
        changeName(file);
    }

    public static void changeName(File file) {
        if (file == null) {
            return;
        }
        if (!file.exists()) {
            return;
        }

        if (file.isDirectory()) {
            File[] files = file.listFiles();
            for (int i = 0; i < files.length; i++) {
                changeName(files[i]);
            }
        } else {
            String path = file.getAbsolutePath();
            System.out.println("path="+path);
            if (path.contains("【瑞客论坛 www.ruike1.com】")) {
                file.renameTo(new File( path.replace("【瑞客论坛 www.ruike1.com】", "")));
            }
        }
    }
}
