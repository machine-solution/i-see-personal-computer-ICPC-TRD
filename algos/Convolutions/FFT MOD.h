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
using comp = complex<double>;

const ld PI = atan2(0, -1);
ll const inf = 1e18;

random_device rd;
mt19937 mersenne(rd());

ll rev(ll a);

// includes

const ll MOD = 998244353;
const ll ROOT = 31;
const ll REV_ROOT = rev(ROOT);
const int ROOT_PW = (1ll << 23);

vector<vector<ll>> w;

void precalc_ws(ll lg_n) {
    w.resize(lg_n + 1);
    w[0].resize(1, 1);
    for (int l = 1; l <= lg_n; ++l) {
        ll n = (1ll << l);
        w[l].resize(n / 2);

        ll fw = ROOT;
        for (int i = 0; i < 23 - l; ++i) {
            fw = (fw * fw) % MOD;
        }
        for (int j = 0; j < (n >> 1); ++j) {
            if (j % 2 == 0) {
                w[l][j] = w[l - 1][j / 2];
            } else {
                w[l][j] = (w[l - 1][j / 2] * fw) % MOD;
            }
        }
    }
}

void fft_inpl(vector<ll>& a, bool invert) {
    int n = a.size();

    // apply permutation by bit_rev
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;
        if (i < j)
            swap(a[i], a[j]);
    }

    for (int len = 2, lg = 1; len <= n; len <<= 1, ++lg) {
        for (int i = 0; i < n; i += len) {
            for (int j = 0; j < (len >> 1); ++j) {
                ll root = w[lg][j];
                if (invert) {
                    root = rev(root);
                }
                ll u = a[i + j], v = (a[i + j + len / 2] * root) % MOD;
                a[i + j] = u + v < MOD ? u + v : u + v - MOD;
                a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + MOD;
            }
        }
    }
    if (invert) {
        int nrev = rev(n);
        for (int i = 0; i < n; ++i) {
            a[i] *= nrev;
            a[i] %= MOD;
        }
    }
}

vector<ll> vect_mult(const vector<ll>& a, const vector<ll>& b) {
    ll sz = a.size() + b.size() - 1;
    ll n = 1;
    int lg_n = 0;
    while (n < sz) {
        n <<= 1;
        ++lg_n;
    }
    if (w.size() <= lg_n) {
        precalc_ws(lg_n);
    }

    vector<ll> fa(n, 0), fb(n, 0);
    for (int i = 0; i < n; ++i) {
        if (i < a.size()) {
            fa[i] = a[i];
        }
        if (i < b.size()) {
            fb[i] = b[i];
        }
    }

    fft_inpl(fa, false);
    fft_inpl(fb, false);
    for (int i = 0; i < n; ++i) {
        fa[i] *= fb[i];
        fa[i] %= MOD;
    }
    fft_inpl(fa, true);

    vector<ll> mult(sz);
    for (int i = 0; i < sz; ++i) {
        mult[i] = fa[i];
    }
    return mult;
}
