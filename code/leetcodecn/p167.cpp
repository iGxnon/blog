// 167. Two Sum II - Input Array Is Sorted
// https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/description/

#include <vector>

using namespace std;

class Solution {
   public:
    // 和 p1 类似
    vector<int> twoSum(vector<int>& numbers, int target) {
        int i = 0;
        int j = numbers.size() - 1;
        while (i < j) {
            int res = numbers[i] + numbers[j];
            if (res == target) {
                return {i + 1, j + 1};
            }
            if (res < target) {
                i++;
                continue;
            }
            if (res > target) {
                j--;
                continue;
            }
        }
        return {};
    }
};