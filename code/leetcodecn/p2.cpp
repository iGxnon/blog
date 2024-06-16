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