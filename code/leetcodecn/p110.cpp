
#include <algorithm>
#include <cstdlib>
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
    // 按定义写递归，对于每个节点的左右子树的高度相差不会大一 1
    bool isBalanced(TreeNode *root) {
        if (!root) return true;
        if (!root->left && !root->right) return true;
        if (!root->left) return depth(root->right) <= 1;
        if (!root->right) return depth(root->left) <= 1;
        // 优化点：depth() 可以存储
        return abs(depth(root->left) - depth(root->right)) <= 1 &&
               isBalanced(root->left) && isBalanced(root->right);
    }

    int depth(TreeNode *root) {
        if (!root) return 0;
        if (!root->left && !root->right) return 1;
        if (!root->left) return 1 + depth(root->right);
        if (!root->right) return 1 + depth(root->left);
        return 1 + max(depth(root->left), depth(root->right));
    }

    bool isBalanced2(TreeNode *root) {
        if (!root) return true;
        return balancedOrDepth(root) != -1;
    }

    // 搜索高度的同时还判断是否平衡，不平衡则直接返回结果
    int balancedOrDepth(TreeNode *root) {
        if (!root) return 0;
        if (!root->left && !root->right) return 1;
        int dl = balancedOrDepth(root->left);
        if (dl == -1) return -1;
        int dr = balancedOrDepth(root->right);
        if (dr == -1) return -1;
        if (abs(dl - dr) > 1) return -1;
        return 1 + max(dl, dr);
    }
};