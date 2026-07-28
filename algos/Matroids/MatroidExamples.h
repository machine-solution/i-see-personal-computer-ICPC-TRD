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

const ll inf = 1e18;

// includes

struct Matroid {
    virtual int size() const = 0;
    virtual void build(const vector<int> &on) = 0;
    virtual vector<int> circuit(int u) const = 0;
    virtual ~Matroid() = default;
};

// independent  <=>  forest
struct ForestMatroid : Matroid {
    int n;
    vector<pair<int, int>> edges;
    ForestMatroid() {
    }
    ForestMatroid(int n_) : n(n_) {
    }
    void ae(int u, int v) {
        edges.emplace_back(u, v);
    }
    inline int size() const override {
        return edges.size();
    }
    void build(const vector<int> &on) override {
        const int m = edges.size();
        g.assign(n, {});
        for (int i = 0; i < m; ++i) {
            if (on[i]) {
                g[edges[i].first].push_back(i);
                g[edges[i].second].push_back(i);
            }
        }
        rs.assign(n, -1);
        pari.assign(n, -1);
        dep.assign(n, -1);
        for (int r = 0; r < n; ++r)
            if (!~rs[r])
                dfs(r, r, -1, 0);
    }
    // i0, then tree edges from edges[i0].first to edges[i0].second
    vector<int> circuit(int i0) const override {
        int u = edges[i0].first, v = edges[i0].second;
        if (rs[u] != rs[v])
            return {};
        vector<int> isU, isV;
        auto upU = [&]() -> void {
            const int i = pari[u];
            isU.push_back(i);
            u ^= edges[i].first ^ edges[i].second;
        };
        auto upV = [&]() -> void {
            const int i = pari[v];
            isV.push_back(i);
            v ^= edges[i].first ^ edges[i].second;
        };
        for (; dep[u] > dep[v]; upU()) {
        }
        for (; dep[u] < dep[v]; upV()) {
        }
        for (; u != v; upU(), upV()) {
        }
        reverse(isV.begin(), isV.end());
        vector<int> is{i0};
        is.insert(is.end(), isU.begin(), isU.end());
        is.insert(is.end(), isV.begin(), isV.end());
        return is;
    }
    vector<vector<int>> g;
    vector<int> rs, pari, dep;
    void dfs(int r, int u, int pi, int d) {
        rs[u] = r;
        pari[u] = pi;
        dep[u] = d;
        for (const int i : g[u])
            if (pi != i)
                dfs(r, edges[i].first ^ edges[i].second ^ u, i, d + 1);
    }
};

// independent  <=>  each connected component contains (<= 1) cycle
struct PseudoforestMatroid : Matroid {
    int n;
    vector<pair<int, int>> edges;
    PseudoforestMatroid() {
    }
    PseudoforestMatroid(int n_) : n(n_) {
    }
    void ae(int u, int v) {
        edges.emplace_back(u, v);
    }
    inline int size() const override {
        return edges.size();
    }
    void build(const vector<int> &on) override {
        const int m = edges.size();
        g.assign(n, {});
        for (int i = 0; i < m; ++i) {
            if (on[i]) {
                g[edges[i].first].push_back(i);
                g[edges[i].second].push_back(i);
            }
        }
        rs.assign(n, -1);
        pari.assign(n, -1);
        dep.assign(n, -1);
        cycs.assign(n, -1);
        for (int r = 0; r < n; ++r)
            if (!~rs[r])
                dfs(r, r, -1, 0);
    }
    vector<int> circuit(int i0) const override {
        vector<int> is;
        // Add edges in the smallest subtree containing us.
        auto addSubtree = [&](vector<int> us) -> void {
            while (true) {
                sort(us.begin(), us.end());
                us.erase(unique(us.begin(), us.end()), us.end());
                if (us.size() == 1)
                    break;
                int mx = -1;
                for (const int u : us)
                    if (mx < dep[u])
                        mx = dep[u];
                for (int &u : us)
                    if (mx == dep[u]) {
                        const int i = pari[u];
                        is.push_back(i);
                        u ^= edges[i].first ^ edges[i].second;
                    }
            }
        };
        const int u = edges[i0].first, v = edges[i0].second;
        if (rs[u] != rs[v]) {
            if (~cycs[rs[u]] && ~cycs[rs[v]]) {
                is.push_back(i0);
                is.push_back(cycs[rs[u]]);
                addSubtree({u, edges[cycs[rs[u]]].first, edges[cycs[rs[u]]].second});
                is.push_back(cycs[rs[v]]);
                addSubtree({v, edges[cycs[rs[v]]].first, edges[cycs[rs[v]]].second});
            }
        } else {
            if (~cycs[rs[u]]) {
                is.push_back(i0);
                is.push_back(cycs[rs[u]]);
                addSubtree({u, v, edges[cycs[rs[u]]].first, edges[cycs[rs[u]]].second});
            }
        }
        return is;
    }
    vector<vector<int>> g;
    vector<int> rs, pari, dep, cycs;
    void dfs(int r, int u, int pi, int d) {
        rs[u] = r;
        pari[u] = pi;
        dep[u] = d;
        for (const int i : g[u]) {
            if (pi != i) {
                const int v = edges[i].first ^ edges[i].second ^ u;
                if (~rs[v]) {
                    cycs[r] = i;
                } else {
                    dfs(r, v, i, d + 1);
                }
            }
        }
    }
};

// independent  <=>  #{ i | colors[i] = c } <= lims[c]
struct PartitionMatroid : Matroid {
    int n;
    vector<int> colors;
    vector<int> lims;
    PartitionMatroid() {
    }
    PartitionMatroid(const vector<int> &colors_, const vector<int> &lims_)
        : n(colors_.size()), colors(colors_), lims(lims_) {
    }
    inline int size() const override {
        return n;
    }
    void build(const vector<int> &on) override {
        iss.assign(lims.size(), {});
        for (int i = 0; i < n; ++i)
            if (on[i])
                iss[colors[i]].push_back(i);
    }
    vector<int> circuit(int i0) const override {
        const int c = colors[i0];
        if (static_cast<int>(iss[c].size()) == lims[c]) {
            vector<int> is = iss[c];
            is.push_back(i0);
            return is;
        } else {
            return {};
        }
    }
    vector<vector<int>> iss;
};

// independent  <=>  vectors are linearly independent over GF(2)
struct LinearMatroid : Matroid {
    int n;
    int maxLog;
    vector<ull> vals;

    int blocks;
    vector<ull> basis;
    vector<vector<ull>> basisMask;

    LinearMatroid() {
    }
    LinearMatroid(const vector<ull> &vals_, int maxLog_ = 64) {
        vals = vals_;
        n = vals.size();
        maxLog = maxLog_;
        blocks = (n + 63) / 64;
    }

    inline int size() const override {
        return n;
    }

    void set_bit(vector<ull> &mask, int i) {
        mask[i >> 6] |= (1ULL << (i & 63));
    }

    void xor_mask(vector<ull> &a, const vector<ull> &b) {
        for (int i = 0; i < blocks; ++i)
            a[i] ^= b[i];
    }

    void add_vector(ull x, int id) {
        vector<ull> cur(blocks, 0);
        set_bit(cur, id);

        for (int b = maxLog - 1; b >= 0; --b) {
            if (((x >> b) & 1ULL) == 0)
                continue;
            if (!basis[b]) {
                basis[b] = x;
                basisMask[b] = cur;
                return;
            }
            x ^= basis[b];
            xor_mask(cur, basisMask[b]);
        }
    }

    void build(const vector<int> &on) override {
        basis.assign(maxLog, 0);
        basisMask.assign(maxLog, vector<ull>(blocks, 0));
        for (int i = 0; i < n; ++i) {
            if (on[i])
                add_vector(vals[i], i);
        }
    }

    vector<int> circuit(int u) const override {
        ull x = vals[u];
        vector<ull> cur(blocks, 0);
        cur[u >> 6] |= (1ULL << (u & 63));

        for (int b = maxLog - 1; b >= 0; --b) {
            if (((x >> b) & 1ULL) == 0)
                continue;
            if (!basis[b]) {
                return {};
            }
            x ^= basis[b];
            for (int i = 0; i < blocks; ++i)
                cur[i] ^= basisMask[b][i];
        }

        vector<int> res;
        for (int i = 0; i < n; ++i) {
            if ((cur[i >> 6] >> (i & 63)) & 1ULL)
                res.push_back(i);
        }
        return res;
    }
};

// independent  <=>  selected left vertices can be fully matched to right part
struct TransversalMatroid : Matroid {
    int n, m;
    vector<vector<int>> g;
    vector<int> cur_on;

    vector<int> matchL, matchR;
    vector<int> used;
    int used_timer;

    TransversalMatroid() {
    }
    TransversalMatroid(int n_, int m_) : n(n_), m(m_) {
        g.assign(n, {});
    }

    void ae(int u, int v) {
        g[u].push_back(v);
    }

    inline int size() const override {
        return n;
    }

    bool dfs_kuhn(int u) {
        if (used[u] == used_timer)
            return false;
        used[u] = used_timer;
        for (int v : g[u]) {
            int w = matchR[v];
            if (w == -1 || dfs_kuhn(w)) {
                matchL[u] = v;
                matchR[v] = u;
                return true;
            }
        }
        return false;
    }

    void build(const vector<int> &on) override {
        cur_on = on;
        matchL.assign(n, -1);
        matchR.assign(m, -1);
        used.assign(n, 0);
        used_timer = 0;
        for (int u = 0; u < n; ++u) {
            if (cur_on[u]) {
                ++used_timer;
                dfs_kuhn(u);
            }
        }
    }

    vector<int> circuit(int u0) const override {
        vector<int> visL(n, 0), visR(m, 0);
        queue<int> q;
        visL[u0] = 1;
        q.push(u0);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : g[u]) {
                if (visR[v])
                    continue;
                visR[v] = 1;
                if (matchR[v] == -1) {
                    return {};
                }
                int w = matchR[v];
                if (!visL[w] && cur_on[w]) {
                    visL[w] = 1;
                    q.push(w);
                }
            }
        }

        vector<int> res;
        for (int u = 0; u < n; ++u) {
            if (u == u0 || (cur_on[u] && visL[u]))
                res.push_back(u);
        }
        return res;
    }
};
