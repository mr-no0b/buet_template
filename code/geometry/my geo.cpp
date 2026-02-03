struct pt {
    double x, y;
    pt() {}
    pt(double _x, double _y) {
        x = _x;
        y = _y;
    }
    pt operator + (const pt &o) const { return pt(x + o.x, y + o.y); }
    pt operator - (const pt &o) const { return pt(x - o.x, y - o.y); }
    ld operator | (const pt &o) const { return x * o.x + y * o.y; }
    ld operator * (const pt &o) const { return x * o.y - y * o.x; } 
    ld dis() { return hypotl(x, y); }
    bool operator < (const pt &o) const {
        return (x < o.x || (x == o.x && y < o.y));
    }
    bool operator == (const pt &o) const {
        return (x == o.x && y == o.y);
    }
};
ld pt_to_seg_dis(pt p, pt a, pt b) {
    if(((p - a) | (b - a)) < 0) {
        return (p - a).dis();
    }
    else {
        if(((p - b) | (a - b)) < 0) {
            return (p - b).dis();
        }
        else {
            return fabs(((p - b) * (b - a)) / (b - a).dis());
        }
    }
}
ld dis_bet_seg(pt a, pt b, pt c, pt d) {
    ld dot1 = ((b - a) * (d - a)) * ((b - a) * (c - a));
    ld dot2 = ((d - c) * (a - c)) * ((d - c) * (b - c));
    if(dot1 <= 0 && dot2 <= 0) {
        cout << 0 << "\n";
        return;
    }
    ld ans = min(pt_to_seg_dis(c, a, b), pt_to_seg_dis(d, a, b));
    ans = min(ans, pt_to_seg_dis(a, c, d));
    ans = min(ans, pt_to_seg_dis(b, c, d));
    return ans;
}
int orientation(pt a, pt b, pt c) {
    ld val = (b - a) * (c - a); 
    if (val > 0) return 1;  
    if (val < 0) return -1;
    return 0;
}
vector<pt> convex_hull(vector<pt> v) {
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    int n = v.size();
    if (n <= 2) return v;
    vector<pt> lower, upper;
    for (int i = 0; i < n; i++) {
        int j = lower.size();
        while (j >= 2 && orientation(lower[j-2], lower.back(), v[i]) != 1) {
            lower.pop_back();
            j--;
        }
        lower.push_back(v[i]);
    }
    for (int i = n-1; i >= 0; i--) {
        int j = upper.size();
        while (j >= 2 && orientation(upper[j-2], upper.back(), v[i]) != 1) {
            upper.pop_back();
            j--;
        }
        upper.push_back(v[i]);
    }
    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}
