// 125. Valid Palindrome
// https://leetcode.com/problems/valid-palindrome/description/

#include <cctype>
#include <string>

using namespace std;

class Solution {
   public:
    bool isValid(char s) {
        return (s >= 'A' && s <= 'Z') || (s >= 'a' && s <= 'z') ||
               (s >= '0' && s <= '9');
    }

    bool isPalindrome(string s) {
        int i = 0;
        int j = s.size() - 1;
        while (i < j) {
            if (!isValid(s[i])) {
                i++;
                continue;
            }
            if (!isValid(s[j])) {
                j--;
                continue;
            }
            if (tolower(s[i]) != tolower(s[j])) {
                return false;
            }
            i++;
            j--;
        }
        return true;
    }
};