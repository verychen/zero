#Object类

object类开始时，需要注册下native方法对应的库
```java
private static native void registerNatives();
    static {
        registerNatives();
    }
```
## getClass
能够获取此Object的运行时类，是利用反射机制完成的。java中类的生命周期，会经历加载、连接(验证、准备、解析)、初始化、使用、卸载五个步骤。加载过程中会经历一个流程java->calss-> 内存(生成Java.lang.Class文件)。生成的.Class文件将会在反射中使用。如果有一个实例那么就可以通过实例的getClass()方法获取该对象的类型类,如果你知道一个类型,那么你可以使用.class()的方法获得该类型的类型类。

public final native Class<?> getClass();

## hashcode
hashCode()方法返回当前对象运行时的hash码，是用于支持散列表数据结构，因为散列表在进行数据存储时依赖hash码决定数据存储的逻辑位置。在程序运行中，无论什么情况下，相同的对象对应的hash码一定是相同的。但是不同的对象有可能会返回相同的hash码。那么其实也代表如果两个对象的hash码不一致，这两个对象一定是不同的。
Object中的equals方法默认比较当前对象的引用，是直接判断this和obj本身的值是否相等，即用来判断调用equals的对象和形参obj所引用的对象是否是同一对象，所谓同一对象就是指内存中同一块存储单元，如果this和obj指向的是同一块内存对象，则返回true,如果this和obj指向的不是同一块内存，则返回false，注意：即便是内容完全相等的两块不同的内存对象，也返回false。
```java
public native int hashCode();
public boolean equals(Object obj) {
        return (this == obj);
    }
```

## clone
protected native Object clone() throws CloneNotSupportedException;

## toString
getName() 以String形式返回类对象的名称（包换包名）。Integer.toHexString(hashCode()) 以对象的哈希码为参数，以16进制无符号整数形式返回此哈希码的字符串表示形式。
```java
public String toString() {
        return getClass().getName() + "@" + Integer.toHexString(hashCode());
    }
```

## notify
多线程的通知方法
public final native void notify();
public final native void notifyAll();

## wait
public final native void wait(long timeout) throws InterruptedException;

## finalize
protected void finalize() throws Throwable { }

##参考
https://zhuanlan.zhihu.com/p/142902467https://zhuanlan.zhihu.com/p/142902467