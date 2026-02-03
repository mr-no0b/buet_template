void solve() {
    int n;
    cin >> n;
    vector<vector<int>> g(n + 1);
    for(int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    vector<int> sz(n + 1);
    vector<ll> dp(n + 1);

    function<void(int, int)> dfs1 = [&](int u, int p) -> void {
        sz[u] = 1;
        for(auto v : g[u]) {
            if(v != p) {
                dfs1(v, u);
                sz[u] += sz[v];
                dp[1] += sz[v]; // root is 1
            }
        }
    };
    dfs1(1, 0);

    function<void(int, int)> dfs2 = [&](int u, int p) -> void {
        for(auto v : g[u]) if(v != p) {
            dp[v] = dp[u] - sz[v] + (n - sz[v]);
            dfs2(v, u);
        }
    };
    dfs2(1, 0);

    for(int i = 1; i <= n; i++) {
        cout << dp[i] << " \n"[i == n];
    }

}
