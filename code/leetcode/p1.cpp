// 1. Two Sum
// https://leetcode.com/problems/two-sum/description/

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

class Solution {
   public:
    // O(N^2)
    vector<int> twoSum(vector<int>& nums, int target) {
        for (int i = 0; i < nums.size(); ++i) {
            for (int j = i + 1; j < nums.size(); ++j) {
                if (nums[i] + nums[j] == target) {
                    return {i, j};
                }
            }
        }
        return {};
    }

    // O(NlogN), two pointers
    // 有意思的是这个会比下面的 O(N) 在测评机里更快一点
    vector<int> twoSum2(vector<int>& nums, int target) {
        vector<pair<int, int>> nums_pos;
        for (int i = 0; i < nums.size(); ++i) {
            nums_pos.push_back(make_pair(nums[i], i));
        }
        sort(nums_pos.begin(), nums_pos.end());
        int i = 0;
        int j = nums_pos.size() - 1;
        for (;;) {
            int sum = nums_pos[i].first + nums_pos[j].first;
            if (sum == target) {
                return {nums_pos[i].second, nums_pos[j].second};
            }
            if (sum > target) {
                j--;
                continue;
            }
            if (sum < target) {
                i++;
                continue;
            }
        }
        return {};
    }

    // O(N), set
    vector<int> twoSum3(vector<int>& nums, int target) {
        unordered_map<int, int> numMap;
        int n = nums.size();

        for (int i = 0; i < n; i++) {
            int complement = target - nums[i];
            if (numMap.count(complement)) {
                return {numMap[complement], i};
            }
            numMap[nums[i]] = i;
        }

        return {};
    }
};