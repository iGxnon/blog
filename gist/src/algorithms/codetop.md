# CodeTop

[3. 无重复字符的最长子串](https://leetcode.cn/problems/longest-substring-without-repeating-characters/description/)

```c++
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        vector<int> idx(128, -1);
        int l = 0, ans = 0;
        for (int r = 0; r < s.size(); r ++) {
            if (idx[s[r]] >= l) {
                l = idx[s[r]] + 1;
            }
            idx[s[r]] = r;
            ans = max(ans, r - l + 1);
        }
        return ans;
    }
};
```

[146. LRU缓存机制](https://leetcode.cn/problems/lru-cache/description/)

```c++
class Node {
public:
    int k, v;
    Node *pre, *nex;
    Node() : k(0), v(0), pre(nullptr), nex(nullptr) {}
    Node(int _k, int _v) : k(_k), v(_v), pre(nullptr), nex(nullptr) {}
};

class LRUCache {
    Node *head, *tail;
    int cap;
    unordered_map<int, Node *> store;

public:
    LRUCache(int capacity) : cap(capacity) {
        // dummy head & tail
        head = new Node();
        tail = new Node();
        head->nex = tail;
        tail->pre = head;
    }

    int get(int key) {
        if (!store.count(key)) {
            return -1;
        }
        Node *n = store[key];
        moveToHead(n);
        return n->v;
    }

    void put(int key, int value) {
        if (store.count(key)) {
            Node *n = store[key];
            n->v = value;
            moveToHead(n);
            return;
        }
        if (store.size() >= cap) {
            Node *toRemove = tail->pre;
            store.erase(toRemove->k);
            removeNode(toRemove);
        }
        Node *n = new Node(key, value);
        moveToHead(n);
        store[key] = n;
    }

    void removeNode(Node *n) {
        n->pre->nex = n->nex;
        n->nex->pre = n->pre;
    }

    void moveToHead(Node *n) {
        if ((n->pre && n->pre->nex == n) || (n->nex && n->nex->pre == n)) {
            removeNode(n);
        }
        n->pre = head;
        n->nex = head->nex;
        head->nex->pre = n;
        head->nex = n;
    }
};
```

[206. 反转链表](https://leetcode.cn/problems/reverse-linked-list/)

```c++
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode *pre = nullptr;
        ListNode *cur = head;
        while (cur) {
            ListNode *next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        return pre;
    }
};
```

[215. 数组中的第K个最大元素](https://leetcode.cn/problems/kth-largest-element-in-an-array/description/)

```cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        int n = nums.size();
        int minVal = *min_element(nums.begin(), nums.end());
        int maxVal = *max_element(nums.begin(), nums.end());
        vector<int> bucket(maxVal - minVal + 1, 0);
        for (int i = 0; i < n; i++) {
            bucket[nums[i] - minVal]++;
        }
        // 每个桶可能不止一个
        int count = 0;
        for (int i = bucket.size() - 1; i >= 0; i--) {
            count += bucket[i];
            // 这个桶加完后超过 k 表示已经找到了
            if (count >= k) {
                return i + minVal;
            }
        }
        return -1;
    }
};
```

[25. K 个一组翻转链表](https://leetcode.cn/problems/reverse-nodes-in-k-group/description/)

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        if (head == nullptr) return head;
        int i = k;
        ListNode *cur = head;
        while (cur && --i > 0) { // --i，提前一格退出，那么 cur 指向最后一个
            cur = cur->next;
        }
        // no more than k;
        if (cur == nullptr) return head;
        ListNode *nextHead = cur->next;
        ListNode *pre = nullptr;
        cur = head;
        while (cur != nextHead) {
            ListNode *tmp = cur->next;
            cur->next = pre;
            pre = cur;
            cur = tmp;
        }
        head->next = reverseKGroup(nextHead, k);
        return pre;
    }
};
```

[15. 三数之和](https://leetcode.cn/problems/3sum/description/)

```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> ans;
        for (int k = 0; k < nums.size() - 2; k++) {
            if (nums[k] > 0) break;
            if (k > 0 && nums[k] == nums[k - 1]) continue;
            int l = k + 1, r = nums.size() - 1;
            while (l < r) {
                int sum = nums[l] + nums[r] + nums[k];
                if (sum == 0) {
                    ans.push_back({nums[l], nums[r], nums[k]});
                    while (l < r && nums[l] == nums[++l]); // && nums[l] == nums[++l] 先加 l + 1，然后跳过所有相等的
                    while (l < r && nums[r] == nums[--r]); // 同上
                }
                if (sum < 0) 
                    while (l < r && nums[l] == nums[++l]);
                if (sum > 0) 
                    while (l < r && nums[r] == nums[--r]);
            }
        }
        return ans;
    }
};
```

[53. 最大子数组和](https://leetcode.cn/problems/maximum-subarray/description/)

```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int dp = nums[0];
        int ans = nums[0];
        for (int i = 1; i < nums.size(); i ++) {
            dp = max(dp + nums[i], nums[i]);
            ans = max(dp, ans);
        }
        return ans;
    }
};
```

[补充题4. 手撕快速排序](https://leetcode.cn/problems/sort-an-array/)

```cpp
class Solution {
public:
    vector<int> sortArray(vector<int>& nums) {
        quick_sort(nums, 0, nums.size() - 1);
        return nums;
    }

    void quick_sort(vector<int>& nums, int l, int r) {
        if (l < r) {
            int pivot = partition(nums, l, r);
            quick_sort(nums, l, pivot - 1);
            quick_sort(nums, pivot + 1, r);
        }
    }

    int partition(vector<int>& nums, int l, int r) {
        int key = nums[l];
        while (l < r) {
            while (l < r && nums[r] >= key) r --;
            nums[l] = nums[r];
            while (l < r && nums[l] <= key) l ++;
            nums[r] = nums[l];
        }
        nums[l] = key;
        return l;
    }
};
```

[21. 合并两个有序链表](https://leetcode.cn/problems/merge-two-sorted-lists/description/)

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode* dummy = new ListNode();
        ListNode* cur = dummy;
        ListNode* cur1 = list1;
        ListNode* cur2 = list2;
        while (cur1 || cur2) {
            if (!cur1) {
                cur->next = cur2;
                cur = cur->next;
                cur2 = cur2->next;
                continue;
            }
            if (!cur2) {
                cur->next = cur1;
                cur = cur->next;
                cur1 = cur1->next;
                continue;
            }
            if (cur1->val < cur2->val) {
                cur->next = cur1;
                cur = cur->next;
                cur1 = cur1->next;
            } else {
                cur->next = cur2;
                cur = cur->next;
                cur2 = cur2->next;
            }
        }
        return dummy->next;
    }
};
```

[5. 最长回文子串](https://leetcode.cn/problems/longest-palindromic-substring/description/)

```cpp
class Solution {
public:
    // dp[i, j] = dp[i + 1, j - 1] && (s[i] == s[j])
    string longestPalindrome(string s) {
        bool dp[1009][1009];
        int maxAns = 0;
        int l = 0;
        // 注意递推方程的性质，i 依赖 i + 1，j 依赖 j - 1
        for (int j = 0; j < s.length(); j++) {
            for (int i = 0; i <= j; i++) {
                if (i == j) {
                    dp[i][j] = true;
                } else if (i == j - 1 && s[i] == s[j]) {
                    dp[i][j] = true;
                } else {
                    dp[i][j] = dp[i + 1][j - 1] && s[i] == s[j];
                }
                if (dp[i][j] && j - i + 1 > maxAns) {
                    maxAns = j - i + 1;
                    l = i;
                }
            }
        }
        return s.substr(l, maxAns);
    }
};
```

[102. 二叉树的层序遍历](https://leetcode.cn/problems/binary-tree-level-order-traversal/description/)

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> ret;
        if (!root) {
            return ret;
        }
        queue<TreeNode *> q;
        q.push(root);
        while (!q.empty()) {
            int currentLevelSize = q.size(); // 这里得保存，不然在 for 循环中 q 会持续变大
            ret.push_back(vector<int>());
            for (int i = 1; i <= currentLevelSize; ++i) {
                auto node = q.front();
                q.pop();
                ret.back().push_back(node->val);
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }
        return ret;
    }
};
```

[1. 两数之和](https://leetcode.cn/problems/two-sum/)

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> numMap;
        int n = nums.size();
        for (int i = 0; i < n; i++) {
            int complement = target - nums[i];
            if (numMap.count(complement)) {
                return {numMap[complement], i};
            }
            numMap[nums[i]] = i;
        }
        return {};
    }
};
```

[33. 搜索旋转排序数组](https://leetcode.cn/problems/search-in-rotated-sorted-array/description/)

```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int n = nums.size();
        int l = 0, r = n - 1;
        while (l <= r) {
            int mid = (l + r) / 2;
            if (nums[mid] == target) return mid;
            // 检查 mid 在哪个有序区间内
            if (nums[0] <= nums[mid]) {
                // 第一个有序区间内，检查是否可以缩减到 0 ~ mid 范围内
                if (nums[0] <= target && target < nums[mid]) {
                    r = mid - 1;
                } else {
                    l = mid + 1;
                }
            } else {
                // 第二个有序区间内，检查是否可以缩减到 mid + 1 ~ n - 1 范围内
                if (nums[mid] < target && target <= nums[n - 1]) {
                    l = mid + 1;
                } else {
                    r = mid - 1;
                }
            }
        }
        return -1;
    }
};
```

[200. 岛屿数量](https://leetcode.cn/problems/number-of-islands/description/)

```cpp
class Solution {
int direction[4][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
public:
    // 常规写法
    int numIslands(vector<vector<char>>& grid) {
        int ans = 0;
        bool mark[300][300];
        memset(mark, 0, sizeof(mark));
        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[i].size(); j++) {
                if (grid[i][j] == '0' || mark[i][j]) {
                    continue;
                }
                dfs(mark, grid, i, j);
                ans++;
            }
        }
        return ans;
    }

    void dfs(bool mark[300][300], vector<vector<char>>& grid, int i, int j) {
        if (i < 0 || j < 0 || i >= grid.size() || j >= grid[i].size() ||
            grid[i][j] == '0') {
            return;
        }
        if (mark[i][j]) {
            return;
        }
        mark[i][j] = true;
        for (auto& d : direction) {
            int di = d[0];
            int dj = d[1];
            dfs(mark, grid, i + di, j + dj);
        }
    }
};
```

[46. 全排列](https://leetcode.cn/problems/permutations/description/)

```cpp
class Solution {
public:
    vector<int> vis;
    vector<int> path;

    void dfs(vector<vector<int>>& ans, vector<int>& nums, int x) {
        if (x >= nums.size()) {
            ans.push_back(path);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (vis[i]) continue;
            vis[i] = 1;
            path.push_back(nums[i]);
            dfs(ans, nums, x + 1);
            vis[i] = 0;
            path.pop_back();
        }
    }

    // 回溯板子题，来自题解 dfs 暴搜，排列组合
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> ans;
        vis.resize(nums.size(), 0);
        dfs(ans, nums, 0);
        return ans;
    }
};
```

[20. 有效的括号](https://leetcode.cn/problems/valid-parentheses/description/)

```cpp
class Solution {
public:
    bool isValid(string s) {
        stack<char> t;
        for (char c : s) {
            if (c == '(' || c == '{' || c == '[') {
                t.push(c);
                continue;
            }
            if (t.empty()) {
                return false;
            }
            char l = t.top();
            t.pop();
            if (c == ')' && l != '(') return false;
            if (c == ']' && l != '[') return false;
            if (c == '}' && l != '{') return false;
        }
        return t.empty();  // 最后的栈一定是空的
    }
};
```

[121. 买卖股票的最佳时机](https://leetcode.cn/problems/best-time-to-buy-and-sell-stock/description/)

```cpp
class Solution {
public:
    // O(N)
    // 先有股票最低点，然后才有可能有比之前还多的利润
    int maxProfit(vector<int>& prices) {
        int buy = 0, profit = 0;
        for (int i = 0; i < prices.size(); ++i) {
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
```

[88. 合并两个有序数组](https://leetcode.cn/problems/merge-sorted-array/description/)

```cpp
class Solution {
public:
    // 常规写法
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        vector<int> ans;
        int i = 0, j = 0;
        while (i < m || j < n) {
            if (j >= n) {
                ans.push_back(nums1[i]);
                i++;
                continue;
            }
            if (i >= m) {
                ans.push_back(nums2[j]);
                j++;
                continue;
            }
            if (nums1[i] <= nums2[j]) {
                ans.push_back(nums1[i]);
                i++;
            } else {
                ans.push_back(nums2[j]);
                j++;
            }
        }
        copy(ans.begin(), ans.end(), nums1.begin());
    }
};
```

[103. 二叉树的锯齿形层序遍历](https://leetcode.cn/problems/binary-tree-zigzag-level-order-traversal/description/)

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<vector<int>> zigzagLevelOrder(TreeNode *root) {
        vector<vector<int>> ans;
        if (!root) {
            return ans;
        }
        queue<TreeNode *> q;
        q.push(root);
        bool flag = false;
        while (!q.empty()) {
            vector<int> l;
            int lsize = q.size();
            for (int i = 0; i < lsize; i++) {
                TreeNode *f = q.front();
                q.pop();
                l.push_back(f->val);
                if (f->left) q.push(f->left);
                if (f->right) q.push(f->right);
            }
            if (flag) {
                reverse(l.begin(), l.end()); // 反转
                flag = false;
            } else {
                flag = true;
            }
            ans.push_back(l);
        }
        return ans;
    }
};
```

[141. 环形链表](https://leetcode.cn/problems/linked-list-cycle/)

```cpp
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    bool hasCycle(ListNode *head) {
        unordered_set<ListNode *> s;
        while (head) {
            if (s.count(head)) {
                return true;
            }
            s.insert(head);
            head = head->next;
        }
        return false;
    }
};

// TODO: 快慢指针
```
