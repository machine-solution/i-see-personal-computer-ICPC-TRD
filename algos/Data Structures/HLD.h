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

struct SegmentTree;

// includes

struct HLD {
    ll n;
    vector<ll> top, pos, h, par;

    ll gpos = 0;

    SegmentTree st;

    void calc_sizes(ll v, vector<char>& used, const vector<vector<ll>>& g, vector<ll>& s) {
        s[v] = 1;
        used[v] = 1;

        for (ll u : g[v]) {
            if (!used[u]) {
                calc_sizes(u, used, g, s);
                s[v] += s[u];
            }
        }
    }

    void build_dec(ll v, ll p, ll pr, vector<char>& used, const vector<vector<ll>>& g, vector<ll>& s) {
        used[v] = 1;
        pos[v] = gpos++;
        top[pos[v]] = pos[p];
        par[pos[v]] = pos[pr];
        if (pr != v)
            h[pos[v]] = h[pos[pr]] + 1;

        vector<ll> child;

        for (ll u : g[v]) {
            if (!used[u]) {
                child.push_back(u);
            }
        }

        for (int i = 1; i < child.size(); ++i) {
            if (s[child[i]] > s[child[0]]) {
                swap(child[i], child[0]);
            }
        }

        if (!child.empty()) {
            build_dec(child[0], p, v, used, g, s);
            for (int i = 1; i < child.size(); ++i) {
                build_dec(child[i], child[i], v, used, g, s);
            }
        }
    }

    HLD(ll N, const vector<vector<ll>>& g, const vector<ll>& a) {
        n = N;

        top.resize(n, 0);
        pos.resize(n, 0);
        h.resize(n, 0);
        par.resize(n, 0);
        vector<char> used(n, 0);
        vector<ll> s(n, 0);

        calc_sizes(0, used, g, s);
        used.assign(n, 0);
        build_dec(0, 0, 0, used, g, s);

        vector<ll> pa(n, 0);
        for (int i = 0; i < n; ++i) {
            pa[pos[i]] = a[i];
        }

        st = SegmentTree(pa);
    }

    ll query(ll u, ll v) {
        ll ans = 0;

        u = pos[u];
        v = pos[v];

        ll pu = top[u];
        ll pv = top[v];

        while (pu != pv) {
            if (h[pu] >= h[pv]) {
                ans += st.query(pu, u + 1).sum;
                u = par[pu];
                pu = top[u];
            } else {
                ans += st.query(pv, v + 1).sum;
                v = par[pv];
                pv = top[v];
            }
        }
        ans += st.query(min(u, v), max(u, v) + 1).sum;

        return ans;
    }

    void change(ll u, ll val) {
        u = pos[u];
        st.change(u, val);
    }
};
