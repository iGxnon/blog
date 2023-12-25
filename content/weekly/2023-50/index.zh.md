---
title: 2023-50 周报
date: 2023-12-26
---

## 上周情况

可能是上上周太摆了，这周总体来说比较勤奋

### 生活

- 运动
  - 跑了 10km ！
    - 几乎是每天都跑了
- 吉他
  - 练琴跳过了《365日》第三周一些难弹的，到第四周好了不少
  - 感觉又进步了一点点，稳定性比之前好了一些
  - ~~编了~~(抄了)一个谱子
    - [https://tabs.ultimate-guitar.com/tab/5078239](https://tabs.ultimate-guitar.com/tab/5078239)
    - 不过这个谱子的 solo 对我来说比较难，制音好难
- 买了个机械闹钟
  - 破林檎手机闹钟的总是在不观测时会随机选择响与不响

### 技术

- 445
  - 做了 Homework 1 的几个 task
    - SQL原来也不好写
- PLT
  - 平时上课没事在看 HoTT book，看得脑瓜痛
  - 根据推友的推荐在学 Agda 这个语言，用于证明一些定理的工具
    - 不过还没正式开始，在看 PLFA 的逻辑基础，有中文翻译对照着看就没那么头痛了
- raknet-rs
  - 把 order 层写完了
  - 给每一层补充了一个单测
    - deduplication 和 fragmentation 层顺带着还补上了 fuzzing test
  - 整体的架构有了一个新的思考
- 工作
  - xline curp client 重构的 PR 补充并拆分开，把原来混乱的逻辑抽象成了两层，也修了一些并发上的 bug
  - 接手了 java client sdk 的开发，重回 java 真舒服，rust 真难写（
    - 顺带着写了一层 clojure client 包裹了 java client

## 下周计划

- 写 raknet-rs 的连接握手和滑动窗口
  - 尝试对 codec 跑一次 benchmark，做一下 profile 再压榨一下性能
    - 虽然说提前优化是万恶之源，但 codec 的 trait 抽象还是很不错的 (Stream 和 Sink)，应该不会有很大问题
- 445 提一提进度
- 再看点 Effective Modern C++ 和 PLFA
- 看海岛吉他乐理

## [青春なんていらないわ](./SeishunnanteIranaiwa.html)

<iframe 
  style="height: 420px;"
  src="./SeishunnanteIranaiwa.html"
  width="100%"
  scrolling="no"
  frameborder="0">
</iframe>