# Java类初始化顺序

Java中的类属性初始化顺序规则是：

1、静态代码块或者属性->普通代码块或者属性->构造函数

再加上两条补充规则：

2、父类先与子类初始化

3、同是静态或者非静态的方法和属性的初始化顺序是按照它们定义的顺序来的

通过代码验证如下：

```java
public class Attr {
    public Attr(){
        System.out.println("attr construct.");
    }

    public Attr(String name){
        System.out.println(name + " attr construct.");
    }
}
```
```java
public class Super {
    {
        System.out.println("super normal block_1");
    }

    static {
        System.out.println("super static block_1.");
    }

    private Attr normalAttr1 = new Attr("superNormalAttr_1");

    public Super(){
        System.out.println("super construct.");
    }

    private static Attr staticAttr1 = new Attr("superStaticAttr_1");

    static {
        System.out.println("super static block_2.");
    }

    {
        System.out.println("super normal block_2.");
    }
}
```
```java
public class Extend extends Super {
    private Attr normalAttr1 = new Attr("extendNormalAttr_1");

    {
        System.out.println("extend normal block_1");
    }

    static {
        System.out.println("extend static block_1.");
    }


    public Extend(){
        System.out.println("extend construct.");
    }

    static {
        System.out.println("extend static block_2.");
    }

    {
        System.out.println("extend normal block_2.");
    }

    private static Attr staticAttr1 = new Attr("extendStaticAttr_1");

    public static void main(String[] args) {
        Extend extend = new Extend();
    }
}
```

测试结果如下:
```
super static block_1.
superStaticAttr_1 attr construct.
super static block_2.
extend static block_1.
extend static block_2.
extendStaticAttr_1 attr construct.
super normal block_1
superNormalAttr_1 attr construct.
super normal block_2.
super construct.
extendNormalAttr_1 attr construct.
extend normal block_1
extend normal block_2.
extend construct.

Process finished with exit code 0
```
通过上述结果，验证了初始化的规则，先父类再子类，先static属性者代码块，再普通属性或者代码块，最后才是构造函数；通过代码块和属性的结果也能看出来，属性和代码块的属性是按照代码顺序来的。