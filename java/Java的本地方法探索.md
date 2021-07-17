# 本地方法探索

https://segmentfault.com/a/1190000022812099

https://gorden5566.com/post/1027.html

实战

本地方法是指java中调用的外部库，一般是使用c/c++编写的，JNI是Java Native Interface的缩写，Java本地接口(JNI)提供了将Java与C/C\+\+、汇编等本地代码集成的方案，该规范使得在 Java 虚拟机内运行的 Java 代码能够与其它编程语言互相操作，包括创建本地方法、更新 Java 对象、调用 Java 方法，引用 Java 类，捕捉和抛出异常等，也允许 Java 代码调用 C/C\+\+或汇编语言编写的程序和库。使用java与本地已编译的代码交互，通常会丧失平台可移植性。但是，有些情况下这样做是可以接受的，甚至是必须的。例如，使用一些旧的库，与硬件、操作系统进行交互，或者为了提高程序的性能。JNI标准至少要保证本地代码能工作在任何Java 虚拟机环境。

入门实战
准备测试代码
```java
public class Sample {
    static {
        // 装载库，保证JVM在启动的时候就会装载，这里的库指的是C程序生成的动态链接库
        // Linux下是.so文件，Windows下是.dll文件
        System.loadLibrary( "ThreadNative" );
    }

    public static void main(String[] args) {
        new MyThread(() -> System.out.println("Java run method...")).start();
    }
}
```

```java
class MyThread implements Runnable {
    private Runnable target;

    public MyThread(Runnable target) {
        this.target = target;
    }

    @Override
    public void run() {
        if (target != null) {
            target.run();
        }
    }

    public synchronized void start() {
        start0();
    }

    private native void start0();
}
```
使用javac命令编程为class文件

使用havah命令编译生成c的头文件
>native 方法在哪个类中就使用javah命令生成对应的头文件，运行 javah 命令需要在包路径外面，javah packageName.className。

使用c语言实现本地方法
```
#include <pthread.h>
#include <stdio.h>
#include "com_fantasy_MyThread.h" // 导入刚刚编译的那个.h文件

pthread_t pid;
void* thread_entity(void* arg)
{
    printf("new Thread!\n");
}

// 这个方法定义要参考.h文件中的方法
JNIEXPORT void JNICALL Java_com_fantasy_MyThread_start0
(JNIEnv *env, jobject obj){
    pthread_create(&pid, NULL, thread_entity, NULL);
    sleep(1);
    printf("main thread %lu, create thread %lu\n", pthread_self(), pid);

    //通过反射调用java中的方法
    //找class，使用 FindClass 方法，参数就是要调用的函数的类的完全限定名，但是需要把点换成/
    jclass cls = (*env)->FindClass(env, "com/fantasy/MyThread");
    //获取 run 方法
    jmethodID mid = (*env)->GetMethodID(env, cls, "run", "()V");
    // 调用方法
    (*env)->CallVoidMethod(env, obj, mid);
    printf("success to call run() method!\n");

}
```

thread.c 文件编译为一个动态链接库，命名规则为 libxxx.so，xxx要跟Java中 System.loadLibrary( "ThreadNative") 指定的字符串保持一致，也就是 ThreadNative，编译命令如下：
```
gcc -fPIC -I /opt/jdk1.8.0_161/include/ -I /opt/jdk1.8.0_161/include/linux -shared -o libThreadNative.so thread.c
```

接下来就可以运行java代码了，运行前，还需要将 .so 文件所在路径加入到path中，这样Java才能找到这个库文件，.so 文件的路径为"/root/libThreadNative.so"
```
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/root/
# java Sample
new Thread!
main thread 139986752292608, create thread 139986681366272
Java run method...
success to call run() method!
```