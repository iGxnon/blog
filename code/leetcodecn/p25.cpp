
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
   public:
    ListNode *reverseKGroup(ListNode *head, int k) {
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