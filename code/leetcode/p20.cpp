#include <stack>
#include <string>

using namespace std;

class Solution {
   public:
    bool isValid(string s) {
        stack<char> t;
        for (char c : s) {
            if (c == '(' || c == '{' || c == '[') {
                t.push(c);
                continue;
            }
            if (t.empty()) {
                return false;
            }
            char l = t.top();
            t.pop();
            if (c == ')' && l != '(') return false;
            if (c == ']' && l != '[') return false;
            if (c == '}' && l != '{') return false;
        }
        return t.empty();  // 最后的栈一定是空的
    }
};