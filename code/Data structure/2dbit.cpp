struct _2dbit {
    struct BITmx {
        vector<int> v;
        int n;
        BITmx(){}
        BITmx(int n) : n(n) {
            v.resize(n + 1);
        }
        void upd(int i, int val) {
            for(; i <= n; i += (i & -i)) {
                v[i] = max(v[i], val);
            }
        }
        int query(int i) {
            int ans = 0;
            for(; i > 0; i -= (i & -i)) {
                ans = max(ans, v[i]);
            }
            return ans;
        }
    };
    vector<vector<int>> cords;
    vector<BITmx> cnt;
    int n;
    _2dbit(int n) : n(n) {
        cnt.resize(n + 1);
        cords.resize(n + 1);
    }
    void add(int i, int val) {
        for(; i <= n; i += (i & -i)) {
            cords[i].push_back(val);
        }
    }
    void build() {
        for(int i = 1; i <= n; i++) {
            auto &v = cords[i];
            sort(v.begin(), v.end());
            v.erase(unique(v.begin(), v.end()), v.end());
            cnt[i] = BITmx(v.size());
        }
    }
    int getidx(int p, int val) {
        auto &v = cords[p];
        return (lower_bound(v.begin(), v.end(), val) - v.begin() + 1);
    }
    void upd(int x, int y, int val) {
        for(int i = x; i <= n; i += (i & -i)) {
            int id = getidx(i, y);
            cnt[i].upd(id, val);
        }
    }
    int query(int x, int y) {
        int ans = 0;
        for(int i = x; i > 0; i -= (i & -i)) {
            int id = getidx(i, y);
            ans = max(ans, cnt[i].query(id - 1));
        }
        return ans;
    }
};

void solve() {
    int n;
    cin >> n;
    vector<array<int, 2>> v(n + 1);
    vector<int> allx, ally;
    for(int i = 1; i <= n; i++) {
        cin >> v[i][0] >> v[i][1];
        allx.push_back(v[i][0]);
        ally.push_back(v[i][1]);
    }
    sort(allx.begin(), allx.end());
    allx.erase(unique(allx.begin(), allx.end()), allx.end());
    auto getidxX = [&](int x) {
        return (lower_bound(allx.begin(), allx.end(), x) - allx.begin() + 1);
    };
    sort(ally.begin(), ally.end());
    ally.erase(unique(ally.begin(), ally.end()), ally.end());
    auto getidxY = [&](int x) {
        return (lower_bound(ally.begin(), ally.end(), x) - ally.begin() + 1);
    };
    int m = allx.size();
    _2dbit bit(m);
    for(int i = 1; i <= n; i++){
        int x = getidxX(v[i][0]), y = getidxY(v[i][1]);
        bit.add(x, y);
    }
    bit.build();
    int ans = 0;
    for(int i = 1; i <= n; i++) {
        int x = getidxX(v[i][0]), y = getidxY(v[i][1]);
        int val = 1 + bit.query(x - 1, y);
        ans = max(ans, val);
        bit.upd(x, y, val);
    }
    cout << ans << "\n";
}
