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

ll inv(ll a, ll m) {
    return a == 1 ? 1 : (1 - inv(m % a, a) * m) / a + m;
}

ll crt(ll a1, ll m1, ll a2, ll m2) {
    if (m1 == 1)
        return a2;
    if (m2 == 1)
        return a1;
    ll t = (a2 - a1) % m2;
    if (t < 0)
        t += m2;
    t = t * inv(m1 % m2, m2) % m2;
    return (a1 + m1 * t) % (m1 * m2);
}
