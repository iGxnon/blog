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

bool cmp(ListNode* a, ListNode* b) { return a->val > b->val; }

class Solution {
   public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        priority_queue<ListNode*, vector<ListNode*>,
                       function<bool(ListNode*, ListNode*)>> // 自定义 compare
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
