class Solution {
   public:
    int myAtoi(string s) {
        int res = 0, sign = 1, start = 0, boundary = INT_MAX / 10;
        if (s.empty()) return 0;
        for (; start < s.size() && s[start] == ' '; start++)
            ;
        if (s[start] == '-') {
            start++;
            sign = -1;
        } else if (s[start] == '+') {
            start++;
        }
        for (; start < s.size(); start++) {
            if (s[start] < '0' || s[start] > '9') break;
            if (res > boundary ||
                res == boundary && (s[start] - '0') > (INT_MAX % 10))
                return sign > 0 ? INT_MAX : INT_MIN;
            res = (res * 10) + (s[start] - '0');
        }
        return res * sign;
    }
};