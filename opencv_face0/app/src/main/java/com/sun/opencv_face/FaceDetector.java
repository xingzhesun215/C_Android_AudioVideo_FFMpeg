package com.sun.opencv_face;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class FaceDetector {
    static {
        System.loadLibrary("native-lib");
    }

    public void init(Context context) {
        String path = copyCascadeToLocal(context);
        loadCascade(path);
    }

    /**
     * 将lbpcascade_frontalface文件复制到本地
     */
    private String copyCascadeToLocal(Context context) {

        try {
            InputStream ris = context.getResources().openRawResource(R.raw.lbpcascade_frontalface);
            File cascadeDir = context.getDir("cascade", Context.MODE_PRIVATE);
            File cascadeFile = new File(cascadeDir, "lbpcascade_frontalface.xml");
            OutputStream os = new FileOutputStream(cascadeFile);
            byte[] buffer = new byte[4 * 1024];
            int bytesRead = -1;
            try {
                while ((bytesRead = ris.read(buffer)) != -1) {
                    os.write(buffer, 0, bytesRead);
                }

                ris.close();
                os.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return cascadeFile.getAbsolutePath();
        } catch (Exception e) {
            Log.e("ME", "出一次了");
        }
        return null;

    }

    public native void loadCascade(String String);

    public native int faceDetectSave(Bitmap bitmap);
}