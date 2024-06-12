### LeetCode（已归档，最新见 ./code/leetcode）

1. 评测机器处理速度数量级: 1s内~10^8计算

2. 快读咒语

```cpp
ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
```

3. 有限元的排序可以牺牲空间创建有序 bucket 来检索排序达到 O(N) 复杂度 | 桶排序 k ~= n 时，算法复杂度 ~ O(N)

> 例如排序26个字母的排列组合，创建26个bucket按索引放入即可，最后按序读出非空的 bucket

