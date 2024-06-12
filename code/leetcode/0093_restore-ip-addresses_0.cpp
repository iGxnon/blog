class Solution {
   public:
    vector<string> ans;
    vector<string> parts;

    vector<string> restoreIpAddresses(string s) {
        dfs(s, 0);
        return ans;
    }

    void dfs(string raw, int pos) {
        if (pos == raw.size()) {
            if (parts.size() == 4) {
                string a = parts[0];
                for (int i = 1; i < parts.size(); i++) {
                    a = a + "." + parts[i];
                }
                ans.push_back(a);
            }
            return;
        }
        // 排除 0
        if (raw[pos] == '0') {
            parts.push_back("0");
            dfs(raw, pos + 1);
            parts.pop_back();
            return;
        }
        // 搜下面的情况
        int n = 0;
        for (int i = pos; i < raw.size(); i++) {
            n++;
            string sub = raw.substr(pos, n);
            if (stoi(sub) <= 255) {
                parts.push_back(sub);
                dfs(raw, pos + n);
                parts.pop_back();
            }
            if (n == 3) break;
        }
    }
};