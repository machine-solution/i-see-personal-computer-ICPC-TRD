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

struct SATSolver {
    ll n;
    vector<vector<ll>> g, gt;
    vector<char> used;
    vector<ll> order, comp;

    void init(ll N) {
        n = N;
        g.assign(2 * n, {});
        gt.assign(2 * n, {});
    }

    // literal: var in [0..n), val=false -> 2*var, val=true -> 2*var+1
    ll lit(ll var, ll val) {
        return 2 * var + (val ? 1 : 0);
    }
    ll neg(ll x) {
        return x ^ 1;
    }

    void add_imp(ll a, ll b) {
        g[a].push_back(b);
        gt[b].push_back(a);
    }

    // (a_var is a_val) OR (b_var is b_val)
    void add_or(ll a_var, ll a_val, ll b_var, ll b_val) {
        ll a = lit(a_var, a_val);
        ll b = lit(b_var, b_val);
        add_imp(neg(a), b);
        add_imp(neg(b), a);
    }

    void dfs1(ll v) {
        used[v] = 1;
        for (ll to : g[v])
            if (!used[to])
                dfs1(to);
        order.push_back(v);
    }

    void dfs2(ll v, ll cl) {
        comp[v] = cl;
        for (ll to : gt[v])
            if (comp[to] == -1)
                dfs2(to, cl);
    }

    // returns empty vector if unsat; otherwise assignment[i] in {0,1}
    vector<ll> solve() {
        used.assign(2 * n, 0);
        order.clear();
        for (ll i = 0; i < 2 * n; ++i)
            if (!used[i])
                dfs1(i);

        comp.assign(2 * n, -1);
        ll j = 0;
        for (ll i = 2 * n - 1; i >= 0; --i) {
            ll v = order[i];
            if (comp[v] == -1)
                dfs2(v, j++);
        }

        vector<ll> ans(n);
        for (ll i = 0; i < n; ++i) {
            if (comp[2 * i] == comp[2 * i + 1])
                return {};
            ans[i] = (comp[2 * i] < comp[2 * i + 1]);
        }
        return ans;
    }
};
