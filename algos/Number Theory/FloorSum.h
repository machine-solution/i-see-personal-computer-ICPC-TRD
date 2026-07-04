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

ll floor_div(ll a, ll b) {
    // b > 0
    if (a >= 0)
        return a / b;
    return -((-a + b - 1) / b);
}

// sum_{x=0}^{n-1} floor((a*x + b) / m), m > 0
ll floor_sum(ll n, ll m, ll a, ll b) {
    if (n <= 0) return 0;
    ll ans = 0;
    auto pull = [&](ll &x, ll w) {
        ll q = floor_div(x, m);
        ans += q * w;
        x -= q * m;
    };
    while (true) {
        pull(a, n * (n - 1) / 2);
        pull(b, n);
        ll y = a * n + b;
        if (y < m) return ans;
        n = y / m;
        b = y % m;
        swap(a, m);
    }
}
