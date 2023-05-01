---
title: Rust 中 move | | async move {} 的奇妙问题
date: 2022-11-26
extra:
  add_toc: true
taxonomies:
  categories:
    - 编程
  tags:
    - Rust
---

# Rust 中 move | | async move {} 的奇妙问题

> 在 Rust 里有非常多的奇妙语法，例如第一次看见 `dyn SomeTrait` 前的 `dyn` 会觉得这是个什么奇怪的东西
>
> 而今天的这个奇妙问题是 async 和 move 之间的关系

## 先从 `{}` 开始

`{}` ，两个大括号，用于标记一个语法块，在 rust 中，这可以理解成一组表达式的集合，一个作用域等等

```rust
fn main() {
    let ret = {
        let mid_a = 114514;
        let mid_b = 1919810;
        mid_a + mid_b
    };
    println!("{}", ret);
}
```

其中被 `{}` 括起来的部分会被当作一个新的作用域，最后一行没有 `;` 结尾的表达式的输出，则代表这个作用域的输出

因为变量 `mid_a` 和 `mid_b` 在一个这个作用域内被赋予了数据的所有权，所以在这个作用域结束后，它们的内存会被回收掉

```rust
fn main() {
    let ret = {
        let mid_a = 114514;
        let mid_b = 1919810;
        mid_a + mid_b
    };
    println!("{}", ret);
    println!("{}", mid_a);
    println!("{}", mid_b);
}

// 编译错误！
// error[E0425]: cannot find value `mid_a` in this scope
//  --> src/main.rs:33:20
//    |
// 33 |     println!("{}", mid_a);
//    |                    ^^^^^ not found in this scope

// error[E0425]: cannot find value `mid_b` in this scope
//  --> src/main.rs:34:20
//    |
// 34 |     println!("{}", mid_b);
//    |                    ^^^^^ not found in this scope

// For more information about this error, try `rustc --explain E0425`.
```

当然也可以理解成外面的作用域看不到内层作用域的变量了，在其他的语言里，也会出现这样的结果

如果按照其他语言里的理解，对上面的代码稍加改造，就可以得到如下可以编译的版本了

```rust
fn main() {
    let mid_a;
    let mid_b;
    let ret = {
        mid_a = 114514;
        mid_b = 1919810;
        mid_a + mid_b
    };
    println!("{}", ret);
    println!("{}", mid_a);
    println!("{}", mid_b);
}
```

并且，如果 `mid_a` 和 `mid_b` 没有实现 `Copy` 特征的话，就可以用 `let mid_c = mid_a` 和 `let mid_d = mid_b` 在新的作用域里拿走它们的所有权，这样在作用域的下文中，就无法使用这两个变量了

```rust
#[derive(Debug)]
struct Hi;

fn main() {
    let mid_a = Hi;
    let mid_b = Hi;
    {
        let mid_c = mid_a;
        let mid_d = mid_b;
    }
    println!("{:?}", mid_a);
    println!("{:?}", mid_b);
}

// 编译错误！
// error[E0382]: borrow of moved value: `mid_a`
//  --> src/main.rs:11:22
//    |
// 5  |     let mid_a = Hi;
//    |         ----- move occurs because `mid_a` has type `Hi`, which does not implement the `Copy` trait
// ...
// 8  |         let mid_c = mid_a;
//    |                     ----- value moved here
// ...
// 11 |     println!("{:?}", mid_a);
//    |                      ^^^^^ value borrowed here after move

```

这些就和其他语言不一样了，也是作为初学者需要弄懂的东西

不过当我发现 `async {}` 语法可以变成  `async move {}` 时，我就在想，会不会有 `move {}` 这种语法呢？

事实上，Rust 并没有这个语法，需要捕获外部的变量的所有权，那就用 `let` 申明一个新的变量去继承所有权即可

## 为什么会有 `move` ?

`move` 这个关键词应该是首先出现在线程那一块

```rust
use std::thread;

#[derive(Debug)]
struct Hi;

fn main() {
    let mid_a = Hi;
    thread::spawn(move || {
        println!("{:?}", mid_a);
    });
}
```

当我们尝试使用闭包去捕获外层的变量，然后传递给 `thread::spawn` 后，一个新启动的线程就可以拿到外面的变量了。

不过如果在闭包前没有使用 `move`，很不幸，编译失败了

```txt
error[E0373]: closure may outlive the current function, but it borrows `mid_a`, which is owned by the current function
  --> src/main.rs:8:19
   |
8  |     thread::spawn(|| {
   |                   ^^ may outlive borrowed value `mid_a`
9  |         println!("{:?}", mid_a);
   |                          ----- `mid_a` is borrowed here
   |
note: function requires argument type to outlive `'static`
  --> src/main.rs:8:5
   |
8  | /     thread::spawn(|| {
9  | |         println!("{:?}", mid_a);
10 | |     });
   | |______^
help: to force the closure to take ownership of `mid_a` (and any other referenced variables), use the `move` keyword
   |
8  |     thread::spawn(move || {
   |                   ++++

For more information about this error, try `rustc --explain E0373`.

```

`mid_a` 很有可能在这个线程还没启动的时候，就已经脱离作用域被清除了！

需要使用 `move` 将 `mid_a` 的所有权一并移动到闭包的作用域下

所以，在 `thread::spawn` 后，就不能使用 `mid_a` 了

事实上，可以写出如下的代码，也能夺走 `mid_a` 的所有权

```rust
#[derive(Debug)]
struct Hi;

fn main() {
    let mid_a = Hi;
    (move || {
        println!("{:?}", mid_a);
    })();  // 使用一个带 move 的闭包夺走 mid_a 的所有权
    println!("{:?}", mid_a);
}

// 编译错误！
// error[E0382]: borrow of moved value: `mid_a`
//  --> src/main.rs:10:22
//    |
// 6  |     let mid_a = Hi;
//    |         ----- move occurs because `mid_a` has type `Hi`, which does not implement the `Copy` trait
// 7  |     (move || {
//    |      ------- value moved into closure here
// 8  |         println!("{:?}", mid_a);
//    |                          ----- variable moved due to use in closure
// 9  |     })();
// 10 |     println!("{:?}", mid_a);
//    |                      ^^^^^ value borrowed here after move
//    |

```

不过，很有趣的是，如果 `mid_a` 的类型实现了 `Copy` 的特征，这样的代码就可以通过编译

```rust
#[derive(Debug, Copy, Clone)]
struct Hi;

fn main() {
    let mid_a = Hi;
    (move || {
        println!("{:?}", mid_a);
    })();
    println!("{:?}", mid_a);
}
```

而使用 `thread::spawn` 时，无论 `mid_a` 是什么类型，都会编译失败，仔细观察使用 `thread::spawn` 编译报错的信息后

```txt
8  |     thread::spawn(|| {
   |                   ^^ may outlive borrowed value `mid_a`
9  |         println!("{:?}", mid_a);
   |                          ----- `mid_a` is borrowed here
   |
note: function requires argument type to outlive `'static`
```

可以看出，这是 `mid_a` 的生命周期的问题，它很有可能活不到线程使用它的时候！如果改成这样，就可以编译得过了

```rust
static MID_A: usize = 1;  // 具有 'static 的生命周期

fn main() {
    thread::spawn(|| {
        println!("{:?}", MID_A);
    }).join().expect("ba ga");
}
```

再改一下（  如果把上面的例子改成这样，则又又又过不了编译了

```rust
fn main() {
    let mid_a: &'static mut i32 = Box::leak(Box::new(42));  // 使用 Box::leak() 强制泄漏内存，并返回出一个 static数据 的引用
    thread::spawn(|| {
        println!("{:?}", mid_a);
    }).join().expect("ba ga");
}

// 编译失败！和上面一样的报错
```

这不对啊，`mid_a` 声明的时候都说明了其生命周期是 `'static` 了，为什么还得加个 `move`，而上面的例子就不需要加了？

`&'static` 约束的是被 `mid_a` 所引用的数据 `42`，它必须是能在程序中一直存活，而对于其引用 `mid_a`，它的生命周期取决于自己所在的作用域

所以，`move` 的作用是将没有实现 `Copy` 特征的类型对应的具有所有权的变量的所有权移动到闭包的作用域内 (emm，大概就是这样)，其实可以理解成这样：**使用了 `move` 的闭包，里面的捕获到的变量会被类似 `let catched = catched` 重新声明了一遍 (未实现 Copy 会夺走所有权)**

所有权转移到闭包内的变量后，其生命周期也会变到闭包的范围内

## `async move {}` 又是什么？

在 `async/await` 风格的异步编程中，被 `async` 描述的函数、语句块 `{}` 都是一块一块的“协程”，rust 的异步运行时可以帮我们在合适的时候将阻塞的一段 `async` 代码块移开，换上可以运行的代码块。程序员将保留 `async` 语法块 ( `Future` 类型 ) 的绝大多数控制权 ( 修改它的 `poll` 实现 )，而不是像 `golang` 一样，go 出来的代码块没办法再控制了。同时，它也会带来比较难受的 `“红蓝函数”` 问题 (https://journal.stuffwithstuff.com/2015/02/01/what-color-is-your-function/)

扯远了，`async {}` 其实是一个实现了 `Future` 的类型，其关联类型 `Ouput` 是 `{}` 语法块的返回

由于语法块 `{}` 被 `async` 描述，可以异步执行，内部的使用的变量的生命周期需要大于等于 `async` 代码块执行时的生命周期，不然就得使用 `move` 将变量的所有权转移到 `async` 代码块内

## 缝合怪 `move || async move {}`

已经看到这里了，其实这个也不是很迷惑了，将其拆开，就可以得到

```rust
move || {
  // 第一个 move 捕获到的变量作用域
  async move {
    // 第二个 move 捕获到的变量作用域
  }
}
```

清晰直白
