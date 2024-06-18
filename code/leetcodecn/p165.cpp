class Solution {
   public:
    vector<string> split(string s, string delimiter) {
        size_t start = 0, end, d_len = delimiter.size();
        vector<string> ans;
        while ((end = s.find(delimiter, start)) != string::npos) {
            ans.push_back(s.substr(start, end - start));
            start = end + d_len;
        }
        ans.push_back(s.substr(start));
        return ans;
    }

    int compareVersion(string version1, string version2) {
        vector<string> v1 = split(version1, ".");
        vector<string> v2 = split(version2, ".");
        for (int i = 0; i < v1.size() || i < v2.size(); i ++) {
            int vv1 = 0, vv2 = 0;
            if (i < v1.size()) {
                vv1 = stoi(v1[i]);
            }
            if (i < v2.size()) {
                vv2 = stoi(v2[i]);
            }
            if (vv1 > vv2) {
                return 1;
            }
            if (vv1 < vv2) {
                return -1;
            }
        }
        return 0;
    }
};