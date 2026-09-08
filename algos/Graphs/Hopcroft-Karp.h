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

struct HopcroftKarp {
    ll n1, n2;
    vector<vector<ll>> g;
    vector<ll> dist;
    vector<ll> mt1, mt2;

    void init(ll N1, ll N2) {
        n1 = N1;
        n2 = N2;
        g.assign(n1, {});
        mt1.assign(n1, -1);
        mt2.assign(n2, -1);
        dist.assign(n1, 0);
    }

    void add_edge(ll u, ll v) {
        // u in [0..n1), v in [0..n2)
        g[u].push_back(v);
    }

    bool bfs() {
        queue<ll> q;
        for (ll i = 0; i < n1; ++i) {
            if (mt1[i] == -1) {
                dist[i] = 0;
                q.push(i);
            } else {
                dist[i] = -1;
            }
        }

        bool found = false;
        while (!q.empty()) {
            ll v = q.front();
            q.pop();

            for (ll to : g[v]) {
                ll u = mt2[to];
                if (u == -1) {
                    found = true;
                } else if (dist[u] == -1) {
                    dist[u] = dist[v] + 1;
                    q.push(u);
                }
            }
        }
        return found;
    }

    bool dfs(ll v) {
        for (ll to : g[v]) {
            ll u = mt2[to];
            if (u == -1 || (dist[u] == dist[v] + 1 && dfs(u))) {
                mt1[v] = to;
                mt2[to] = v;
                return true;
            }
        }
        dist[v] = -1;
        return false;
    }

    ll solve() {
        ll matching = 0;
        while (bfs()) {
            for (ll i = 0; i < n1; ++i) {
                if (mt1[i] == -1) {
                    if (dfs(i)) {
                        ++matching;
                    }
                }
            }
        }
        return matching;
    }
};

