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

const ll MOD = 998244353;

random_device rd;
mt19937 mersenne(rd());

// includes

vector<vector<comp>> w;

void precalc_ws(ll lg_n) {
    w.resize(lg_n + 1);
    w[0].resize(1, 1);
    for (int l = 1; l <= lg_n; ++l) {
        ll n = (1ll << l);
        w[l].resize(n / 2);

        // need cosl, sinl for complex<ld>
        comp fw(cos(2 * PI / n), sin(2 * PI / n));
        for (int j = 0; j < (n >> 1); ++j) {
            if (j % 2 == 0) {
                w[l][j] = w[l - 1][j / 2];
            } else {
                w[l][j] = w[l - 1][j / 2] * fw;
            }
        }
    }
}

void fft_inpl(vector<comp>& a, bool invert) {
    int n = a.size();

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
                comp root = w[lg][j];
                if (invert)
                    root = conj(root);
                comp u = a[i + j], v = a[i + j + (len >> 1)] * root;
                a[i + j] = u + v;
                a[i + j + (len >> 1)] = u - v;
            }
        }
    }
    if (invert) {
        double rev_n = 1.0 / n;
        for (int i = 0; i < n; ++i) {
            a[i] *= rev_n;
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

    vector<comp> p(n, 0);
    for (int i = 0; i < n; ++i) {
        p[i] = comp(i < a.size() ? a[i] : 0, i < b.size() ? b[i] : 0);
    }

    fft_inpl(p, false);
    for (int i = 0; i < n; ++i) {
        p[i] *= p[i];
    }
    fft_inpl(p, true);

    vector<ll> mult(sz);
    for (int i = 0; i < sz; ++i) {
        // need roundl for complex<ld>
        mult[i] = round(p[i].imag() * 0.5);
    }
    return mult;
}
