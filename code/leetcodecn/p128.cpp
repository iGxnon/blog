class Solution {
   public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> s(nums.begin(), nums.end());
        int res = 0;
        for (int num : nums) {
            if (s.find(num - 1) == s.end()) {  // 如果它是第一个
                int cur = num;
                int curStreak = 1;
                while (s.find(cur + 1) != s.end()) {  // 往后找
                    cur++;
                    curStreak++;
                }
                res = max(res, curStreak);
            }
        }
        return res;
    }
};