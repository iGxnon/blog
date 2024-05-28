#include <algorithm>
#include <string>

using namespace std;

class Solution {
   public:
    string addStrings(string num1, string num2) {
        string ans = "";
        int c1 = (int)num1.size();
        int c2 = (int)num2.size();
        int cm = max(c1, c2);
        int carry = 0;
        for (int i = 1; i <= cm; i++) {
            int sum = 0;
            if (c1 - i >= 0) {
                sum += num1[c1 - i] - '0';
            }
            if (c2 - i >= 0) {
                sum += num2[c2 - i] - '0';
            }
            sum += carry;
            ans.push_back('0' + (sum % 10));
            carry = sum / 10;
        }
        // 最后记得带上 carry
        if (carry) {
            ans.push_back('0' + carry);
        }
        reverse(ans.begin(), ans.end());
        return ans;
    }
};