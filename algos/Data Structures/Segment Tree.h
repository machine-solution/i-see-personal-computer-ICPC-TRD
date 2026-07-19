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

const ll inf = 1e18;

// includes

struct SegmentTree {
    struct Node {
        ll val;

        Node(ll Val = inf) : val(Val) {
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
        return Node(min(n1.val, n2.val));
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

    ll descent(ll v, ll l, ll r, ll ql, ll qr, ll x) {
        if (r <= ql || qr <= l)
            return -1;
        if (tree[v].val >= x)
            return -1;
        if (l + 1 == r)
            return l;

        ll m = (l + r) / 2;
        ll left = descent(v * 2 + 1, l, m, ql, qr, x);
        if (left != -1)
            return left;
        return descent(v * 2 + 2, m, r, ql, qr, x);
    }

    ll descent(ll ql, ll qr, ll x) {
        return descent(0, 0, n, ql, qr, x);
    }
};
