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

    int b_search(vector<int>& nums, int l, int r, int target) {
        // [l, r)
        while (l < r) {
            int mid = l + ((r - l) >> 1);
            if (nums[mid] == target) return mid;
            if (nums[mid] > target) {
                r = mid;  // [l, r)，所以到 mid
            } else {
                l = mid + 1;
            }
        }
        return -1;
    }

    // 自己的写法
    int search2(vector<int>& nums, int target) {
        if (nums.empty()) {
            return -1;
        }
        int l = 0, r = nums.size();
        while (l < r) {
            int mid = l + ((r - l) >> 1);
            if (nums[mid] == target) return mid;
            if (nums[mid] > nums[0]) {
                if (nums[0] <= target && nums[mid] > target) { // 注意这里判断范围不能只判断 nums[mid] > target
                    return b_search(nums, 0, mid, target);
                }
                l = mid + 1;
            } else {
                if (target > nums[mid] && target <= nums[nums.size() - 1]) { // 同理，这里判断范围不能只判断 target > nums[mid]
                    return b_search(nums, mid + 1, nums.size(), target);
                }
                r = mid;
            }
        }
        return -1;
    }
};

int main() {
    vector<int> data = {4, 5, 6, 7, 0, 1, 2};
    Solution s;
    s.search2(data, 0);
    return 0;
}