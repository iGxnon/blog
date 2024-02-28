// 121. Best Time to Buy and Sell Stock
// https://leetcode.com/problems/best-time-to-buy-and-sell-stock/description/

#include <vector>

using namespace std;

class Solution {
public:
    // O(N)
    // 先有股票最低点，然后才有可能有比之前还多的利润
    int maxProfit(vector<int>& prices) {
        int buy = 0, profit = 0;
        for (int i = 0; i < prices.size(); ++ i) {
            if (prices[i] < prices[buy]) {
                buy = i;
            }
            int gain = prices[i] - prices[buy];
            if (gain > profit) {
                profit = gain;
            }
        }
        return profit;
    }

};