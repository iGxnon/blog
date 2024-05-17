#include <stack>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
   public:
    ListNode *reverseList(ListNode *head) {
        ListNode *pre = nullptr;
        ListNode *cur = head;
        while (cur) {
            ListNode *next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        return pre;
    }

    // 栈
    ListNode *reverseList2(ListNode *head) {
        std::stack<ListNode *> s;
        ListNode *cur = head;
        while (cur) {
            s.push(cur);
            cur = cur->next;
        }
        ListNode *newHead = nullptr;
        ListNode *newCur = nullptr;
        while (!s.empty()) {
            if (!newHead) {
                newHead = s.top();
                newCur = newHead;
            } else {
                newCur->next = s.top();
                newCur = newCur->next;
            }
            s.pop();
        }
        if (newCur) {
            newCur->next = nullptr;
        }
        return newHead;
    }

    // 递归
    ListNode *reverseList3(ListNode *head) {
        if (!head || !head->next) {
            return head;
        }
        // 反转头节点后的节点
        ListNode *newHead = reverseList3(head->next);
        // 反转当前头节点
        head->next->next = head;
        head->next = nullptr;
        return newHead;
    }
};