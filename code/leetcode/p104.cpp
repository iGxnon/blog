#include <queue>

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
    // 层序遍历，最直接的想法
    int maxDepth(TreeNode *root) {
        if (!root) return 0;
        int ans = 0;
        queue<TreeNode *> q;
        q.push(root);
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                TreeNode *f = q.front();
                q.pop();
                if (f->left) q.push(f->left);
                if (f->right) q.push(f->right);
            }
            ans += 1;
        }
        return ans;
    }

    // 递归
    int maxDepth2(TreeNode *root) {
        if (!root) return 0;
        if (!root->left && !root->right) return 1;
        if (!root->left) return 1 + maxDepth2(root->right);
        if (!root->right) return 1 + maxDepth2(root->left);
        return 1 + max(maxDepth2(root->left), maxDepth2(root->right));
    }

    int result;
    // 前序遍历，回溯
    void getdepth(TreeNode* node, int depth) {
        result = depth > result ? depth : result; // 中
        if (!node->left && !node->right) return ;
        if (node->left) { // 左
            depth++;    // 深度+1
            getdepth(node->left, depth);
            depth--;    // 回溯，深度-1
        }
        if (node->right) { // 右
            depth++;    // 深度+1
            getdepth(node->right, depth);
            depth--;    // 回溯，深度-1
        }
        return ;
    }

    int maxDepth3(TreeNode* root) {
        if (!root) return 0;
        result = 0;
        getdepth(root, 1);
        return result;
    }
};