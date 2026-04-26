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

struct FenwickTree {
    vector<ll> tree;
    ll n;

    FenwickTree(ll N) {
        n = N;
        tree.assign(n, 0);
    }

    ll F(ll x) {
        return x & (x + 1);
    }

    ll H(ll x) {
        return x | (x + 1);
    }

    // [0, r)
    ll query(ll r) {
        if (r > n) {
            r = n;
        }
        --r;
        ll ans = 0;
        while (r >= 0) {
            ans ^= tree[r];
            r = F(r) - 1;
        }
        return ans;
    }

    // [l, r)
    ll query(ll l, ll r) {
        return query(r) ^ query(l);
    }

    void add(ll id, ll val) {
        while (id < n) {
            tree[id] ^= val;
            id = H(id);
        }
    }
};
