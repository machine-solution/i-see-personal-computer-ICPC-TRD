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
struct SparseTable {
    ll n, k;
    vector<T> a;
    vector<vector<T>> sparse;
    vector<ll> length;

    SparseTable(vector<T>& start) {
        n = start.size();
        k = 0;
        for (ll d = 1; d <= n; d *= 2, ++k) {
        }
        a.assign(start.begin(), start.end());
        sparse.assign(n, vector<T>(k));
        countSparse();
        precalc();
    }

    void precalc() {
        length.assign(n + 1, 0);
        ll t = 0;
        for (int l = 1; l <= n; ++l) {
            if ((1ll << (t + 1)) < l)
                ++t;
            length[l] = t;
        }
    }

    void countSparse() {
        for (int i = 0; i < n; ++i) {
            sparse[i][0] = a[i];
        }
        for (int j = 1; j < k; ++j) {
            for (int i = 0; i < n; ++i) {
                ll d = (1ll << (j - 1));
                sparse[i][j] = max(sparse[i][j - 1], sparse[min(n - 1, i + d)][j - 1]);
            }
        }
    }

    T query(ll l, ll r) {
        ll t = length[r - l];

        ll d = (1ll << t);

        return max(sparse[l][t], sparse[r - d][t]);
    }
};
