#include <unordered_map>

// 哈希表 + 双链表（带头尾伪节点方便处理边界）
// - 注意移动到头的时候要从原来的位置删除（removeNode）

using namespace std;

class Node {
   public:
    int k, v;
    Node *pre, *nex;
    Node() : k(0), v(0), pre(nullptr), nex(nullptr) {}
    Node(int _k, int _v) : k(_k), v(_v), pre(nullptr), nex(nullptr) {}
};

class LRUCache {
    Node *head, *tail;
    int cap;
    unordered_map<int, Node *> store;

   public:
    LRUCache(int capacity) : cap(capacity) {
        // 伪节点
        head = new Node();
        tail = new Node();
        head->nex = tail;
        tail->pre = head;
    }

    int get(int key) {
        if (!store.count(key)) {
            return -1;
        }
        Node *n = store[key];
        moveToHead(n);
        return n->v;
    }

    void put(int key, int value) {
        if (store.count(key)) {
            Node *n = store[key];
            n->v = value;
            moveToHead(n);
            return;
        }
        if (store.size() >= cap) {
            // evit
            Node *toRemove = tail->pre;
            store.erase(toRemove->k);
            removeNode(toRemove);
        }
        Node *n = new Node(key, value);
        moveToHead(n);
        store[key] = n;
    }

    void removeNode(Node *n) {
        n->pre->nex = n->nex;
        n->nex->pre = n->pre;
    }

    void moveToHead(Node *n) {
        if ((n->pre && n->pre->nex == n) || (n->nex && n->nex->pre == n)) {
            removeNode(n);
        }
        n->pre = head;
        n->nex = head->nex;
        head->nex->pre = n;
        head->nex = n;
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */