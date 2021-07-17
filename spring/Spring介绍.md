# `Spring`介绍
`Spring`是一个框架，可以让java开发人员更方便的开发J2EE产品
## `Spring`的特点
### 框架特征
+ 轻量
+ 控制反转IOC技术减少依赖，低耦合
+ 面向切面，隔离了日志和事务等对业务代码的侵入
+ 容器，管理应用对象配置和生命周期
+ 框架，提供了很多基础功能，还可以方便的扩展
+ MVC，综合解决方案，分层分级

### 特性
+ IOC控制反转和依赖注入DI，低耦合
+ AOP编程，面向切面编程
+ 事务管理，声明式
+ 测试支持，JUNIT
+ 方便集成各种框架
+ 封装了简单易用的Java EE API
+ Java源码设计巧妙，是学习典范

## `Spring Framework`基础介绍
* `Spring Core`<br>
spring_core.jar，这个jar文件包含Spring框架基本的核心工具类，Spring其它组件要都要使用到这个包里的类，
是其它组件的基本核心，当然你也可以在自己的应用系统中使用这些工具类。
* `Spring beans`<br>
spring-beans.jar,这个jar文件是所有应用都要用到的，它包含访问配置文件、创建和管理bean以及进行Inversion of Control / Dependency Injection（IoC/DI）
操作相关的所有类。如果应用只需基本的IoC/DI支持，引入spring-core.jar及spring- beans.jar文件就可以了。
* `Spring AOP`<br>
spring-aop.jar，这个jar文件包含在应用中使用Spring的 AOP特性时所需的类。使用基于AOP的Spring特性，如声明型事务管理（Declarative Transaction Management），也要在应用里包含这个jar包。
* `Spring Context`<br>
spring-context.jar，这个jar文件为Spring核心提供了大量扩展。可以找到使用Spring ApplicationContext特性时所需的全部类，JDNI所需的全部类，UI方面的用来与模板（Templating）引擎如 Velocity、FreeMarker、JasperReports集成的类，以及校验Validation方面的相关类。
* `Spring Web`<br>
Spring web相关
* `Spring Web MVC`<br>
Spring MVC框架围绕DispatcherServlet这个核心展开，它负责截获请求并将其分派给相应的处理器处理。
Spring MVC框架包括注解驱动控制器、请求及响应的信息处理、视图解析、本地化解析、上传文件解析、异常处理以及表单标签绑定等内容。
* `Spring ORM`<br>
ORM的全称是Object Relational Mapping，即对象关系映射。Spring集成来进行对象和数据库映射用的。
* `Spring DAO`<br>
Spring对多个持久化技术提供了集成支持，包括 Hibernate、 MyBatis、JPA、JDO；<br>
Spring提供一个简化JDBC API操作的Spring JDBC框架。<br>
Spring面向DAO制定了一个通用的异常体系，屏蔽具体持久化技术的异常，使业务层和具体的持久化技术实现解耦。<br>
Spring提供了模板类简化各种持久化技术的使用。<br>

**参考**

![`Spring`体系框架](https://upload-images.jianshu.io/upload_images/24808978-ed59988f54c0041f.png)<br>
![`Spring`依赖关系](https://upload-images.jianshu.io/upload_images/24808978-f4acda7230a2661c.png?imageMogr2/auto-orient/strip|imageView2/2/format/webp)
![`Spring``spring`依赖关系2](https://images2018.cnblogs.com/blog/1030141/201806/1030141-20180626204427608-1945320769.png)

## `Spring`家族
基于spring，发展起来的一套整体的开发框架；

### Spring Framework
Spring Framework 是一个 Java/Java EE/.NET 的分层应用程序框架。该框架基于 Expert One-on-One Java EE Design and Development（ISBN 0-7645-4385-7）一文中的代码，并最初由 Rod Johnson 和 Juergen Hoeller et al 开发。主要包括的内容有：<br>
* core: dependency injection, events, resources, i18n, validation, data binding, type conversion, SpEL, AOP.
* Testing: mock objects, TestContext framework, Spring MVC Test, WebTestClient.
* Data Access: transactions, DAO support, JDBC, ORM, Marshalling XML.
* Spring MVC and Spring WebFlux web frameworks.
* Integration: remoting, JMS, JCA, JMX, email, tasks, scheduling, cache.
* Languages: Kotlin, Groovy, dynamic languages.

### Spring Boot
Spring Boot的哲学就是约定大于配置。既然很多东西都是一样的，为什么还要去配置。提供引导配置，能够直接启动。
+ 自动配置 Spring-boot-starter 开箱即用依赖模块
+ 简化统一配置文件
+ 内嵌了如Tomcat，Jetty,所有的依赖都打到一个jar包里面，可以直接java -jar 运行
+ 监控管理actuator

**参考**
![Spring Boot架构](http://201905.oss-cn-hangzhou.aliyuncs.com/cs/5606289-c079db724a68169fd6486e6e1addc35d.png)

### Spring Cloud
Spring Cloud基于Spring Boot，为微服务体系开发中的架构问题，提供了一整套的解决方案——服务注册与发现，服务消费，服务保护与熔断，网关，分布式调用追踪，分布式配置管理等。
+ Spring Cloud Eureka<br>
Eureka负责服务的注册于发现，构成Eureka体系的包括：服务注册中心、服务提供者、服务消费者。参考Zookeeper
+ Spring Cloud Ribbon<br>
Spring Cloud Feign 是一个声明web服务客户端，这使得编写Web服务客户端更容易，使用Feign 创建一个接口并对它进行注解，
它具有可插拔的注解支持包括Feign注解与JAX-RS注解，Feign还支持可插拔的编码器与解码器，Spring Cloud 增加了对 Spring MVC的注解，
Spring Web 默认使用了HttpMessageConverters, Spring Cloud 集成 Ribbon 和 Eureka 提供的负载均衡的HTTP客户端 Feign。
+ Spring Cloud Hystrix<br>
断路器，断路器本身是一种开关装置，用于我们家庭的电路保护，防止电流的过载，当线路中有电器发生短路的时候，断路器能够及时切换故障的电器，防止发生过载、发热甚至起火等严重后果。
+ Spring Cloud Config<br>
Config Server用于配置属性的存储，存储的位置可以为Git仓库、SVN仓库、本地文件等，Config Client用于服务属性的读取。
+ Spring Cloud Zuul<br>
路由网关
+ Spring Cloud Bus<br>
消息总线

**参考**
![Spring Cloud组件结构](https://pic2.zhimg.com/80/v2-18320cf1b34bc79f4406534beb6ba2e8_720w.jpg?source=1940ef5c)