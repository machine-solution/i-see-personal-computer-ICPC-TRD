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

struct CentroidDecomp {
    ll n;
    vector<ll> parent;

    void calc_sizes(ll v, ll mark, vector<ll>& used, vector<vector<ll>>& g, vector<ll>& s) {
        s[v] = 1;
        used[v] = mark;

        for (ll u : g[v]) {
            if (used[u] < mark) {
                calc_sizes(u, mark, used, g, s);
                s[v] += s[u];
            }
        }
    }

    ll find_centroid(ll v, ll mark, vector<ll>& used, vector<vector<ll>>& g, vector<ll>& s, ll sz) {
        used[v] = mark;

        for (ll u : g[v]) {
            if (used[u] < mark) {
                if (s[u] > sz / 2) {
                    return find_centroid(u, mark, used, g, s, sz);
                }
            }
        }

        return v;
    }

    void set_parent(ll v, ll mark, vector<ll>& used, vector<vector<ll>>& g, ll cent) {
        if (v != cent)
            parent[v] = cent;
        used[v] = mark;

        for (ll u : g[v]) {
            if (used[u] < mark) {
                set_parent(u, mark, used, g, cent);
            }
        }
    }

    CentroidDecomp(ll N, vector<vector<ll>>& g) {
        n = N;
        parent.assign(n, -1);

        vector<ll> used(n, 0);
        vector<ll> s(n, 0);
        ll cnt_used = 0;
        ll mark = 0;

        while (cnt_used < n) {
            ++mark;
            ll pass_mark = mark;
            for (int i = 0; i < n; ++i) {
                if (used[i] >= pass_mark)
                    continue;
                calc_sizes(i, pass_mark, used, g, s);
                ++mark;
                ll cent = find_centroid(i, mark, used, g, s, s[i]);

                ++mark;
                set_parent(i, mark, used, g, cent);
                used[cent] = inf;
                ++cnt_used;
            }
        }
    }
};
