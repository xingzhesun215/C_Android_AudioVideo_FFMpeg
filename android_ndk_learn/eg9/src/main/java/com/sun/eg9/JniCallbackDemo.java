package com.sun.eg9;

import android.os.Build;
import android.util.Log;

import androidx.annotation.Keep;

public class JniCallbackDemo {
    private static final String TAG = "JniCallbackDemo";

    static {
        System.loadLibrary("jni_callback");
    }

    Listener listener;

    public void setListener(Listener listener) {
        this.listener = listener;
    }

    /**
     * 时间统计
     */
    private int timeCount;

    /**
     * 开始计时
     */
    public native void startTiming();

    /**
     * 停止计时
     */
    public native void stopTiming();

    /**
     * 输出开始计时后的每一秒
     */
    @Keep
    private void printTime() {
        Log.e(TAG, "timeCount = " + timeCount);
        timeCount++;
        if (listener != null) {
            listener.counter(timeCount);
        }
    }


    /**
     * Keep ：保证类名不被混淆
     */
    @Keep
    public static class JniHandler {

        /**
         * Keep ：保证方法名不被混淆
         */
        @Keep
        public static String getBuildVersion() {
            return Build.VERSION.RELEASE;
        }

        @Keep
        public long getRuntimeMemorySize() {
            return Runtime.getRuntime().freeMemory();
        }

        @Keep
        private void updateStatus(String msg) {
            if (msg.toLowerCase().contains("error")) {
                Log.e("JniHandler", "Native Err: " + msg);
            } else {
                Log.i("JniHandler", "Native Msg: " + msg);
            }
        }
    }

    interface Listener {
        void counter(int count);
    }
}