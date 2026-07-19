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

random_device rd;
mt19937 mersenne(rd());

struct Treap {
    Treap* left;
    Treap* right;
    ll x;
    unsigned int y;
    unsigned int size;
    ll dp;

    Treap(ll val = 0) {
        left = nullptr;
        right = nullptr;
        size = 1;
        x = val;
        y = mersenne();
        update(this);
    }

    friend ll get_dp(Treap* node) {
        if (node == nullptr)
            return 0;
        return node->dp;
    }

    friend unsigned int get_size(Treap* node) {
        if (node == nullptr)
            return 0;
        return node->size;
    }

    friend void update(Treap* node) {
        if (node == nullptr)
            return;
        node->size = 1 + get_size(node->left) + get_size(node->right);
        node->dp = max({node->x, get_dp(node->left), get_dp(node->right)});
    }

    // split by size
    friend pair<Treap*, Treap*> split_k(Treap* node, unsigned int k) {
        if (node == nullptr)
            return {nullptr, nullptr};
        if (get_size(node->left) < k) {
            auto res = split_k(node->right, k - get_size(node->left) - 1);
            node->right = res.first;
            update(node);
            return {node, res.second};
        } else {
            auto res = split_k(node->left, k);
            node->left = res.second;
            update(node);
            return {res.first, node};
        }
    }

    // split by dp: dp in left < k
    friend pair<Treap*, Treap*> split_dp(Treap* node, unsigned int k) {
        if (node == nullptr)
            return {nullptr, nullptr};
        if (get_dp(node) < k) {
            auto res = split_dp(node->right, k);
            node->right = res.first;
            update(node);
            return {node, res.second};
        } else {
            auto res = split_dp(node->left, k);
            node->left = res.second;
            update(node);
            return {res.first, node};
        }
    }

    friend Treap* merge(Treap* left, Treap* right) {
        if (left == nullptr)
            return right;
        if (right == nullptr)
            return left;
        if (left->y > right->y) {
            left->right = merge(left->right, right);
            update(left);
            return left;
        } else {
            right->left = merge(left, right->left);
            update(right);
            return right;
        }
    }

    friend Treap* insert(Treap* node, unsigned int pos, ll val) {
        auto res = split_k(node, pos);
        Treap* tr = new Treap(val);
        return merge(merge(res.first, tr), res.second);
    }

    friend Treap* erase(Treap* node, unsigned int pos) {
        auto res = split_k(node, pos + 1);
        auto left_res = split_k(res.first, pos);
        return merge(left_res.first, res.second);
    }

    // return {element, root}
    friend pair<Treap*, Treap*> kth_element(Treap* node, unsigned int k) {
        if (get_size(node) < k)
            return {nullptr, node};
        auto res = split_k(node, k + 1);
        auto left_res = split_k(res.first, k);
        return {left_res.second, merge(left_res.first, merge(left_res.second, res.second))};
    }

    // return sub segment [l, r) and remaineng Treap [0, l) + [r + 1, size)
    friend pair<Treap*, Treap*> cut(Treap* node, unsigned int l, unsigned int r) {
        auto res = split_k(node, r);
        auto left_res = split_k(res.first, l);
        return {left_res.second, merge(left_res.first, res.second)};
    }
};
