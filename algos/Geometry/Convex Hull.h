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

#define all(a) a.begin(), a.end()

using ll = long long;
using lll = __int128_t;
using ld = long double;
using ull = unsigned long long;

const ld PI = atan2l(0, -1);
const ld EPS = 1e-9;

ll sign(ld x) {
    if (x < -EPS)
        return -1;
    if (x > EPS)
        return 1;
    return 0;
}

struct Point {
    ld x = 0, y = 0;

    Point(ld X = 0, ld Y = 0) : x(X), y(Y) {
    }

    ld abs2() const {
        return x * x + y * y;
    }

    ld abs() const {
        return sqrtl(abs2());
    }

    ld ang() const {
        return atan2l(y, x);
    }

    friend Point operator+(const Point& a, const Point& b) {
        return Point(a.x + b.x, a.y + b.y);
    }

    friend Point operator-(const Point& a, const Point& b) {
        return Point(a.x - b.x, a.y - b.y);
    }

    friend Point operator-(const Point& a) {
        return Point(-a.x, -a.y);
    }

    friend Point operator*(const Point& a, ld w) {
        return Point(a.x * w, a.y * w);
    }

    friend Point operator*(ld w, const Point& a) {
        return a * w;
    }

    friend Point operator/(const Point& a, ld w) {
        return Point(a.x / w, a.y / w);
    }

    friend Point& operator*=(Point& a, ld w) {
        a.x *= w;
        a.y *= w;
        return a;
    }

    friend istream& operator>>(istream& in, Point& p) {
        return in >> p.x >> p.y;
    }

    friend ostream& operator<<(ostream& out, const Point& p) {
        return out << p.x << ' ' << p.y;
    }

    friend ld vectp(const Point& a, const Point& b) {
        return a.x * b.y - a.y * b.x;
    }

    friend ll sign_vectp(const Point& a, const Point& b) {
        return sign(vectp(a, b));
    }

    friend ld scalp(const Point& a, const Point& b) {
        return a.x * b.x + a.y * b.y;
    }

    friend ll sign_scalp(const Point& a, const Point& b) {
        return sign(scalp(a, b));
    }

    lll operator*(const Point& p) const {
        return (lll)x * p.x + (lll)y * p.y;
    }

    lll operator^(const Point& p) const {
        return (lll)x * p.y - (lll)y * p.x;
    }

    friend bool operator==(const Point& a, const Point& b) {
        return sign(a.x - b.x) == 0 && sign(a.y - b.y) == 0;
    }
};

struct Segment {
    Point p1, p2;

    Segment(const Point& P1 = {}, const Point& P2 = {}) : p1(P1), p2(P2) {
    }

    friend istream& operator>>(istream& in, Segment& seg) {
        return in >> seg.p1 >> seg.p2;
    }
};

struct Line {
    Point p1, p2;

    Line(const Point& P1 = {}, const Point& P2 = {}) : p1(P1), p2(P2) {
    }

    Line(const Segment& seg) : p1(seg.p1), p2(seg.p2) {
    }

    friend istream& operator>>(istream& in, Line& line) {
        return in >> line.p1 >> line.p2;
    }
};

// includes

lll is_ccw_trio(const Point& p1, const Point& p2, const Point& p3) {
    return (p2 - p1) ^ (p3 - p1);
}

vector<Point> convex_hull(vector<Point> pts) {
    auto more = [](const Point& p1, const Point& p2) { return p1.x == p2.x ? p1.y > p2.y : p1.x > p2.x; };
    ll n = pts.size();
    if (n <= 1) {
        return pts;
    }
    for (int i = 0; i < n - 1; ++i) {
        if (more(pts[i], pts[n - 1])) {
            swap(pts[i], pts[n - 1]);
        }
    }

    Point p0 = pts.back();
    pts.pop_back();
    sort(all(pts), [&](const Point& p1, const Point& p2) {
        lll vp = is_ccw_trio(p0, p1, p2);
        return vp > 0;
    });
    Point pf = pts[0];

    sort(all(pts), [&](const Point& p1, const Point& p2) {
        lll vp = is_ccw_trio(p0, p1, p2);
        if (vp != 0)
            return vp > 0;
        if (is_ccw_trio(p0, pf, p1) == 0) {
            return (p1 - p0).abs() < (p2 - p0).abs();
        } else {
            return (p1 - p0).abs() > (p2 - p0).abs();
        }
    });

    vector<Point> hull;
    hull.push_back(p0);
    for (const Point& p : pts) {
        while (hull.size() >= 2) {
            const Point& a = hull[hull.size() - 2];
            const Point& b = hull[hull.size() - 1];

            if (is_ccw_trio(a, b, p) < 0) {
                hull.pop_back();
            } else {
                break;
            }
        }
        hull.push_back(p);
    }

    return hull;
}
