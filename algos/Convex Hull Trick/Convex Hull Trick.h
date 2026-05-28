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

// monotone lines
struct ConvexHullTrick {
    // a, b of length k, p of lengths max(0, k - 1)
    // all lines is a[i] * x + b[i]
    // and line i > 0 maximum from x = p[i - 1] to x = p[i] - 1
    vector<ll> a, b, p;

    // a1 < a2
    static ll intersect(ll a1, ll b1, ll a2, ll b2) {
        ll den = a2 - a1;
        ll num = b1 - b2;
        if (num >= 0) {
            return (num + den - 1) / den;
        } else {
            return num / den;
        }
    }

    // na >= a[i] forall i
    void add_line(ll na, ll nb) {
        if (a.empty()) {
            a.push_back(na);
            b.push_back(nb);
            return;
        }
        if (a.back() > na) {
            return;
        }
        while (!p.empty() && a.back() * p.back() + b.back() <= na * p.back() + nb) {
            p.pop_back();
            a.pop_back();
            b.pop_back();
        }
        // else na == a.back() and in one concrete point
        // new line leq of old > in all points new line >
        if (a.back() < na) {
            p.push_back(intersect(a.back(), b.back(), na, nb));
            a.push_back(na);
            b.push_back(nb);
        }
    }

    ll query(ll x) {
        if (a.empty()) {
            return -inf;
        }
        ll l = 0, r = (ll)a.size();
        while (r - l > 1) {
            ll m = (r + l) / 2;
            if (p[m - 1] <= x) {
                l = m;
            } else {
                r = m;
            }
        }
        return a[l] * x + b[l];
    }
};
