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

// includes

int poly_deg(const vector<ll>& a) {
    int d = (int)a.size() - 1;
    while (d >= 0 && a[d] == 0)
        --d;
    return d;
}

vector<ll> poly_trim(vector<ll> a) {
    while (!a.empty() && a.back() == 0)
        a.pop_back();
    return a;
}

vector<ll> poly_rev(vector<ll> a, ll n) {
    a.resize(n, 0);
    reverse(a.begin(), a.end());
    return a;
}

// a % b, requires b.back() != 0
vector<ll> poly_mod(vector<ll> a, vector<ll> b) {
    a = poly_trim(a);
    b = poly_trim(b);
    int n = poly_deg(a), m = poly_deg(b);
    if (n < m)
        return a;
    ll k = n - m + 1;
    vector<ll> q = vect_mult(poly_rev(a, n + 1), inv_mod_xm(poly_rev(b, m + 1), k));
    q.resize(k, 0);
    reverse(q.begin(), q.end());
    vector<ll> qb = vect_mult(q, b);
    vector<ll> r(m, 0);
    for (int i = 0; i < m; ++i) {
        ll ai = i < a.size() ? a[i] : 0;
        ll bi = i < qb.size() ? qb[i] : 0;
        r[i] = (ai + MOD - bi % MOD) % MOD;
    }
    return poly_trim(r);
}

vector<vector<ll>> multipoint_tree;
vector<ll> multipoint_xs, multipoint_ys;

void multipoint_build(int v, int l, int r) {
    if (r - l == 1) {
        multipoint_tree[v] = {(MOD - multipoint_xs[l] % MOD) % MOD, 1};
        return;
    }
    int mid = (l + r) / 2;
    multipoint_build(2 * v, l, mid);
    multipoint_build(2 * v + 1, mid, r);
    multipoint_tree[v] = vect_mult(multipoint_tree[2 * v], multipoint_tree[2 * v + 1]);
}

void multipoint_dfs(int v, int l, int r, vector<ll> p) {
    p = poly_mod(p, multipoint_tree[v]);
    if (r - l == 1) {
        multipoint_ys[l] = p.empty() ? 0 : p[0];
        return;
    }
    int mid = (l + r) / 2;
    multipoint_dfs(2 * v, l, mid, p);
    multipoint_dfs(2 * v + 1, mid, r, p);
}

// evaluate poly p at xs[i]; |p| = n, |xs| = m, O((n + m) log^2 m)
vector<ll> multipoint_eval(vector<ll> p, vector<ll> xs) {
    int m = xs.size();
    if (m == 0)
        return {};
    multipoint_xs = xs;
    multipoint_ys.assign(m, 0);
    multipoint_tree.assign(4 * m, {});
    multipoint_build(1, 0, m);
    multipoint_dfs(1, 0, m, p);
    return multipoint_ys;
}
