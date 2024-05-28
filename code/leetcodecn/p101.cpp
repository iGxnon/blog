#include <stack>

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
    // 思路：变种后序遍历，从最后一层的两端开始，如果不相等则终止
    bool isSymmetric(TreeNode *root) {
        if (!root) return true;
        return compare(root->left, root->right);
    }

    bool compare(TreeNode *l, TreeNode *r) {
        if (!l && !r) return true;
        if (l && !r || (!l && r) || l->val != r->val) return false;
        // 比较左子树的左边和右子树的右边 && 左子树的右边和右子树的左边
        return compare(l->left, r->right) && compare(l->right, r->left);
    }

    // 另一种比较常规的想法，用栈一个个比较
    bool isSymmetric2(TreeNode *root) {
        if (!root) return true;
        stack<TreeNode *> st;
        st.push(root->left);
        st.push(root->right);
        while (!st.empty()) {
            TreeNode *leftNode = st.top();
            st.pop();
            TreeNode *rightNode = st.top();
            st.pop();
            if (!leftNode && !rightNode) {
                continue;
            }
            if ((!leftNode || !rightNode ||
                 (leftNode->val != rightNode->val))) {
                return false;
            }
            st.push(leftNode->left);
            st.push(rightNode->right);
            st.push(leftNode->right);
            st.push(rightNode->left);
        }
        return true;
    }
};