vector<vector<ll>> dis(n, vector<ll>(n, inf));
for (int i = 0; i < n; i++) dis[i][i] = 0;
for(int i = 1; i <= m; i++) {
    int u, v, w; cin >> u >> v >> w;
    dis[u][v] = min(dis[u][v], 1LL * w);
}
for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dis[i][k] < inf && dis[k][j] < inf) {
                dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
            }
        }
    }
}
/* if (d[i][k] + d[k][j] < d[i][j] - EPS)
      d[i][j] = d[i][k] + d[k][j]; */
for (int k = 0; k < n; k++) {
    if(dis[k][k] >= 0) continue;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dis[i][k] < inf && dis[k][j] < inf) {
                dis[i][j] = -inf; // negative cycle found
            }
        }
    }
}
// dis[u][v] == inf -> not path, -inf -> negative cycle

