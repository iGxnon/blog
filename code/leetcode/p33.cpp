#include <vector>

using namespace std;

class Solution {
   public:
    // 变种二分
    int search(vector<int>& nums, int target) {
        int n = nums.size();
        if (!n) {
            return -1;
        }
        if (n == 1) {
            return nums[0] == target ? 0 : -1;
        }
        int l = 0, r = n - 1;
        while (l <= r) {
            int mid = (l + r) / 2;
            if (nums[mid] == target) return mid;
            // 检查 mid 在哪个有序区间内
            if (nums[0] <= nums[mid]) {
                // 第一个有序区间内，检查是否可以缩减到 0 ~ mid 范围内
                if (nums[0] <= target && target < nums[mid]) {
                    r = mid - 1;
                } else {
                    l = mid + 1;
                }
            } else {
                // 第二个有序区间内，检查是否可以缩减到 mid + 1 ~ n - 1 范围内
                if (nums[mid] < target && target <= nums[n - 1]) {
                    l = mid + 1;
                } else {
                    r = mid - 1;
                }
            }
        }
        return -1;
    }
};
