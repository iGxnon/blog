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
    int ans = -1001;
    int maxPathSum(TreeNode *root) {
        treeSum(root);
        return ans;
    }

    int treeSum(TreeNode *root) {
        int cur = root->val;
        int sumL = 0, sumR = 0;
        if (root->left) {
            sumL = treeSum(root->left);
            cur = max(cur, cur + sumL);
        }
        if (root->right) {
            sumR = treeSum(root->right);
            cur = max(cur, cur + sumR);
        }
        // 此时 cur = max(root->val, root->val + sumL, root->val + sumR,
        // root->val + sumL + sumR)， ans 就来自于各个子树路径和的计算过程中
        ans = max(ans, cur);

        // 但是不能直接返回 cur，需要剔除 root->val + sumL + sumR 这种情况
        // 的情况（即，不能同时选择左右两条路径，然后返回给上层，上层只能选择一条路径）

        if (sumL == 0 ||
            sumR == 0) {  // 这种情况表示左右子树有一个是空的（或者加起来是
                          // 0，当它为空），这时候可以直接返回了
            return cur;
        }
        // 只能选左右子树最大的那个
        if (sumL > 0 && sumL > sumR) {
            return root->val + sumL;
        }
        if (sumR > 0 && sumR > sumL) {
            return root->val + sumR;
        }
        // 左右子树都是负数，那就不选了！
        return root->val;
    }

    // 上面的简化一下，就是这样
    int treeSum2(TreeNode *root) {
        int sumL = 0, sumR = 0;
        if (root->left) {
            sumL = treeSum(root->left);
        }
        if (root->right) {
            sumR = treeSum(root->right);
        }
        // ans 来自 max(root->val, root->val + sumL, root->val + sumR, root->val
        // + sumL + sumR)
        ans = max(ans, max({root->val, root->val + sumL, root->val + sumR,
                            root->val + sumL + sumR}));
        // 但只能返回给上层 max(root->val, root->val + sumL, root->val + sumR)
        return max({root->val, root->val + sumL, root->val + sumR});
    }
};