#include <utility>
#include <vector>

using namespace std;

class Solution {
   public:
    vector<int> vis;
    vector<int> path;

    void dfs(vector<vector<int>>& ans, vector<int>& nums, int x) {
        if (x >= nums.size()) {
            ans.push_back(path);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (vis[i]) continue;
            vis[i] = 1;
            path.push_back(nums[i]);
            dfs(ans, nums, x + 1);
            vis[i] = 0;
            path.pop_back();
        }
    }

    // 回溯板子题，来自题解 dfs 暴搜，排列组合
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> ans;
        vis.resize(nums.size(), 0);
        dfs(ans, nums, 0);
        return ans;
    }

    void dfs2(vector<vector<int>>& ans, vector<int>& nums, int x) {
        if (x == nums.size()) {
            ans.emplace_back(nums);
            return;
        }
        for (int i = x; i < nums.size(); i ++) {
            swap(nums[i], nums[x]);
            dfs2(ans, nums, x + 1);
            swap(nums[i], nums[x]);
        }
    }

    // 官方题解，回溯，基于两两交换
    vector<vector<int>> permute2(vector<int>& nums) {
        vector<vector<int>> ans;
        dfs2(ans, nums, 0);
        return ans;
    }
};