struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
   public:
    ListNode *reverseBetween(ListNode *head, int left, int right) {
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