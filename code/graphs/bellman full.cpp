// if reach can possible from 1 to n 
vector<array<int, 3>> edges;
vector<vector<int>> g(n + 1);
edges.push_back({u, v, -w});
g[v].push_back(u);
vector<ll> dis(n + 1, inf); 
dis[1] = 0;
// n - 1 relaxation
for(int c = 1; c < n; c++) {
    bool relax = false;
    for(auto [u, v, w] : edges) {
        if(dis[u] != inf && dis[u] + w < dis[v]) {
            dis[v] = dis[u] + w;
            relax = true;
        }
    }
    if(!relax) false;
}
vector<bool> bad(n);
for(auto [u, v, w] : edges) {
    if(dis[u] != inf && dis[u] + w < dis[v]) {
        bad[v] = 1;
    }
}       
bool flag = 1;
queue<int> Q;
Q.push(n);
vector<bool> vis(n + 1);
while(not Q.empty()) {
    int u = Q.front(); Q.pop();
    if(bad[u]) {
        flag = 0;
        break;
    }
    if(vis[u]) continue;
    vis[u] = 1;
    for(auto v : g[u]) {
        Q.push(v);
    }
}
// negatative cycle print
edges.push_back({u, v, w});
vector<ll> dis(n + 1, inf);
vector<int> par(n + 1, -1);
int node = -1;
for(int i = 1; i <= n; i++) {
    if(dis[i] != inf) continue;
    dis[i] = 0;
    for(int c = 1; c <= n; c++) {
        bool relax = 1;
        for(auto [u, v, w] : edges) {
            if(dis[u] != inf && dis[u] + w < dis[v]) {
                dis[v] = dis[u] + w;
                par[v] = u;
                if(c == n) node = v;
                relax = 0;
            }
        }
        if(relax) break;
    }
    if(node != -1) break;
}
if(node == -1) {
    cout << "NO\n";
} 
else {
    // Get a node in the cycle by walking back n times
    for(int i = 0; i < n; i++) node = par[node];

    vector<int> path;
    int cur = node;
    do {
        path.push_back(cur);
        cur = par[cur];
    } while(cur != node);
    path.push_back(node);
    reverse(path.begin(), path.end());

    cout << "YES\n";
    for(int x : path) cout << x << " ";
    cout << "\n";
}
