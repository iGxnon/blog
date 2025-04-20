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

[236. 二叉树的最近公共祖先](https://leetcode.cn/problems/lowest-common-ancestor-of-a-binary-tree/)

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (root == q || root == p || !root) return root;
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        if (left && right) return root;
        if (!left) return right;
        return left;
    }
};
```

[92. 反转链表 II](https://leetcode.cn/problems/reverse-linked-list-ii/description/)

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
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode *dummy = new ListNode();
        dummy->next = head;
        ListNode *pre = dummy;
        for (int i = 0; i < left - 1; i++) pre = pre->next;
        ListNode *subHead = pre->next;
        ListNode *subPre = nullptr;
        ListNode *subCur = subHead;
        ListNode *next;
        for (int i = 0; i < right - left + 1; i++) {
            next = subCur->next;
            subCur->next = subPre;
            subPre = subCur;
            subCur = next;
        }
        ListNode *subTail = subPre;
        pre->next = subTail;
        subHead->next = subCur;
        return dummy->next;
    }
};
```

[54. 螺旋矩阵](https://leetcode.cn/problems/spiral-matrix/description/)

```cpp
class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        vector<int> ans;
        if (matrix.empty()) return ans;  // 若数组为空，直接返回答案
        int u = 0;                       // 上边界
        int l = 0;                       // 左边界
        int d = matrix.size() - 1;       // 下边界
        int r = matrix[0].size() - 1;    // 右边界
        while (true) {
            for (int i = l; i <= r; ++i) ans.push_back(matrix[u][i]);  // 向右移动直到最右
            if (++u > d) break;  // 增加上边界，上边界大于下边界，退出
            for (int i = u; i <= d; ++i) ans.push_back(matrix[i][r]);  // 向下
            if (--r < l) break;  // 重新设定有边界
            for (int i = r; i >= l; --i) ans.push_back(matrix[d][i]);  // 向左
            if (--d < u) break;  // 重新设定下边界
            for (int i = d; i >= u; --i) ans.push_back(matrix[i][l]);  // 向上
            if (++l > r) break;  // 重新设定左边界
        }
        return ans;
    }
};
```

[300. 最长上升子序列](https://leetcode.cn/problems/longest-increasing-subsequence/description/)

```cpp
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        // dp[i] = max(dp[j], 0 <= j < i and nums[i] > nums[j]) + 1
        int dp[2501];
        fill_n(dp, 2501, 1);
        int ans = 1;
        for (int i = 0; i < nums.size(); i++) {
            for (int j = 0; j < i; j++) {
                if (nums[i] > nums[j]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
            ans = max(ans, dp[i]);
        }
        return ans;
    }
};
```

[23. 合并K个排序链表](https://leetcode.cn/problems/merge-k-sorted-lists/description/)

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
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        auto cmp = [](ListNode* a, ListNode* b) { return a->val > b->val };
        priority_queue<ListNode*, vector<ListNode*>,
                       decltype(cmp)>
            pq(cmp);

        ListNode* dummy = new ListNode();
        ListNode* cur = dummy;
        for (auto node : lists) {
            if (node) pq.push(node);
        }
        while (!pq.empty()) {
            auto node = pq.top();
            pq.pop();
            cur->next = node;
            cur = cur->next;
            if (node->next) pq.push(node->next);
        }
        return dummy->next;
    }
};
```

[143. 重排链表](https://leetcode.cn/problems/reorder-list/description/)

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
    void reorderList(ListNode* head) {
        if (!head) {
            return;
        }
        vector<ListNode *> s;
        ListNode *cur = head;
        int n = 0;
        while (cur) {
            s.push_back(cur);
            cur = cur->next;
            n++;
        }
        // 双指针
        int i = 0, j = n - 1;
        while (i < j) {
            s[i]->next = s[j];
            i++;
            if (i == j) break;
            s[j]->next = s[i];
            j--;
        }
        s[i]->next = nullptr;
    }
};
```

[415. 字符串相加](https://leetcode.cn/problems/add-strings/description/)

```cpp
class Solution {
public:
    string addStrings(string num1, string num2) {
        string ans = "";
        int c1 = (int)num1.size();
        int c2 = (int)num2.size();
        int cm = max(c1, c2);
        int carry = 0;
        for (int i = 1; i <= cm; i++) {
            int sum = 0;
            if (c1 - i >= 0) {
                sum += num1[c1 - i] - '0';
            }
            if (c2 - i >= 0) {
                sum += num2[c2 - i] - '0';
            }
            sum += carry;
            ans.push_back('0' + (sum % 10));
            carry = sum / 10;
        }
        // 最后记得带上 carry
        if (carry) {
            ans.push_back('0' + carry);
        }
        reverse(ans.begin(), ans.end());
        return ans;
    }
};
```

[160. 相交链表](https://leetcode.cn/problems/intersection-of-two-linked-lists/description/)

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
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        ListNode *cur1 = headA;  // headA -> tailA -> headB -> tailB;
        ListNode *cur2 = headB;  // headB -> tailB -> headA -> tailA;
        while (cur1 != cur2) {
            if (cur1)
                cur1 = cur1->next;
            else
                cur1 = headB;
            if (cur2)
                cur2 = cur2->next;
            else
                cur2 = headA;
        }
        return cur1;
    }
};
```

[56. 合并区间](https://leetcode.cn/problems/merge-intervals/description/)

```cpp
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        vector<vector<int>> ans;
        sort(intervals.begin(), intervals.end());
        int n = intervals.size();
        // [1,2][2,3][4,5] => [1,2][1,3](插入 [1,3])[4,5]
        // [1,4][2,3] => [1,4][1,4]
        // 遍历到倒数第二个
        for (int i = 0; i < n-1; i ++) {
            // 前一个末尾比后一个开头大，把后一个开头改成前一个开头
            if (intervals[i][1] >= intervals[i+1][0]) {
                intervals[i+1][0] = intervals[i][0];
            }
            // 前一个末尾比后一个末尾还大，那把后一个末尾也改成前一个末尾
            if (intervals[i][1] >= intervals[i+1][1]) {
                intervals[i+1][1] = intervals[i][1];
            }
            // 出现了空隙，说明这个没办法继续合并了
            if (intervals[i][1] < intervals[i+1][0]) {
                ans.push_back(intervals[i]);
            }
        }
        // 最后一个一定是合并的结果
        ans.push_back(intervals[n-1]);
        return ans;
    }
};
```

[42. 接雨水](https://leetcode.cn/problems/trapping-rain-water/description/)

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        // lmax
        // |    rmax
        // | # |
        // ————
        // l   r
        int l = 0, r = h.size() - 1, lmax = -1, rmax = -1, ans = 0;
        while (l < r) {
            lmax = max(lmax, h[l]);
            rmax = max(rmax, h[r]);
            ans += (lmax < rmax) ? lmax - h[l++] : rmax - h[r--];
        }
        return ans;
    }
};
```

[124. 二叉树中的最大路径和](https://leetcode.cn/problems/binary-tree-maximum-path-sum/description/)

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
    int ans = -1001;
    int maxPathSum(TreeNode* root) {
        treeSum(root);
        return ans;
    }

    int treeSum(TreeNode *root) {
        int sumL = 0, sumR = 0;
        if (root->left) {
            sumL = treeSum(root->left);
        }
        if (root->right) {
            sumR = treeSum(root->right);
        }
        // ans 来自 max(root->val, root->val + sumL, root->val + sumR, root->val
        // + sumL + sumR)
        ans = max(ans, max({root->val, root->val + sumL, root->val + sumR,
                            root->val + sumL + sumR}));
        // 但只能返回给上层 max(root->val, root->val + sumL, root->val + sumR)
        return max({root->val, root->val + sumL, root->val + sumR});
    }
};
```

[72. 编辑距离](https://leetcode.cn/problems/edit-distance/description/)

```cpp
class Solution {
public:
    int minDistance(string word1, string word2) {
        int n1 = word1.size();
        int n2 = word2.size();

        if (n1 * n2 == 0) return max(n1, n2);

        // dp[i][j] 表示 word1 中 [0, i) 和 word2 中 [0, j) （注意是不包括 i, j）的最短编辑距离
        int dp[501][501];
        memset(dp, 0, sizeof(dp));
        for (int i = 0; i < 501; i ++) {
            dp[i][0] = i;
            dp[0][i] = i;
        }

        for (int i = 1; i <= n1; i ++)
            for (int j = 1; j <= n2; j ++) {
                int left = dp[i-1][j] + 1;
                int down = dp[i][j-1] + 1;
                int left_down = dp[i-1][j-1];
                if (word1[i-1] != word2[j-1]) // 判断末尾的字符是否相等，相等则不用修改
                    left_down ++;
                dp[i][j] = min({left, down, left_down});
            }

        return dp[n1][n2];
    }
};
```

[142. 环形链表 II](https://leetcode.cn/problems/linked-list-cycle-ii/description/)

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
    ListNode *detectCycle(ListNode *head) {
        if (!head) {
            return nullptr;
        }
        unordered_set<ListNode *> s;
        ListNode *cur = head;
        while (cur) {
            if (s.count(cur)) return cur;
            s.insert(cur);
            cur = cur->next;
        }
        return nullptr;
    }
};
```

[1143. 最长公共子序列](https://leetcode.cn/problems/longest-common-subsequence/description/)

```cpp
class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        // dp[i][j] = dp[i-1][j-1] + 1, text1[i -1] == text2[j - 1];
        // dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]), text1[i -1] != text2[j - 1];
        int m = text1.length(), n = text2.length();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1));
        for (int i = 1; i <= m; i++) {
            char c1 = text1.at(i - 1);
            for (int j = 1; j <= n; j++) {
                char c2 = text2.at(j - 1);
                if (c1 == c2) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        return dp[m][n];
    }
};
```

[19. 删除链表的倒数第 N 个结点](https://leetcode.cn/problems/remove-nth-node-from-end-of-list/description/)

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
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode* dummy = new ListNode();
        dummy->next = head;
        ListNode* cur = head;
        int count = 0;
        while (cur) {
            count++;
            cur = cur->next;
        }
        ListNode* pre = dummy;
        cur = head;
        for (int i = 0; i < count - n; i++) {
            pre = cur;
            cur = cur->next;
        }
        pre->next = cur->next;
        return dummy->next;
    }
};
```

[93. 复原IP地址](https://leetcode.cn/problems/restore-ip-addresses/description/)

```cpp
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
        if (raw[pos] == '0') {
            parts.push_back("0");
            dfs(raw, pos + 1);
            parts.pop_back();
            return;
        }
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
```

[82. 删除排序链表中的重复元素 II](https://leetcode.cn/problems/remove-duplicates-from-sorted-list-ii/description/)

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
    ListNode* deleteDuplicates(ListNode* head) {
        if (!head) {
            return head;
        }
        ListNode* dummy = new ListNode(0, head);
        ListNode* cur = dummy;
        while (cur->next && cur->next->next) {
            if (cur->next->val == cur->next->next->val) {
                int x = cur->next->val;
                while (cur->next && cur->next->val == x) {
                    cur->next = cur->next->next;
                }
            } else {
                cur = cur->next;
            }
        }
        return dummy->next;
    }
};
```

[4. 寻找两个正序数组的中位数](https://leetcode.cn/problems/median-of-two-sorted-arrays/description/)

```cpp
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int i = 0, j = 0, k = 0, pre = 0, cur = 0, n1 = nums1.size(), n2 = nums2.size();
        int m = n1 + n2;
        int mid = m >> 1;
        while (k <= mid) {
            pre = cur;
            if (i < n1 && j < n2) {
                if (nums1[i] < nums2[j]) {
                    cur = nums1[i];
                    i++;
                } else {
                    cur = nums2[j];
                    j++;
                }
            } else if (i < n1) {
                cur = nums1[i];
                i++;
            } else {
                cur = nums2[j];
                j++;
            }
            k++;
        }
        if (m % 2 == 0) {
            return (float)(pre + cur) / 2;
        }
        return (float)cur;
    }
};
```

[199. 二叉树的右视图](https://leetcode.cn/problems/binary-tree-right-side-view/description/)

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
    vector<int> rightSideView(TreeNode* root) {
        vector<int> ans;
        if (root == nullptr) return ans;
        deque<TreeNode*> q;
        q.push_back(root);
        while (!q.empty()) {
            int n = q.size();
            for (int i = 0; i < n; i ++) {
                TreeNode* front = q.front();
                q.pop_front();
                if (i == n-1) {
                    ans.push_back(front->val);
                }
                if (front->left) {
                    q.push_back(front->left);
                }
                if (front->right) {
                    q.push_back(front->right);
                }
            }
        }
        return ans;
    }
};
```

[94. 二叉树的中序遍历](https://leetcode.cn/problems/binary-tree-inorder-traversal/description/)

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
    vector<int> ans;
    vector<int> inorderTraversal(TreeNode* root) {
        if (!root) {
            return ans;
        }
        inorderTraversal(root->left);
        ans.push_back(root->val);
        inorderTraversal(root->right);
        return ans;
    }
};
```

[704. 二分查找](https://leetcode.cn/problems/binary-search/description/)

```cpp
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int l = 0, r = nums.size();
        while (l < r) {
            int mid = l + ((r - l) >> 1);
            if (nums[mid] == target) return mid;
            if (nums[mid] > target) {
                r = mid;
            } else {
                l = mid + 1;
            }
        }
        return -1;
    }
};
```

[232. 用栈实现队列](https://leetcode.cn/problems/implement-queue-using-stacks/description/)

```cpp
class MyQueue {
public:
    stack<int> s1;
    stack<int> s2;

    MyQueue() {}

    void push(int x) { s1.push(x); }

    int pop() {
        while (!s1.empty()) {
            s2.push(s1.top());
            s1.pop();
        }
        int ret = s2.top();
        s2.pop();
        while (!s2.empty()) {
            s1.push(s2.top());
            s2.pop();
        }
        return ret;
    }

    int peek() {
        while (!s1.empty()) {
            s2.push(s1.top());
            s1.pop();
        }
        int ret = s2.top();
        while (!s2.empty()) {
            s1.push(s2.top());
            s2.pop();
        }
        return ret;
    }

    bool empty() { return s1.empty(); }
};

/**
 * Your MyQueue object will be instantiated and called as such:
 * MyQueue* obj = new MyQueue();
 * obj->push(x);
 * int param_2 = obj->pop();
 * int param_3 = obj->peek();
 * bool param_4 = obj->empty();
 */
```

[22. 括号生成](https://leetcode.cn/problems/generate-parentheses/description/)

```cpp
class Solution {
public:
    vector<string> ans;
    string op;

    void dfs(int l, int r) {
        if (l == 0 && r == 0) {
            ans.push_back(op);
            return;
        }
        if (l > 0) {
            op.push_back('(');
            dfs(l - 1, r + 1);
            op.pop_back();
        }
        if (r > 0) {
            op.push_back(')');
            dfs(l, r - 1);
            op.pop_back();
        }
    }

    vector<string> generateParenthesis(int n) {
        dfs(n, 0);
        return ans;
    }
};
```

[31. 下一个排列](https://leetcode.cn/problems/next-permutation/description/)

```cpp
class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        int n = nums.size() - 1;
        while (n > 0 && nums[n] <= nums[n - 1]) {
            n--;
        }
        if (n == 0) {
            reverse(nums.begin(), nums.end());
        } else {
            int i = n;
            while (i < nums.size() && nums[i] > nums[n - 1]) {
                i++;
            }
            swap(nums[n - 1], nums[i - 1]);
            reverse(nums.begin() + n, nums.end());
        }
    }
};
```

[148. 排序链表](https://leetcode.cn/problems/sort-list/description/)

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
    // merge sort
    ListNode* sortList(ListNode* head) {
        if (head == nullptr || head->next == nullptr) return head;
        // find the middle node
        ListNode *fast = head->next, *slow = head;
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        ListNode* r = slow->next;
        slow->next = nullptr;
        return merge(sortList(head), sortList(r));
    }

    ListNode* merge(ListNode* l, ListNode* r) {
        ListNode* dummy = new ListNode();
        ListNode* c = dummy;
        while (l != nullptr && r != nullptr) {
            if (l->val <= r->val) {
                c->next = l;
                c = c->next;
                l = l->next;
            } else {
                c->next = r;
                c = c->next;
                r = r->next;
            }
        }
        if (l != nullptr) {
            c->next = l;
        }
        if (r != nullptr) {
            c->next = r;
        }
        return dummy->next;
    }
};
```

[165. 比较版本号](https://leetcode.cn/problems/compare-version-numbers/description/)

```cpp
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
```

[69. x 的平方根](https://leetcode.cn/problems/sqrtx/description/)

```cpp
class Solution {
public:
    int mySqrt(int x) {
        long long l = 0, r = x;
        while (l < r) {
            long long mid = (l + r + 1) / 2;
            if (x >= mid * mid) {
                l = mid;
            } else {
                r = mid - 1;
            }
        }
        return r;
    }
};
```

[239. 滑动窗口最大值](https://leetcode.cn/problems/sliding-window-maximum/description/)

```cpp
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        int n = nums.size();
        priority_queue<pair<int, int>> q;
        for (int i = 0; i < k; ++i) {
            q.emplace(nums[i], i);
        }
        vector<int> ans = {q.top().first};
        for (int i = 1; i + k <= n; i++) {
            q.emplace(nums[i + k - 1], i + k - 1);
            while (q.top().second < i) {
                q.pop();
            }
            ans.push_back(q.top().first);
        }
        return ans;
    }
};
```

[8. 字符串转换整数 (atoi)](https://leetcode.cn/problems/string-to-integer-atoi/description/)

```cpp
class Solution {
public:
    int myAtoi(string s) {
        int res = 0, sign = 1, start = 0, boundary = INT_MAX / 10;
        if (s.empty()) return 0;
        for (; start < s.size() && s[start] == ' '; start++);
        if (s[start] == '-') {
            start++;
            sign = -1;
        } else if (s[start] == '+') {
            start++;
        }
        for (; start < s.size(); start++) {
            if (s[start] < '0' || s[start] > '9') break;
            if (res > boundary ||
                res == boundary && (s[start] - '0') > (INT_MAX % 10))
                return sign > 0 ? INT_MAX : INT_MIN;
            res = (res * 10) + (s[start] - '0');
        }
        return res * sign;
    }
};
```

[2. 两数相加](https://leetcode.cn/problems/add-two-numbers/description/)

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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode* dummy = new ListNode();
        int carry = 0;
        ListNode *c1 = l1, *c2 = l2, *c = dummy;
        while (c1 != nullptr || c2 != nullptr) {
            int sum = carry;
            if (c1 != nullptr) {
                sum += c1->val;
                c1 = c1->next;
            }
            if (c2 != nullptr) {
                sum += c2->val;
                c2 = c2->next;
            }
            carry = sum / 10;
            c->next = new ListNode(sum % 10);
            c = c->next;
        }
        if (carry > 0) {
            c->next = new ListNode(carry);
        }
        return dummy->next;
    }
};
```

[70. 爬楼梯](https://leetcode.cn/problems/climbing-stairs/description/)

```cpp
class Solution {
public:
    int climbStairs(int n) {
        int dp[50] = {0};
        dp[1] = 1;
        dp[2] = 2;
        for (int i = 3; i <= n; i ++) {
            dp[i] = dp[i - 2] + dp[i - 1];
        }
        return dp[n];
    }
};
```
