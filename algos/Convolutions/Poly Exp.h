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

const ll MOD = 998244353;

vector<ll> vect_mult(const vector<ll>& a, const vector<ll>& b);
vector<ll> log_mod_xm(const vector<ll>& a, ll n);
vector<ll> cut_prefix(const vector<ll>& a, ll m);

// includes

// a[0] == 0; returns exp(a) mod x^n
vector<ll> exp_mod_xm(const vector<ll>& a, ll n) {
    if (n <= 0)
        return {};
    vector<ll> f = {1};
    ll len = 1;
    while (len < n) {
        ll nlen = min(2 * len, n);
        vector<ll> g = cut_prefix(a, nlen);
        vector<ll> lf = log_mod_xm(f, nlen);
        for (int i = 0; i < nlen; ++i)
            g[i] = (g[i] + MOD - lf[i]) % MOD;
        g[0] = (g[0] + 1) % MOD;
        f = cut_prefix(vect_mult(f, g), nlen);
        len = nlen;
    }
    return f;
}
