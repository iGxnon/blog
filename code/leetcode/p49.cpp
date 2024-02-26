// 49. Group Anagrams
// https://leetcode.com/problems/group-anagrams/description/

#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class Solution {
public:
    // O(N*K*logK)
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> str_map;
        for (auto str : strs) {
            string str_cpy = str;
            sort(str_cpy.begin(), str_cpy.end());
            str_map[str_cpy].push_back(str);
        }
        vector<vector<string>> res;
        for (auto entry : str_map) {
            res.push_back(entry.second);
        }
        return res;
    }

    string getSignature(const string& s) {
        vector<int> count(26, 0);
        for (char c : s) {
            count[c - 'a']++;
        }

        stringstream ss;
        for (int i = 0; i < 26; i++) {
            if (count[i] != 0) {
                ss << (char)('a' + i) << count[i];
            }
        }
        return ss.str();
    }

    // O(N*K)
    // 把排序部分剪枝了，一共只有26个元素，在评测机上还不如上面那个 :P
    vector<vector<string>> groupAnagrams2(vector<string>& strs) {
        vector<vector<string>> ans;
        unordered_map<string, int> groups;
        for (const string& s : strs) {
            auto sig = getSignature(s);
            if (groups.count(sig)) {
                ans[groups[sig]].push_back(s);
            } else {
                groups[sig] = ans.size();
                ans.push_back({s});
            }
        }
        return ans;
    }
};