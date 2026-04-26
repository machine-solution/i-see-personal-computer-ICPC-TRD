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

struct SegmentTreeMassSimple {
    struct Node {
        ll size, val, inc;

        Node(ll Val = 0, ll Size = 0, ll Inc = 0) {
            val = Val;
            size = Size;
            inc = Inc;
        }
    };

    ll n;
    vector<Node> tree;

    Node merge(Node n1, Node n2) {
        return Node(n1.val + n2.val, n1.size + n2.size);
    }

    SegmentTreeMassSimple(const vector<ll>& start) {
        n = start.size();

        tree.resize(n * 2);

        for (int i = 0; i < n; ++i) {
            tree[i + n] = Node(start[i], 1, 0);
        }

        for (int i = n - 1; i >= 0; --i) {
            tree[i] = merge(tree[i << 1], tree[i << 1 | 1]);
        }
    }

    void apply(ll i, ll val) {
        tree[i].val += val * tree[i].size;
        tree[i].inc += val;
    }

    void push(ll i) {
        if (i > 1) {
            push(i >> 1);
        }
        if (i < n) {
            apply(i << 1, tree[i].inc);
            apply(i << 1 | 1, tree[i].inc);
            tree[i].inc = 0;
        }
    }

    // fix node i and all parents
    void fix(ll i) {
        push(i);
        while (i > 1) {
            ll j = i >> 1;
            tree[j] = merge(tree[j << 1], tree[j << 1 | 1]);
            i = j;
        }
    }

    Node query(ll l, ll r) {
        l += n;
        r += n;

        Node ansl, ansr;

        push(l);
        push(r - 1);

        while (r > l) {
            if (l & 1) {
                ansl = merge(ansl, tree[l++]);
            }
            if (r & 1) {
                ansr = merge(tree[--r], ansr);
            }

            l >>= 1;
            r >>= 1;
        }

        return merge(ansl, ansr);
    }

    void change(ll l, ll r, ll inc) {
        l += n;
        r += n;

        ll l0 = l, r0 = r;

        push(l);
        push(r - 1);

        while (r > l) {
            if (l & 1) {
                apply(l++, inc);
            }
            if (r & 1) {
                apply(--r, inc);
            }

            l >>= 1;
            r >>= 1;
        }

        fix(l0);
        fix(r0 - 1);
    }
};
