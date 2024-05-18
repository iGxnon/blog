struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
   public:
    // 后序遍历，如果发现有 p q 则返回 p q，如果左右子树分别发现一个，那么根就是最近公共祖先
    // 如果只有一边子树有，那么 p q 都在同一边，返回 p 或者 q 本身
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (root == q || root == p || !root) return root;
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        if (left && right) return root;
        if (!left) return right;
        return left;
    }
};