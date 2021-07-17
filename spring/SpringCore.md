# `Spring core technologies`
Spring的核心技术的包括四个，namespace:org.springframework，主要是：
> spring-core.jar包是spring的基础包，提供很多工具类供使用，依赖commons-logging包；主要包含下面的几个包：<br>
> * asm
> * cglib
> * core
> * lang
> * objenesis
> * util

> spring-context.jar包是spring的容器包，namespace:org.springframework，管理上下文容器<br>
> * cache
> * context
> * ejb
> * format
> * classloading
> * jmx
> * jndi
> * remoting
> * scheduling
> * scripting
> * stereotype
> * ui
> * validation

> spring-bean.jar包是spring的bean相关的包.<br>
> * bean

> spring-expression.jar表达式解析<br>
> * expression


## `Spring Bean & Context`
**IOC**<br>
IoC 也称为依赖项注入(DI)。在此过程中，对象仅通过构造函数参数，工厂方法的参数或在构造或从工厂方法返回后在对象实例上设置的属性来定义其依赖项(即，与它们一起使用的其他对象) 。然后，容器在创建 bean 时注入那些依赖项。此过程从根本上讲是通过使用类的直接构造或诸如服务定位器模式之类的控件来控制其依赖项的实例化或位置的 bean 本身的逆过程(因此称为 Control Inversion)。

### Bean依赖
* 依赖构造函数注入：有参构造函数注入使用constructor-arg标签name是要注入的构造函数的参数名,引用类型使用‘ref=’给对象赋值为在配置文件中注册过的的对象id，注入内容是普通数据类型，使用`value=`赋值，赋的值可以是任意值，可以是`${}`，还可以使用`index="0"`通过顺序指定值；
* setter方法注入：配置文件使用`property`标签配置注入内容`<property name="" ref=""></property>`；
* 注解注入：通过`@Component`、`@Service`、`@Controller`、`@Repository`、`@Bean`声明bean（使用在方法上），通过`component-scan`扫描生成bean，通过`@Autowired`类型注入，`@Resource`依赖id注入，`@Qualifier`结合`@Autowird`指定名字依赖注入，`@Value`注入普通属性，`@Scope`表示bean的作用范围；
* 懒加载：bean默认都是一开始就实现并且是单例的，如果想定制的话，就要使用lazy延迟实力化；通过xml的`lazy-init`配置，或者单实例注解`@Bean`增加`@Lazy`注解；

**参考**  
https://zhuanlan.zhihu.com/p/61744838 TODO 实现原理解析<br>
https://zhuanlan.zhihu.com/p/61337718<br>
https://zhuanlan.zhihu.com/p/129859832<br>

### Bean的作用域范围
Scope|	Description
---|---
singleton|(默认)将每个 Spring IoC 容器的单个 bean 定义范围限定为单个对象实例。
prototype|将单个 bean 定义的作用域限定为任意数量的对象实例。
request|将单个 bean 定义的范围限定为单个 HTTP 请求的生命周期。也就是说，每个 HTTP 请求都有一个在单个 bean 定义后面创建的 bean 实例。仅在可感知网络的 Spring ApplicationContext中有效。
session|将单个 bean 定义的范围限定为 HTTP Session的生命周期。仅在可感知网络的 Spring ApplicationContext上下文中有效。
application|将单个 bean 定义的范围限定为ServletContext的生命周期。仅在可感知网络的 Spring ApplicationContext上下文中有效。
websocket|将单个 bean 定义的范围限定为WebSocket的生命周期。仅在可感知网络的 Spring ApplicationContext上下文中有效。

### 定制bean属性
**Spring bean的生命周期**<br>
![Spring bean life line](https://pic3.zhimg.com/754a34e03cfaa40008de8e2b9c1b815c_r.jpg?source=1940ef5c)

**Bean实例的定制点**<br>
>`Lifeccycle Callbacks`
>* InitializingBean接口：Spring将调用它们的afterPropertiesSet方法，作用与在配置文件中对Bean使用init-method声明初始化的作用一样，都是在Bean的全部属性设置成功后执行的初始化方法。
>* DispostbleBean接口：Spring将调用它的destory方法，作用与在配置文件中对Bean使用destory-method属性的作用一样，都是在Bean实例销毁前执行的方法。
>* 默认的`init()`方法和`destroy()`方法；
>* 使用xml的`init-method="init"`或者注解的@PostConstruct和@PreDestroy指定方法；
>* 同时存在时，顺序为：@PostConstruct、@PreDestroy注解的方法->InitializingBean、DisposableBean接口方法->init()、destroy()方法；
>* 上面的定制点都是在容器启动后的定制，如果有容器启动停止过程中需求，需要使用Lifecycle、LifecycleProcessor、SmartLifecycle接口，可以指定要执行的动作，以及定制bean的启动停止顺序；

>`ApplicationContextAware`和`BeanNameAware`
* BeanNameAware接口：获取bean id接口，bean类实现该接口；
* BeanFactoryAware接口：获取bean factory；
* ApplicationContextAware接口：获取bean的context；
* spring提供了的扩展点，具体可以参考：https://docs.spring.io/spring-framework/docs/current/reference/html/core.html#aware-list

### Bean定义继承
子类Bean可以继承父类Bean的属性和方法，有必要的话重载一些方法。

### 容器扩展点
spring提供了容器层面的扩展点，可以对容器的所有bean起作用；
* BeanPostProcessor：接口定义了回调方法，您可以实现这些回调方法，以提供自己的(或覆盖容器的默认值)实例化逻辑，依赖项解析逻辑等。如果您想在 Spring 容器完成实例化，配置和初始化 bean 之后实现一些自定义逻辑，则可以插入一个或多个BeanPostProcessor实现。您可以配置多个BeanPostProcessor实例，并且可以通过设置order属性来控制这些BeanPostProcessor实例的执行 Sequences。仅当BeanPostProcessor实现Ordered接口时，才可以设置此属性。如果您编写自己的BeanPostProcessor，则也应该考虑实现Ordered接口。有关更多详细信息，请参见BeanPostProcessor和Ordered接口的 javadoc。另请参见BeanPostProcessor 实例的编程注册上的 注解。
* BeanFactoryPostProcessor：bean工厂的回调方法；
* 使用接口FactoryBean实现初始化定制；
* 
### 基于注解的容器配置
* @Required：检查bean必须存在；
* @Autowired：自动根据类型注入bean；
* @Primary：指定bean作为首选实例；
* @Resource：自动根据名称注入bean；
* @Value：注入表达式属性；
* @PostConstruct @PreDestroy：定制初始化和销毁方法；

### 路径扫描和容器管理
通过`@Component`、`@Service`、`@Controller`、`@Repository`、`@Bean`声明bean（使用在方法上），通过`component-scan`扫描生成bean；

### JSR 330标准注解

### 基于Java的容器配置
* @Bean
* @Configuration
* @PropertySource

### 容器的环境抽象

### `LoadTimeWeaver `

### `ApplicationContext`的其他能力

### 参考
https://zhuanlan.zhihu.com/p/74832770<br>
https://zhuanlan.zhihu.com/p/129859832<br>
https://www.zhihu.com/question/48427693/answer/723146648<br>
https://zhuanlan.zhihu.com/p/129859832<br>
https://www.zhihu.com/question/38597960/answer/77600561<br>

## Resources

## Validation,Data Binding,and Type Conversion

## spEL

## Spring AOP
AOP有多种实现方式，Aspectj和Spring AOP；
### Spring AOP
### APIs
* Pointcut
* Advice
* Advisor
* ProxyFactoryBean
* Concise Proxy
* ProxyFactory
* Manipulating Advised Objects
* auto-proxy
* TargetSource
* Define new Advice Types

<https://www.jianshu.com/p/872d3dbdc2ca><br>
<https://blog.csdn.net/a128953ad/article/details/50509437>

## Null-safety

## Data Buffers and Codecs

## Loggging

## 参考
[spring是什么](https://baike.baidu.com/item/Spring/85061)<br>
<https://blog.csdn.net/ivan820819/article/details/79744797><br>
<https://www.jianshu.com/p/1c07f909cdc1><br>
<https://www.cnblogs.com/yoci/p/10642523.html><br>
<https://www.chkui.com/article/spring/spring_core_context_and_ioc><br>
<https://blog.csdn.net/szwandcj/article/details/507629><br>