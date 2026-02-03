queue<int> Q;
for(int i = 1; i <= n; i++) {
    if(indeg[i] == 0) Q.push(i);
}
vector<int> topo;
while(!Q.empty()) {
    int u = Q.front(); Q.pop();
    topo.push_back(u);
    for(auto v : g[u]) {
        if(--indeg[v] == 0) {
            Q.push(v);
        }
    }
}
vector<int> dp(n + 1, -1e9);
vector<int> par(n + 1, 0);
dp[1] = 1;
for(auto u : topo) {
    if(dp[u] == -1e9) continue;
    for(auto v : g[u]) {
        if(dp[u] + 1 > dp[v]) {
            dp[v] = dp[u] + 1;
            par[v] = u;
        }
    }
}
 