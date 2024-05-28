struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
   public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        ListNode *cur1 = headA;  // headA -> tailA -> headB -> tailB;
        ListNode *cur2 = headB;  // headB -> tailB -> headA -> tailA;
        // 如上，cur1 和 cur2 走的总长度一样，当 cur1 和 cur2
        // 都进入彼此的链中，它们到达末尾的距离相同，如果有交叉，那么就在末尾前就相等，没有交叉则返回末尾
        while (cur1 != cur2) {
            if (cur1)
                cur1 = cur1->next;
            else
                cur1 = headB;
            if (cur2)
                cur2 = cur2->next;
            else
                cur2 = headA;
        }
        return cur1;
    }
};