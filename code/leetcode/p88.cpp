#include <vector>

using namespace std;

class Solution {
   public:
    // 常规写法
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        vector<int> ans;
        int i = 0, j = 0;
        while (i < m || j < n) {
            if (j >= n) {
                ans.push_back(nums1[i]);
                i++;
                continue;
            }
            if (i >= m) {
                ans.push_back(nums2[j]);
                j++;
                continue;
            }
            if (nums1[i] <= nums2[j]) {
                ans.push_back(nums1[i]);
                i++;
            } else {
                ans.push_back(nums2[j]);
                j++;
            }
        }
        copy(ans.begin(), ans.end(), nums1.begin());
    }

    // 因为 nums1 后面是空的，所以可以考虑从后往前排
};