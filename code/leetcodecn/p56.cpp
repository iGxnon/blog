#include <algorithm>
#include <vector>

using namespace std;

class Solution {
   public:
    // 自己写的，什么鬼东西？？TLE
    vector<vector<int>> merge(const vector<vector<int>>& intervals) {
        vector<vector<int>> ans;
        int mi = intervals[0][0];
        int ma = intervals[0][1];
        for (auto i : intervals) {
            mi = min(mi, i[0]);
            ma = max(ma, i[1]);
        }
        int i = mi;
        while (i <= ma) {
            int mii = ma;  // 第一个大于 i 的 start
            for (auto ii : intervals) {
                if (ii[0] == i)
                    goto merge;  // 刚刚好就是这个区间的开始，直接开始合并
                if (ii[0] > i) {
                    mii = min(mii, ii[0]);
                }
            }
            i = mii;  // 找到了这个区间的开始
        merge:
            vector<int> a = {i};
            bool flag = true;
            // 反复遍历，找到区间能伸展的最大值
            while (flag) {
                flag = false;
                for (auto ii : intervals) {
                    if (i >= ii[0] && i < ii[1]) {
                        i = ii[1];
                        flag = true;
                    }
                }
            }
            a.push_back(i);
            ans.push_back(a);
            i++;  // 探测下一个区间
        }
        return ans;
    }

    // 题解
    vector<vector<int>> merge2(vector<vector<int>>& intervals) {
        int n = intervals.size();
        vector<vector<int>> ans;
        // 排序 保证数组升序
        sort(intervals.begin(), intervals.end());
        for (int i = 0; i < n; i++) {
            // 判断是否越界 两个数组是否有重合部分
            if (i < n - 1 && intervals[i][1] >= intervals[i + 1][0]) {
                // 更新原数组
                // 例：[1,2][2,3][5,6] => [1,2][1,3][5,6]
                intervals[i + 1][0] = min(intervals[i][0], intervals[i + 1][0]);
                intervals[i + 1][1] = max(intervals[i + 1][1], intervals[i][1]);
            }
            // 如果和下一个数组没有重合部分 则推入答案数组
            // 注意 如果i == n - 1 代表数组末尾 一定推入答案数组
            else if (i == n - 1 || intervals[i][1] < intervals[i + 1][0]) {
                ans.push_back({intervals[i][0], intervals[i][1]});
            }
        }
        return ans;
    }

    // 二刷
    vector<vector<int>> merge3(vector<vector<int>>& intervals) {
        vector<vector<int>> ans;
        sort(intervals.begin(), intervals.end());
        int n = intervals.size();
        // [1,2][2,3][4,5] => [1,2][1,3](插入 [1,3])[4,5]
        // [1,4][2,3] => [1,4][1,4]
        // 遍历到倒数第二个
        for (int i = 0; i < n-1; i ++) {
            // 前一个末尾比后一个开头大，把后一个开头改成前一个开头
            if (intervals[i][1] >= intervals[i+1][0]) {
                intervals[i+1][0] = intervals[i][0];
            }
            // 前一个末尾比后一个末尾还大，那把后一个末尾也改成前一个末尾
            if (intervals[i][1] >= intervals[i+1][1]) {
                intervals[i+1][1] = intervals[i][1];
            }
            // 出现了空隙，说明这个没办法继续合并了
            if (intervals[i][1] < intervals[i+1][0]) {
                ans.push_back(intervals[i]);
            }
        }
        // 最后一个一定是合并的结果
        ans.push_back(intervals[n-1]);
        return ans;
    }
};
