**集合学习参考资料**

[集合框架面试题](https://snailclimb.gitee.io/javaguide/#/docs/java/collection/Java%E9%9B%86%E5%90%88%E6%A1%86%E6%9E%B6%E5%B8%B8%E8%A7%81%E9%9D%A2%E8%AF%95%E9%A2%98?id=_111-java-%e9%9b%86%e5%90%88%e6%a6%82%e8%a7%88)

**集合知识整理**
![集合关系图](https://guide-blog-images.oss-cn-shenzhen.aliyuncs.com/source-code/dubbo/java-collection-hierarchy.png)
List(对付顺序的好帮手)： 存储的元素是有序的、可重复的。
Set(注重独一无二的性质): 存储的元素是无序的、不可重复的。
Map(用 Key 来搜索的专家): 使用键值对（kye-value）存储，类似于数学上的函数 y=f(x)，“x”代表 key，"y"代表 value，Key 是无序的、不可重复的，value 是无序的、可重复的，每个键最多映射到一个值。

先来看一下 Collection 接口下面的集合。

1.1.3.1. List
Arraylist： Object[]数组
Vector：Object[]数组
LinkedList： 双向链表(JDK1.6 之前为循环链表，JDK1.7 取消了循环)
1.1.3.2. Set
HashSet（无序，唯一）: 基于 HashMap 实现的，底层采用 HashMap 来保存元素
LinkedHashSet：LinkedHashSet 是 HashSet 的子类，并且其内部是通过 LinkedHashMap 来实现的。有点类似于我们之前说的 LinkedHashMap 其内部是基于 HashMap 实现一样，不过还是有一点点区别的
TreeSet（有序，唯一）： 红黑树(自平衡的排序二叉树)
再来看看 Map 接口下面的集合。

1.1.3.3. Map
HashMap： JDK1.8 之前 HashMap 由数组+链表组成的，数组是 HashMap 的主体，链表则是主要为了解决哈希冲突而存在的（“拉链法”解决冲突）。JDK1.8 以后在解决哈希冲突时有了较大的变化，当链表长度大于阈值（默认为 8）（将链表转换成红黑树前会判断，如果当前数组的长度小于 64，那么会选择先进行数组扩容，而不是转换为红黑树）时，将链表转化为红黑树，以减少搜索时间
LinkedHashMap： LinkedHashMap 继承自 HashMap，所以它的底层仍然是基于拉链式散列结构即由数组和链表或红黑树组成。另外，LinkedHashMap 在上面结构的基础上，增加了一条双向链表，使得上面的结构可以保持键值对的插入顺序。同时通过对链表进行相应的操作，实现了访问顺序相关逻辑。详细可以查看：《LinkedHashMap 源码详细分析（JDK1.8）》
Hashtable： 数组+链表组成的，数组是 HashMap 的主体，链表则是主要为了解决哈希冲突而存在的
TreeMap： 红黑树（自平衡的排序二叉树）

**HashMap的长度要保持是2的幂次方**
Hash值的范围值-2147483648 到 2147483647，需要将hashcode映射到数组上：
```java
final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
                   boolean evict) {
        Node<K,V>[] tab; Node<K,V> p; int n, i;
        if ((tab = table) == null || (n = tab.length) == 0)
            n = (tab = resize()).length;
        if ((p = tab[i = (n - 1) & hash]) == null)
            **tab[i] = newNode(hash, key, value, null);**
        	// ...
        }
```
从代码可以看出，是使用&运算来坐映射的；为了保证相同的hashcode映射到相同的位置，通常会想到使用%，但是$运算速度跟快，如果被除数是2的倍数的话，$运算和%运算的结果是一样的
15%4=00001111&011
同事顺便复习了下计算机的数字表示法，有原码、反码和补码，使用最高位标示正负，为了统一加减计算，都统一使用+代替，就出现了反码；但是反码解决不了正负0的问题，补码出现解决了这个问题，参考：https://www.cnblogs.com/starry-skys/p/11997091.html
```
减法替换为加法：1 - 1 = 1 + -1
原码：1 + -1 = 00000001 + 10000001 = 10000010 //结果非预期，产生了反码
补码：1 + -1 = 00000001 + 11111110 = 11111111 //结果为-0，那么产生了+0和-0的分析，0无正负之分，产生了补码
反码：1 + -1 = 00000001 + 11111111 = 00000000 // 补1，高位进1丢掉，就是正0
```

**ConcurrentHashMap的性能优化历程**
