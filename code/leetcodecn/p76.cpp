#include <bits/stdc++.h>

using namespace std;

class Solution {
   public:
    unordered_map<char, int> require, owned;

    bool check() {
        for (auto& r : require) {
            if (owned.find(r.first) == owned.end()) return false;
            if (owned[r.first] < r.second) return false;
        }
        return true;
    }

    string minWindow(string s, string t) {
        int l = 0, r = 0;  // [l, r]

        for (auto& c : t) require[c]++;

        int min_len = INT_MAX;
        int ans_l = -1;

        while (r < s.size()) {
            // 添加 owned
            if (require.find(s[r]) != require.end()) {
                owned[s[r]]++;
            }
            while (check() && l <= r) {
                if (min_len > r - l + 1) {
                    min_len = r - l + 1;
                    ans_l = l;  // 这里如果用 substr 切的话会超出内存限制...
                                // 所以记录一下 ans_l
                }
                // 移除 l 时检查是否需要修改 owned
                if (require.find(s[l]) != require.end()) {
                    owned[s[l]]--;
                }
                l++;
            }
            r++;
        }

        return ans_l == -1 ? "" : s.substr(ans_l, min_len);
    }
};

int main() {
    Solution s;
    s.minWindow("ADOBECODEBANC", "ABC");
}