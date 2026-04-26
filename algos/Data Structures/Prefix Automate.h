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

struct PrefixAutomate {
    const static int A = 26;

    struct Node {
        int next[A] = {-1};
        int link = -1;

        int prev = -1;
        int pe = -1;

        int has = 0;
    };

    vector<Node> tree;

    PrefixAutomate() {
        tree.push_back(Node());
    }

    void add(const string& s) {
        int v = 0;
        for (char c : s) {
            int e = c - 'a';
            if (tree[v].next[e] == -1) {
                tree[v].next[e] = tree.size();
                tree.push_back(Node());
                tree.back().prev = v;
                tree.back().pe = e;
            }
            v = tree[v].next[e];
        }
        tree[v].has = 1;
    }

    void addVect(const vector<string>& vs) {
        for (const auto& s : vs) {
            add(s);
        }
    }

    void del(const string& s) {
        int v = 0;
        for (char c : s) {
            int e = c - 'a';
            if (tree[v].next[e] == -1) {
                return;
            }
            v = tree[v].next[e];
        }
        tree[v].has = 0;
    }

    void ahoCorasick() {
        queue<int> que;

        tree[0].prev = 0;
        tree[0].pe = -1;

        tree[0].link = 0;
        for (int i = 0; i < A; ++i) {
            if (tree[0].next[i] == -1) {
                tree[0].next[i] = 0;
            } else {
                que.push(tree[0].next[i]);
            }
        }

        while (!que.empty()) {
            int v = que.front();
            que.pop();

            if (tree[v].link == -1) {
                if (v == 0 || tree[v].prev == 0)
                    tree[v].link = 0;
                else
                    tree[v].link = tree[tree[v].prev].next[tree[v].pe];
            }

            for (int i = 0; i < A; ++i) {
                if (tree[v].next[i] == -1) {
                    tree[v].next[i] = tree[tree[v].link].next[i];
                } else {
                    que.push(tree[v].next[i]);
                }
            }
        }
    }
};
