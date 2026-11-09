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
    vector<ll> e, pot;
    ll n;

    FlowSolver(ll N) {
        n = N;
        g.resize(n);
        e.assign(n, 0);
        pot.assign(n, 0);
    }

    void add_edge(ll u, ll v, ll w, ll c) {
        g[u].push_back(edges.size());
        edges.push_back({u, v, w, 0, c});
        g[v].push_back(edges.size());
        edges.push_back({v, u, 0, 0, -c});
    }

    ll dijkstra(vector<ll>& par, ll delta) {
        vector<ll> d(n, inf);
        par.assign(n, -1);
        priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> q;
        for (int i = 0; i < n; ++i) {
            if (e[i] >= delta) {
                d[i] = 0;
                q.push({d[i], i});
            }
        }

        ll t = -1;

        while (!q.empty()) {
            auto [dv, v] = q.top();
            q.pop();

            if (dv != d[v]) {
                continue;
            }

            if (e[v] <= -delta) {
                t = v;
                break;
            }

            for (ll id : g[v]) {
                if (edges[id].cap - edges[id].flow < delta) {
                    continue;
                }
                ll u = edges[id].b;
                ll ncost = dv + edges[id].cost + pot[v] - pot[u];
                if (d[u] > ncost) {
                    d[u] = ncost;
                    q.push({d[u], u});
                    par[u] = id;
                }
            }
        }
        if (t == -1) {
            return -1;
        }
        for (int i = 0; i < n; ++i) {
            pot[i] += min(d[i], d[t]);
        }

        return t;
    }

    ll push_flow(ll t, const vector<ll>& par, ll delta) {
        ll v = t;
        ll cost = 0;
        while (par[v] != -1) {
            ll id = par[v];
            edges[id].flow += delta;
            edges[id ^ 1].flow -= delta;
            v = edges[par[v]].a;
            cost += delta * edges[id].cost;
        }
        e[t] += delta;
        e[v] -= delta;

        return cost;
    }

    ll delta_iter(ll delta) {
        ll cost = 0;
        // maxing negative edges
        for (ll id = 0; id < edges.size(); ++id) {
            if (edges[id].cap - edges[id].flow < delta) {
                continue;
            }

            if (edges[id].cost + pot[edges[id].a] - pot[edges[id].b] < 0) {
                ll dflow = edges[id].cap - edges[id].flow;
                cost += dflow * edges[id].cost;
                edges[id].flow += dflow;
                edges[id ^ 1].flow -= dflow;
                e[edges[id].a] -= dflow;
                e[edges[id].b] += dflow;
            }
        }

        vector<ll> par;
        ll t = -1;
        while ((t = dijkstra(par, delta)) != -1) {
            cost += push_flow(t, par, delta);
        }

        return cost;
    }

    ll cap_scaling(ll s, ll t, ll F) {
        ll max_w = 0;
        for (const auto& edge : edges) {
            max_w = max(max_w, edge.cap - edge.flow);
        }
        ll delta = 1;
        while ((delta << 1) <= max_w) {
            delta <<= 1;
        }
        e[s] = F;
        e[t] = -F;

        ll cost = 0;
        while (delta >= 1) {
            cost += delta_iter(delta);
            delta >>= 1;
        }
        return cost;
    }
};
