
#include <queue>
#include <utility>
#include <vector>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right)
        : val(x), left(left), right(right) {}
};

class Solution {
   public:
    // BFS，队列里同时还存储了层级
    vector<vector<int>> levelOrder(TreeNode *root) {
        queue<pair<TreeNode *, int>> q;
        if (!root) {
            return {};
        }
        vector<vector<int>> ans;
        q.push({root, 0});
        while (!q.empty()) {
            TreeNode *head = q.front().first;
            int l = q.front().second;
            q.pop();
            if (ans.size() <= l) {
                vector<int> level;
                level.push_back(head->val);
                ans.push_back(level);
            } else {
                ans[l].push_back(head->val);
            }
            if (head->left) {
                q.push({head->left, l + 1});
            }
            if (head->right) {
                q.push({head->right, l + 1});
            }
        }
        return ans;
    }

    // 题解 BFS，没有存储层级，但是 BFS 每次遍历是遍历一层，所以当前队列还剩下的就是当前层的数量
    vector<vector<int>> levelOrder2(TreeNode *root) {
        vector<vector<int>> ret;
        if (!root) {
            return ret;
        }
        queue<TreeNode *> q;
        q.push(root);
        while (!q.empty()) {
            int currentLevelSize = q.size(); // 这里得保存，不然在 for 循环中 q 会持续变大
            ret.push_back(vector<int>());
            for (int i = 1; i <= currentLevelSize; ++i) {
                auto node = q.front();
                q.pop();
                ret.back().push_back(node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }

        return ret;
    }
};