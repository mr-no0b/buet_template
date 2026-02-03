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
    
    vector<int> down(n + 1);
    vector<int> best1(n + 1, -1), best2(n + 1, -1);
    function<int(int, int)> dfs1 = [&](int u, int p) -> int {
        for(auto v : g[u]) if(v != p) {
            int cand = 1 + dfs1(v, u);                          
            if (cand > best1[u]) { best2[u] = best1[u]; best1[u] = cand; }
            else if (cand > best2[u]) best2[u] = cand;
            down[u] = max(down[u], cand); 
        }
        return down[u];
    };
    dfs1(1, 0);

    vector<int> up(n + 1);
    function<void(int, int)> dfs2 = [&](int u, int p) -> void {
        for(auto v : g[u]) if(v != p) {
            int val = (down[v] + 1 == best1[u]) ? best2[u] : best1[u];
            up[v] = 1 + max(up[u], val);
            dfs2(v, u);
        } 
        
    };
    dfs2(1, 0);

    for(int i = 1; i <= n; i++) {
        cout << max(up[i], down[i]) << " \n"[i == n];
    }
}
