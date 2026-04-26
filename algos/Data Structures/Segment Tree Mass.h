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

struct SegmentTreeMass {
    struct Node {
        ll sum = 0;
        ll add = 0;

        Node(ll Sum = 0, ll Add = 0) {
            sum = Sum;
            add = Add;
        }
    };

    Node neutral = Node();
    ll n;
    vector<Node> tree;

    SegmentTreeMass(const vector<ll>& start) {
        n = start.size();
        tree.resize(4 * n + 3);
        init(start);
    }

    SegmentTreeMass(ll N) {
        n = N;
        tree.resize(4 * n + 3);
        vector<ll> start(n, 0);
        init(start);
    }

    Node merge(Node n1, Node n2) {
        return Node(n1.sum + n2.sum);
    }

    void fix(ll v, ll l, ll r) {
        tree[v] = merge(tree[v * 2 + 1], tree[v * 2 + 2]);
    }

    void apply(ll v, ll l, ll r, ll val) {
        tree[v].add += val;
        tree[v].sum += val * (r - l);
    }

    void push(ll v, ll l, ll r) {
        ll m = (r + l) / 2;

        apply(v * 2 + 1, l, m, tree[v].add);
        apply(v * 2 + 2, m, r, tree[v].add);
        tree[v].add = 0;
    }

    void init(ll v, ll l, ll r, const vector<ll>& start) {
        if (l + 1 == r) {
            tree[v] = Node(start[l], 0);
            return;
        }

        ll m = (r + l) / 2;
        init(v * 2 + 1, l, m, start);
        init(v * 2 + 2, m, r, start);
        fix(v, l, r);
    }

    void init(const vector<ll>& start) {
        init(0, 0, n, start);
    }

    // [l: r)
    Node query(ll v, ll l, ll r, ll ql, ll qr) {
        if (ql <= l && r <= qr) {
            return tree[v];
        }
        if (r <= ql || qr <= l) {
            return neutral;
        }

        ll m = (r + l) / 2;
        push(v, l, r);
        return merge(query(v * 2 + 1, l, m, ql, qr), query(v * 2 + 2, m, r, ql, qr));
    }

    Node query(ll ql, ll qr) {
        return query(0, 0, n, ql, qr);
    }

    void add(ll v, ll l, ll r, ll ql, ll qr, ll val) {
        if (ql <= l && r <= qr) {
            apply(v, l, r, val);
            return;
        }
        if (r <= ql || qr <= l) {
            return;
        }

        ll m = (r + l) / 2;
        push(v, l, r);
        add(v * 2 + 1, l, m, ql, qr, val);
        add(v * 2 + 2, m, r, ql, qr, val);
        fix(v, l, r);
    }

    void add(ll ql, ll qr, ll val) {
        add(0, 0, n, ql, qr, val);
    }
};
