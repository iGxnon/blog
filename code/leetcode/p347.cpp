// 347. Top K Frequent Elements
// https://leetcode.com/problems/top-k-frequent-elements/description/

#include <algorithm>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

class Solution {
public:
    // O(NlogN)
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> store;
        for (auto num : nums) {
            store[num] ++;
        }
        vector<pair<int, int>> freq;
        for (auto v : store) {
            freq.push_back(v);
        }
        sort(freq.begin(), freq.end(), [](const auto& a,const auto& b){
            return a.second > b.second;
        });
        vector<int> res;
        for (int i = 0; i < k; i ++) {
            res.push_back(freq[i].first);
        }
        return res;
    }

    // 或者根据最大的 freq 初始化一个 bucket，填满 bucket 然后查找算法复杂度是 0(N)
    // （有限元的排序都可以牺牲空间创建有序 bucket 来检索排序）
};