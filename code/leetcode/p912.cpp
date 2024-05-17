
#include <cstdlib>
#include <vector>
using namespace std;

class Solution {
   public:
    // 快排板子题
    // - 内部排序，不需要外部存储
    vector<int> sortArray(vector<int>& nums) {
        quickSort(nums, 0, nums.size() - 1);
        return nums;
    }

    void quickSort(vector<int>& nums, int l, int r) {
        if (l < r) {
            int pivot = partition(nums, l, r);
            quickSort(nums, l, pivot - 1);
            quickSort(nums, pivot + 1, r);
        }
    }

    int partition(vector<int>& nums, int l, int r) {
        int pivot = nums[l];
        while (l < r) {
            while (l < r && nums[r] >= pivot) r--; // 这里判断 l < r 和 >=
            nums[l] = nums[r];
            while (l < r && nums[l] <= pivot) l++;
            nums[r] = nums[l];
        }
        nums[l] = pivot;
        return l;
    }

    // 若只快排（
    // 开了一堆浪费的空间
    vector<int> sortArray2(vector<int>& nums) { return quickSort2(nums); }

    vector<int> quickSort2(vector<int>& nums) {
        if (nums.size() <= 1) {
            return nums;
        }
        vector<int> less;
        vector<int> more;
        int pivot = nums[0];
        for (int i = 1; i < nums.size(); i++) {  // 注意从 1 开始
            if (nums[i] <= pivot) {
                less.push_back(nums[i]);
            }
            if (nums[i] > pivot) {
                more.push_back(nums[i]);
            }
        }
        vector<int> sortedL = quickSort2(less);
        sortedL.push_back(pivot);
        vector<int> sortedR = quickSort2(more);
        for (int& num : sortedR) {
            sortedL.push_back(num);
        }
        return sortedL;
    }
};