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

struct SegmentTreeBeats {
    struct Node {
        ll val = 0;
        ll firstMax = -inf;
        ll cntMax = 0;
        ll secondMax = -inf;

        ll min_eq = inf;

        Node(ll Val = 0, ll FirstMax = -inf, ll CntMax = 0, ll SecondMax = -inf) {
            val = Val;
            firstMax = FirstMax;
            cntMax = CntMax;
            secondMax = SecondMax;
        }
    };

    Node fromOne(ll a) {
        return Node(a, a, 1, -inf);
    }

    Node neutral = Node();
    ll n;
    vector<Node> tree;

    SegmentTreeBeats(const vector<ll>& start) {
        n = start.size();
        tree.resize(4 * n + 3);
        init(start);
    }

    SegmentTreeBeats(ll N) {
        n = N;
        tree.resize(4 * n + 3);
        vector<ll> start(n, 0);
        init(start);
    }

    Node merge(Node n1, Node n2) {
        Node res(n1.val + n2.val);

        if (n1.firstMax > n2.firstMax) {
            res.firstMax = n1.firstMax;
            res.cntMax = n1.cntMax;
            res.secondMax = max(n1.secondMax, n2.firstMax);
        } else if (n1.firstMax < n2.firstMax) {
            res.firstMax = n2.firstMax;
            res.cntMax = n2.cntMax;
            res.secondMax = max(n1.firstMax, n2.secondMax);
        } else {
            res.firstMax = n1.firstMax;
            res.cntMax = n1.cntMax + n2.cntMax;
            res.secondMax = max(n1.secondMax, n2.secondMax);
        }
        return res;
    }

    void fix(ll v, ll l, ll r) {
        tree[v] = merge(tree[v * 2 + 1], tree[v * 2 + 2]);
    }

    void init(ll v, ll l, ll r, const vector<ll>& start) {
        if (l + 1 == r) {
            tree[v] = fromOne(start[l]);
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

    bool breakCondition(ll v, ll l, ll r, ll val) {
        return tree[v].firstMax <= val;
    }

    bool tagCondition(ll v, ll l, ll r, ll val) {
        return tree[v].secondMax < val;
    }

    void apply(ll v, ll l, ll r, ll val) {
        // only if tagCondition() == true
        ll diff = min(tree[v].firstMax, val) - tree[v].firstMax;
        tree[v].val += tree[v].cntMax * diff;
        tree[v].firstMax += diff;

        tree[v].min_eq = min(val, tree[v].min_eq);
    }

    void push(ll v, ll l, ll r) {
        ll m = (r + l) / 2;

        apply(v * 2 + 1, l, m, tree[v].min_eq);
        apply(v * 2 + 2, m, r, tree[v].min_eq);
        tree[v].min_eq = inf;
    }

    void update(ll v, ll l, ll r, ll ql, ll qr, ll val) {
        if (r <= ql || qr <= l || breakCondition(v, l, r, val)) {
            return;
        }
        if (ql <= l && r <= qr && tagCondition(v, l, r, val)) {
            apply(v, l, r, val);
            return;
        }

        ll m = (r + l) / 2;
        push(v, l, r);
        update(v * 2 + 1, l, m, ql, qr, val);
        update(v * 2 + 2, m, r, ql, qr, val);
        fix(v, l, r);
    }

    void update(ll ql, ll qr, ll val) {
        update(0, 0, n, ql, qr, val);
    }

    pair<ll, ll> descent(ll v, ll l, ll r, ll ql, ll qr, ll x) {
        if (r <= ql || qr <= l)
            return {-1, x};
        if (ql <= l && r <= qr) {
            if (tree[v].val < x)
                return {-1, x - tree[v].val};
            if (l + 1 == r)
                return {l + 1, x};
        }
        if (l + 1 == r)
            return {-1, x};

        ll m = (r + l) / 2;
        push(v, l, r);
        auto left = descent(v * 2 + 1, l, m, ql, qr, x);
        if (left.first != -1)
            return left;
        return descent(v * 2 + 2, m, r, ql, qr, left.second);
    }

    ll descent(ll ql, ll qr, ll x) {
        if (x <= 0)
            return ql;
        return descent(0, 0, n, ql, qr, x).first;
    }
};
