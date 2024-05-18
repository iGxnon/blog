#include <vector>

using namespace std;

class Solution {
   public:
    // 二分查找板子题
    int searchInsert(vector<int>& nums, int target) {
        int l = 0;
        int r = nums.size() - 1;
        while (l <= r) {
            int mid = l + ((r - l) >> 1);
            if (nums[mid] > target) {
                r = mid - 1;
            } else if (nums[mid] < target) {
                l = mid + 1;
            } else {
                return mid;
            }
        }
        // 如果找不到，l 表示第一个大于 target 的值，也就是插入 target 的索引的位置
        return l;
    }
};