// find Longest zig zag path usnig BIT and DP 
// red means peak ^ 
// white means vally v 
// \/\/\/\/

struct BIT{                      // stores (length , last-pos)
    using Node = pair<int,int>;
    int n; vector<Node> t;        // (0,-1) = “nothing”
    BIT() {}
    BIT(int _n) : n(_n), t(n + 2, {0,-1}) {}
    Node query(int i) {           // prefix-max
        Node res{0,-1};
        for (; i >= 1; i -= i & -i)
            if (t[i].first > res.first) res = t[i];
        return res;
    }
    void upd(int i, Node v) {
        for (; i <= n; i += i & -i)
            if (v.first > t[i].first) t[i] = v;
    }
}; 

void solve() {
    int n;
    cin >> n;
    vector<int> v(n);
    for(auto &x : v) cin >> x;

    vector<int> tmp = v;
    sort(tmp.begin(), tmp.end());
    tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
    int m = tmp.size();
    vector<int> id(n), rid(n);
    for(int i = 0; i < n; i++) {
        id[i] = lower_bound(tmp.begin(), tmp.end(), v[i]) - tmp.begin() + 1;
        rid[i] = m - id[i] + 1;
    }

    BIT red(n), white(n);

    vector<vector<int>> dp(2, vector<int>(n + 1, 0)); // 0 -> red 1 -> white
    vector<vector<int>> pre(2, vector<int>(n + 1, -1)); // 0 -> red 1 -> white

    for(int i = 0; i < n; i++) { 
        // current is red
        auto [lenw, posw] = white.query(id[i] - 1);
        dp[0][i] = lenw + 1;
        pre[0][i] = posw;
        red.upd(rid[i], {dp[0][i], i});
        // current is white
        auto [lenr, posr] = red.query(rid[i] - 1);
        dp[1][i] = lenr + 1;
        pre[1][i] = posr;
        white.upd(id[i], {dp[1][i], i});
    }

    int mx = 0, pos = 0, f = 0;
    for(int i = 0; i < n; i++) {
        if(mx < dp[0][i]) {
            mx = dp[0][i];
            pos = i;
            f = 0;
        }
        if(mx < dp[1][i]) {
            mx = dp[1][i];
            pos = i;
            f = 1;
        }
    }

    vector<int> ans;
    while(pos >= 0) {
        ans.push_back(v[pos]);
        pos = pre[f][pos];
        f ^= 1;
    }
    reverse(ans.begin(), ans.end());
    cout << ans.size() << "\n";
    for(auto x : ans) {
        cout << x << " ";
    }
    cout << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int tc = 1;
    //cin >> tc;
    while (tc--) solve();
    return 0;
}
