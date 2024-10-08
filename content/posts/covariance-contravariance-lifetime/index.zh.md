---
title: 协变逆变与生命周期
date: 2023-02-17
extra:
  add_toc: true
taxonomies:
  categories:
    - 编程
  tags:
    - Rust
    - 生命周期
---

## 协变逆变与不变

看个例子：

今天泡泡准备拿一个面向对象的语言来写程序，那么就决定是你了 `Typescript`！(事实上泡泡并不会太写 `Typescript`，只是泡泡讨厌用 `Java` 而已)。

于是泡泡麻溜地写了三个类 `Animal`，`Dog`，`Cat`。根据泡泡的常识，`Dog` 和 `Cat` 都是继承 `Animal` 的，一些看起来都非常美好！

泡泡造了两个函数，它们类似这样：

```typescript
const feed: (dog: Dog) => Dog = function (dog) {
  console.log("泡泡外出打猎中")
  console.log("泡泡回家了")
  console.log("泡泡正在喂：", dog)
  return dog;
}

function care(method: (animal: Animal) => Animal);  // 照顾动物的方法？
```

泡泡还没想好如何实现 `care` 函数，但有一个待解决的问题：`feed` 能放进 `care` 函数中么？按照常理来说， `Dog` 是 `Animal` 的子类型，这样转换是没有问题的，这叫**协变** (covariant) 。

再想想！事实上，`care` 函数可不管你的 `method` 接收具体是什么动物，两天后泡泡忘了这茬事，可能就会拿一只 `Cat` 扔给 `method`！这时候就悲剧了，一只小猫会被喂食(`feed`)成一只小狗？即使编译器不疯，人也会疯掉！**这样的协变是不安全的。**

所以 `care` 签名中，`method` 接收的类型得是 `Dog` ！

```typescript
function careDog(method: (dog: Dog) => Dog);  // 这样就不会有问题了！

function careCat(method: (cat: Cat) => Cat);  // 用这个来照顾猫猫
```

两天后，泡泡又写了一个类 `Puppy`，它继承 `Dog`。这时泡泡想了想，一只成年的狗狗(`Dog`)应该具备自己照顾自己的能力！不能老惯着它！于是把 `careDog` 函数签名给改了：

```typescript
function carePuppy(method: (puppy: Puppy) => Puppy);
```

但是不管是大狗还是小狗，都得靠泡泡外出打猎才能生存，所以 `feed` 的函数签名是不需要变的。

那么这时候 `feed` 还能放进 `carePuppy` 函数里么？因为 `carePuppy` 是永远不会把一只猫猫丢进 `method` 中的，泡泡不用担心喂错了动物。这叫**逆变**(contravariant)，虽然有点反直觉：参数中的 `Dog`  能转换成 `Puppy`，但这是安全的。

但看了看 `method` 的返回，是 `Puppy`？可是 `feed` 返回的是 `Dog` 啊？虽然大多数情况下，喂一只小狗并把它返回，它还是一只小狗，但总有一天它会被喂成大狗！这时候可不能再把它当成小狗了！这时候，返回中的 `Dog` 转换成 `Puppy` 这种逆变是不安全的。

所以最终 `carePuppy` 得改成这样才是安全的：

```typescript
function carePuppy(method: (puppy: Puppy) => Dog);
```

而对于**不变**(invariant)，这很直观，既不是协变也不是逆变，也就是说 `care` 中的 `method` 只能和 `feed` 声明是一模一样的才可以。

### Tips: Typescript中的变化

在 `Typescript` 中，上面例子中的逆变和协变都是符合语法的，即使它可能不够安全。

<img src="ts.png" style="zoom: 33%;" />

## 泛型哪去了？

上面的例子里好像没有泛型哎？很多时候，协变逆变都和泛型有着千丝万缕的关系。它们其实可以联系起来：

函数的类型可以这样表示：`A -> B`，输入 A 输出 B

而一个带泛型的类型 `A<T>`，它的类型可以这样表示：`T -> A`，知道了 `T`，就可以得到 `A` 的具体类型

于是它们就可以联系起来了！

对于使用协变的 `List<? extends Animal>`，要注意在确定这个 `List` 里究竟装了什么的前提下不要尝试去改动这个 `List`，不然会不安全。

对于使用逆变的 `List<? super Dog>`，不要期望这个 `List` 里拿出来的东西总是 `Dog`。

要避免踩坑的话，最好还是遵循前人总结的规律：`PECS` (*Producer `extends` and Consumer `super`*)，生产者使用协变，这样生产出来的东西随时转换成 `extends` 后的上限就可以使用，外面也无法往集合中添加新元素（限制 `extends` 一个抽象类，外面无法实例化一个对象写入）；消费者使用逆变，不用在乎 `List ` 里到底是什么类型，能确保的是不会把错误的类型写入（实际上几乎不用逆变，因为反直觉，直接用 `List<Object>` 就好了）。

都想要？那就老老实实的写 `List<T>`，别整那些幺蛾子！

## Rust，你怎么看？

Rust 中普通类型（没有引用）没有继承（`trait` 可以有类似的关系，但 `trait` 不是类型），直接一拳干碎了类型协变和逆变的问题。

事实上，虽然 Rust 类型没有继承，但还有一种有类似继承的这种关系的东西：生命周期 (`lifetime`)

<img src="lifetime.png" style="zoom: 50%;" />

根据子类能安全地 `cast` 成父类， `'long` 的生命周期能够安全地转换成 `'short` 的生命周期，就可以得到下面的继承图：

<img src="lifetime_cast.png" style="zoom:50%;" />

这就有继承关系了！并且 `'static` 的生命周期是所有生命周期的"子类型"，这和 `Object` 是所以对象的父类一样，只不过是反过来的。

当然你可以认为是反过来的，即认为 `'static` 是所有生命周期的"父类型" (这更符合集合论的直觉？)，只不过这时协变和逆变就相互对调了(你会发现这样看的话，逆变会比协变更常用，但众所周知，逆变是反直觉的，所以说还是认为 `'static` 的生命周期是所有生命周期的"子类型" 最好)。

那么，对于一个持有生命周期标记的类型 `A<'a>`，它的类型就可以表示成 `'a -> A`，知道了生命周期，就可以确定 `A` 的具体类型（生命周期也是类型声明中的一部分），这就回到了最开始的那个例子，要面临协变和逆变的问题了——吗？

机智的 Rust 早就考虑到了这点了！先说结论：(https://doc.rust-lang.org/nomicon/subtyping.html)

| Type           | 'a        | T                 | U         |     |
| -------------- | --------- | ----------------- | --------- | --- |
| &'a T          | covariant | covariant         |           | *   |
| &'a mut T      | covariant | invariant         |           | *   |
| Box\<T>        |           | covariant         |           | *   |
| Vec\<T>        |           | covariant         |           |     |
| UnsafeCell\<T> |           | invariant         |           | *   |
| Cell\<T>       |           | invariant         |           |     |
| fn(T) -> U     |           | **contra**variant | covariant | *   |
| *const T       |           | covariant         |           |     |
| *mut T         |           | invariant         |           |     |

关于它们的解释都可以在《The Rustonomicon》3.8 节看到详细内容

解释几个带 * 的：

`&'a T` 对 `'a` 和 `T` 是协变的，例如 `&'static A<'static>` 可以协变为 `&'some1 A<'some2>`，这是安全的。常规的协变里，不安全是因为将容器进行了更改，而这里的 `&'a T` 它是不可变引用，是无法进行更改的，尝试收缩一个生命周期，并使用收缩后的结果总是安全的。

`&'a mut T` 对 `'a` 是协变的，对 `T` 是**不变**的，例如  `&'static mut A<'static>` 可以协变为  `&'some mut A<'static>` ，原理类似上面，但是不能将其协变为 `&'some1 mut A<'some2>`，因为这里的 `A` 是可变的，对其修改会影响到外面的  `&'static mut A<'static>`，可能导致 `A<'static>` 的生命周期标记失效。

例如《The Rustonomicon》中举的例子：

```rust
fn evil_feeder<T>(input: &mut T, val: T) {
    *input = val;
}

fn main() {
    let mut mr_snuggles: &'static str = "meow! :3";  // mr. snuggles forever!!
    {
        let spike = String::from("bark! >:V");
        let spike_str: &str = &spike;                // Only lives for the block
        evil_feeder(&mut mr_snuggles, spike_str);    // EVIL!
    }
    println!("{}", mr_snuggles);                     // Use after free?
}

```

编译器会认为 `&mut T` 中的 `T` 还是 `&'static T`，而不是协变成 `&'some T`，而对其赋值的 `val` (spike_str) 不具备 `'static` 的生命周期约束，这将会编译失败！

`Box<T>` 对 `T` 是协变的，因为能够修改 `Box` 内部的必定是持有所有权或者是可变引用，持有所有权的**只有一个地方**，生命周期改了就改了，其他地方也不知道，这没有关系；持有引用例如 `&'a mut Box<T>` ，这对 `Box<T>` 是不变的，所以不用担心。与其类似的还有 `Vec、Hashmap`。

`Cell<T>` 和 `RefCell<T>` 等使用了 `UnsafeCell<T>` 的类型，对 `T` 都是不变的。它们都具有内部可变性，即持有不可变引用都可以改变自身，如果还是可以协变的话就会和 `&'a mut T` 对 `T` 可以协变一样会出现问题。

`fn(T) -> U` 函数类型，对 `T` 是**逆变**的，对 `U` 是协变的，这个可以类比刚开始讨论的例子：如果我们需要处理一个 `'short` 的 `T`，那么最好得写一个可以处理 `T<'long>` 的函数签名 ( `'short -> 'long` 的逆变 )，这样对其处理时，不会错误的将不合适的生命周期写给 `T`， 对于返回 `U`，将一个长的生命周期缩短并返回给外面总是安全的 (协变)。

另外，对于自己定义的结构体，如果一个泛型 `A` 被多个字段使用，对其的型变关系遵循下列规定：

- 如果所有对 `A` 的使用都是协变的，那么 MyType 对 `A` 也是协变的
- 如果所有对 `A` 的使用都是逆变的，那么 MyType 对 `A` 也是逆变的
- 否则，MyType 在 `A` 上是不变的

```rust
use std::cell::Cell;

struct MyType<'a, 'b, A: 'a, B: 'b, C, D, E, F, G, H, In, Out, Mixed> {
    a: &'a A,     // covariant over 'a and A
    b: &'b mut B, // covariant over 'b and invariant over B

    c: *const C,  // covariant over C
    d: *mut D,    // invariant over D

    e: E,         // covariant over E
    f: Vec<F>,    // covariant over F
    g: Cell<G>,   // invariant over G

    h1: H,        // would also be covariant over H except...
    h2: Cell<H>,  // invariant over H, because invariance wins all conflicts

    i: fn(In) -> Out,       // contravariant over In, covariant over Out

    k1: fn(Mixed) -> usize, // would be contravariant over Mixed except..
    k2: Mixed,              // invariant over Mixed, because invariance wins all conflicts
}
```

### 案例

```rust
/// std::task::wake::Context

/// The context of an asynchronous task.
///
/// Currently, `Context` only serves to provide access to a [`&Waker`](Waker)
/// which can be used to wake the current task.
#[stable(feature = "futures_api", since = "1.36.0")]
#[cfg_attr(not(bootstrap), lang = "Context")]
pub struct Context<'a> {
    waker: &'a Waker,
    // Ensure we future-proof against variance changes by forcing
    // the lifetime to be invariant (argument-position lifetimes
    // are contravariant while return-position lifetimes are
    // covariant).
    _marker: PhantomData<fn(&'a ()) -> &'a ()>,
    // Ensure `Context` is `!Send` and `!Sync` in order to allow
    // for future `!Send` and / or `!Sync` fields.
    _marker2: PhantomData<*mut ()>,
}
```

如上，标准库中的 `Context` 中包含一个生命周期 `'a`，为了防止上下文的生命周期发生变化(variance)，加了一个 `_marker: PhantomData<fn(&'a ()) -> &'a ()>`，其参数对 `'a` 逆变，返回对 `'a` 协变，总体上对 `'a` 的变化不确定，所以最后确定为 `不变`。

PS：关于这里的 `Ensure we future-proof against variance changes` 可以参考下面的讨论：

https://users.rust-lang.org/t/what-is-effect-of-the-forcing-the-lifetime-to-be-invariant/61774

截取回答：

>   Lifetime is not "from this until that". Lifetime is only "until that", since by the very existence of the lifetime-annotated reference you guaratee that the object exists at the current point - the important question is "when it ceases to exist", not the exact span.

大概TA想说的是：你不能使用生命周期的变化 (variance) 强行给 `Context<'a>` “续命” 到 `Context<'b>`(`where 'b: 'a`，从写入角度上来看是，收缩 `'b` 到 `'a`，可以安全协变)，`Context` 的生命周期在创建时就应该定死。

## 结语

协变和逆变问题确实比较烧脑，我相信过不了几天我就会又不懂了，于是写下这篇文章备忘。

我在纸上画生命周期图示的时候，突然发现了它也有"继承"这种关系，想到 `Rust` 里消失的继承竟然会用这种方式重现，于是联想到了协变和逆变，惊奇地发现网上也有人有相同的想法，最终在《The Rustonomicon》找到了关于这个问题的文章。

## 引用

- [1] [The Rustonomicon](https://doc.rust-lang.org/nomicon/subtyping.html)
- [2] [Rust 秘典（死灵书）](https://nomicon.purewhite.io/subtyping.html)
- [2] [深入理解 TypeScript](https://jkchao.github.io/typescript-book-chinese/tips/covarianceAndContravariance.html)
