struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution {
   public:
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
        ListNode* cur1 = list1;
        ListNode* cur2 = list2;
        ListNode* head = new ListNode();
        ListNode* cur = head;
        while (1) {
            if (!cur1) {
                while (cur2) {
                    cur->next = cur2;
                    cur = cur->next;
                    cur2 = cur2->next;
                }
                break;
            }
            if (!cur2) {
                while (cur1) {
                    cur->next = cur1;
                    cur = cur->next;
                    cur1 = cur1->next;
                }
                break;
            }
            if (cur1->val <= cur2->val) {
                cur->next = cur1;
                cur = cur->next;
                cur1 = cur1->next;
            } else {
                cur->next = cur2;
                cur = cur->next;
                cur2 = cur2->next;
            }
        }
        return head->next;
    }
};