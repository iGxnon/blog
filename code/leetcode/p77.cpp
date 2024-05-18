
#include <vector>

using namespace std;

class Solution {
   public:
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> ans;
        vector<int> comb;
        vector<int> vis(n + 1, 0);
        backtrace(ans, comb, vis, n, k);
        return ans;
    }

    void backtrace(vector<vector<int>>& ans, vector<int>& comb,
                   vector<int>& vis, int n, int k) {
        if (!k) {
            ans.emplace_back(comb);
            return;
        }
        for (int i = 1; i <= n; i++) {
            if (vis[i]) continue;  // 记录访问后的节点
            if (!comb.empty() && i < comb.back())
                continue;  // 保证 comb 内是升序
            vis[i] = 1;
            comb.push_back(i);
            backtrace(ans, comb, vis, n, k - 1);
            comb.pop_back();
            vis[i] = 0;
        }
    }

    vector<vector<int>> result;  // 存放符合条件结果的集合
    vector<int> path;            // 用来存放符合条件结果

    void backtracking(int n, int k, int startIndex) {
        if (path.size() == k) {
            result.push_back(path);
            return;
        }
        for (int i = startIndex; i <= n - (k - path.size()) + 1; i++) {  // 剪枝
            path.push_back(i);          // 处理节点
            backtracking(n, k, i + 1);  // 递归
            path.pop_back();            // 回溯，撤销处理的节点
        }
    }

    vector<vector<int>> combine2(int n, int k) {
        backtracking(n, k, 1);
        return result;
    }
};