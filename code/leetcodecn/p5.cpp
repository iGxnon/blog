
#include <string>

using namespace std;

class Solution {
   public:
    // dp[i, j] = dp[i + 1, j - 1] && (s[i] == s[j])
    string longestPalindrome(string s) {
        bool dp[1009][1009];
        int maxAns = 0;
        int l = 0;
        // 注意递推方程的性质，i 依赖 i + 1，j 依赖 j - 1
        for (int j = 0; j < s.length(); j++) {
            for (int i = 0; i <= j; i++) {
                if (i == j) {
                    dp[i][j] = true;
                } else if (i == j - 1 && s[i] == s[j]) {
                    dp[i][j] = true;
                } else {
                    dp[i][j] = dp[i + 1][j - 1] && s[i] == s[j];
                }
                if (dp[i][j] && j - i + 1 > maxAns) {
                    maxAns = j - i + 1;
                    l = i;
                }
            }
        }
        return s.substr(l, maxAns);
    }

    // TODO: 中心拓展
};