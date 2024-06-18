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
    ListNode* trainingPlan(ListNode* head, int cnt) {
        vector<ListNode*> s;
        ListNode* cur = head;
        while (cur) {
            s.push_back(cur);
            cur = cur->next;
        }
        return s[s.size() - cnt];
    }
};