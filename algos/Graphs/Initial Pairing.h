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

bool kuhn_dfs(int v, vector<char>& used, char mark, vector<vector<int>>& g, vector<int>& mt);

// includes

int init_pairing(int n1, int n2, const vector<vector<int>>& g, vector<int>& mt, vector<int>& rmt) {
    auto PACK = [](int x, int y) { return ((ll)x << 30) + y; };
    auto SUNPACK = [](int x) {
        const static ll mask = (1ll << 30) - 1;
        return x & mask;
    };

    vector<int> deg(n1, 0);
    vector<vector<int>> ng(n1);
    vector<vector<pair<int, int>>> rg(n2);
    vector<int> next(n1, 0);
    set<ll> que;
    for (int i = 0; i < n1; ++i) {
        for (auto j : g[i]) {
            ++deg[i];
            rg[j].push_back({i, ng[i].size()});
            ng[i].push_back(j);
        }
        que.insert(PACK(deg[i], i));
    }

    int pairing = 0;

    while (!que.empty()) {
        auto res = *que.begin();
        que.erase(res);

        int i = SUNPACK(res);
        if (deg[i] == 0) {
            continue;
        }

        while (ng[i][next[i]] == -1) {
            ++next[i];
        }
        int j = ng[i][next[i]++];

        deg[i] = 0;
        for (auto ks : rg[j]) {
            int k = ks.first;
            int sz = ks.second;
            if (deg[k] != 0) {
                ng[k][sz] = -1;
                que.erase(PACK(deg[k], k));
                --deg[k];
                que.insert(PACK(deg[k], k));
            }
        }
        mt[j] = i;
        rmt[i] = j;
        ++pairing;
    }
    return pairing;
}

vector<int> fast_kuhn(int n1, int n2, vector<vector<int>>& g) {
    vector<char> used(n1, 0);
    vector<int> mt(n2, -1), rmt(n1, -1);

    init_pairing(n1, n2, g, mt, rmt);

    int mark = 0;

    for (int run = 1; run;) {
        ++mark;
        run = 0;
        for (int i = 0; i < n1; ++i) {
            if (rmt[i] == -1 && kuhn_dfs(i, used, mark, g, mt)) {
                run = 1;
            }
        }

        for (int j = 0; j < n2; ++j) {
            if (mt[j] != -1) {
                rmt[mt[j]] = j;
            }
        }
    }

    return mt;
}
