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
        g.assign((int)(2 * n), {});
        gt.assign((int)(2 * n), {});
    }

    // literal: var in [0..n), val=false -> 2*var, val=true -> 2*var+1
    ll lit(ll var, ll val) { return 2 * var + (val ? 1 : 0); }
    ll neg(ll x) { return x ^ 1; }

    void add_imp(ll a, ll b) {
        g[(int)a].push_back(b);
        gt[(int)b].push_back(a);
    }

    // (a_var is a_val) OR (b_var is b_val)
    void add_or(ll a_var, ll a_val, ll b_var, ll b_val) {
        ll a = lit(a_var, a_val);
        ll b = lit(b_var, b_val);
        add_imp(neg(a), b);
        add_imp(neg(b), a);
    }

    void dfs1(ll v) {
        used[(int)v] = 1;
        for (ll to : g[(int)v]) if (!used[(int)to]) dfs1(to);
        order.push_back(v);
    }

    void dfs2(ll v, ll cl) {
        comp[(int)v] = cl;
        for (ll to : gt[(int)v]) if (comp[(int)to] == -1) dfs2(to, cl);
    }

    // returns empty vector if unsat; otherwise assignment[i] in {0,1}
    vector<ll> solve() {
        used.assign((int)(2 * n), 0);
        order.clear();
        for (ll i = 0; i < 2 * n; ++i) if (!used[(int)i]) dfs1(i);

        comp.assign((int)(2 * n), -1);
        ll j = 0;
        for (ll i = 2 * n - 1; i >= 0; --i) {
            ll v = order[(int)i];
            if (comp[(int)v] == -1) dfs2(v, j++);
        }

        vector<ll> ans((int)n);
        for (ll i = 0; i < n; ++i) {
            if (comp[(int)(2 * i)] == comp[(int)(2 * i + 1)]) return {};
            ans[(int)i] = (comp[(int)(2 * i)] < comp[(int)(2 * i + 1)]);
        }
        return ans;
    }
};

