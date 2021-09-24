package com.sun.yuvtest;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.util.Log;


import java.nio.ByteBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLFrameRenderer implements Renderer {
    private static final String Tag = "GLFrameRenderer";
    private final GLSurfaceView mTargetSurface;
    private final GLProgram prog = new GLProgram(0);
    private int mVideoWidth = -1, mVideoHeight = -1;
    private ByteBuffer y;
    private ByteBuffer u;
    private ByteBuffer v;
    private byte[] ybytebuf;
    private byte[] ubytebuf;
    private byte[] vbytebuf;

    private int yarraySize;
    private int uvarraySize;

    public GLFrameRenderer(GLSurfaceView surface) {
        mTargetSurface = surface;
    }

    public interface SurfaceCreateListener{
        void onSurfaceCreated();
        void onSurfaceChanged(int width, int height);
    }
    private SurfaceCreateListener surfaceCreateListener;
    public void setSurfaceCreateListener(SurfaceCreateListener listener){
        surfaceCreateListener=listener;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
       Log.e(Tag, "GLFrameRenderer :: onSurfaceCreated");
        if (!prog.isProgramBuilt()) {
            prog.buildProgram();
            Log.e(Tag, "GLFrameRenderer :: buildProgram done");
        }
        if(surfaceCreateListener !=null){
            surfaceCreateListener.onSurfaceCreated();
        }
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Log.e(Tag, "GLFrameRenderer :: onSurfaceChanged width:" + width + " " +
                "height:" + height);
        GLES20.glViewport(0, 0, width, height);
        mVideoWidth=-1;
        mVideoHeight=-1;
        if(surfaceCreateListener !=null){
            surfaceCreateListener.onSurfaceChanged(width, height);
        }
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        synchronized (this) {
            if (y != null && mVideoWidth != -1 && mVideoWidth != -1) {
                prog.buildTextures(y, u, v, mVideoWidth, mVideoHeight);
                GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
                prog.drawFrame();
            }
        }
    }

    public void update(int w, int h, byte[] buffer) {
        if (mVideoWidth != w || mVideoHeight != h) {
            Log.e(Tag,
                    "update para w:" + w + " h:" + h + " mVideoWidth:" + mVideoWidth + " " +
                            "mVideoHeight:" + mVideoHeight);
            mVideoWidth = w;
            mVideoHeight = h;
            yarraySize = mVideoWidth * mVideoHeight;
            uvarraySize = yarraySize / 4;
            inlinebuf_alloc();
            inlinebuf_wrap();
        }

        inlinebuf_copy(buffer);

        // request to render  
        mTargetSurface.requestRender();
    }

    //mod by wangsy 20150527
    // 采用byte实现System.arraycopy，比NIO的put效率高N倍
    private synchronized void inlinebuf_alloc() {
        ybytebuf = new byte[yarraySize];
        ubytebuf = new byte[uvarraySize];
        vbytebuf = new byte[uvarraySize];
    }

    private synchronized void inlinebuf_wrap() {
        y = ByteBuffer.wrap(ybytebuf, 0, yarraySize);
        u = ByteBuffer.wrap(ubytebuf, 0, uvarraySize);
        v = ByteBuffer.wrap(vbytebuf, 0, uvarraySize);
    }

    private synchronized void inlinebuf_copy(byte[] buffer) {
        System.arraycopy(buffer, 0, ybytebuf, 0, yarraySize);
        System.arraycopy(buffer, yarraySize, ubytebuf, 0, uvarraySize);
        System.arraycopy(buffer, yarraySize + uvarraySize, vbytebuf, 0, uvarraySize);
    }

}  
