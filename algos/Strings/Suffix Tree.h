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

// includes

struct SuffixTree {
    struct Node {
        ll l = 0, r = 0, par = -1, link = -1;
        map<char, ll> to;

        Node(ll L = 0, ll R = 0, ll Par = -1) : l(L), r(R), par(Par), link(-1) {
        }

        ll len() const {
            return r - l;
        }
    };

    string s;
    vector<Node> nodes;
    ll n = 0, sz = 1;
    ll v = 0, pos = 0;

    ll go(ll v, ll& pos, ll l, ll r) {
        while (l < r) {
            if (pos == nodes[v].len()) {
                auto it = nodes[v].to.find(s[l]);
                if (it == nodes[v].to.end())
                    return -1;
                v = it->second;
                pos = 0;
                continue;
            }
            if (s[nodes[v].l + pos] != s[l])
                return -1;
            ll rem = nodes[v].len() - pos;
            if (r - l < rem) {
                pos += r - l;
                return v;
            }
            l += rem;
            pos = nodes[v].len();
        }
        return v;
    }

    ll split(ll v, ll pos) {
        if (pos == nodes[v].len())
            return v;
        if (pos == 0)
            return nodes[v].par;
        Node u = nodes[v];
        ll mid = sz++;
        nodes[mid] = Node(u.l, u.l + pos, u.par);
        nodes[u.par].to[s[u.l]] = mid;
        nodes[mid].to[s[u.l + pos]] = v;
        nodes[v].par = mid;
        nodes[v].l += pos;
        return mid;
    }

    ll get_link(ll v) {
        if (nodes[v].link != -1)
            return nodes[v].link;
        if (nodes[v].par == -1)
            return 0;
        ll to = get_link(nodes[v].par);
        ll p = nodes[to].len();
        ll nv = go(to, p, nodes[v].l + (nodes[v].par == 0), nodes[v].r);
        return nodes[v].link = split(nv, p);
    }

    void extend(ll i) {
        while (true) {
            ll p = pos;
            ll nv = go(v, p, i, i + 1);
            if (nv != -1) {
                v = nv;
                pos = p;
                return;
            }
            ll mid = split(v, pos);
            ll leaf = sz++;
            nodes[leaf] = Node(i, n, mid);
            nodes[mid].to[s[i]] = leaf;
            v = get_link(mid);
            pos = nodes[v].len();
            if (mid == 0)
                break;
        }
    }

    SuffixTree() {
    }

    SuffixTree(string str) {
        s = str;
        n = s.size();
        nodes.assign(2 * n + 5, Node());
        nodes[0] = Node(0, 0, -1);
        sz = 1;
        v = 0;
        pos = 0;
        for (ll i = 0; i < n; ++i)
            extend(i);
    }
};
