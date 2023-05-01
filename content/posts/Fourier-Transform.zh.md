---
title: Fourier Transform
date: 2022-03-24
extra:
  katex: true
taxonomies:
  categories: [数学]
  tags: [算法]
---

# 傅里叶变换

> **傅里叶变换**（法语：Transformation de Fourier、英语：Fourier transform）是一种线性[积分变换](https://zh.wikipedia.org/wiki/积分变换)，用于信号在[时域](https://zh.wikipedia.org/wiki/時域)（或空域）和[频域](https://zh.wikipedia.org/wiki/频域)之间的变换，在[物理学](https://zh.wikipedia.org/wiki/物理学)和[工程学](https://zh.wikipedia.org/wiki/工程学)中有许多应用。因其基本思想首先由[法国](https://zh.wikipedia.org/wiki/法国)学者[约瑟夫·傅里叶](https://zh.wikipedia.org/wiki/约瑟夫·傅里叶)系统地提出，所以以其名字来命名以示纪念。实际上傅里叶变换就像化学分析，确定物质的基本成分；信号来自自然界，也可对其进行分析，确定其基本成分
>
> —— [Wikipedia](https://zh.wikipedia.org/wiki/傅里叶变换)

## 重要公式

1. 连续傅里叶变换

$$
\hat{f}(\xi)=\int_{-\infty}^{\infty} f(x) e^{-2 \pi i x \xi} d x
$$

2. 连续傅里叶逆变换

$$
f(x)=\int_{-\infty}^{\infty} \hat{f}(\xi) e^{2 \pi i \xi x} d \xi
$$

3. 离散傅里叶变换(Discrete Fourier Transform)

$$
\hat{x}[k]=\sum_{n=0}^{N-1} e^{-i \frac{2 \pi}{N} n k} x[n] \quad k=0,1, \ldots, N-1
$$

4. 离散傅里叶逆变换

$$
x[n]=\frac{1}{N} \sum_{k=0}^{N-1} e^{i \frac{2 \pi}{N} n k} \hat{x}[k] \quad n=0,1, \ldots, N-1
$$

## 图形化

![img](https://pic2.zhimg.com/v2-674ebbd43e39e73c4856c6bdb236f605_b.webp)

## 傅里叶级数

> 在[数学](https://zh.wikipedia.org/wiki/数学)中，**傅里叶级数**（英语：Fourier series，[/ˈfʊrieɪ, -iər/](https://zh.wikipedia.org/wiki/Help:英語國際音標)）是把类似[波](https://zh.wikipedia.org/wiki/波)的函数表示成很多简单[正弦波](https://zh.wikipedia.org/wiki/正弦波)加权合的方式
>
> —— [Wikipedia](https://zh.wikipedia.org/wiki/傅里叶级数)

+ 傅里叶系数($a_{n},b_{n}$为原函数展开的傅里叶级数的sine和cosine的系数，P 为原函数 $s(x)$ 可积区间长度)

$$
a_{n}=\frac{2}{P} \int_{P} s(x) \cdot \cos \left(2 \pi x \frac{n}{P}\right) d x
$$

$$
b_{n}=\frac{2}{P} \int_{P} s(x) \cdot \sin \left(2 \pi x \frac{n}{P}\right) d x
$$

+ 傅里叶级数(N阶)

$$
s_{N}(x)=\frac{a_{0}}{2}+\sum_{n=1}^{N}\left(a_{n} \cos \left(\frac{2 \pi n x}{P}\right)+b_{n} \sin \left(\frac{2 \pi n x}{P}\right)\right)
$$

引入欧拉公式

$$
e^{i\theta} = \cos\theta + i\sin\theta
$$

+ 复数傅里叶级数

$$
s_{N}(x)=\sum_{n=-N}^{N} c_{n} \cdot e^{i \frac{2 \pi n x}{P}}
$$

 ## 两个应用

1. 核磁共振成像

   > 依据所释放的能量在物质内部不同结构环境中不同的衰减，通过外加[梯度](https://zh.wikipedia.org/wiki/梯度)磁场检测所发射出的[电磁波](https://zh.wikipedia.org/wiki/电磁波)，即可得知构成这一物体[原子核](https://zh.wikipedia.org/wiki/原子核)的位置和种类，据此可以绘制成物体内部的结构图像。

   > 采用调节频率的方法来达到核磁共振。由[线圈](https://zh.wikipedia.org/wiki/电感元件)向样品发射电磁波，调制振荡器的作用是使射频电磁波的频率在样品共振频率附近连续变化。当频率正好与核磁共振频率吻合时，射频振荡器的输出就会出现一个[吸收峰](https://zh.wikipedia.org/w/index.php?title=吸收峰&action=edit&redlink=1)，这可以在[示波器](https://zh.wikipedia.org/wiki/示波器)上显示出来，同时由频率计即刻读出这时的共振频率值。
   >
   > 氢核是人体成像的首选核种：人体各种组织含有大量的水和[碳氢化合物](https://zh.wikipedia.org/wiki/烃)，所以氢核的核磁共振灵活度高、信号强，当施加一射频脉冲信号时，氢核能态发生变化，射频过后，氢核返回初始能态，共振产生的电磁波便发射出来。原子核振动的微小差别可以被精确地检测到，经过进一步的计算机处理，即可能获得反应组织化学结构组成的三维图像，从中我们可以获得包括组织中水分差异以及水分子运动的信息。这样，病理变化就能被记录下来。
   >
   > —— Wikipedia

   + 大量氢核在[驰缓](https://zh.wikipedia.org/wiki/弛緩_(核磁共振))时释放共振产生的电磁波，形成的空域信号经过傅立叶变换就可以分离出频域信号

   + [核磁共振为何知道-Bilibili](https://www.bilibili.com/video/BV1di4y1y7au)

<center><iframe src="//player.bilibili.com/player.html?aid=551948596&bvid=BV1di4y1y7au&cid=540193822&page=1" scrolling="no" border="0" frameborder="no" framespacing="0" allowfullscreen="true"> </iframe></center>

2. 快速傅立叶变换求多项式乘积

   > 快速傅里叶变换（Fast Fourier Transform，FFT）是一种可在 $O(nlogn)$ 时间内完成的离散傅里叶变换（Discrete Fourier transform，DFT)算法。

   + [一小时学会快速傅里叶变换](https://zhuanlan.zhihu.com/p/31584464)
