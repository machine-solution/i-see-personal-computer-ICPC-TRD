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

struct PersistentSegmentTree {
    struct Node {
        int left = -1;
        int right = -1;
        ll val = inf;
        ll add = 0;
    };

    ll n;
    vector<Node> pool;
    vector<int> roots;

    PersistentSegmentTree() {
    }

    PersistentSegmentTree(const vector<ll>& start) {
        n = start.size();
        roots.push_back(build(0, n, start));
    }

    int new_node() {
        pool.push_back(Node());
        return pool.size() - 1;
    }

    int clone(int v) {
        pool.push_back(pool[v]);
        return pool.size() - 1;
    }

    void fix(int v) {
        pool[v].val = min(pool[pool[v].left].val, pool[pool[v].right].val);
    }

    void apply(int v, ll val) {
        pool[v].val += val;
        pool[v].add += val;
    }

    void push(int v) {
        if (pool[v].add == 0)
            return;
        pool[v].left = clone(pool[v].left);
        pool[v].right = clone(pool[v].right);
        apply(pool[v].left, pool[v].add);
        apply(pool[v].right, pool[v].add);
        pool[v].add = 0;
    }

    int build(ll l, ll r, const vector<ll>& start) {
        int v = new_node();
        if (l + 1 == r) {
            pool[v].val = start[l];
            return v;
        }
        ll m = (l + r) / 2;
        pool[v].left = build(l, m, start);
        pool[v].right = build(m, r, start);
        fix(v);
        return v;
    }

    ll query(int v, ll l, ll r, ll ql, ll qr, ll lazy) {
        if (r <= ql || qr <= l)
            return inf;
        if (ql <= l && r <= qr)
            return pool[v].val + lazy;

        lazy += pool[v].add;
        ll m = (l + r) / 2;
        return min(query(pool[v].left, l, m, ql, qr, lazy), query(pool[v].right, m, r, ql, qr, lazy));
    }

    ll query(int t, ll ql, ll qr) {
        return query(roots[t], 0, n, ql, qr, 0);
    }

    void add(int v, ll l, ll r, ll ql, ll qr, ll val) {
        if (r <= ql || qr <= l)
            return;
        if (ql <= l && r <= qr) {
            apply(v, val);
            return;
        }

        push(v);
        ll m = (l + r) / 2;
        if (ql < m) {
            pool[v].left = clone(pool[v].left);
            add(pool[v].left, l, m, ql, qr, val);
        }
        if (m < qr) {
            pool[v].right = clone(pool[v].right);
            add(pool[v].right, m, r, ql, qr, val);
        }
        fix(v);
    }

    void add(int t, ll ql, ll qr, ll val) {
        int root = clone(roots[t]);
        add(root, 0, n, ql, qr, val);
        roots.push_back(root);
    }
};
