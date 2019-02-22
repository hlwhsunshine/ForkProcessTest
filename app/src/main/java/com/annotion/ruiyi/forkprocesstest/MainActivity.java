package com.annotion.ruiyi.forkprocesstest;

import android.content.Context;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import java.io.File;
import java.lang.reflect.Method;

import dalvik.system.BaseDexClassLoader;
import dalvik.system.DexClassLoader;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.bt).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                JniHelper.forkProcess();



                //File jarPath = new File("data/JarTest.jar");

                //DexClassLoader loader = new DexClassLoader(jarPath.getAbsolutePath(),getDir("dex1",0).getAbsolutePath(),null,getClassLoader());


                //try {
               //     Class clz = loader.loadClass("com.annotion.ruiyi.testjar.JarTest");
                //    Method method = clz.getMethod("test", new Class[0]);
                //    method.invoke(null,new Object[0]);
                //} catch (Exception e) {
                //    e.printStackTrace();
                //}

            }

        });
        findViewById(R.id.bt2).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //aa();
                JniHelper.testJar("system/framework/JarTest.jar","","com.annotion.ruiyi.testjar.JarTest","test");
            }
        });


    }

    public void aa(){
        File dir =getDir("dex", 0);
        Class loadClass = null;
        try {
            loadClass = new DexClassLoader("/data/root.jar", dir.getAbsolutePath(), null, getClassLoader()).loadClass("com.kingo.root.lib.FastRootImpl");
            Object newInstance = loadClass.getConstructor(new Class[]{Context.class}).newInstance(new Object[]{this});
            Log.e("test","----------------"+(Boolean) loadClass.getMethod("copyRoot", new Class[0]).invoke(newInstance, new Object[0]));
        } catch (Exception e) {
            e.printStackTrace();
        }



    }
}
