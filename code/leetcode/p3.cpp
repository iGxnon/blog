// 3. Longest Substring Without Repeating Characters
// https://leetcode.com/problems/longest-substring-without-repeating-characters/description/

#include <algorithm>
#include <set>
#include <string>
#include <vector>

using namespace std;

class Solution {
   public:
    // O(N)
    // 如果找到第一个重复的字符，那么窗口的左边需要提前到之前这个字符出现的位置的后面，
    // 这样窗口内均不重复
    int lengthOfLongestSubstring(string s) {
        int l = 0, r = 0, m = 0;
        // 记录 char => index
        vector<int> setIdx(128, -1);
        for (; r < s.length(); r++) {
            // !! 如果当前字符的 index 超过 l（则在区间内，说明重复了）
            if (setIdx[s[r]] >= l) {
                l = setIdx[s[r]] + 1;
            }
            setIdx[s[r]] = r;
            m = max(m, r - l + 1);
        }
        return m;
    }

    // O(N^2)
    int lengthOfLongestSubstring2(string s) {
        int m = 0;
        for (int i = 0; i < s.length(); i++) {
            set<int> dup;
            for (int j = i; j < s.length(); j++) {
                if (dup.count(s[j])) {
                    break;
                }
                m = max(m, j - i + 1);
                dup.insert(s[j]);
            }
        }
        return m;
    }
};
