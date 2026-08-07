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
vector<ll> inv_mod_xm(const vector<ll>& b, ll m);
vector<ll> cut_prefix(const vector<ll>& a, ll m);
ll bin_pow(ll a, ll n);

// includes

vector<ll> poly_deriv(const vector<ll>& a) {
    if (a.size() <= 1)
        return {0};
    vector<ll> r(a.size() - 1);
    for (int i = 1; i < a.size(); ++i)
        r[i - 1] = a[i] * i % MOD;
    return r;
}

vector<ll> poly_integr(const vector<ll>& a) {
    vector<ll> r(a.size() + 1);
    r[0] = 0;
    for (int i = 0; i < a.size(); ++i)
        r[i + 1] = a[i] * bin_pow(i + 1, MOD - 2) % MOD;
    return r;
}

// a[0] == 1; returns log(a) mod x^n
vector<ll> log_mod_xm(const vector<ll>& a, ll n) {
    if (n <= 0)
        return {};
    vector<ll> t = vect_mult(poly_deriv(cut_prefix(a, n)), inv_mod_xm(a, n));
    return cut_prefix(poly_integr(cut_prefix(t, n - 1)), n);
}
