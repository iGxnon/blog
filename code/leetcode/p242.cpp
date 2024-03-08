// 242. Valid Anagram
// https://leetcode.com/problems/valid-anagram/description/

#include <string>
#include <unordered_map>

using namespace std;

class Solution {
   public:
    bool isAnagram(string s, string t) {
        unordered_map<char, int> store;
        for (auto ch : s) {
            store[ch]++;
        }
        for (auto ch : t) {
            store[ch]--;
        }
        for (auto ch : store) {
            if (ch.second != 0) {
                return false;
            }
        }
        return true;
    }
};
