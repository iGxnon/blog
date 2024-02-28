// 424. Longest Repeating Character Replacement
// https://leetcode.com/problems/longest-repeating-character-replacement/description/

#include <string>
#include <vector>

using namespace std;

class Solution {
public:
    // O(N) ？？？？ 为什么
    int characterReplacement(string s, int k) {
        vector<int> arr(26, 0);
        int l = 0, res = 0, m = 0;
        for (int r = 0; r < s.size(); ++r) {
            arr[s[r] - 'A']++;
            m = max(m, arr[s[r] - 'A']);
            // 比较抽象，如果滑动窗口大小
            if (r - l + 1 - m > k) {
                arr[s[l] - 'A']--;
                l++;
            }
            res = max(res, r - l + 1);
        }
        return res;
    }
};