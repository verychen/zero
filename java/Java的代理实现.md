# Java代理实现
今天学习Java的代理实现，看看最基础的Java代理如何实现，主要分为三种方式：静态代理、动态代理，动态代理有多种实现方式，如jdk实现，cglib实现；
+ 静态代理实现

```java
package javabasis.proxy;

import jdk.nashorn.internal.objects.annotations.Setter;
import org.springframework.cglib.proxy.Enhancer;
import org.springframework.cglib.proxy.MethodInterceptor;
import org.springframework.cglib.proxy.MethodProxy;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

public class ProxyTest {
    public static void main(String[] args) {
        // 静态代理
        new StaticProxy(new Printer()).print();
    }

    interface Print{
        String print();
    }

    static class Printer implements Print {
        @Override
        public String print(){
            System.out.println("print");
            return "print";
        }
    }

    static class StaticProxy implements Print {

        private Print printer;

        public StaticProxy(Print print) {
            this.printer = print;
        }

        @Override
        public String print(){
            System.out.println("print before");
            printer.print();
            System.out.println("print after");
            return "print";
        }
    }
}
```
代理类和实现类实现同一个接口；
+ jdk动态代理

```java
package javabasis.proxy;

import jdk.nashorn.internal.objects.annotations.Setter;
import org.springframework.cglib.proxy.Enhancer;
import org.springframework.cglib.proxy.MethodInterceptor;
import org.springframework.cglib.proxy.MethodProxy;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

public class ProxyTest {
    public static void main(String[] args) {
        // JDK动态代理
        ((Print)JdkProxyFactory.getProxy(new Printer())).print();
    }

    interface Print{
        String print();
    }

    static class Printer implements Print {
        @Override
        public String print(){
            System.out.println("print");
            return "print";
        }
    }

    static class StaticProxy implements Print {

        private Print printer;

        public StaticProxy(Print print) {
            this.printer = print;
        }

        @Override
        public String print(){
            System.out.println("print before");
            printer.print();
            System.out.println("print after");
            return "print";
        }
    }

    static class PrintInvocationHandler implements InvocationHandler {
        private final Object target;

        public PrintInvocationHandler(Object target) {
            this.target = target;
        }

        @Override
        public Object invoke(Object proxy, Method method, Object[] args)
                throws Throwable {
            System.out.println("print before");
            Object res = method.invoke(target, args);
            System.out.println("print after");

            return res;
        }
    }

    static public class JdkProxyFactory {
        public static Object getProxy(Object target) {
            return Proxy.newProxyInstance(
                    target.getClass().getClassLoader(), // 目标类的类加载
                    target.getClass().getInterfaces(),  // 代理需要实现的接口，可指定多个
                    new PrintInvocationHandler(target)   // 代理对象对应的自定义 InvocationHandler
            );
        }
    }
}
```
不需要再生成代理类的class，JVM动态加载生成class；

+ CGLIB实现（spring）

```java
package javabasis.proxy;

import jdk.nashorn.internal.objects.annotations.Setter;
import org.springframework.cglib.proxy.Enhancer;
import org.springframework.cglib.proxy.MethodInterceptor;
import org.springframework.cglib.proxy.MethodProxy;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

public class ProxyTest {
    public static void main(String[] args) {

        // Cglib代理
        ((Printer2)CglibProxyFactory.getProxy(Printer2.class)).print();
    }

    static public class Printer2 {
        public String print(){
            System.out.println("print");
            return "print";
        }
    }

    static public class PrinterMethodInterceptor implements MethodInterceptor{
        @Override
        public Object intercept(Object var1, Method var2, Object[] var3, MethodProxy var4) throws Throwable{
            //调用方法之前，我们可以添加自己的操作
            System.out.println("before method " + var2.getName());
            Object object = var4.invokeSuper(var1, var3);
            //调用方法之后，我们同样可以添加自己的操作
            System.out.println("after method " + var2.getName());
            return object;
        }
    }

    public static class CglibProxyFactory {
        public static Object getProxy(Class<?> clazz) {
            // 创建动态代理增强类
            Enhancer enhancer = new Enhancer();
            // 设置类加载器
            enhancer.setClassLoader(clazz.getClassLoader());
            // 设置被代理类
            enhancer.setSuperclass(clazz);
            // 设置方法拦截器
            enhancer.setCallback(new PrinterMethodInterceptor());
            // 创建代理类
            return enhancer.create();
        }
    }
}
```
不需要实现接口，普通方法也可以使用代理

他们的执行结果如下:
```
print before
print
print after
```

被代理类的话需要时public属性的，不然没有办法访问，会有权限检查报错；主要是需要调用类的默认构造函数，需要时public权限，能够被访问到；
```
Exception in thread "main" org.springframework.cglib.core.CodeGenerationException: java.lang.IllegalAccessError-->class $javabasis.proxy.ProxyTest$Printer2$$EnhancerByCGLIB$$383abd1d cannot access its superclass javabasis.proxy.ProxyTest$Printer2
	at org.springframework.cglib.core.ReflectUtils.defineClass(ReflectUtils.java:526)
	at org.springframework.cglib.core.AbstractClassGenerator.generate(AbstractClassGenerator.java:359)
	at org.springframework.cglib.proxy.Enhancer.generate(Enhancer.java:582)
	at org.springframework.cglib.core.AbstractClassGenerator$ClassLoaderData$3.apply(AbstractClassGenerator.java:106)
	at org.springframework.cglib.core.AbstractClassGenerator$ClassLoaderData$3.apply(AbstractClassGenerator.java:104)
	at org.springframework.cglib.core.internal.LoadingCache$2.call(LoadingCache.java:54)
	at java.util.concurrent.FutureTask.run(FutureTask.java:266)
	at org.springframework.cglib.core.internal.LoadingCache.createEntry(LoadingCache.java:61)
	at org.springframework.cglib.core.internal.LoadingCache.get(LoadingCache.java:34)
	at org.springframework.cglib.core.AbstractClassGenerator$ClassLoaderData.get(AbstractClassGenerator.java:130)
```
代理也可以直接代理接口，看似多此一举，但是可以通过很多的代理类handler来附加很多处理，再方法拦截器PrinterMethodInterceptor中做处理
```java
public static void main(String[] args) {
        // Cglib代理
        ((Print)CglibProxyFactory.getProxy(Print.class)).print();
    }
```


spring aop和接口注解编程
结合代理和自动注入参见下面的例子
https://zhuanlan.zhihu.com/p/29348799



遗留问题
spring的cglib和cglib包的关系是啥?
https://github.com/cglib/cglib



