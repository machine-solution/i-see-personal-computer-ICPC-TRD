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

bool point_on_line(const Point& p, const Line& line) {
    return sign_vectp(p - line.p1, line.p2 - line.p1) == 0;
}

bool point_on_segment(const Point& p, const Segment& seg) {
    return sign_vectp(p - seg.p2, seg.p1 - seg.p2) == 0 &&
           sign_scalp(p - seg.p2, seg.p1 - seg.p2) >= 0 &&
           sign_scalp(p - seg.p1, seg.p2 - seg.p1) >= 0;
}

bool is_segment_intersects_line(const Segment& seg, const Line& line) {
    return sign_vectp(seg.p1 - line.p1, line.p2 - line.p1) *
               sign_vectp(seg.p2 - line.p1, line.p2 - line.p1) <=
           0;
}

bool is_segment_intersects_segment(const Segment& seg1, const Segment& seg2) {
    if (sign_vectp(seg1.p2 - seg1.p1, seg2.p2 - seg2.p1) == 0) {
        return point_on_segment(seg1.p1, seg2) || point_on_segment(seg1.p2, seg2) ||
               point_on_segment(seg2.p1, seg1) || point_on_segment(seg2.p2, seg1);
    }
    return is_segment_intersects_line(seg1, Line(seg2)) &&
           is_segment_intersects_line(seg2, Line(seg1));
}

// intersection of supporting lines; segments must be non-parallel
Point lines_intersection(const Line& a, const Line& b) {
    Point A = a.p1, B = a.p2, C = b.p1, D = b.p2;
    ld t = vectp(C - A, D - C) / vectp(B - A, D - C);
    return A + (B - A) * t;
}

// unique intersection point; call only if segments properly intersect (non-collinear)
Point segments_intersection(const Segment& a, const Segment& b) {
    return lines_intersection(Line(a), Line(b));
}

// foot of perpendicular from p to line
Point project(const Point& p, const Line& line) {
    Point d = line.p2 - line.p1;
    return line.p1 + d * (scalp(p - line.p1, d) / d.abs2());
}

// reflection of p over line
Point reflect(const Point& p, const Line& line) {
    return project(p, line) * 2 - p;
}
