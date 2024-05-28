#include <algorithm>

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
    int minDepth(TreeNode *root) {
        if (!root) return 0;
        return depth(root, 1);
    }

    int depth(TreeNode *root, int d) {
        // 如果是叶子节点的话，就到当前深度了
        if (!root->left && !root->right) return d;
        // 如果少了一边子树的话，就计算另一半子树
        if (!root->left) return depth(root->right, d + 1);
        if (!root->right) return depth(root->left, d + 1);
        // 两边子树最少的
        return min(depth(root->left, d + 1), depth(root->right, d + 1));
    }

    // 或者层序遍历，遇到第一个叶子节点就返回当前层数
};