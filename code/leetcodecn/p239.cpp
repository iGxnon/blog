#include <bits/stdc++.h>

using namespace std;

class Solution {
   public:
    // TLE, O(n*k)
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector<int> ans;
        for (int i = 0; i + k <= nums.size(); i++) {
            int m = INT_MIN;
            for (int j = i; j < i + k; j++) {
                m = max(m, nums[j]);
            }
            ans.push_back(m);
        }
        return ans;
    }

    // TLE 51/51，剪枝
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        vector<int> ans;
        int m = INT_MIN;
        int mi = -1;
        // invariant: 窗口内最大的数 m 和它的下标 mi
        for (int i = 0; i < k; i++) {
            if (nums[i] > m) {
                m = nums[i];
                mi = i;
            }
        }
        ans.push_back(m);
        for (int i = 1; i + k <= nums.size(); i++) {
            // i, i + 1, i + 2, ... i + k - 1
            if (mi >= i) {  // 如果之前的最大值也在滑动后的区间内
                if (nums[i + k - 1] >= m) {  // 如果滑入的值更大
                    m = nums[i + k - 1];
                    mi = i + k - 1;
                }
            } else {  // 如果之前的最大值刚好被滑出
                if (nums[i + k - 1] >= m) {  // 如果滑入的更大
                    m = nums[i + k - 1];
                    mi = i + k - 1;
                } else {
                    // 否则只能再搜索一遍
                    m = INT_MIN;
                    for (int j = i; j < i + k; j++) {
                        if (nums[j] > m) {
                            m = nums[j];
                            mi = j;
                        }
                    }
                }
            }
            ans.push_back(m);
        }
        return ans;
    }

    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        int n = nums.size();
        priority_queue<pair<int, int>> q;
        for (int i = 0; i < k; ++i) {
            q.emplace(nums[i], i);
        }
        vector<int> ans = {q.top().first};
        for (int i = 1; i + k <= n; i++) {
            q.emplace(nums[i + k - 1], i + k - 1);
            // 注意，不需要精确的把所有 second < i 的节点全删除，只有 top
            // 上的元素会对 ans 有影响，只需移除 top 上不符合的即可
            while (q.top().second < i) {
                q.pop();
            }
            ans.push_back(q.top().first);
        }
        return ans;
    }

    // TODO：单调队列
};
