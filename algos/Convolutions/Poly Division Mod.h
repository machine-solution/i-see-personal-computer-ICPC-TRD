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
void fft_inpl(vector<ll>& a, bool invert);
ll bin_pow(ll a, ll n);

// includes

vector<ll> cut_prefix(const vector<ll>& a, ll m) {
    vector<ll> r(m, 0);
    for (int i = 0; i < a.size() && i < m; ++i)
        r[i] = a[i];
    return r;
}

// b[0] invertible; returns b^{-1} mod x^m
vector<ll> inv_mod_xm(const vector<ll>& b, ll m) {
    if (m <= 0 || b.empty() || b[0] % MOD == 0)
        return {};
    vector<ll> inv(1, bin_pow((b[0] % MOD + MOD) % MOD, MOD - 2));
    ll len = 1;
    while (len < m) {
        ll nlen = min(2 * len, m);
        vector<ll> t = vect_mult(cut_prefix(b, nlen), inv);
        t.resize(nlen, 0);
        for (int i = 0; i < nlen; ++i)
            t[i] = (MOD - t[i]) % MOD;
        t[0] = (t[0] + 2) % MOD;
        inv = vect_mult(inv, t);
        inv.resize(nlen, 0);
        len = nlen;
    }
    return inv;
}

// returns c, where c * b == a (mod x^m), requires b[0] != 0
vector<ll> div_mod_xm(const vector<ll>& a, const vector<ll>& b, ll m) {
    if (m <= 0)
        return {};
    return cut_prefix(vect_mult(cut_prefix(a, m), inv_mod_xm(b, m)), m);
}
