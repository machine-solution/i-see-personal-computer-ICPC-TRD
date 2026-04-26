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

    void calc_sizes(ll v, vector<char>& used, vector<vector<ll>>& g, vector<ll>& s) {
        s[v] = 1;
        used[v] = 1;

        for (ll u : g[v]) {
            if (!used[u]) {
                calc_sizes(u, used, g, s);
                s[v] += s[u];
            }
        }
    }

    ll find_centroid(ll v, vector<char>& used, vector<vector<ll>>& g, vector<ll>& s, ll sz) {
        used[v] = 1;

        for (ll u : g[v]) {
            if (!used[u]) {
                if (s[u] > sz / 2) {
                    return find_centroid(u, used, g, s, sz);
                }
            }
        }

        return v;
    }

    void set_parent(ll v, vector<char>& used, vector<vector<ll>>& g, ll cent) {
        if (v != cent)
            parent[v] = cent;
        used[v] = 1;

        for (ll u : g[v]) {
            if (!used[u]) {
                set_parent(u, used, g, cent);
            }
        }
    }

    CentroidDecomp(ll N, vector<vector<ll>>& g) {
        n = N;
        parent.assign(n, -1);

        vector<char> cused(n, 0);
        vector<ll> s(n, 0);
        ll cnt_used = 0;

        while (cnt_used < n) {
            vector<char> sused = cused, used = cused, pused = cused;

            for (int i = 0; i < n; ++i) {
                if (!sused[i]) {
                    calc_sizes(i, sused, g, s);
                    ll cent = find_centroid(i, used, g, s, s[i]);

                    cused[cent] = 1;
                    ++cnt_used;
                    set_parent(i, pused, g, cent);
                }
            }
        }
    }
};
