package com.sun.eg5;

import android.os.Environment;
import android.util.Log;

import java.io.File;

public class Encryptor {
    private static final String TAG="Encryptor";

    static {
        System.loadLibrary("encryptor");
    }
    private final String BASE_URL = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator;
    /**
     * 加密
     *
     * @param normalPath  文件路径
     * @param encryptPath 加密之后的文件路径
     */
    public native static void encryption(String normalPath, String encryptPath);
    /**
     * 解密
     *
     * @param encryptPath 加密之后的文件路径
     * @param decryptPath 解密之后的文件路径
     */
    public native static void decryption(String encryptPath, String decryptPath);

    /**
     * 创建文件
     *
     */
    private native void createFile(String normalPath);

    /**
     * 测试加解密
     */
    public void test(){
        String fileName="testJni.txt";
        String encryptPath=encryption(fileName);
        decryption(encryptPath);
    }

    /**
     * 加密
     * @param fileName
     * @return 加密文件
     */
    public String encryption(String fileName){
        String normalPath=BASE_URL+fileName;
        File file=new File(normalPath);
        if(!file.exists()){
            createFile(normalPath);
            Log.e(TAG,"文件保存了");
        }
        String encryptPath = BASE_URL + "encryption_" + fileName;
        Encryptor.encryption(normalPath, encryptPath);
        Log.e(TAG, "加密完成了...");
        return encryptPath;
    }

    /**
     * 解密
     */
    public void decryption(String encryptPath) {
        if (!new File(encryptPath).exists()) {
            Log.d(TAG, "解密文件不存在");
            return;
        }
        String decryptPath = encryptPath.replace("encryption_", "decryption_");
        Encryptor.decryption(encryptPath, decryptPath);
        Log.e(TAG, "解密完成了...");
    }
}
