
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
   public:
    // 递归办法，来自题解
    ListNode *reverseKGroup(ListNode *head, int k) {
        if (!head) return head;
        ListNode *newHead = head;
        int i = 1; // 包含这个 head，算上一个
        while (newHead && newHead->next && i < k) {
            newHead = newHead->next;
            i++;
        }
        if (i < k) return head; // 如果不足 k 个，则无需反转，返回旧的头

        ListNode *h = head;
        ListNode *pre = nullptr;
        ListNode *next = newHead->next;
        while (h != next) {
            ListNode *t = h->next;
            h->next = pre;
            pre = h;
            h = t;
        }
        head->next = reverseKGroup(next, k); // 旧的头的下一个是下个 group 反转后的新的头
        return newHead;
    }

    // TODO：自己的写法
    ListNode *reverseKGroup2(ListNode *head, int k) {
        return nullptr;
    }
};