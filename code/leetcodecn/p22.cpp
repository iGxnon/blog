class Solution {
public:
    vector<string> ans;
    string op;

    // 回溯，l 表示当前还剩多少个 ( 没用，r 表示当前可以填充多少个 )
    void dfs(int l, int r) {
        if (l == 0 && r == 0) {
            ans.push_back(op);
            return;
        }
        if (l > 0) {
            op.push_back('(');
            dfs(l - 1, r + 1); // 少一个 (，多一个 )
            op.pop_back();
        }
        if (r > 0) {
            op.push_back(')');
            dfs(l, r - 1); // 少一个 )，l 不变
            op.pop_back();
        }
    }

    vector<string> generateParenthesis(int n) {
        // 一开始可以填 n 个 (，0 个 )
        dfs(n, 0);
        return ans;
    }
};