package com.sun.yuvtest;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.ImageFormat;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.hardware.Camera;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.ImageView;

import androidx.annotation.Nullable;


import com.sun.yuvutil.YuvUtil;

import java.io.ByteArrayOutputStream;
import java.util.List;
/*
在 Android 平台使用 Camera API 进行视频的采集，分别使用 SurfaceView、TextureView 来预览 Camera 数据，取到 NV21 的数据回调
* */

public class CameraActivity extends Activity {

    private String TAG = CameraActivity.class.getName();

    private ImageView preview_image;
    private SurfaceView surfaceviewrgb;
    private SurfaceView surfaceviewyuv;
    private GLSurfaceView glsurfaceviewyuv;
    private Camera camera_rgb;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_surfaceview);
        initView();
        initData();
    }

    private GLFrameRenderer mGlFrameRenderer;

    protected void initView() {
        preview_image = this.findViewById(R.id.preview_image);
        surfaceviewrgb = this.findViewById(R.id.surfaceviewrgb);
        surfaceviewyuv = this.findViewById(R.id.surfaceviewyuv);
        glsurfaceviewyuv = this.findViewById(R.id.glsurfaceviewyuv);
        glsurfaceviewyuv.setEGLContextClientVersion(3);
        mGlFrameRenderer = new GLFrameRenderer(glsurfaceviewyuv);
        glsurfaceviewyuv.setRenderer(mGlFrameRenderer);
        mGlFrameRenderer.setSurfaceCreateListener(listener);
        YuvUtil.init(640, 480, 640, 480);

    }

    private final GLFrameRenderer.SurfaceCreateListener listener = new GLFrameRenderer.SurfaceCreateListener() {
        @Override
        public void onSurfaceCreated() {
            Log.e(TAG, "onSurfaceCreated");
        }

        @Override
        public void onSurfaceChanged(int width, int height) {
            Log.e(TAG, "current:" + width + " x " + height);
        }
    };

    public void onClick111(View view) {
        startCamera();
    }


    protected void initData() {
        startCamera();

    }

    public static List<Camera.Size> supportedVideoSizes;
    public static List<Camera.Size> previewSizes;

    public int showWidth = 640;
    public int showHeight = 480;
    private byte[] mYuvCropBytes = new byte[showWidth * showHeight * 3 / 2];


    public void startCamera() {

        camera_rgb = Camera.open(0);

        Camera.Parameters params = camera_rgb.getParameters();  //通过getParameters获取参数

        supportedVideoSizes = params.getSupportedPictureSizes();
        previewSizes = params.getSupportedPreviewSizes();

        for (int i = 0; i < supportedVideoSizes.size(); i++) {
            Log.e("ME", (i + 1) + "-支持分辨率 " + supportedVideoSizes.get(i).width + " : " + supportedVideoSizes.get(i).height);
        }
        for (int i = 0; i < previewSizes.size(); i++) {
            Log.e("ME", (i + 1) + "-预览分辨率 " + previewSizes.get(i).width + " : " + previewSizes.get(i).height);
        }

        camera_rgb.setDisplayOrientation(90);
        /**
         * 设置获取帧
         */
        Camera.Parameters parameters2 = camera_rgb.getParameters();
        parameters2.setPreviewSize(1280, 720);
        parameters2.setPreviewFormat(ImageFormat.NV21);
        camera_rgb.setParameters(parameters2);
        camera_rgb.setPreviewCallback(new Camera.PreviewCallback() {
            @Override
            public void onPreviewFrame(byte[] data, Camera camera) {
                Camera.Size size2 = camera.getParameters().getPreviewSize();
                curThreadName("onpreviewFrame  camera_rgb " + size2.width + "   " + size2.height);


                YuvUtil.compressYUV(data,  showWidth, showHeight,mYuvCropBytes, showWidth, showHeight,1,90,false);
//                YuvUtil.yuvI420ToNV21(data, mYuvCropBytes,showHeight, showWidth);
                mGlFrameRenderer.update(showWidth, showHeight, mYuvCropBytes);

                if (50 == 0) {
                    Log.e("ME", "得到帧大小    " + data.length);
                    Camera.Size size = camera.getParameters().getPreviewSize();
                    try {
                        YuvImage image = new YuvImage(data, ImageFormat.NV21, size.width, size.height, null);
                        if (image != null) {
                            ByteArrayOutputStream stream = new ByteArrayOutputStream();
                            image.compressToJpeg(new Rect(0, 0, size.width, size.height), 80, stream);
                            Bitmap bmp = BitmapFactory.decodeByteArray(stream.toByteArray(), 0, stream.size());

                            preview_image.setImageBitmap(adjustPhotoRotation(bmp, 00));
                            stream.close();
                        }
                    } catch (Exception ex) {
                        Log.e("Sys", "Error:" + ex.getMessage());
                    }
                }


            }
        });
        curThreadName("开始预览");
        Log.e("ME", "开始预览");


        surfaceviewrgb.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder holder) {
                try {
                    curThreadName("surfaceCreatedf rgb");
                    camera_rgb.setPreviewDisplay(holder);
                    Log.e("ME", "开始预览 rgb");
                    camera_rgb.startPreview();

                } catch (Exception e) {

                }
            }

            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
                curThreadName("surfaceChanged");
            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {
                curThreadName("surfaceDestroyed");
                holder.removeCallback(this);
            }
        });
    }

    Handler mHandler = new Handler();


    public void curThreadName(String tag) {
        Log.e("ME", Thread.currentThread().getName() + "____" + tag);
    }

    /**
     * bitmap选择角度
     *
     * @param bm
     * @param orientationDegree
     * @return
     */
    Bitmap adjustPhotoRotation(Bitmap bm, final int orientationDegree) {

        Matrix m = new Matrix();
        m.setRotate(orientationDegree, (float) bm.getWidth() / 2, (float) bm.getHeight() / 2);
        float targetX, targetY;
        if (orientationDegree == 90) {
            targetX = bm.getHeight();
            targetY = 0;
        } else {
            targetX = bm.getHeight();
            targetY = bm.getWidth();
        }
        final float[] values = new float[9];
        m.getValues(values);

        float x1 = values[Matrix.MTRANS_X];
        float y1 = values[Matrix.MTRANS_Y];

        m.postTranslate(targetX - x1, targetY - y1);

        Bitmap bm1 = Bitmap.createBitmap(bm.getHeight(), bm.getWidth(), Bitmap.Config.ARGB_8888);

        Paint paint = new Paint();
        Canvas canvas = new Canvas(bm1);
        canvas.drawBitmap(bm, m, paint);


        return bm1;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (camera_rgb != null) {
            camera_rgb.stopPreview();
            camera_rgb.setPreviewCallback(null);
            camera_rgb.release();
            camera_rgb = null;
        }
    }

    public static void startActivity(Context context) {
        context.startActivity(new Intent(context, CameraActivity.class));
    }
}
