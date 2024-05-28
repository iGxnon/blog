#include <vector>

using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
   public:
    void reorderList(ListNode *head) {
        if (!head) {
            return;
        }
        vector<ListNode *> s;
        ListNode *cur = head;
        int n = 0;
        while (cur) {
            s.push_back(cur);
            cur = cur->next;
            n++;
        }
        // 双指针
        int i = 0, j = n - 1;
        while (i < j) {
            s[i]->next = s[j];
            i++;
            if (i == j) break;
            s[j]->next = s[i];
            j--;
        }
        s[i]->next = nullptr;
    }
};