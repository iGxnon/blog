#include <cstring>
#include <vector>

using namespace std;

class Solution {
    const int direction[4][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

   public:
    // 常规写法
    int numIslands(vector<vector<char>>& grid) {
        int ans = 0;
        bool mark[300][300];
        memset(mark, 0, sizeof(mark));
        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[i].size(); j++) {
                if (grid[i][j] == '0' || mark[i][j]) {
                    continue;
                }
                dfs(mark, grid, i, j);
                ans++;
            }
        }
        return ans;
    }

    void dfs(bool mark[300][300], vector<vector<char>>& grid, int i, int j) {
        if (i < 0 || j < 0 || i >= grid.size() || j >= grid[i].size() ||
            grid[i][j] == '0') {
            return;
        }
        if (mark[i][j]) {
            return;
        }
        mark[i][j] = true;
        for (auto& d : direction) {
            int di = d[0];
            int dj = d[1];
            dfs(mark, grid, i + di, j + dj);
        }
    }
};