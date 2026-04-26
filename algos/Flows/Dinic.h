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
    struct DinicEdge {
        ll a, b, cap, flow;
    };

    vector<DinicEdge> edges;
    vector<vector<ll>> g;
    ll n;

    FlowSolver(ll N) {
        n = N;
        g.resize(n);
    }

    void add_edge(ll u, ll v, ll w) {
        g[u].push_back(edges.size());
        edges.push_back({u, v, w, 0});
        g[v].push_back(edges.size());
        edges.push_back({v, u, 0, 0});
    }

    bool bfs(ll s, ll t, vector<ll>& d) {
        queue<ll> q;
        d.assign(n, -1);
        d[s] = 0;
        q.push(s);

        while (!q.empty()) {
            ll v = q.front();
            q.pop();

            for (ll id : g[v]) {
                ll u = edges[id].b;
                if (d[u] == -1) {
                    if (edges[id].cap > edges[id].flow) {
                        d[u] = d[v] + 1;
                        q.push(u);
                    }
                }
            }
        }

        return d[t] != -1;
    }

    ll dfs(ll v, ll t, ll flow, vector<ll>& d, vector<ll>& ptr) {
        if (flow == 0) {
            return 0;
        }
        if (v == t) {
            return flow;
        }

        for (; ptr[v] < g[v].size(); ++ptr[v]) {
            ll id = g[v][ptr[v]];
            ll u = edges[id].b;

            if (d[u] == d[v] + 1) {
                ll add_flow = dfs(u, t, min(flow, edges[id].cap - edges[id].flow), d, ptr);
                if (add_flow > 0) {
                    edges[id].flow += add_flow;
                    edges[id ^ 1].flow -= add_flow;
                    return add_flow;
                }
            }
        }
        return 0;
    }

    ll dinic(ll s, ll t) {
        ll flow = 0;
        vector<ll> d;
        while (bfs(s, t, d)) {
            vector<ll> ptr(n, 0);
            while (ll add_flow = dfs(s, t, inf, d, ptr)) {
                flow += add_flow;
            }
        }
        return flow;
    }
};
