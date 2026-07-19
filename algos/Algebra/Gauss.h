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

// a: n x (m + 1), last column is free terms
// returns one solution (free vars = 0), empty vector if no solutions
vector<ld> gauss(vector<vector<ld>> a) {
    const ld EPS = 1e-12;
    ll n = a.size();
    if (n == 0) {
        return {};
    }
    ll m = a[0].size() - 1;

    vector<ll> where(m, -1);
    for (ll col = 0, row = 0; col < m && row < n; ++col) {
        ll sel = row;
        for (ll i = row; i < n; ++i) {
            if (fabsl(a[i][col]) > fabsl(a[sel][col]))
                sel = i;
        }
        if (fabsl(a[sel][col]) < EPS)
            continue;

        swap(a[sel], a[row]);
        where[col] = row;

        for (ll i = 0; i < n; ++i) {
            if (i == row)
                continue;
            ld c = a[i][col] / a[row][col];
            if (fabsl(c) < EPS)
                continue;
            for (ll j = col; j <= m; ++j)
                a[i][j] -= a[row][j] * c;
        }
        ++row;
    }

    vector<ld> ans(m, 0);
    for (ll i = 0; i < m; ++i) {
        if (where[i] != -1)
            ans[i] = a[where[i]][m] / a[where[i]][i];
    }

    for (ll i = 0; i < n; ++i) {
        ld sum = 0;
        for (ll j = 0; j < m; ++j)
            sum += ans[j] * a[i][j];
        if (fabsl(sum - a[i][m]) > EPS)
            return {};
    }
    return ans;
}
