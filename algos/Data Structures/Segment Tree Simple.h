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

struct SegmentTreeSimple {
    ll n;
    vector<ll> tree;

    SegmentTreeSimple(ll N) {
        n = N;
        tree.resize(n * 2);
    }

    SegmentTreeSimple(const vector<ll>& start) {
        n = start.size();
        tree.resize(n * 2);

        for (int i = 0; i < n; ++i) {
            tree[i + n] = start[i];
        }
        for (int i = n - 1; i > 0; --i) {
            tree[i] = tree[i << 1] + tree[i << 1 | 1];
        }
    }

    void change(ll i, ll val) {
        i += n;
        tree[i] = val;
        while (i > 1) {
            tree[i >> 1] = tree[i] + tree[i ^ 1];
            i >>= 1;
        }
    }

    ll query(ll l, ll r) {
        ll ans = 0;

        l += n;
        r += n;

        while (l < r) {
            if (l & 1) {
                ans += tree[l++];
            }
            if (r & 1) {
                ans += tree[--r];
            }

            l >>= 1;
            r >>= 1;
        }

        return ans;
    }
};
