

#include <set>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
   public:
    // 基本方法
    bool hasCycle(ListNode *head) {
        set<ListNode *> s;
        while (head) {
            if (s.count(head)) {
                return true;
            }
            s.insert(head);
            head = head->next;
        }
        return false;
    }

    // 快慢指针，如果快指针移动到末尾了，则说明没有环，否则快指针总会和慢指针相遇
    bool hasCycle2(ListNode* head) {
        if (head == nullptr || head->next == nullptr) {
            return false;
        }
        ListNode* slow = head;
        ListNode* fast = head->next;
        while (slow != fast) {
            if (fast == nullptr || fast->next == nullptr) {
                return false;
            }
            slow = slow->next;
            fast = fast->next->next;
        }
        return true;
    }
};