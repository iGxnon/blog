class Solution {
public:
    int minDistance(string word1, string word2) {
        int n1 = word1.size();
        int n2 = word2.size();

        if (n1 * n2 == 0) return max(n1, n2);

        // dp[i][j] 表示 word1 中 [0, i) 和 word2 中 [0, j) （注意是不包括 i, j）的最短编辑距离
        int dp[501][501];
        memset(dp, 0, sizeof(dp));
        for (int i = 0; i < 501; i ++) {
            dp[i][0] = i;
            dp[0][i] = i;
        }

        for (int i = 1; i <= n1; i ++)
            for (int j = 1; j <= n2; j ++) {
                int left = dp[i-1][j] + 1;
                int down = dp[i][j-1] + 1;
                int left_down = dp[i-1][j-1];
                if (word1[i-1] != word2[j-1]) // 判断末尾的字符是否相等，相等则不用修改
                    left_down ++;
                dp[i][j] = min({left, down, left_down});
            }

        return dp[n1][n2];
    }
};
