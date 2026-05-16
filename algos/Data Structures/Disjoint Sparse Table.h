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
using comp = complex<ld>;

const ld PI = atan2(0, -1);
ll const inf = 1e18;

const ll MOD = 998244353;

random_device rd;
mt19937 mersenne(rd());

// includes

struct DisjointSparseTable {
    vector<vector<ll>> dp;

    ll neutral() {
        return 0;
    }
    ll merge(ll a, ll b) {
        return a + b;
    }

    DisjointSparseTable() {
    }

    DisjointSparseTable(const vector<ll>& a) {
        ll n = a.size();
        ll k = 0;
        ll nn = 1;
        while (nn <= n) {
            ++k;
            nn <<= 1;
        }

        dp.assign(k, vector<ll>(n + 1, neutral()));

        ll t = 0;
        for (ll len = 1; len <= n; len <<= 1, ++t) {
            for (ll cent = len; cent <= n; cent += len * 2) {
                dp[t][cent - 1] = a[cent - 1];
                for (ll i = cent - 2; i >= cent - len; --i) {
                    dp[t][i] = merge(a[i], dp[t][i + 1]);
                }

                dp[t][cent] = neutral();
                for (ll i = cent + 1; i < min(cent + len, n + 1); ++i) {
                    dp[t][i] = merge(dp[t][i - 1], a[i - 1]);
                }
            }
        }
    }

    // query on [l, r)
    ll query(ll l, ll r) {
        if (r <= l)
            return neutral();
        ll t = 63 - __builtin_clzll((ull)(l ^ r));
        return merge(dp[t][l], dp[t][r]);
    }
};
