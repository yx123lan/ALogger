package com.zxtcode.alogger;

/**
 * Created by tarent on 2016/12/7.
 */

public final class ALogger {

    public static final int ERROR = 6;
    public static final int WARN = 5;
    public static final int INFO = 4;
    public static final int DEBUG = 3;
    public static final int VERBOSE = 2;

    private String mTag;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public ALogger(String tag) {
        mTag = tag;
    }

    public void e(String msg) {
        println(ERROR, mTag, msg);
    }

    public void w(String msg) {
        println(WARN, mTag, msg);
    }

    public void i(String msg) {
        println(INFO, mTag, msg);
    }

    public void d(String msg) {
        println(DEBUG, mTag, msg);
    }

    public void v(String msg) {
        println(VERBOSE, mTag, msg);
    }

    private native void println(int priority, String tag, String msg);

    public native void init(String path);
}
