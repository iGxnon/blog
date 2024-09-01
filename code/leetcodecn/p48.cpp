class Solution {
public:
    void rotate(vector<vector<int>>& matrix) {
        vector<int> order; // 不能用矩阵，那我用数组（
        int n = matrix.size();
        for (int j = 0; j < n; j ++) {
            for (int i = n - 1; i >= 0; i --)  {
                order.push_back(matrix[i][j]);
            }
        }
        for (int m = 0; m < order.size(); m ++) {
            matrix[m/n][m%n] = order[m];
        }
    }

    void rotate(vector<vector<int>>& matrix) {
        int n = matrix.size();
        // 上下翻转
        for (int i = 0; i < n / 2; i ++) {
            for (int j = 0; j < n; j ++) {
                swap(matrix[i][j], matrix[n-i-1][j]);
            }
        }
        // 主对角线反转 _| 形反转
        for (int i = 0; i < n; i ++) {
            for (int j = 0; j < i; j ++) {
                // i = 1, j = 0;
                swap(matrix[i][j], matrix[j][i]);
            }
        }
    }
};