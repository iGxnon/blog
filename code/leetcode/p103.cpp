#include <algorithm>
#include <queue>
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
    vector<vector<int>> zigzagLevelOrder(TreeNode *root) {
        vector<vector<int>> ans;
        if (!root) {
            return ans;
        }
        queue<TreeNode *> q;
        q.push(root);
        // 用一个 flag 交替反转
        bool flag = false;
        while (!q.empty()) {
            vector<int> l;
            int lsize = q.size();  // 这里得保存，不然在 for 循环中 q 会持续变大
            for (int i = 0; i < lsize; i++) {
                TreeNode *f = q.front();
                q.pop();
                l.push_back(f->val);
                if (f->left) q.push(f->left);
                if (f->right) q.push(f->right);
            }
            if (flag) {
                reverse(l.begin(), l.end()); // 反转
                flag = false;
            } else {
                flag = true;
            }
            ans.push_back(l);
        }
        return ans;
    }

    // 或者用双端队列，交替从两个方向 pop
};