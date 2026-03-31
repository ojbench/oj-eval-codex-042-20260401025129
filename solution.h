#ifndef SPEEDCIRCULARLIST_H
#define SPEEDCIRCULARLIST_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

constexpr int s_prime = 31;

inline int log2(int x) {
    int ans = 0;
    while (x != 1) {
        x /= 2;
        ++ans;
    }
    return ans;
}

template<typename T, int b_prime>
class SpeedCircularLinkedList {
public:
    struct Node {
        // You may add more members, but cannot modify existing ones
        std::map<std::string, T> kv_map;
        int bound = 0;
        Node* next = nullptr;
        Node** fast_search_list = nullptr;

        // extra helper members (allowed by problem statement)
        int index = 0; // index of this node in traversal order starting from head

        Node(int b, int fast_size) : bound(b) {
            if (fast_size > 0) {
                fast_search_list = new Node*[fast_size];
                for (int i = 0; i < fast_size; ++i) fast_search_list[i] = nullptr;
            } else {
                fast_search_list = nullptr;
            }
        }

        ~Node() {
            if (fast_search_list) {
                delete [] fast_search_list;
                fast_search_list = nullptr;
            }
        }
    };

private:
    // Do not add more member variables
    Node* head = nullptr;
    int fast_search_list_size = 0;
    int list_size = 0;

    static int GetHashCode(std::string str) {
        long long ans = 0;
        for (auto& ch : str) {
            ans = (ans * s_prime + ch) % b_prime;
        }
        return static_cast<int>((ans + b_prime) % b_prime);
    }

    void BuildFastSearchList() {
        if (!head || list_size <= 1 || fast_search_list_size <= 0) return;
        // Collect nodes in order starting from head
        std::vector<Node*> nodes;
        nodes.reserve(list_size);
        Node* cur = head;
        for (int i = 0; i < list_size; ++i) {
            cur->index = i;
            nodes.push_back(cur);
            cur = cur->next;
        }
        // Fill fast search list: k-th is 2^k successor
        for (int i = 0; i < list_size; ++i) {
            for (int k = 0; k < fast_search_list_size; ++k) {
                int step = 1 << k;
                nodes[i]->fast_search_list[k] = nodes[(i + step) % list_size];
            }
        }
    }

    // Find node whose bound is the first >= code
    Node* FindNodeForCode(int code) const {
        if (!head) return nullptr;
        // If <= head bound, return head
        if (code <= head->bound) return head;
        // Otherwise, target index is in (0..list_size-1], before wrap
        // Use skip-list style forward jumps without wrapping
        const Node* cur = head;
        if (fast_search_list_size > 0 && list_size > 1 && head->fast_search_list) {
            for (int k = fast_search_list_size - 1; k >= 0; --k) {
                while (true) {
                    Node* nxt = cur->fast_search_list[k];
                    if (!nxt) break;
                    // Disallow wrap-around moves (index must strictly increase)
                    if (nxt->index <= cur->index) break;
                    if (nxt->bound < code) {
                        cur = nxt;
                        // try to move further at same level
                        continue;
                    }
                    break;
                }
            }
            return cur->next; // next node has bound >= code
        }
        // Fallback linear traversal
        Node* p = head->next;
        while (p != head) {
            if (code <= p->bound) return p;
            p = p->next;
        }
        return head; // should not reach here due to code < b_prime and last bound == b_prime
    }

public:
    explicit SpeedCircularLinkedList(std::vector<int> node_bounds) {
        list_size = static_cast<int>(node_bounds.size());
        if (list_size == 0) {
            head = nullptr;
            fast_search_list_size = 0;
            return;
        }
        fast_search_list_size = (list_size > 1) ? log2(list_size) : 0;

        // Create nodes
        head = new Node(node_bounds[0], fast_search_list_size);
        Node* prev = head;
        for (int i = 1; i < list_size; ++i) {
            Node* cur = new Node(node_bounds[i], fast_search_list_size);
            prev->next = cur;
            prev = cur;
        }
        // Complete the circle
        prev->next = head;

        // Assign indices in order and build FS list
        Node* cur = head;
        for (int i = 0; i < list_size; ++i) {
            cur->index = i;
            cur = cur->next;
        }
        BuildFastSearchList();
    }

    ~SpeedCircularLinkedList() {
        if (!head) return;
        Node* cur = head;
        for (int i = 0; i < list_size; ++i) {
            Node* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        head = nullptr;
        list_size = 0;
        fast_search_list_size = 0;
    }

    void put(std::string str, T value) {
        int code = GetHashCode(str);
        // Do not modify above line
        Node* target = FindNodeForCode(code);
        if (!target) return;
        target->kv_map[str] = value;
    }

    T get(std::string str) {
        int code = GetHashCode(str);
        // Do not modify above line
        Node* target = FindNodeForCode(code);
        if (target) {
            auto it = target->kv_map.find(str);
            if (it != target->kv_map.end()) return it->second;
        }
        return T();
    }

    void print() {
        if (!head) {
            std::cout << "";
            return;
        }
        Node* cur = head;
        for (int i = 0; i < list_size; ++i) {
            std::cout << "[Node] Bound = " << cur->bound << ", kv_map_size = " << cur->kv_map.size() << '\n';
            cur = cur->next;
        }
    }

    int size() const { return list_size; }
};

#endif // SPEEDCIRCULARLIST_H

