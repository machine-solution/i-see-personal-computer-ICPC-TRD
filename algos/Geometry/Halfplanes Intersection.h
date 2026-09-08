#include "Basic.h"
#include <limits>

// includes

// Closed half-plane to the left of p1 -> p2; p1 != p2.
struct Halfplane {
    Point p, d;
    ld angle;

    Halfplane(const Line& line) : p(line.p1), d(line.p2 - line.p1) {
        d = d / d.abs();
        if (d.y == 0) d.y = 0; // Canonicalize signed zero at the angle cut.
        angle = d.ang();
    }

    bool outside(const Point& q) const {
        return vectp(d, q - p) < -EPS;
    }

    friend Point intersection(const Halfplane& a, const Halfplane& b) {
        return a.p + a.d * (vectp(b.p - a.p, b.d) / vectp(a.d, b.d));
    }
};

// O(n log n) time, O(n) memory. Uses Point, Line, EPS from Basic.h.
// Returns CCW vertices of intersection with [-bound, bound]^2; bound > 0.
// Choose bound to contain the desired answer; unbounded regions are clipped.
// Returns {} if empty, {p} for a point, {a, b} for a segment.
// Vertices within EPS are considered equal; no area threshold is used.
// Floating point: adjust EPS / rescale coordinates for the problem.
vector<Point> halfplanes_intersection(const vector<Line>& lines, ld bound) {
    vector<Halfplane> h;
    for (const Line& line : lines) {
        h.emplace_back(line);
    }
    vector<Point> box = {{-bound, -bound}, {bound, -bound},
                         {bound, bound}, {-bound, bound}};
    for (int i = 0; i < 4; ++i) {
        h.emplace_back(Line(box[i], box[(i + 1) % 4]));
    }
    // Exact comparison: using EPS in a sort comparator is invalid.
    sort(h.begin(), h.end(), [](const Halfplane& a, const Halfplane& b) {
        return a.angle < b.angle;
    });

    // If opposite constraints force a line, clip its parameter by ALL sides.
    // Called at most once, followed by return: O(n), including degeneracies.
    auto on_line = [&](const Halfplane& a) -> vector<Point> {
        ld lo = -numeric_limits<ld>::infinity(), hi = -lo;
        for (const Halfplane& b : h) {
            ld k = vectp(b.d, a.d), c = vectp(b.d, a.p - b.p);
            if (fabsl(k) <= EPS) {
                if (c < -EPS) return {};
            } else if (k > 0) {
                lo = max(lo, -c / k);
            } else {
                hi = min(hi, -c / k);
            }
        }
        if (lo > hi + EPS) return {};
        if (hi - lo <= EPS) return {a.p + a.d * ((lo + hi) / 2)};
        return {a.p + a.d * lo, a.p + a.d * hi};
    };

    deque<Halfplane> q;
    for (const Halfplane& cur : h) {
        while (q.size() > 1 && cur.outside(intersection(q[q.size() - 2], q.back()))) {
            q.pop_back();
        }
        while (q.size() > 1 && cur.outside(intersection(q[0], q[1]))) {
            q.pop_front();
        }
        if (!q.empty() && fabsl(vectp(q.back().d, cur.d)) <= EPS) {
            if (scalp(q.back().d, cur.d) < 0) {
                if (fabsl(vectp(q.back().d, cur.p - q.back().p)) <= EPS) {
                    return on_line(q.back());
                }
                return {}; // With the bounding box, a feasible strip cannot occur here.
            }
            if (cur.outside(q.back().p)) {
                q.pop_back(); // Keep the more restrictive parallel half-plane.
            } else {
                continue;
            }
        }
        q.push_back(cur);
    }
    while (q.size() > 2 && q.front().outside(intersection(q[q.size() - 2], q.back()))) {
        q.pop_back();
    }
    while (q.size() > 2 && q.back().outside(intersection(q[0], q[1]))) {
        q.pop_front();
    }
    if (q.size() < 3) return {};
    if (fabsl(vectp(q.back().d, q.front().d)) <= EPS) {
        if (scalp(q.back().d, q.front().d) < 0 &&
            fabsl(vectp(q.back().d, q.front().p - q.back().p)) <= EPS) {
            return on_line(q.back());
        }
        return {};
    }
    vector<Point> res;
    for (int i = 0; i < q.size(); ++i) {
        Point p = intersection(q[i], q[(i + 1) % q.size()]);
        if (res.empty() || (p - res.back()).abs() > EPS) res.push_back(p);
    }
    if (res.size() > 1 && (res.front() - res.back()).abs() <= EPS) res.pop_back();
    // A collapsed deque may describe a point excluded by discarded sides.
    // Check the point / both segment endpoints against all original constraints.
    if (res.size() <= 2) {
        for (const Point& p : res) {
            for (const Halfplane& a : h) {
                if (a.outside(p)) return {};
            }
        }
    }
    return res;
}
