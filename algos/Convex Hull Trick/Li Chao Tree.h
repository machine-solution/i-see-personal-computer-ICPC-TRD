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
#include <iomanip>

using namespace std;

using ll = long long;
using ld = long double;
using ull = unsigned long long;

const ll inf = 1e18;

#define all(a) a.begin(), a.end()
#define rall(a) a.rbegin(), a.rend()

// includes

struct LiChaoMax {
    struct Line {
        ld a = 0;
        ld b = -inf;

        Line(ld A = 0, ld B = -inf) : a(A), b(B) {
        }

        ld get(ld x) {
            return a * x + b;
        }
    };

    ll n;
    vector<ll> c;
    vector<Line> tree;

    LiChaoMax(const vector<ll>& C) {
        c = C;
        n = c.size();
        tree.resize(4 * n + 3);
    }

    void add_line(ll v, ll l, ll r, Line line) {
        ll m = (l + r) / 2;

        bool lef = line.get(c[l]) > tree[v].get(c[l]);
        bool mid = line.get(c[m]) > tree[v].get(c[m]);

        if (mid) {
            swap(line, tree[v]);
        }

        if (l + 1 == r) {
            return;
        }

        if (lef != mid) {
            add_line(v * 2 + 1, l, m, line);
        } else {
            add_line(v * 2 + 2, m, r, line);
        }
    }

    void add_line(ld a, ld b) {
        add_line(0, 0, n, Line(a, b));
    }

    ld query(ll v, ll l, ll r, ll k) {
        ld res = tree[v].get(c[k]);

        if (l + 1 == r) {
            return res;
        }

        ll m = (l + r) / 2;

        if (k < m) {
            res = max(res, query(v * 2 + 1, l, m, k));
        } else {
            res = max(res, query(v * 2 + 2, m, r, k));
        }

        return res;
    }

    ld query_id(ll k) {
        return query(0, 0, n, k);
    }

    ld query_x(ll x) {
        ll k = lower_bound(all(c), x) - c.begin();
        return query_id(k);
    }
};
