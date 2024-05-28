#include <algorithm>
#include <vector>

using namespace std;

class Solution {
   public:
    // 官方题解
    // dp[i] 表示前 i 个元素并以 **第i的元素结尾** 的最长子序列长度
    // dp[i] = max(dp[j], 0 <= j < i and nums[i] > nums[j]) + 1
    // dp[i] = 1，其他情况
    int lengthOfLIS(vector<int>& nums) {
        int dp[2501];
        // 设置其他情况
        fill_n(dp, 2501, 1);

        int ans = 1;
        for (int i = 0; i < nums.size(); i++) {
            for (int j = 0; j < i; j++) {
                if (nums[i] > nums[j]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
            ans = max(ans, dp[i]);
        }
        return ans;
    }
};