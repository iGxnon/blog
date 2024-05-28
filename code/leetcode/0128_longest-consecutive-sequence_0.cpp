class Solution {
   public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> s(nums.begin(), nums.end());
        int res = 0;
        for (int num : nums) {
            if (s.find(num - 1) == s.end()) {
                int cur = num;
                int curStreak = 1;
                while (s.find(cur + 1) != s.end()) {
                    cur++;
                    curStreak++;
                }
                res = max(res, curStreak);
            }
        }
        return res;
    }
};