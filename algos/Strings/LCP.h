#pragma gcc optimize("Ofast");

#include <iostream>
#include <algorithm>
#include <vector>

#define all(a) a.begin(), a.end()

using namespace std;
using ll = long long;

// includes

// p - suffix massive output
// c - suffix massive classes
vector<ll> calc_lcp(ll n, const string& s, const vector<ll>& c, const vector<ll>& p) {
    vector<ll> lcp(n - 1, 0);

    ll pref = 0;

    for (int i = 0; i < n - 1; ++i) {
        ll pos = c[i];
        ll j = p[pos - 1];
        while (i + pref < n && j + pref < n && s[i + pref] == s[j + pref]) {
            ++pref;
        }
        lcp[pos - 1] = pref;
        pref = max(0ll, pref - 1);
    }

    return lcp;
}
