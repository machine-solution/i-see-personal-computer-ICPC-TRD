#pragma gcc optimize("Ofast");

#include <iostream>
#include <algorithm>
#include <vector>

#define all(a) a.begin(), a.end()

using namespace std;
using ll = long long;

// includes

void radix_sort(vector<pair<pair<ll, ll>, ll>>& prs) {
    ll n = prs.size();
    vector<ll> cnt(n, 0);
    for (auto mem : prs) {
        cnt[mem.first.first]++;
    }
    vector<ll> ptr(n, 0);
    for (int i = 1; i < n; ++i) {
        ptr[i] = ptr[i - 1] + cnt[i - 1];
    }

    vector<pair<pair<ll, ll>, ll>> new_prs(n);
    for (auto mem : prs) {
        ll i = mem.first.first;
        new_prs[ptr[i]++] = mem;
    }
    swap(prs, new_prs);
}

// s already includes last symbol '$' (-inf)
pair<vector<ll>, vector<ll>> suffix_array(const vector<ll>& s) {
    ll n = s.size();
    vector<ll> p(n), c(n);

    {  // k == 0
        vector<pair<ll, ll>> prs(n);
        for (int i = 0; i < n; ++i) {
            prs[i] = {s[i], i};
        }
        sort(all(prs));
        for (int i = 0; i < n; ++i) {
            p[i] = prs[i].second;
        }

        c[p[0]] = 0;
        for (int i = 1; i < n; ++i) {
            if (prs[i].first == prs[i - 1].first) {
                c[p[i]] = c[p[i - 1]];
            } else {
                c[p[i]] = c[p[i - 1]] + 1;
            }
        }
    }

    ll k = 0;

    while ((1ll << k) < n) {
        vector<pair<pair<ll, ll>, ll>> prs(n);
        for (int i = 0; i < n; ++i) {
            ll t = p[i];
            ll j = ((t - (1ll << k)) % n + n) % n;
            prs[i] = {{c[j], c[t]}, j};
        }

        radix_sort(prs);

        for (int i = 0; i < n; ++i) {
            p[i] = prs[i].second;
        }

        c[p[0]] = 0;
        for (int i = 1; i < n; ++i) {
            if (prs[i].first == prs[i - 1].first) {
                c[p[i]] = c[p[i - 1]];
            } else {
                c[p[i]] = c[p[i - 1]] + 1;
            }
        }

        ++k;
    }

    return {p, c};
}
