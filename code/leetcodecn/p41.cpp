class Solution {
   public:
    // O(nlogn)
    int firstMissingPositive(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int bef = 0;
        for (auto n : nums) {
            if (n <= 0) continue;
            if (n > bef + 1) {  // n 可能等于 bef
                return bef + 1;
            }
            bef = n;
        }
        return bef + 1;
    }

    // O(n)
    int firstMissingPositive(vector<int>& nums) {
        unordered_set<int> s(nums.begin(), nums.end());
        int i = 1;
        while (1) {
            if (s.count(i) == 0) {
                return i;
            }
            i++;
        }
        return -1;
    }

    int firstMissingPositive(vector<int>& nums) {
        int n = nums.size();
        for (int i = 0; i < n; i++) {
            // 注意：如果交换的两个数一样的话，会陷入死循环，这时跳过即可
            while (nums[i] >= 1 && nums[i] <= n && nums[i] != i + 1 &&
                   nums[i] != nums[nums[i] - 1]) {
                swap(nums[i], nums[nums[i] - 1]);
            }
        }
        for (int i = 0; i < n; i++) {
            if (nums[i] != i + 1) {
                return i + 1;
            }
        }
        return n + 1;
    }
};