#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
const int N = 1e7 + 9, mod = 1e9 + 7; 
const ld inf = 1e100;
const ld eps = 1e-9;
const ld PI = acos((ld)-1.0);
int sign(ld x) { return (x > eps) - (x < -eps); }
struct PT {
    ll x, y;
    PT() { x = 0, y = 0; }
    PT(ll x, ll y) : x(x), y(y) {}
    PT operator - (const PT &a) const { return PT(x - a.x, y - a.y); }
    PT operator * (const ll a) const { return PT(x * a, y * a); }
    friend PT operator * (const ll &a, const PT &b) { return PT(a * b.x, a * b.y); }
    PT operator / (const ll a) const { return PT(x / a, y / a); }
    bool operator == (PT a) const { return sign(a.x - x) == 0 && sign(a.y - y) == 0; }
    bool operator != (PT a) const { return !(*this == a); }
    bool operator < (PT a) const { return sign(a.x - x) == 0 ? y < a.y : x < a.x; }
    bool operator > (PT a) const { return sign(a.x - x) == 0 ? y > a.y : x > a.x; }
};
istream &operator >> (istream &in, PT &p) { return in >> p.x >> p.y; }
ostream &operator << (ostream &out, PT &p) { return out << "(" << p.x << "," << p.y << ")"; }
inline ll dot(PT a, PT b) { return a.x * b.x + a.y * b.y; }
inline ll dist2(PT a, PT b) { return dot(a - b, a - b); }
inline double dist(PT a, PT b) { return sqrt(dot(a - b, a - b)); }
inline double cross(PT a, PT b) { return a.x * b.y - a.y * b.x; }
inline double cross2(PT a, PT b, PT c) { return cross(b - a, c - a); }
inline int orientation(PT a, PT b, PT c) { return sign(cross(b - a, c - a)); }

// intersection point between segment ab and segment cd assuming unique intersection exists
bool seg_seg_intersection(PT a, PT b, PT c, PT d, PT &ans) {
    ld oa = cross2(c, d, a), ob = cross2(c, d, b);
    ld oc = cross2(a, b, c), od = cross2(a, b, d);
    if (oa * ob < 0 && oc * od < 0){
        ans = (a * ob - b * oa) / (ob - oa);
        return 1;
    }
    else return 0;
}
// returns true if  point p is on line segment ab
bool is_point_on_seg(PT a, PT b, PT p) {
    if (fabs(cross(p - b, a - b)) < eps) {
        if (p.x < min(a.x, b.x) - eps || p.x > max(a.x, b.x) + eps) return false;
        if (p.y < min(a.y, b.y) - eps || p.y > max(a.y, b.y) + eps) return false;
        return true;
    }
    return false;
}
// intersection point between segment ab and segment cd assuming unique intersection exists
set<PT> seg_seg_intersection_inside(PT a,  PT b,  PT c,  PT d) {
    PT ans;
    if (seg_seg_intersection(a, b, c, d, ans)) return {ans};
    set<PT> se;
    if (is_point_on_seg(c, d, a)) se.insert(a);
    if (is_point_on_seg(c, d, b)) se.insert(b);
    if (is_point_on_seg(a, b, c)) se.insert(c);
    if (is_point_on_seg(a, b, d)) se.insert(d);
    return se;
}

ld area(vector<PT> &p) {
    ld ans = 0; int n = p.size();
    for (int i = 0; i < n; i++) ans += cross(p[i], p[(i + 1) % n]);
    return fabs(ans) * 0.5;
}

ll area2(const vector<PT>& p) {
    ll ans = 0;
    int n = p.size();
    for (int i = 0; i < n; i++) {
        ans += cross(p[i], p[(i + 1) % n]);
    }
    return abs(ans); // not devided by 2 for integers
}

bool is_point_on_polygon(vector<PT> &p, const PT& z) {
    int n = p.size();
    for (int i = 0; i < n; i++) {
        if (is_point_on_seg(p[i], p[(i + 1) % n], z)) return 1;
    }
    return 0;
}

// returns 1e9 if the point is on the polygon 
int winding_number(vector<PT> &p, const PT& z) { // O(n)
    if (is_point_on_polygon(p, z)) return 1e9;
    int n = p.size(), ans = 0;
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        bool below = p[i].y < z.y;
        if (below != (p[j].y < z.y)) {
            auto orient = orientation(z, p[j], p[i]);
            if (orient == 0) return 0;
            if (below == (orient > 0)) ans += below ? 1 : -1;
        }
    }
    return ans;
}

// -1 if strictly inside, 0 if on the polygon, 1 if strictly outside
int is_point_in_polygon(vector<PT> &p, const PT& z) { // O(n)
    int k = winding_number(p, z);
    return k == 1e9 ? 0 : k == 0 ? 1 : -1;
}

// -1 if strictly inside, 0 if on the polygon, 1 if strictly outside
// it must be strictly convex, otherwise make it strictly convex first
int is_point_in_convex(vector<PT> &p, const PT& x) { // O(log n)
    int n = p.size(); assert(n >= 3);
    int a = orientation(p[0], p[1], x), b = orientation(p[0], p[n - 1], x);
    if (a < 0 || b > 0) return 1;
    int l = 1, r = n - 1;
    while (l + 1 < r) {
        int mid = l + r >> 1;
        if (orientation(p[0], p[mid], x) >= 0) l = mid;
        else r = mid;
    }
    int k = orientation(p[l], p[r], x);
    if (k <= 0) return -k;
    if (l == 1 && a == 0) return 0;
    if (r == n - 1 && b == 0) return 0;
    return -1;
}

// Count boundary lattice points using GCD on each edge
ll boundary_lattice_points(const vector<PT>& p) {
    ll b = 0;
    int n = p.size();
    for (int i = 0; i < n; i++) {
        PT a = p[i], c = p[(i + 1) % n];
        b += __gcd(abs(a.x - c.x), abs(a.y - c.y));
    }
    return b;
}

// Computes (interior, boundary) lattice points using Pick's Theorem
pair<ll, ll> polygon_lattice_points(const vector<PT>& p) {
    // Pick's Theorem:
    // Area = I + B/2 - 1 
    // I = the number of interior lattice points
    // B = the number of boundary lattice points
    ll A2 = area2(p);               // Twice the area
    ll B = boundary_lattice_points(p);
    ll I = (A2 - B + 2) / 2;
    return {I, B};
}

// distance, p1, p2
pair<ll, pair<PT, PT>> closest_pair_recursive(vector<PT>& pts, vector<PT>& tmp, int l, int r) {
    if (r - l <= 3) {
        pair<ll, pair<PT, PT>> res = {LLONG_MAX, {PT(), PT()}};
        for (int i = l; i < r; ++i) {
            for (int j = i + 1; j < r; ++j) {
                ll d = dist2(pts[i], pts[j]);
                if (d < res.first) {
                    res = {d, {pts[i], pts[j]}};
                }
            }
        }
        sort(pts.begin() + l, pts.begin() + r, [](const PT& a, const PT& b) {
            return a.y < b.y;
        });
        return res;
    }

    int m = (l + r) / 2;
    ll midx = pts[m].x;

    auto left = closest_pair_recursive(pts, tmp, l, m);
    auto right = closest_pair_recursive(pts, tmp, m, r);
    auto res = min(left, right);

    merge(pts.begin() + l, pts.begin() + m,
          pts.begin() + m, pts.begin() + r,
          tmp.begin() + l, [](const PT& a, const PT& b) {
              return a.y < b.y;
          });

    copy(tmp.begin() + l, tmp.begin() + r, pts.begin() + l);

    vector<PT> strip;
    for (int i = l; i < r; ++i) {
        if ((pts[i].x - midx) * (pts[i].x - midx) >= res.first) continue;
        for (int j = strip.size() - 1; j >= 0; --j) {
            ll dy = pts[i].y - strip[j].y;
            if (dy * dy >= res.first) break;
            ll d = dist2(pts[i], strip[j]);
            if (d < res.first) {
                res = {d, {pts[i], strip[j]}};
            }
        }
        strip.push_back(pts[i]);
    }

    return res;
}

// distance, point1, point2
pair<ll, pair<PT, PT>> minimum_euclidean_distance(vector<PT>& pts) {
    int n = pts.size();
    sort(pts.begin(), pts.end(), [](const PT& a, const PT& b) {
        return a.x < b.x;
    });
    vector<PT> tmp(n);
    return closest_pair_recursive(pts, tmp, 0, n);
}

// returns the boundary points of the convex hull.
vector<PT> ConvexHull(vector<PT>&p, int n) {
    int sz = 0;
    vector<PT> hull(n + n);
    sort(p.begin(), p.end());
  for(int i = 0; i < n; ++i) {
        while (sz > 1 and cross2(hull[sz - 2], hull[sz - 1], p[i]) < 0) --sz;
        hull[sz++] = p[i];
    }
    for(int i = n - 2, j = sz + 1; i >= 0; --i) {
        while (sz >= j and cross2(hull[sz - 2], hull[sz - 1], p[i]) < 0) --sz;
        hull[sz++] = p[i];
    }
    hull.resize(sz - 1);
    return hull;
}   


// keep only the corners that form a strictly-convex polygon
vector<PT> make_strict_convex(const vector<PT>& P) {
    int n = (int)P.size();
    vector<PT> R;
    for (int i = 0; i < n; ++i) {
        PT prv = P[(i + n - 1) % n], cur = P[i], nxt = P[(i + 1) % n];
        if (orientation(prv, cur, nxt) != 0) R.push_back(cur);
    }
    return R;
}

// return true if a polygon strictly insider other
bool polygon_polygon_intersect(vector<PT> v1, vector<PT> v2) {
    auto H1 = ConvexHull(v1, v1.size());
    auto H2 = ConvexHull(v2, v2.size());
    vector<PT> all = v1;
    for(auto it : v2) {
        all.push_back(it);
    }
    auto H = ConvexHull(all, all.size());
    bool ok = (H == H1 || H == H2);
    return ok;
    /*v1 = make_strict_convex(v1);
    reverse(v1.begin(), v1.end());
    v2 = make_strict_convex(v2);
    reverse(v2.begin(), v2.end());
    if (v1.size() < 3 || v2.size() < 3) {
        return false;
    }
    bool ok = true;
    for (const auto &pt : v2)
        if (is_point_in_convex(v1, pt) != -1) { ok = false; break; }

    if (!ok) {                             
        ok = true;
        for (const auto &pt : v1)
            if (is_point_in_convex(v2, pt) != -1) { ok = false; break; }
    }
    return ok;*/
}

// Projection of point c onto line ab
PT project_from_point_to_line(PT a, PT b, PT c) {
    return a + (b - a) * dot(c - a, b - a) / dot(b - a, b - a);
}

// Reflection of point c across line ab
PT reflection_from_point_to_line(PT a, PT b, PT c) {
    PT p = project_from_point_to_line(a, b, c);
    return p + (p - c);
}

// Minimum distance from point c to line ab
ld dist_from_point_to_line(PT a, PT b, PT c) {
    return fabs(cross(b - a, c - a)) / sqrtl(dist2(a, b));
}

// Minimum distance from point c to segment ab
ld dist_from_point_to_seg(PT a, PT b, PT c) {
    ll r = dist2(a, b);
    if (r == 0) return dist(c, a);
    ld t = max(0.0L, min(1.0L, (ld)dot(c - a, b - a) / r));
    PT proj = a + (b - a) * t;
    return dist(c, proj);
}

// Minimum distance between two segments ab and cd
ld dist_from_seg_to_seg(PT a, PT b, PT c, PT d) {
    PT dummy;
    if (seg_seg_intersection(a, b, c, d, dummy)) return 0;
    return min({
        dist_from_point_to_seg(a, b, c),
        dist_from_point_to_seg(a, b, d),
        dist_from_point_to_seg(c, d, a),
        dist_from_point_to_seg(c, d, b)
    });
}

// Checks if a polygon is strictly convex
bool is_convex(const vector<PT>& p) {
    int n = p.size();
    bool has_pos = false, has_neg = false;
    for (int i = 0; i < n; i++) {
        ll z = cross2(p[i], p[(i + 1) % n], p[(i + 2) % n]);
        has_pos |= (z > 0);
        has_neg |= (z < 0);
    }
    return !(has_pos && has_neg);
}

// Centroid (center of mass) of a polygon
PT centroid(const vector<PT>& p) {
    ld A = 0;
    PT c(0, 0);
    int n = p.size();
    for (int i = 0; i < n; i++) {
        ll cross_val = cross(p[i], p[(i + 1) % n]);
        A += cross_val;
        c.x += (p[i].x + p[(i + 1) % n].x) * cross_val;
        c.y += (p[i].y + p[(i + 1) % n].y) * cross_val;
    }
    A *= 0.5;
    c.x /= (6.0 * A);
    c.y /= (6.0 * A);
    return c;
}

// Angle between vectors (in radians)
ld get_angle(PT a, PT b) {
    ld costheta = (ld)dot(a, b) / sqrtl(dist2(a, {0, 0}) * dist2(b, {0, 0}));
    return acos(max((ld)-1.0, min((ld)1.0, costheta)));
}

// Intersection point of two infinite lines (ab and cd), returns false if parallel
bool line_line_intersection(PT a, PT b, PT c, PT d, PT &ans) {
    ld a1 = a.y - b.y, b1 = b.x - a.x, c1 = cross(a, b);
    ld a2 = c.y - d.y, b2 = d.x - c.x, c2 = cross(c, d);
    ld det = a1 * b2 - a2 * b1;
    if (fabs(det) < eps) return false;
    ans = PT((b1 * c2 - b2 * c1) / det, (c1 * a2 - a1 * c2) / det);
    return true;
}

// Check if lines ab and cd are parallel or collinear
// 0 = not parallel, 1 = parallel, 2 = collinear
int is_parallel(PT a, PT b, PT c, PT d) {
    ld cross1 = fabs(cross(b - a, d - c));
    if (cross1 < eps) {
        if (fabs(cross(a - b, a - c)) < eps && fabs(cross(c - d, c - a)) < eps) return 2;
        else return 1;
    }
    return 0;
}

// Angle bisector vector of angle <abc
PT angle_bisector(PT a, PT b, PT c) {
    PT p = a - b, q = c - b;
    return p + q * sqrtl((ld)dot(p, p) / dot(q, q));
}

// Point at distance d from a towards b (along line ab)
PT point_along_line(PT a, PT b, ld d) {
    assert(a != b);
    PT v = b - a;
    ld len = sqrtl(dot(v, v));
    return a + v * (d / len);
}

// Rotate a point counter-clockwise by angle t (radians) around origin
PT rotateccw(PT a, ld t) {
    return PT(a.x * cos(t) - a.y * sin(t), a.x * sin(t) + a.y * cos(t));
}

// Rotate a point clockwise by angle t (radians) around origin
PT rotatecw(PT a, ld t) {
    return PT(a.x * cos(t) + a.y * sin(t), -a.x * sin(t) + a.y * cos(t));
}

// 90-degree counter-clockwise rotation
PT rotateccw90(PT a) { return PT(-a.y, a.x); }

// 90-degree clockwise rotation
PT rotatecw90(PT a) { return PT(a.y, -a.x); }

// Check point in triangle (a, b, c)
// -1 = inside, 0 = on edge, 1 = outside
int is_point_in_triangle(PT a, PT b, PT c, PT p) {
    if (sign(cross(b - a, c - a)) < 0) swap(b, c);
    int c1 = sign(cross(b - a, p - a));
    int c2 = sign(cross(c - b, p - b));
    int c3 = sign(cross(a - c, p - c));
    if (c1 < 0 || c2 < 0 || c3 < 0) return 1;
    if (c1 + c2 + c3 != 3) return 0;
    return -1;
}


