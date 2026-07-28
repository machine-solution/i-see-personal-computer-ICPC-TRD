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

struct Matroid {
    virtual int size() const = 0;
    virtual void build(const vector<int>& on) = 0;
    virtual vector<int> circuit(int u) const = 0;
    virtual ~Matroid() = default;
};

// includes

// Find max weight independet set.
//   score: max weight
//   on: optimal independent set (0/1)
// M0, M1: matroid
//   size(): |ground set|
//   build(on): Current independent set is given.
//   circuit(u): Returns the circuit by adding u (must contain u), or {} if still independent.
pair<ll, vector<int>> matroidIntersection(Matroid& m0, Matroid& m1, const vector<ll>& weights) {
    const int n = weights.size();
    vector<int> on(n, 0);
    while (true) {
        m0.build(on);
        m1.build(on);
        vector<vector<int>> graph(n + 2);
        for (int u = 0; u < n; ++u) {
            if (!on[u]) {
                const vector<int> c0 = m0.circuit(u);
                if (c0.empty())
                    graph[n].push_back(u);
                for (const int v : c0)
                    if (u != v)
                        graph[v].push_back(u);
                const vector<int> c1 = m1.circuit(u);
                if (c1.empty())
                    graph[u].push_back(n + 1);
                for (const int v : c1)
                    if (u != v)
                        graph[u].push_back(v);
            }
        }
        queue<int> que;
        vector<int> inq(n + 2, 0);
        vector<int> prv(n + 2, -1);
        vector<ll> dist(n + 2, 0);
        prv[n] = -2;
        inq[n] = 1;
        que.push(n);
        while (!que.empty()) {
            const int u = que.front();
            que.pop();
            inq[u] = 0;
            for (const int v : graph[u]) {
                ll cc = dist[u];
                if (v < n)
                    on[v] ? (cc -= weights[v]) : (cc += weights[v]);
                if (!~prv[v] || dist[v] < cc) {
                    prv[v] = u;
                    dist[v] = cc;
                    if (!inq[v]) {
                        inq[v] = 1;
                        que.push(v);
                    }
                }
            }
        }
        if (~prv[n + 1] && dist[n + 1] > 0) {  // alarm: delete  && dist[n + 1] > 0 if want max size
            for (int u = n + 1; (u = prv[u]) != n;)
                on[u] ^= 1;
        } else {
            break;
        }
    }
    ll score = 0;
    for (int u = 0; u < n; ++u)
        if (on[u])
            score += weights[u];
    return make_pair(score, on);
};
