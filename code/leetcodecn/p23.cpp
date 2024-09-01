#include <functional>
#include <queue>
#include <vector>

using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution {
   public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        auto cmp = [](ListNode* a, ListNode* b) { return a->val > b->val };
        priority_queue<ListNode*, vector<ListNode*>,
                       decltype(cmp)>  // 最方便的一集
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

    // dumb way
    ListNode* mergeKLists2(vector<ListNode*>& lists) {
        vector<ListNode*> cur(lists.begin(), lists.end());
        auto check = [](vector<ListNode*>& v) {
            for (auto n : v) {
                if (n != nullptr) return true;
            }
            return false;
        };
        auto minimal = [](vector<ListNode*>& v) {
            int minV = 1e9;
            ListNode* minN = nullptr;
            int nth = -1;
            for (int i = 0; i < v.size(); i++) {
                ListNode* n = v[i];
                if (n && n->val < minV) {
                    minV = n->val;
                    minN = n;
                    nth = i;
                }
            }
            v[nth] = v[nth]->next;
            return minN;
        };
        ListNode* dummy = new ListNode();
        ListNode* w = dummy;
        while (check(cur)) {
            w->next = minimal(cur);
            w = w->next;
        }
        return dummy->next;
    }
};
