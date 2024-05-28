struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution {
   public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // 经典 dummy
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