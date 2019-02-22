#include<stdio.h>
#include<jni.h>
#include<stdlib.h>
#include<android/log.h>



#include<sys/types.h>
#include<fcntl.h>
#include<limits.h>


#include <string>


#include <unistd.h> //execl启动进程相关
#include <sys/wait.h>  //fork子进程相关
#include "com_annotion_ruiyi_forkprocesstest_JniHelper.h"

#define TAG    "TestJniLog" // 这个是自定义的LOG的标识
#define LOGD(...)  __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__) // 定义LOGD类型
#define BUFSZ PIPE_BUF
 void start(void);
 const int N = 300;


/*C字符串转JNI字符串*/
jstring stoJstring(JNIEnv* env, const char* pat) {
        jclass strClass = env->FindClass("Ljava/lang/String;");
        jmethodID ctorID = env->GetMethodID(strClass, "<init>",
                "([BLjava/lang/String;)V");
        jbyteArray bytes = env->NewByteArray( strlen(pat));
        env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
        jstring encoding = env->NewStringUTF( "utf-8");
        return (jstring)env->NewObject( strClass, ctorID, bytes,
                encoding);
}
/*JNI字符串转C字符串*/
char* jstringTostring(JNIEnv* env, jstring jstr) {
        char* rtn = NULL;
        jclass clsstring = env->FindClass( "java/lang/String");
        jstring strencode = env->NewStringUTF( "utf-8");
        jmethodID mid = env->GetMethodID( clsstring, "getBytes",
                "(Ljava/lang/String;)[B");
        jbyteArray barr= (jbyteArray)env->CallObjectMethod( jstr, mid,
                strencode);
        jsize alen = env->GetArrayLength( barr);
        jbyte* ba = env->GetByteArrayElements( barr, JNI_FALSE);
        if (alen> 0) {
                rtn = (char*)malloc(alen + 1);
                memcpy(rtn, ba, alen);
                rtn[alen] = 0;
        }
        env->ReleaseByteArrayElements(barr, ba, 0);
        return rtn;
}


JNIEXPORT void Java_com_annotion_ruiyi_forkprocesstest_JniHelper_forkProcess
  (JNIEnv * env, jclass jo){
    LOGD("A进程的 pid=%d", getpid());
      LOGD("A进程的 ppid=%d", getppid());
      LOGD("A进程的 uid=%d", getuid());

    int pid = fork();//fork出一个字进程
    //int pid = -1;


    //execl("/system/bin/su", "su", "-c", "./data/process", (char *)0);//开启一个新的进程替换原来的进程

    LOGD("fork pid: %d",pid);
    if(pid < 0){
        //LOGD("first fork() error pid %d,so exit",pid);
        exit(0);
    }else if(pid != 0){
        //LOGD("first fork(): I'am father pid=%d", getpid());
    }else{
       // LOGD("first fork(): I'am child pid=%d", getpid());
       // //LOGD("first fork(): I'am child ppid=%d", getppid());
       // LOGD("first fork(): I'am child uid=%d", getuid());
        start();




    }
  }


  JNIEXPORT void JNICALL Java_com_annotion_ruiyi_forkprocesstest_JniHelper_testJar
    (JNIEnv * env,jclass cls,jstring path1, jstring path2, jstring className, jstring methodName){

            LOGD("进入调用jar方法");

             //找到ClassLoader类
        	 jclass classloaderClass =env->FindClass("java/lang/ClassLoader");

        	 //找到ClassLoader类中的静态方法getSystemClassLoader
        	 jmethodID getsysloaderMethod =env->GetStaticMethodID(classloaderClass, "getSystemClassLoader","()Ljava/lang/ClassLoader;");

        	  //调用ClassLoader中的getSystemClassLoader方法，返回ClassLoader对象
        	  jobject loader =env->CallStaticObjectMethod(classloaderClass,getsysloaderMethod);


        	  //jar包存放位置
        	  jstring dexpath = path1;

        	  //优化后的jar包存放位置
        	  jstring dex_odex_path = path2;

        	  //找到DexClassLoader类
        	  jclass dexLoaderClass =env->FindClass("dalvik/system/DexClassLoader");

        	  //获取DexClassLoader的构造函数ID
        	  jmethodID initDexLoaderMethod =env->GetMethodID(dexLoaderClass, "<init>","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V");

        	  //新建一个DexClassLoader对象
                LOGD("第二个路径%s",jstringTostring(env,dex_odex_path));
        	  jobject dexLoader =env->NewObject(dexLoaderClass,initDexLoaderMethod, dexpath, dex_odex_path, NULL, loader);
                if(NULL == dexLoader){
                    LOGD("dexLoader为空");
                }
                LOGD("新建一个DexClassLoader对象");
        	  //找到DexClassLoader中的方法findClass
        	  jmethodID findclassMethod =env->GetMethodID(dexLoaderClass,"loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

        	  //jmethodID findclassMethod =env->GetMethodID(dexLoaderClass,"findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
        	  LOGD("找到DexClassLoader中的方法findClass");

        	  //如果返回空,那就找DexClassLoader的loadClass方法
        	  //说明：老版本的SDK中DexClassLoader有findClass方法，新版本SDK中是loadClass方法
        	    if(NULL==findclassMethod)
        	    {

        	          LOGD("findclassMethod为空");
        	       //    findclassMethod =env->GetMethodID(dexLoaderClass,"loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
        	    }
        	    //存储需要调用的类
        	     LOGD("存储需要调用的类:%s",jstringTostring(env,className));
        	  jstring javaClassName = className;

        	  //调用DexClassLoader的loadClass方法，加载需要调用的类
        	  jclass javaClientClass=(jclass)env->CallObjectMethod(dexLoader,findclassMethod,javaClassName);
               if(NULL == javaClientClass){
                LOGD("javaClientClass为空");
               }
                LOGD("加载需要调用的类");
        	  //将jstring类型的方法名转换为utf8编码的字符串

        	  //const char* func =env->GetStringUTFChars(methodName, NULL);
        	    //获取加载的类中的方法
        	  //
        	  jmethodID inject_method =env->GetStaticMethodID(javaClientClass, "test", "()V");
        	  //调用加载的类中的静态方法
        	   LOGD("调用加载的类中的静态方法");

        	 env->CallStaticVoidMethod(javaClientClass,inject_method);

    }

  void start(void){
        char buf[BUFSZ];
        FILE *fp;
        char cmd[150] = "ps | grep ./data/process";
        //const char *sysCommand = cmd.data();
        if ((fp = popen(cmd, "r")) == NULL) {
            LOGD("cmd error");
            return;
        }
        if((fgets(buf,BUFSZ,fp))!= NULL )
        {
        int count = atoi(buf);
        LOGD("count:%d",count);
        pclose(fp);
        if((count - 1) == 0){
            LOGD("1./data/process 没有运行");
            execl("/system/bin/su", "su", "-c", "./data/process", (char *)0);
        }else{
            LOGD("./data/process 在运行");
        }
        }else{
        LOGD("2./data/process 没有运行");
        execl("/system/bin/su", "su", "-c", "./data/process", (char *)0);

        }




    }




