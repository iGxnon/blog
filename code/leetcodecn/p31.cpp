class Solution {
   public:
    void nextPermutation(vector<int>& nums) {
        int n = nums.size() - 1;
        while (n > 0 && nums[n] <= nums[n - 1]) {
            n--;
        }
        // [n, nums.size()) 为降序，且 nums[n] > nums[n - 1]
        if (n == 0) {
            // 如果全部是降序则整体反转
            reverse(nums.begin(), nums.end());
        } else {
            // 在 [n, nums.size()) 内找到一个较小的 nums[k]，使得 nums[k] > nums[n - 1]
            int i = n;
            while (i < nums.size() && nums[i] > nums[n - 1]) {
                i++;
            }
            // k = i - 1
            swap(nums[n - 1], nums[i - 1]); // 交换 nums[n - 1] 和 nums[k]
            // 反转 [n, nums.size()) 为升序
            reverse(nums.begin() + n, nums.end());
        }
    }
};