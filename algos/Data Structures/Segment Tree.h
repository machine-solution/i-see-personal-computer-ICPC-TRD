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

struct SegmentTree {
    struct Node {
        ll sum;

        Node(ll Sum = 0) : sum(Sum) {
        }
    };

    Node neutral = Node();

    ll n;
    vector<Node> tree;

    void init(ll v, ll l, ll r, const vector<ll>& start) {
        if (l + 1 == r) {
            tree[v] = Node(start[l]);
            return;
        }

        ll m = (l + r) / 2;

        init(v * 2 + 1, l, m, start);
        init(v * 2 + 2, m, r, start);
        tree[v] = merge(tree[v * 2 + 1], tree[v * 2 + 2]);
    }

    SegmentTree() {
    }

    SegmentTree(const vector<ll>& start) {
        n = start.size();
        tree.resize(n * 4 + 3);
        init(0, 0, n, start);
    }

    Node merge(Node n1, Node n2) {
        return Node(n1.sum + n2.sum);
    }

    Node query(ll v, ll l, ll r, ll ql, ll qr) {
        if (ql <= l && r <= qr)
            return tree[v];
        if (qr <= l || r <= ql)
            return neutral;

        ll m = (l + r) / 2;
        return merge(query(v * 2 + 1, l, m, ql, qr), query(v * 2 + 2, m, r, ql, qr));
    }

    Node query(ll ql, ll qr) {
        return query(0, 0, n, ql, qr);
    }

    void change(ll v, ll l, ll r, ll id, ll val) {
        if (l + 1 == r && l == id) {
            tree[v] = Node(val);
            return;
        }
        if (r <= id || id < l) {
            return;
        }

        ll m = (l + r) / 2;
        change(v * 2 + 1, l, m, id, val);
        change(v * 2 + 2, m, r, id, val);
        tree[v] = merge(tree[v * 2 + 1], tree[v * 2 + 2]);
    }

    void change(ll id, ll val) {
        change(0, 0, n, id, val);
    }
};
