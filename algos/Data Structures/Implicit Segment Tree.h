#include <iostream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <map>
#include <set>
#include <fstream>
#include <string>
#include <bitset>
#include <ctime>
#include <random>
#include <iterator>
#include <cmath>
#include <queue>
#include <deque>
#include <complex>
#include <chrono>

using namespace std;

using ll = long long;
using ld = long double;
using ull = unsigned long long;

// includes

struct ImplicitSegmentTree {
    struct Node {
        Node* left = nullptr;
        Node* right = nullptr;
        ll sum = 0;
        ll add = 0;
    };

    Node neutral = Node();
    ll n;
    Node* root = nullptr;

    Node merge(Node n1, Node n2) {
        Node res;
        res.sum = n1.sum + n2.sum;
        return res;
    }

    void fix(Node* node) {
        node->sum = 0;
        if (node->left) {
            node->sum += node->left->sum;
        }
        if (node->right) {
            node->sum += node->right->sum;
        }
    }

    void apply(Node* node, ll l, ll r, ll val) {
        node->sum += val * (r - l);
        node->add += val;
    }

    void push(Node* node, ll l, ll r) {
        if (node->add == 0) {
            return;
        }
        ll m = (l + r) / 2;
        if (!node->left) {
            node->left = new Node();
        }
        if (!node->right) {
            node->right = new Node();
        }
        apply(node->left, l, m, node->add);
        apply(node->right, m, r, node->add);
        node->add = 0;
    }

    ImplicitSegmentTree() {
    }

    ImplicitSegmentTree(ll N) {
        n = N;
    }

    ImplicitSegmentTree(const vector<ll>& start) {
        n = start.size();
        for (ll i = 0; i < n; i++) {
            change(i, start[i]);
        }
    }

    void change(Node*& node, ll l, ll r, ll id, ll val) {
        if (!node) {
            node = new Node();
        }
        if (l + 1 == r) {
            node->sum = val;
            node->add = 0;
            return;
        }
        push(node, l, r);
        ll m = (l + r) / 2;
        if (id < m) {
            change(node->left, l, m, id, val);
        } else {
            change(node->right, m, r, id, val);
        }
        fix(node);
    }

    void change(ll id, ll val) {
        change(root, 0, n, id, val);
    }

    void add(Node*& node, ll l, ll r, ll ql, ll qr, ll val) {
        if (qr <= l || r <= ql) {
            return;
        }
        if (!node) {
            node = new Node();
        }
        if (ql <= l && r <= qr) {
            apply(node, l, r, val);
            return;
        }
        push(node, l, r);
        ll m = (l + r) / 2;
        add(node->left, l, m, ql, qr, val);
        add(node->right, m, r, ql, qr, val);
        fix(node);
    }

    void add(ll ql, ll qr, ll val) {
        add(root, 0, n, ql, qr, val);
    }

    Node query(Node* node, ll l, ll r, ll ql, ll qr) {
        if (!node || qr <= l || r <= ql) {
            return neutral;
        }
        if (ql <= l && r <= qr) {
            return *node;
        }
        push(node, l, r);
        ll m = (l + r) / 2;
        return merge(query(node->left, l, m, ql, qr), query(node->right, m, r, ql, qr));
    }

    Node query(ll ql, ll qr) {
        return query(root, 0, n, ql, qr);
    }
};
