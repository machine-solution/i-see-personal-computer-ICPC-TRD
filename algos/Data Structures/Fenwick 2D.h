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

template <class T>
struct Fenwick2D {
    ll n = 0, m = 0;
    vector<vector<T>> bit;

    Fenwick2D() = default;
    Fenwick2D(int n_, int m_) {
        init(n_, m_);
    }

    void init(int n_, int m_) {
        n = n_;
        m = m_;
        bit.assign((int)n + 1, vector<T>((int)m + 1, T{}));
    }

    // add delta at (x, y), 0-indexed
    void add(ll x, ll y, T delta) {
        for (ll i = x + 1; i <= n; i += i & -i)
            for (ll j = y + 1; j <= m; j += j & -j)
                bit[i][j] += delta;
    }

    // sum over [0..x] x [0..y], 0-indexed; returns 0 if x<0 or y<0
    T sum_prefix(ll x, ll y) const {
        if (x < 0 || y < 0)
            return T{};
        T res{};
        for (ll i = x + 1; i > 0; i -= i & -i)
            for (ll j = y + 1; j > 0; j -= j & -j)
                res += bit[i][j];
        return res;
    }

    // sum over rectangle [x1..x2] x [y1..y2], inclusive, 0-indexed
    T sum_rect(ll x1, ll y1, ll x2, ll y2) const {
        return sum_prefix(x2, y2) - sum_prefix(x1 - 1, y2) - sum_prefix(x2, y1 - 1) + sum_prefix(x1 - 1, y1 - 1);
    }
};
