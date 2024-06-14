class Solution {
   public:
    int mySqrt(int x) {
        long long l = 0, r = x;
        while (l < r) {
            long long mid = (l + r + 1) / 2;
            if (x >= mid * mid) {
                l = mid;
            } else {
                r = mid - 1;
            }
        }
        return r;
    }

    int mySqrt(int x) {
        int l = 0, r = x, ans = -1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if ((long long)mid * mid <= x) {
                ans = mid;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }
        return ans;
    }
};