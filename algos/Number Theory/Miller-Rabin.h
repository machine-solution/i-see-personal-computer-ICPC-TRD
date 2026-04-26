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

ll bin_pow_mod(ll a, ll n, ll mod);

// includes

bool _miller_rabin_test(ll n, ll d, ll s, ll a) {
    ll x = bin_pow_mod(a, d, n);
    for (int i = 0; i < s; ++i) {
        ll y = ((__int128_t)x * x) % n;
        if (y == 1 && x != 1) {
            return false;
        }

        x = y;
    }
    if (x != 1) {
        return false;
    }
    return true;
}

bool is_prime_ml(ll n) {
    if (n <= 1) {
        return false;
    }
    // all primes
    vector<ll> as = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    ll d = n - 1;
    ll s = 0;
    while ((d & 1) == 0) {
        ++s;
        d >>= 1;
    }
    for (ll a : as) {
        if (n == a) {
            return true;
        }
        if (n % a == 0) {
            return false;
        }
        if (!_miller_rabin_test(n, d, s, a)) {
            return false;
        }
    }
    return true;
}
