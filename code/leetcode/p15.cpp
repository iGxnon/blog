#include <algorithm>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

class Solution {
   public:
    // 常规解法，twoSum 升级版，会 TLE
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> ans;
        unordered_map<int, int> store;
        set<pair<int, pair<int, int>>> dedup;
        for (int i = 0; i < nums.size(); i++) {
            store[nums[i]] = i;
        }
        for (int i = 0; i < nums.size() - 1; i++) {
            for (int j = i + 1; j < nums.size(); j++) {
                int require = -(nums[i] + nums[j]);
                if (store.count(require) && store[require] != i &&
                    store[require] != j) {

                    // 这里的去重比较卡时间，可以手动排列组合
                    vector<int> an = {nums[i], nums[j], require};
                    sort(an.begin(), an.end());
                    if (dedup.count({an[0], {an[1], an[2]}})) {
                        continue;
                    }

                    dedup.insert({an[0], {an[1], an[2]}});
                    ans.push_back(an);
                }
            }
        }
        return ans;
    }

    // 双指针
    vector<vector<int>> threeSum2(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> ans;
        for (int k = 0; k < nums.size() - 2; k++) {
            if (nums[k] > 0) break;
            if (k > 0 && nums[k] == nums[k - 1]) continue;
            int l = k + 1, r = nums.size() - 1;
            while (l < r) {
                int sum = nums[l] + nums[r] + nums[k];
                if (sum == 0) {
                    ans.push_back({nums[l], nums[r], nums[k]});
                    while (l < r && nums[l] == nums[++l]); // && nums[l] == nums[++l] 先加 l + 1，然后跳过所有相等的
                    while (l < r && nums[r] == nums[--r]); // 同上
                }
                if (sum < 0) 
                    while (l < r && nums[l] == nums[++l]);
                if (sum > 0) 
                    while (l < r && nums[r] == nums[--r]);
            }
        }
        return ans;
    }
};