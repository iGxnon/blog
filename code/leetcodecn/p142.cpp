#include <unordered_set>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

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