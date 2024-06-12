#include <string>
#include <vector>

using namespace std;

class Solution {
   public:
    // 不会捏
    int minDistance(string word1, string word2) {
        int n = word1.length();
        int m = word2.length();

        // 有一个字符串为空串
        if (n * m == 0) return n + m;

        // DP 数组
        // 约定 A 为 word1 中 0-i 的子字符串，B 为 work2 中 0-j 的子字符串
        // dp[i][j] 为 A 到 B 的编辑距离
        vector<vector<int>> D(n + 1, vector<int>(m + 1));

        // 边界状态初始化
        for (int i = 0; i < n + 1; i++) {
            D[i][0] = i;
        }
        for (int j = 0; j < m + 1; j++) {
            D[0][j] = j;
        }

        // 计算所有 DP 值
        for (int i = 1; i < n + 1; i++) {
            for (int j = 1; j < m + 1; j++) {
                // 当前编辑距离可由之前的 A 串添加 1 个字符
                int left = D[i - 1][j] + 1;
                // 同理，也可有 B 串添加一个字符
                int down = D[i][j - 1] + 1;
                // 如果之前的结果本就相等，则不需要修改
                int left_down = D[i - 1][j - 1];
                // 不相等则需要替换一个字符
                if (word1[i - 1] != word2[j - 1]) left_down += 1;
                // 四种方案里最小的
                D[i][j] = min(left, min(down, left_down));
            }
        }
        return D[n][m];
    }
};