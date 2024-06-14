class Solution {
   public:
    int search(vector<int>& nums, int target) {
        int l = 0, r = nums.size();
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

    int search(vector<int>& nums, int target) {
        int l = 0, r = nums.size() - 1;
        // [l, r]
        while (l <= r) {
            int mid = l + ((r - l) >> 1);
            if (nums[mid] == target) return mid;
            if (nums[mid] > target) {
                r = mid - 1;  // [l, r]，所以 mid - 1
            } else {
                l = mid + 1;
            }
        }
        return -1;
    }
};