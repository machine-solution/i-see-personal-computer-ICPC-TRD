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
#include <bits/extc++.h>
#include <chrono>

using namespace std;

using ll = long long;
using ld = long double;
using ull = unsigned long long;
using comp = complex<ld>;

ll inf = 1e18;

// includes

struct FlowSolver {
    struct Edge {
        ll a, b, cap, flow, cost;
    };

    vector<Edge> edges;
    vector<vector<ll>> g;
    ll n;

    FlowSolver(ll N) {
        n = N;
        g.resize(n);
    }

    void add_edge(ll u, ll v, ll w, ll c) {
        g[u].push_back(edges.size());
        edges.push_back({u, v, w, 0, c});
        g[v].push_back(edges.size());
        edges.push_back({v, u, 0, 0, -c});
    }

    bool dijkstra(ll s, ll t, vector<ll>& d, vector<ll>& par) {
        vector<ll> nd(n, inf);
        par.assign(n, -1);
        priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> q;
        nd[s] = 0;
        q.push({nd[s], s});

        while (!q.empty()) {
            auto [ndv, v] = q.top();
            q.pop();

            if (ndv != nd[v]) {
                continue;
            }

            for (ll id : g[v]) {
                if (edges[id].cap == edges[id].flow) {
                    continue;
                }
                ll u = edges[id].b;
                ll ncost = ndv + edges[id].cost + d[v] - d[u];
                if (nd[u] > ncost) {
                    nd[u] = ncost;
                    q.push({nd[u], u});
                    par[u] = id;
                }
            }
        }
        for (int i = 0; i < n; ++i) {
            if (nd[i] != inf) {
                d[i] += nd[i];
            }
        }

        return nd[t] != inf;
    }

    pair<ll, ll> push_flow(ll s, ll t, const vector<ll>& par) {
        vector<ll> path;
        ll add_flow = inf;
        ll v = t;
        while (v != s) {
            path.push_back(par[v]);
            v = edges[par[v]].a;
            add_flow = min(add_flow, edges[par[v]].cap - edges[par[v]].flow);
        }
        ll cost = 0;
        for (ll id : path) {
            cost += add_flow * edges[id].cost;
            edges[id].flow += add_flow;
            edges[id ^ 1].flow -= add_flow;
        }
        return {add_flow, cost};
    }

    // initial costs must be non-negative
    pair<ll, ll> ssp(ll s, ll t) {
        ll flow = 0, cost = 0;
        vector<ll> d(n, 0), par;
        while (dijkstra(s, t, d, par)) {
            auto [add_flow, add_cost] = push_flow(s, t, par);
            flow += add_flow;
            cost += add_cost;
        }
        return {flow, cost};
    }
};
