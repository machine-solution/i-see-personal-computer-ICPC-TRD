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
        g.assign((int)n1, {});
        mt1.assign((int)n1, -1);
        mt2.assign((int)n2, -1);
        dist.assign((int)n1, 0);
    }

    void add_edge(ll u, ll v) {
        // u in [0..n1), v in [0..n2)
        g[(int)u].push_back(v);
    }

    bool bfs() {
        queue<ll> q;
        for (ll i = 0; i < n1; ++i) {
            if (mt1[(int)i] == -1) {
                dist[(int)i] = 0;
                q.push(i);
            } else {
                dist[(int)i] = -1;
            }
        }

        bool found = false;
        while (!q.empty()) {
            ll v = q.front();
            q.pop();

            for (ll to : g[(int)v]) {
                ll u = mt2[(int)to];
                if (u == -1) {
                    found = true;
                } else if (dist[(int)u] == -1) {
                    dist[(int)u] = dist[(int)v] + 1;
                    q.push(u);
                }
            }
        }
        return found;
    }

    bool dfs(ll v) {
        for (ll to : g[(int)v]) {
            ll u = mt2[(int)to];
            if (u == -1 || (dist[(int)u] == dist[(int)v] + 1 && dfs(u))) {
                mt1[(int)v] = to;
                mt2[(int)to] = v;
                return true;
            }
        }
        dist[(int)v] = -1;
        return false;
    }

    ll solve() {
        ll matching = 0;
        while (bfs()) {
            for (ll i = 0; i < n1; ++i) {
                if (mt1[(int)i] == -1) {
                    if (dfs(i)) {
                        ++matching;
                    }
                }
            }
        }
        return matching;
    }
};

