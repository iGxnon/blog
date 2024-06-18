class Solution {
   public:
    int coinChange(vector<int>& coins, int amount) {
        unordered_set<int> s(coins.begin(), coins.end());
        int dp[10001];
        fill_n(dp, 10001, -1);
        dp[0] = 0;
        // dp[i] = coins.count(i) > 0 ? 1 : min([dp[i - coin] + 1 for coin in
        // coins], i - coin > 0 && dp[i - coin] != -1)
        for (int i = 1; i <= amount; i++) {
            if (s.count(i) > 0)
                dp[i] = 1;
            else {
                int mm = INT_MAX;
                for (auto& coin : coins) {
                    if (i - coin > 0 && dp[i - coin] != -1)
                        mm = min(mm, dp[i - coin] + 1);
                }
                dp[i] = mm == INT_MAX ? -1 : mm;
            }
        }
        return dp[amount];
    }
};