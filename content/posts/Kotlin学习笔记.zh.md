---
title: Kotlin 学习笔记
date: 2022-01-11
extra:
  add_toc: true
taxonomies:
  tags:
    - kotlin
---

# 前言

终于有一天，被咖啡 ☕️ 折磨疯的泡泡开始学起了 Kt

只是 记录一些简单的知识点

# 正文

#### 关键字

就这关键字数量看得我头皮发麻

记录几个怪的

- reified

范型虽然可以代表所有有所有权的类型，但失去了一个具体类型具有的某些功能

例如类型判断(只针对 java 这种靠类型擦除技术实现的泛型)，如果没有 reified 修饰 T 和 inline 修饰函数 `p !is T` 会爆红，因为编译把 T 抹了会有语法错误

下面的代码会被在调用处被展平，T 会被替换成调用时传入的类型

```kotlin
inline fun <reified T TreeNode.findParentOfType(): T? {
    var p = parent
    while (p != null && p !is T) {
        p = p.parent
    }
    return p as T?
}
```

- actual
- expect

和多平台有关，由 expect 修饰一个函数签名，来定义一个不同平台上的函数
接着由 actual 修饰对应平台实现的代码

```kotlin
//Common
expect fun randomUUID(): String

//Android
import java.util.*
actual fun randomUUID() = UUID.randomUUID().toString()

//iOS
import platform.Foundation.NSUUID
actual fun randomUUID(): String = NSUUID().UUIDString()

//...
```

- tailrec

修饰一个尾递归的函数，编译时优化成迭代

- operator

操作符重载，还可以用于属性委托
修饰一个成员方法或者一个拓展函数

- typealias

字如其名，给类型起别称的

```kotlin
typealias NodeSet = Set<Network.Node>

typealias MyHandler = (Int, String, Any) - Unit

typealias Predicate<T = (T) - Boolean
```

- @xxx:AnnotationType
- @xxx:[AnnotationType1, AnnotationType2]

当 kotlin 的部分奇妙语法的注解转换到 java 上需要对注解进行合理注释

xxx 可以为

- file 在文件顶层使用，为整个文件的元素都加上这个注解
- property 使用这个会让这个注解对 Java 不可见
- field 在主构造器里使用，生成标注主构造器内参数的字段上的注解
- get 在属性或者主构造器内参数中使用，生成的注解在 getter 方法上
- set 同上
- receiver 在拓展函数处使用
- param 在构造参数前生成注解
- setparam 属性的 setter 参数前生成注解
- delegate 为委托属性存储其委托实例的字段


#### 函数最后一个参数是 lambda 表达式就可以放到括号外面

```kotlin
fun foo(input: String, call: (String) - Boolean) {
    println(call(input))
}

fun main() {
    foo("1") {
        str ->  when(str) { // 可以用 when(it) {
            "1" -> true
            else -> false
        }
    }
}

// 如果只有一个参数并且是lambda表达式的话，圆括号都能省去

var data = ...
data.run {
    println(data)
}
```

#### `if, when(类似switch)`等不仅仅是语句块，而是表达式，可以返回一个值给一个变量赋值

#### `for`循环不能经典三段式了，取而代之的是`for in`结构

```kotlin
// 简单
// 1..100 表示从 1 到 100 的范围，左闭右闭
for (i in 1..100) {
    println("Hi")
}

// 倒序 100 到 1
// downTo 是 Int 的拓展中缀函数，类似于运算符(运算等级最低) 等价 100.downTo(1)
for (i in 100 downTo 1) {
    println("Hello${i}")
}
```

#### 为空执行&不为空执行 ( That's pretty good 💗

```kotlin
var data = ...
// data 不为空执行，空检测
data?.run {
    println(data)
}

// data 为空执行
data?:run {
	println("NULL")
}
```

#### 常用 ADT

- ArrayList
  - 创建可以由 mutableListOf()/arrayListOf() 简化
  - listOf() 会创建一个不可修改的 List
- Array
  - 对应 java 中的 Type[]
- LinkedHashSet
  - 创建可以由 mutableSetOf()/linkedSetOf() 简化
  - setOf() 会创建一个不可修改的 Set
- LinkedHashMap
  - 创建可以由 mutableMapOf()/linkedMapOf() 简化
  - mapOf() 会创建一个不可修改的 Map
- HashMap/HashSet
  - 创建可以由 hashMapOf()/hashSetOf() 简化

除了 Set，其余的都可以用 [] 访问元素(推荐)

#### `let, with, run, apply, also` 使用

摘自 [https://blog.csdn.net/u013064109/article/details/78786646](https://blog.csdn.net/u013064109/article/details/78786646) (编程全靠 CSDN)

| 函数  | inline 结构                                                          | 函数体内使用的对象 | 返回值       | 是否是扩展函数 |
| ----- | -------------------------------------------------------------------- | ------------------ | ------------ | -------------- |
| let   | fun <T, R> T.let(block: (T) -> R): R = block(this)                   | it                 | 闭包形式返回 | 是             |
| with  | fun <T, R> with(receiver: T, block: T.() -> R): R = receiver.block() | this               | 闭包形式返回 | 否             |
| run   | fun <T, R> T.run(block: T.() -> R): R = block()                      | this               | 闭包形式返回 | 是             |
| apply | fun T.apply(block: T.() -> Unit): T { block(); return this }         | this               | 返回 this    | 是             |
| also  | fun T.also(block: (T) -> Unit): T { block(this); return this }       | it                 | 返回 this    | 是             |

#### Kotlin 中的类

- `Class` 普通类
- `Abstract Class` 抽象类
- `Inner Class` 内部类 因为持有外部类的实例才能访问内部类，所以内部类里可以访问外部类实例的属性
- `Nested Class` 嵌套类 和 Java 的 static class 一样，嵌套类里不能访问外部类的实例属性和方法
- `Sealed Class` 密封类 表示一种受限的层次结构，如`Animal`，可以向下细分`Dog`之类的
- `Enum Class` 枚举类
- `Data Class` 数据类 会自动生成字段的 getter 和 setter，重写 hashCode, equals, toString 方法
- `Interface` 接口类

- `Object` 单例对象(非类，只有一个实例)

```kotlin
sealed class Animal
data class Dog(val age: Int) : Animal()
data class Cat(val age: Int) : Animal()
object NotAnimal : Animal()

// 使用 sealed class 后， when判断类型不用写else
fun getAge(animal: Animal): Int = when(animal) {
    is Dog -> animal.age
    is Cat -> animal.age
    NotAnimal -> Int.NaN
}
```

#### Kotlin 中的构造函数

分为：

- 主构造函数
- 次构造函数

主构造函数声明在类名称的后面

```kotlin
fun main() {
    val s = Student()
    println(s.name)
}

@Suppress("UNUSED")
class Student(sno: String, grade: Int) : Person("田所浩二") {

    init {
        if (sno == "114514" || grade == 1919810) {
            println("啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊")
        }
    }

    constructor() : this("114514", 1919810)
}

open class Person(var name: String)
```

次构造函数由 constructor() 命名，必须直接或间接调用主构造函数

```kotlin
@Suppress("UNUSED")
class Student(sno: String, grade: Int) : Person("田所浩二") {

    init {
        if (sno == "114514" || grade == 1919810) {
            println("啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊啊")
        }
    }

    constructor(sno: String) : this(sno, 1919810)

    constructor() : this("114514")

}
```

无主构造函数的特殊情况

```kotlin
@Suppress("UNUSED")
class Student : Person {
    private val sno : String

    constructor(name: String, sno: String) : super(name) {
        this.sno = sno
    }

    constructor(name: String) : super(name) {
        this.sno = "null"
    }
}

open class Person(var name: String)
```

#### 可见性修饰符

- private (只有当前类可用)
- public (默认不写就是 public)
- protected (当前类，子类可用) [Java 中格外加了同一包下可用]
- internal (当前模块下可以用)

#### 懒加载

和 OC 差不多，不过实现起来比 OC 简单

```kotlin
val lazyValue: String by lazy {
    println("computed!")
    "Hello"
}

fun main(args: Array<String>) {
    println(lazyValue)
    println(lazyValue)
}

// 打印结果
// computed！
// Hello

// Hello
```

#### 协程

> 待填坑

#### inline, noinline, crossinline 介绍

带有 inline(`内联`)的函数编译后会被展开放到调用处，连同函数的 lambda 参数也会被展开

```kotlin
fun main() {
    testFunc1("hi")
}

// 这个 inline 没有意义，甚至会反优化到处展开导致字节码膨胀
inline fun testFunc1(str: String) {
    println(str)
}

// 编译后大概会变成这样
fun main() {
    println("hi")
}
```

```kotlin
fun main() {
    testFunc1("hello") {
        println(" world!")
    }
}

inline fun testFunc1(str: String, lam : () -> Unit) {
    print(str)
    lam()
}

// 大概编译后会变成这样
// 这样kotlin就不会创建函数对象包裹lambda表达式了，这个函数就可以放到高频率的地方使用了
fun main() {
    print("hello")
    println(" world!")
}
```

noinline 就是阻止`内联`，放到函数参数，这个函数参数就不会被展开

防止返回这个lambda对象时被展开了它就没了，那我返回个嘚

```kotlin

fun main() {
    val repeat = testFunc1("paopao", {
        print("hello! ")
    }, {
        println(" bye!")
    })
    repeat()
}

inline fun testFunc1(str: String, lam1 : () -> Unit, noinline lam2 : () -> Unit): () -> Unit {
    lam1()
    print(str)
    lam2()
    return lam2
}

// 大概这样
fun main() {
    print("hello! ")
    print("paopao")
    val repeat = {
        println(" bye!")
    }
    repeat()
}

```

以上解释了为什么 kt 里的不是内联函数的 `lambda` 参数不允许使用 `return` ( `return@xxx` 的除外)

带有 inline 的高阶函数会把函数参数(lambda)展开，放到调用处，那这个 return 就直接把调用的函数结束了...

不带 inline 的高阶函数则不会，那我写一个 lambda 还要考虑 return 到底结束哪个函数，kt 索性就: 你不是内联函数的 lambda 参数，return 就别用了，使用最后一行表达式的值作为 lambda 返回值得了

所以，**内联函数的 lambda 参数里的 return 结束的是外面的函数**

同样又有一个问题，如果内联函数这个 lambda 的执行和这个内联函数差了几个`栈帧`怎么办?

```kotlin
// 例如这样
inline fun testFunc1(lam1 : () -> Unit, noinline lam2 : () -> Unit): () -> Unit {
    {
        lam1() // 这个lambda会在另一个栈帧内执行
    }()
    lam2()
    return lam2
}

fun main() {
	testFunc1({
        println("hi")
        return
    }, {
        println("I am ok")
    })
}
```

lam1 里的 return 到底结束了谁???? 不是说好了结束 main 的吗?

其实这样会报错，编译不了，内联函数里 lambda 参数不允许间接调用

想用? 加 crossinline

```kotlin
inline fun testFunc1(crossinline lam1 : () -> Unit, noinline lam2 : () -> Unit): () -> Unit {
    {
        lam1() // 这个lambda会在另一个栈帧内执行
    }()
    lam2()
    return lam2
}

fun main() {
	testFunc1({
        println("hi")
        //return
    }, {
        println("I am ok")
    })
}
```

与此同时，**lam1 内将不能使用 return**

#### infix 中缀表示法

说实在的，我不懂它存在的意义... 🍬 多了会吃腻

- 必须作用在一个对象内，成员函数(方法)，或者作为一个类的拓展函数
- 参数只能有一个
- 参数不能是可变参数，没有默认值

```kotlin
// 然后就可以像用 + - * / 一样用这个方法

infix fun Int.add(x: Int): Int {
	return 1 + 2
}

// 用中缀表示法调用该函数
1 add 2

// 等同于这样
1.add(2)
```

#### 属性委托

#### 协变和逆变

> 待填坑
