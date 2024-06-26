#include <vector>

using namespace std;

class Solution {
   public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        vector<int> ans;
        if (matrix.empty()) return ans;  // 若数组为空，直接返回答案
        int u = 0;                       // 上边界
        int l = 0;                       // 左边界
        int d = matrix.size() - 1;       // 下边界
        int r = matrix[0].size() - 1;    // 右边界
        while (true) {
            for (int i = l; i <= r; ++i)
                ans.push_back(matrix[u][i]);  // 向右移动直到最右
            if (++u > d) break;  // 增加上边界，上边界大于下边界，退出
            for (int i = u; i <= d; ++i) ans.push_back(matrix[i][r]);  // 向下
            if (--r < l) break;  // 重新设定有边界
            for (int i = r; i >= l; --i) ans.push_back(matrix[d][i]);  // 向左
            if (--d < u) break;  // 重新设定下边界
            for (int i = d; i >= u; --i) ans.push_back(matrix[i][l]);  // 向上
            if (++l > r) break;  // 重新设定左边界
        }
        return ans;
    }
};