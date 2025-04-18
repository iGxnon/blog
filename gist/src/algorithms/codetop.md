# CodeTop

[无重复字符的最长子串](https://leetcode.cn/problems/longest-substring-without-repeating-characters/description/)

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

[LRU 缓存](https://leetcode.cn/problems/lru-cache/description/)

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

[反转链表](https://leetcode.cn/problems/reverse-linked-list/)

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