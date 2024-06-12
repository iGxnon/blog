class Solution {
   public:
    // 这个递归方程为什么有问题？
    // dp[i][j] = max(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]) + int(text1[i -1] == text2[j - 1])
    int longestCommonSubsequence(string text1, string text2) {
        // dp[i][j] = dp[i-1][j-1] + 1, text1[i -1] == text2[j - 1];
        // dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]), text1[i -1] != text2[j - 1];
        int m = text1.length(), n = text2.length();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1));
        for (int i = 1; i <= m; i++) {
            char c1 = text1.at(i - 1);
            for (int j = 1; j <= n; j++) {
                char c2 = text2.at(j - 1);
                if (c1 == c2) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        return dp[m][n];
    }
};
