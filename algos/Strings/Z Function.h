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

vector<ll> z_function(string s) {
    ll n = s.size();
    vector<ll> z(n, 0);
    ll r = 0;
    for (int i = 1; i < n; ++i) {
        if (z[i - r] < r + z[r] - i)
            z[i] = z[i - r];
        else {
            z[i] = max(r + z[r] - i, 0ll);
            while (i + z[i] < n && s[i + z[i]] == s[z[i]])
                ++z[i];
            r = i;
        }
    }
    return z;
}
