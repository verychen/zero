#include <pthread.h>
#include <stdio.h>
#include "MyThread.h" // 导入刚刚编译的那个.h文件

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