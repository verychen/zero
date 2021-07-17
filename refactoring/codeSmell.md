# 代码的坏味道
## 重复代码(Duplicated Code)
## 过长函数(Long Method)
## 过大的类(Large Class)
## 过长参数列(Long Parameter List)
## 发散式变化(Divergent Change)
一个类受多种变化的影响
## 霰弹式修改(Shotgun Surgery)
一种变化引发多个类相应修改
##  依恋情结(Feature Envy)
函数对某个类的兴趣高过自己所处类的兴趣
## 数据泥团(Data Clumps)
相同的若干项数据出现在不同地方，这些绑在一起出现的数据应该有属于它们自己的对象
## 基本类型偏执(Private Obsession)
很多人不愿意在小任务上运用小对象
##  switch惊悚现身(Switch Statements)
switch语句会在很多地方重复出现，一改则需全改
## 平行继承体系(Parallel Inheritance Hierarchies)
当你为某一个类增加子类时，也必须为另一个类相应增加一个类
##  冗赘类(Lazy Class)
如果一个类不值得存在，那就让它消失
## 夸夸其谈的未来星(Speculative Generality)
预留的无用的抽象类，无用的抽象参数
##  令人迷惑的暂时字段(Temporary Field)
类中某个字段只为某些特殊情况而设置
## 过度耦合的消息链(Message Chains)
用户向一个对象请求另一个对象，然后再向后者请求另一个对象......
## 中间人(Middle Man)
无用的委托，过多的中间层
## 狎昵关系(Inappropriate Intimacy)
两个类过于亲密，一个类过于关注另一个类的成员
##  异曲同工的类(Alternative Classes with DifferentInterfaces)
不同名字的类或函数，作者相同的事
## 不完美的库类(Incomplete Library Class)
类库设计不可能完美
## 纯数据类(Data Class)
一个类拥有一些字段以及用于访问这些字段的函数，除此之外一无长物
## 被拒绝的遗赠(Refused Bequest)
子类不想继承超类所有的函数和数据，只想挑几样来玩
##  过多的注释(Comments)