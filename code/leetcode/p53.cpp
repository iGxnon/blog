#include <algorithm>
#include <vector>

using namespace std;

class Solution {
   public:
    // dp:
    // dp 内的元素表示连续到该点最大的值：max(自己本身, 自己本身 + **连续到之前的点的最大的值**[子问题])
    // 状态转移方程: dp[i] = max{ dp[i - 1] + nums[i], nums[i] }
    // 最后的答案就是 dp 数组内最大的数值
    int maxSubArray(vector<int>& nums) {
        if (nums.size() == 1) {
            return nums[0];
        }
        vector<int> dp(nums.size(), 0);
        dp[0] = nums[0];
        int ans = nums[0];
        for (int i = 1; i < nums.size(); i++) {
            if (dp[i - 1] + nums[i] > nums[i]) {
                dp[i] = dp[i - 1] + nums[i];
            } else {
                dp[i] = nums[i];
            }
            ans = max(ans, dp[i]);
        }
        return ans;
    }

    // dp下一个状态只取决于上一个状态，所以可以优化一下
    int maxSubArray2(vector<int>& nums) {
        int pre = 0, maxAns = nums[0];
        for (const auto &x: nums) {
            pre = max(pre + x, x);
            maxAns = max(maxAns, pre);
        }
        return maxAns;
    }
};