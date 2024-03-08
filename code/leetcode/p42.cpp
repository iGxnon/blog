// 42. Trapping Rain Water
// https://leetcode.com/problems/trapping-rain-water/description/

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class Solution {
   public:
    // TLE: 318 / 322
    // O(N*K*K)
    // 对每一个空气方块左右扫描，如果都有 block 则可以存水
    int trap(vector<int>& height) {
        int layer = *max_element(height.begin(), height.end());
        int sum = 0;
        while (layer--) {
            for (int i = 1; i < height.size() - 1; ++i) {
                if (height[i] > layer) {  // not air
                    continue;
                }
                // scan left & right
                int l = i, r = i;
                l--;
                r++;
                for (;;) {
                    if (height[l] > layer && height[r] > layer) {
                        sum++;
                        break;
                        ;
                    }
                    if (height[l] <= layer) {
                        l--;
                    }
                    if (height[r] <= layer) {
                        r++;
                    }
                    if (l == -1 || r == height.size()) {
                        // not water
                        break;
                    }
                }
            }
        }
        return sum;
    }

    // TLE: 320 / 322
    // O(N*K)
    // 对每一层扫描，记录多个 block 中间的空隙数量
    int trap2(vector<int>& height) {
        int layer = *max_element(height.begin(), height.end());
        int sum = 0;
        while (layer--) {
            int l = -1;
            for (int i = 0; i < height.size(); ++i) {
                if (height[i] > layer) {  // block
                    if (l == -1) {
                        l = i;
                        continue;
                    }
                    sum += i - l - 1;
                    l = i;
                }
            }
        }
        return sum;
    }

    // AC
    // O(N)
    // 比较抽象，宏观层面理解：想象成一个碗，记录两边最高点，扫描碗里低于最低边的空间即可。
    int trap3(vector<int>& h) {
        int l = 0, r = h.size() - 1, lmax = -1, rmax = -1, ans = 0;
        while (l < r) {
            lmax = max(lmax, h[l]);
            rmax = max(rmax, h[r]);
            ans += (lmax < rmax) ? lmax - h[l++] : rmax - h[r--];
        }
        return ans;
    }
};
