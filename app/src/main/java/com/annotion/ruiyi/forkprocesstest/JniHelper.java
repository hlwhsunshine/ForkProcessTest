package com.annotion.ruiyi.forkprocesstest;

/**
 * Function:
 * Project:ForkProcessTest
 * Date:2019/2/20
 * Created by xiaojun .
 */

public class JniHelper {
    static {
        System.loadLibrary("testprocess");
    }
    public static native void forkProcess();
    public static native void testJar(String path1, String path2, String className, String methodName);

}
