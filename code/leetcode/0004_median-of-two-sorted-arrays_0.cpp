class Solution {
   public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int i = 0, j = 0, k = 0, pre = 0, cur = 0, n1 = nums1.size(),
            n2 = nums2.size();
        int m = n1 + n2;
        int mid = m >> 1;
        while (k <= mid) {
            pre = cur;
            if (i < n1 && j < n2) {
                if (nums1[i] < nums2[j]) {
                    cur = nums1[i];
                    i++;
                } else {
                    cur = nums2[j];
                    j++;
                }
            } else if (i < n1) {
                cur = nums1[i];
                i++;
            } else {
                cur = nums2[j];
                j++;
            }
            k++;
        }
        if (m % 2 == 0) {
            return (float)(pre + cur) / 2;
        }
        return (float)cur;
    }
};