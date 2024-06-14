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
    // ???
    ListNode* sortList(ListNode* head) {
        vector<int> temp;
        ListNode* cur = head;
        while (cur) {
            temp.push_back(cur->val);
            cur = cur->next;
        }
        sort(temp.begin(), temp.end());
        int i = 0;
        cur = head;
        while (cur) {
            cur->val = temp[i++];
            cur = cur->next;
        }
        return head;
    }

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
