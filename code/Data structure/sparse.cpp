struct Spares_table { // now 1-based
    vector<int> a;
    vector<vector<int>> t;
    int n, q, lg;
    Spares_table() {}
    Spares_table(int n, int q) : n(n), q(q) {
        lg = __lg(max(1, n)) + 1;
        a.assign(n + 1, 0);
        t.assign(n + 2, vector<int>(lg + 1, 0));
        take_input();
    }
    void take_input() {
        for (int i = 1; i <= n; i++) cin >> a[i];
        build();
    }
    void build() {
        if (n <= 1) return;
        // Build on diffs d[i] = |a[i] - a[i-1]| for i = 2..n
        for (int i = 2; i <= n; i++) t[i][0] = abs(a[i] - a[i - 1]);
        for (int j = 1; (1 << j) <= n; j++) {
            for (int i = 2; i + (1 << j) - 1 <= n; i++) {
                t[i][j] = gcd(t[i][j - 1], t[i + (1 << (j - 1))][j - 1]);
            }
        }
    }
    // Query on the original array [l..r] (1-based).
    // Returns GCD of |a[i] - a[i-1]| for i in (l..r], and 0 if l == r.
    int query(int l, int r) {
        if (l >= r) return 0;
        int len = r - l;
        int k = __lg(len);
        return gcd(t[l + 1][k], t[r - (1 << k) + 1][k]);
    }
};
