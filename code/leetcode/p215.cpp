#include <algorithm>
#include <vector>

using namespace std;

class Solution {
   public:
    // lazy method, AC
    int findKthLargest(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        return nums[nums.size() - k];
    }

    // quick select, O(nlogn), TLE :(
    int findKthLargest2(vector<int>& nums, int k) { 
        return quickSelect(nums, 0, nums.size() - 1, nums.size() - k);
     }

    int quickSelect(vector<int>& nums, int l, int r, int k) {
        if (l == r) {
            return nums[l];
        }
        int pivot = nums[l];
        int i = l;
        int j = r;
        while (i < j) {
            while (i < j && nums[j] >= pivot) {
                j--;
            }
            nums[i] = nums[j];
            while (i < j && nums[i] <= pivot) {
                i++;
            }
            nums[j] = nums[i];
        }
        nums[i] = pivot;
        if (i == k) {
            return pivot;
        } else if (i < k) {
            return quickSelect(nums, i + 1, r, k);
        } else {
            return quickSelect(nums, l, i - 1, k);
        }
    }

    // bucket sort, O(n), AC
    int findKthLargest3(vector<int>& nums, int k) { 
        int n = nums.size();
        int minVal = *min_element(nums.begin(), nums.end());
        int maxVal = *max_element(nums.begin(), nums.end());
        vector<int> bucket(maxVal - minVal + 1, 0);
        for (int i = 0; i < n; i++) {
            bucket[nums[i] - minVal]++;
        }
        // 每个桶可能不止一个
        int count = 0;
        for (int i = bucket.size() - 1; i >= 0; i--) {
            count += bucket[i];
            // 这个桶加完后超过 k 表示已经找到了
            if (count >= k) {
                return i + minVal;
            }
        }
        return -1;
    }
};