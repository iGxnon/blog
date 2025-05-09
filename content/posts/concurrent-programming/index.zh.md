---
title: 并发编程
date: 2022-04-26
extra:
  add_toc: true
  katex: true
taxonomies:
  tags:
    - 并发
---

> 来自：[2022 春季南京大学操作系统公开课(蒋炎岩)](https://www.bilibili.com/video/BV1Cm4y1d7Ur/) 多处理器编程部分

# 入门多处理器编程

> Concurrent: existing, happening, or done at the same time.
> In computer science, concurrency refers to the ability of different parts or units of a program,
> alogrithmn, or problem to be executed out-of-order or in partial order, without affecting the
> final outcome. (Wikipedia)

系统调用的代码是世界上最早的并发程序

> 人类控制不了并发所带来的灾难，人类只能写好能控制的了的代码

## 状态机

1. 多线程程序每一个线程对应一个状态机，每创建一个线程就是创建一个状态机
1. 不同的状态机的状态转移都可以在不同 cpu 上并行运行

## 原子性

> 一段代码执行独占整个计算机系统

### 破坏原子性

- 单处理器多线程

    线程在运行时可能被打断，切换到另一个线程执行，这会打破原子性

    如果编译后的指令只有一条的话那么不会破坏原子性，比如 add 指令
    线程在一个 CPU 时间片里通常会至少执行一个指令，单处理器每个物理时刻
    只会执行某个程序的某个指令，所以指令的执行是串行的，这可以保证
    原子性

- 多处理器多线程

    线程可以在不同处理器并行执行，这本就是非原子性的

    编译后的指令即使只有一个也会破坏原子性
    多处理器中的多线程可以运行在不同的处理器上，即使编译后只有一个指令
    也可能会导致这条指令被多个处理器同时执行破坏原子性

### 安全的 printf

> printf 总是可以正常工作，因为它是线程安全的

### 实现原子性

> 互斥

- lock(&lk)
- unlock(&lk)

使用锁去保证指令执行的串行

## 顺序性

> 指令在系统执行的顺序性

### 破坏顺序性

- 编译优化

  编译器可以在可见的串行逻辑中优化逻辑
  **这可能不能保证并发运行的顺序逻辑正常** 
  
  比如
  
  ```c
  extern int done;
  void join() {
    while (!done) {
    }
  }
  ```
  
  希望线程二可以改变 done 来控制 join 退出
  可能会被优化成 `if (!done) while(1)`
  
- 指令重排
  
  现代处理器其实也是一种编译器
  它可以将汇编代码转化成处理器运行的更小的操作 $\mu ops$：
  fetch -> issue -> execute -> commit

现代处理器可以保证在可见的流程中不改变输出结果从而自行编排$\mu ops$
如一起 issue 多条指令，那么到底哪个指令先执行就不得而知了
比如对应到状态机的状态迁移:

```c
x=1;
y=1;
printf("%d, %d", x, y);
```

可以被重排为

```c
y=1;
x=1;
printf("%d, %d", x, y);
```

但是多线程程序的流程是无法预知的，多线程程序中某一个线程可以保证该线程单独运行时的顺序性不会被指令重排打乱，但是不能保证很多线程同时运行时系统的顺序性不被指令重排序打乱

## 可见性

> 系统中不同线程之间的资源可见性

- 指令重排
  
  继上面的指令重排
  每一次由于 cpu 的指令重排序可能会破坏并发程序的顺序逻辑

  指令的重新排序不是没有规则的，它不会将互相依赖的指令重排序
  
  例如 x=1; y=x 不会重排成 y=x; x=1
  
  多核 CPU 内部会维护一个指令依赖性的有向无环图(无环是因为现在的指令不会依赖未来的指令)
  
  从而尽可能多地将没有依赖性的指令一起 issue 出去执行，以提升效率
  
  但是并发程序之间的逻辑依赖多核 CPU 是不可见的，就是上面说的多线程程序的流程是无法预知的
  
  例如在 A 线程里 x = 1; printf(y); 在 B 线程里 y = 1；printf(x); 可能会导致输出 0 0 的反常情况, 因为 B 线程里的 printf(x) 与 A 线程里的 x = 1 没有可见性，构建不了(很难构建)依赖关系。那么如果 x = 1 对 x 写的时候缓存没有命中, 就会暂时放一放 x，先对 y 读取，这时线程 B 也是这样的话读取到的 x 和 y 都是 0
  
  线程之间的逻辑互相没有可见性，一切的逻辑优化都可能会导致并发程序出问题

## 一致性

> 系统中不同线程之间对资源操作与访问的一致性

- 缓存

  内存的存取速度终究是达不到 CPU 计算的速度，缓存就是拿来补内存速度不足的短板的，L1, L2, L3 级缓存等等
  
  有了缓存，加快的计算机处理的速度，可牺牲的是多线程之间的可见性和一致性

- 内存模型
  
  x86 的内存模型为计算机保证了一致性，可以在上面 write x 和 read y 之间插入一条原子指令，或者 fence 指令，这就会强制将 x 写入共享内存再去读取 y，可这样就牺牲了性能。而 arm 等架构的内存模型的计算机每一个处理器会有一份单独的内存，内存之间随时进行同步，这本就很难保证一致性

## 入门到放弃

### 不原子

**甚至一条指令都无法保证原子性**

### 能乱序

**编译器能够使并发程序编译后的指令对内存访问变得乱序** 

**就算变成了指令，CPU 在处理指令的时候也会变得乱序**

### 不可见不一致

**并发程序的逻辑关系是不可见的**

**缓存也会导致资源操作的不可见性和不一致性**

# 理解并发程序执行

## 互斥

### 失败的互斥

```c
int lock = UNLOCK;

void critical_section() {
retry:
    if (lock != UNLOCK) {
        goto retry;
    }
    lock = LOCK;

    // critical_section

    lock = UNLOCK
}
```

**两个线程可以同时进入 if 判断并得到 false 结果，然后一起进入 critical_section**

### 奇怪的互斥尝试(Peterson 算法)

![](peterson.png)

手快 🈶️，手慢 🈚️
即使穷举状态机模型的所有情况均证明 peterson 算法可以保证互斥
**但是仍然不能保证这个算法在多核处理器上能正常工作**

## 绘制状态机

**多线程程序的执行 = 状态机** 

**每一步确定一个状态机执行** 

**绘制状态机**

# 并发控制

## 原子性——互斥

### 自旋锁(spin lock)

> **利用原子指令 xchg (exchange), 读取一个值并把手上的值交换**

```c
int table = YES;

void lock() {
retry:
    int got = xchg(&table, NOPE);
    if (got == NOPE)
        goto retry;
    assert(got == YES)
}

void unlock() {
    xchg(&table, YES)
}
```

```c
int locked = 0;
void lock() { while(xchg(&locked, 1)); }
void unlock() { xchg(&locked, 0); }
```

- 没有其他线程或者很少有其他线程抢锁的情况下加锁很快(一条原子指令就完事了)
- 如果有很多线程在抢锁的话会导致自旋等待浪费 CPU

### 互斥锁(Mutex)

> **系统调用**

- **sysall(SYSCALL_lock, &lk);**

  **尝试获取锁，但如果失败，把这个线程挂起，切换到其他线程里执行**

- **syscall(SYSCALL_unlock, &lk);**

  **释放锁，如果有其他线程等待这把锁的话就唤醒那个线程去获取锁**

- 上锁失败不会空转消耗 CPU
- 上锁成功了也需要退出系统调用内核(syscall)，耗时

### Futex: Fast Userspace muTexes

> 缝合怪（全都要

- 加锁成功就和 Spin lock 一样只有一个原子指令就完了
- 加锁失败就和睡眠锁一样执行系统调用休眠
- 释放锁时将在等待锁休眠的线程唤醒

### Compare & Swap (CAS)

- 利用原子指令 cmpxchg (比较并交换) 来原子性修改某个值
- 函数签名例如： func CompareAndSwapInt64(addr \*int64, old, new int64) (swapped bool)

## 顺序一致可见性——同步(Synchronization)

> 两个或两个以上的随着时间变化的量在变化过程中保持一定的相对关系

- 线程同步：在某一个时间点共同达到互相已知的状态

## 生产者&消费者

- 同步
  
  生产者：队列里有空位就生产填补
  消费者：队列里有元素就消费取出

## 条件变量(Conditional Variables|CV)

> 条件变量的 API

- wait(cv, mutex)

  调用之前保证已经取得了 mutex
  释放 mutex 并进入睡眠

- signal/notify(cv)

  唤醒等待 cv 的线程中其中一个(通常唤醒后要把锁给它)

- broadcast/notifyAll(cv)
  
  唤醒所有等待 cv 的线程(通常唤醒的所有线程都会去争夺这把锁)

### Java 中的 wait 和 notify 实现单 producer/consumer 模型

这里的 条件变量和锁是同一个变量 cvAndLock 来代表的
这也是 Java 实现条件变量同步的一个机制
对锁调用 notify 唤醒之前持有过这把锁的睡眠线程，并把锁给它
对锁 wait 让持有这把锁的线程睡眠并把锁释放

```java
public class CV{
    public static void main(String[] args) {
        List<Object> queue = new LinkedList<>();
        Object cvAndLock = new Object();

        Thread producer = new Thread(() -> {
            synchronized(cvAndLock) {
                while (true) {
                    if (queue.size() >= 5) {
                        try {
                            System.out.println("Producer: queue is full, i'll gonna sleep. zzz");
                            cvAndLock.wait();
                            System.out.println("Producer: what? queue is not full? thanks to wake me up!");
                        } catch (Exception ignored) {
                        }
                    }
                    queue.add(new Object());
                    cvAndLock.notify();
                    System.out.println("put one into queue, now queue:" + queue);
                }

            }
        });

        Thread consumer = new Thread(() -> {
            synchronized(cvAndLock) {
                while (true) {
                    if (queue.isEmpty()) {
                        try {
                            System.out.println("Consumer: queue is empty, i'll gonna sleep. zzz");
                            cvAndLock.wait();
                            System.out.println("Consumer: what? queue is not empty? thanks to wake me up!");
                        } catch (Exception ignored) {
                        }
                    }
                    queue.remove(0);
                    cvAndLock.notify();
                    System.out.println("get one from queue, now queue:" + queue);
                }

            }
        });

        producer.start();
        consumer.start();
    }
}
```

### 多 producer/consumer 模型

我们希望 producer 中调用 notify 时会唤醒一个 consumer，但只有一个条件变量可能会又唤醒一个 producer 生产导致 bug，consumer 中调用 notify 时同理
**解决办法：保证不能唤醒了错误的线程，或者使用两个条件变量**

- Go 实现 1

```go
package test

import (
	"fmt"
	"sync"
	"testing"
)

var n, count = 5, 0

func TestSync(t *testing.T) {
	cv := sync.NewCond(&sync.Mutex{})

	for i := 0; i < 8; i++ {
		go Producer(cv)
		go Consumer(cv)
	}

	<-make(chan struct{})
}

func Producer(cond *sync.Cond) {
	for {
		cond.L.Lock()
		for !(count != n) { // to assume count != n
			fmt.Println("=== Produce start waiting...")
			cond.Wait() // if count == n, continue waiting.
		}
		count++
		fmt.Println("produce a num, now:", count)
		cond.L.Unlock()
		cond.Broadcast() // to avoid waking a producer up and cause dead lock
	}

}

func Consumer(cond *sync.Cond) {
	for {
		cond.L.Lock()
		for !(count != 0) { // to assume count != 0
			fmt.Println("=== Consumer start waiting...")
			cond.Wait() // if count == 0, continue waiting.
		}
		count--
		fmt.Println("consume a num, now:", count)
		cond.L.Unlock()
		cond.Broadcast() // to avoid waking a consumer up and cause dead lock
	}
}
```

- Java 实现 2

```java
import java.util.concurrent.atomic.AtomicInteger;

public class Test {
    public static void main(String[] args)  {
        AtomicInteger count = new AtomicInteger(); // 这里不需要Atomic类型，但是Java闭包只能传递常量值(基本数据类型或者对象引用)
        int n = 5;

        Object cvLock1 = new Object();
        Object cvLock2 = new Object();

        for (int i = 0; i < 8; i++) {
            new Thread(() -> {
                while (true) {
                    synchronized (cvLock1) {
                        while (!(count.get() != n)) {
                            try {
                                cvLock1.wait();
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }
                        assert count.get() != n;
                        count.getAndIncrement();
                        System.out.println("produce a num, now: "+count.get());
                    }
                    synchronized (cvLock2) {
                        cvLock2.notify();
                    }
                }
            }).start();

            new Thread(() -> {
                while (true) {
                    synchronized (cvLock2) {
                        while (!(count.get() != 0)) {
                            try {
                                cvLock2.wait();
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }
                        assert count.get() != 0;
                        count.getAndDecrement();
                        System.out.println("consume a num, now: "+count.get());
                    }
                    synchronized (cvLock1) {
                        cvLock1.notify();
                    }
                }
            }).start();
        }

    }
}
```

整体过程就是：

1. **加互斥锁**
1. **循环检查条件是否不满足，不满足就睡眠直至被唤醒继续循环检查到满足，满足到第 3 步**
1. **断言条件满足(Debug 使用)**
1. **执行操作**
1. **释放互斥锁**
1. **广播条件变量，唤醒所有睡眠的线程**

## 信号量机制

> 信号量相当于一个令牌

- P(&sem)：等待一个信号量并返回，没有信号量则会让线程休眠直到有信号量才唤醒
- V(&sem)：产生一个信号量，如果有等待信号量的线程就把信号量给它，否则放到信号量池里

### 实现生产者消费者

```c
void producer() {
    P(&empty); // 从empty里请求一个信号量
    count ++;
    V(&fill);  // 产生一个信号量到fill里
}

void consumer() {
    P(&fill); // 从fill里请求一个信号量
    count --;
    V(&empty);  // 产生一个信号量到empty里
}
```

## 管道 Channel 同步

管道自带阻塞与唤醒
可以建立在条件变量上实现

- java 实现

```java
class Chan<T> {

    private LinkedList<T> ch = new LinkedList<>();

    private int length;

    public Chan(int length) {
        assert length > 0;
        this.length = length;
    }

    public synchronized T get() {
        while (!(ch.size() != 0)) {
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        assert ch.size() != 0;
        T ret = ch.removeFirst();
        this.notifyAll();
        return ret;
    }

    public synchronized void put(T v) {
        while (!(ch.size() != length)) {
            try {
                this.wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        assert ch.size() < length;
        ch.addLast(v);
        this.notifyAll();
    }

    @Override
    public String toString() {
        return "Chan{" +
                "ch=" + ch +
                ", length=" + length +
                '}';
    }
}
```

# 业务上的并发编程

## 高性能计算程序(单机器超算)

> A technology that harmesses the power of supercomputers or computer clusters to solve complex problems requiring massive computation

### 挑战

#### 计算图如何分解

- 计算图需要容易并行化
- 生产者/消费者模型解决同步问题

#### 线程如何通信

## 数据中心的并发编程(多机器分布式)

> A network of computing and storage resources that enable the delivery of shared applications and data

### 挑战

- 数据一致性 (Consistency)
- 服务时刻保持高可用 (Availability)
- 容忍机器离线 (Partition tolerance)

## 单台机器数据处理的并发编程

### 并行处理请求

- 关键指标: QPS (Query per second), tail latency, ...

### 设计并发程序的工具

1. 线程

   操作系统的最小执行单元，上下文切换较慢
   线程擅长处理密集型计算任务和 IO 任务，可以利用很多的处理器，而协程是 OS 不可见的，用户不合理调度会使得处理器利用率低(一个协程执行 io 进入 syscall 后阻塞内核线程，导致这个线程剩下的协程干等[用户没有把协程切换到另一个处理器的情况下])

2. 协程

   比线程更轻量，切换更快
   协程擅长切换，处理大量的并发任务，用户合理调度下可以防止大量任务上下文切换时造成很大性能损失；协程还可用于防止重要线程阻塞，如安卓开发中总是把网络请求部分打包成一个协程扔到其他线程去执行，然后拿到请求结果刷新 UI 界面，防止阻塞 UI 刷新的主线程，如果直接另起一个线程执行网络请求会造成性能损失，即使使用线程池剔除初始化的损失也可能会因为线程的切换损失性能

3. Goroutine
   
   缝合怪
   自古缝合怪往往是性能最优的
   它结合了线程和协程的特点，实现了上下文切换快，调度靠运行时而非用户(用户永远会写出窒息的调度方法，不如自行设计一个高性能的调度模型 "GPM")，进一步蚕食 CPU 的价值

### 实现生产者消费者的工具——同步并发程序

1. 条件变量
1. 信号量

1. 管道 Channel
   > **Do not communicate by sharing memory; instead, share memory by communicating.**
   > —— 《Effevtive Go》

**共享内存 = 万恶之源**

并发问题来自于共享内存的失效，为了防止共享内存失效，只能加锁保护它，于是诞生了各种锁，条件变量这些同步工具

回头想一想，为什么要共享内存?
是因为我们需要让共享内存帮助我们进行并发程序间的通信。让不同线程协助工作，只需制定一个共享内存协议：job 数 大于 0 时消费者消费，job 数 等于 0 时生产者生产。

可让线程通信真的只能靠共享内存吗？线程中的通信真的只能维护一个共同的共享内存协议来进行通信吗？
不是！
回归原始，如果两个人需要通信怎么办？
直接和 Ta 说一声就行了。通信不只有共享内存的手段，想要通信，只需维护一个线程到线程之间的线路，线程间彼此交流就行了，这就是 **share memory by communicating.**

这个线路就是 **Channel**

Channel 其实和条件变量，信号量不是同一个级别的东西，Channel 本身就算一个共享内存，只不过它和普通的共享内存不一样的是：它不作为一个**信号**共享内存，而是作为一个**媒介**实现*分享*内存
Channel 的底层实现就需要共享内存，例如要保证很多数据写入 Channel 时不会出现并发问题，就需要一把共享的互斥锁；唤醒等待 Channel 的线程时，也需要使用条件变量。

**Channel 是上层 API 对锁，条件变量等底层同步工具的封装所形成的一种新的同步工具，让用户不再重复造轮子，也让用户不再因为使用共享内存进行通信导致并发问题而苦恼**

## 人机交互(UI)的并发编程

### 特点——不太复杂

1. 没有太多数据

   一个页面的数据不会太多，太多了人也看不下来
   页面的渲染布局 浏览器/APP 可以自己帮我们完成

2. 没有太多 IO
   
   一些网络 IO，和一些页面元素读取的 IO

### Javascript 单线程 + 事件模型

程序串行执行，没有锁的概念，也不会出现并发问题
每次执行都完成一个事件，事件就是一个原子的操作
网络 IO 请求使用异步操作创建一个事件，当请求拿到结果或者失败的时候再产生一个事件渲染页面

# 并发的 Debug

软件是需求(规约)在计算机数字世界的投影

## 防御性编程

把程序需要满足的条件用 assert 表达出来
编写代码时会丢失信息，可程序不知道这些信息，算一个简单的减法
就可能出现 0 - 100 = 4294967196 (无符号整数溢出)，这时就可以补充一些信息
例如 assert(0-100 < 0)，补充一个 0 减去一个正数都小于 0 的信息，从而实现防御性编程

## 死锁

> A deadlock is a state in while each member of a group is waiting for another member, including itself, to take action.

### 必要条件

- 互斥：一个资源每次只能被一个进程使用
- 请求与保持：一个进程请求资源阻塞时，不释放已获得的资源
- 不剥夺：进程已获得的资源不能强行剥夺
- 循环等待：若干个进程直接形成头尾相连的循环等待资源关系

实际开发中很难破坏这四个条件来避免死锁

### 避免死锁

#### AA-Deadlock

- AA 型死锁容易检测，及早报告，及早修复

#### ABBA-Deadlock

- 任何时刻系统中的锁都是有限的
- 严格按照**固定的顺序**获得所有锁(消除 "循环等待")

## 数据竞争

> 不同的线程同时访问同一段内存，且至少有一个是写

- 两个内存访问在 赛跑，跑赢 的操作先执行

### 避免数据竞争

用互斥锁保护好共享数据，消灭所有数据竞争

## Debug 工具

### Lockdep——运行时的死锁检查

Lockdep 为每一把锁分配一个确定唯一的 "allocation site"
并断言系统中同一个 "allocation site" 的锁存在全局且唯一的上锁顺序

### ThreadSanitizer——运行时的数据竞争检查

为所有事件建立一个 happens-before 关系图
如果存在未连接的事件点说明可能会发生数据竞争

### 动态程序分析工具——Sanitizers

在事件发生时记录
解析记录检查问题

- AddressSanitizer 非法内存访问
- ThreadSanitizer 数据竞争
- MemorySanitizer 未初始化读取
- UBSanitizer undefined behavior

# 结语

希望这些对你有用 :)
