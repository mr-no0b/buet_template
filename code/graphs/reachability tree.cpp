/* 
- Find the minimal/maximal weight of the edges 
when traversing from vertex u to vertex v
- merge the nodes with new nodes using DSU
- max/min weight will be the root of the final tree
*/
struct Reachability_tree {
    vector<vector<int>> g, par;
    vector<int> dep, dsupar, cost, subtree;
    vector<ll> tot;
    int id, n, m, lg, timer;
    Reachability_tree(int n, int m) : id(n), n(n), m(m), g(n + m + 1), 
            dsupar(n + m + 1), dep(n + m + 1), cost(n + m + 1),
            tot(n + m + 1), subtree(n + m + 1) {
        iota(dsupar.begin(), dsupar.end(), 0);
        lg = log2(n + m + 1) + 1;
        par.assign(n + m + 1, vector<int>(lg + 1));
        timer = 0;
        build();
    }
    int find(int v) {
        if (dsupar[v] == v) return v;
        return dsupar[v] = find(dsupar[v]);
    }
    void join(int u, int v, int w) {
        u = find(u);
        v = find(v);
        if(u == v) return;
        int new_node = ++id;
        g[new_node].push_back(u); 
        g[u].push_back(new_node); 
        g[new_node].push_back(v); 
        g[v].push_back(new_node); 
        cost[new_node] = max(cost[u], cost[v]);
        dsupar[u] = new_node;
        dsupar[v] = new_node;
    } 
    void dfs(int u, int p) {
        par[u][0] = p;
        dep[u] = dep[p] + 1;
        subtree[u] = (u <= n);
        for(int i = 1; i <= lg; i++) {
            par[u][i] = par[par[u][i - 1]][i - 1];
        }
        for(auto v : g[u]) if(v != p) {
            dfs(v, u);
            subtree[u] += subtree[v];
        }
    }
    int lca(int u, int v) {
        if(dep[u] < dep[v]) swap(u, v); // make sure that dep[u] > dep[v]
        for(int k = lg; k >= 0; k--) {
            if(dep[par[u][k]] >= dep[v]) u = par[u][k];
        }
        if(u == v) return v;
        for(int k = lg; k >= 0; k--) {
            if(par[u][k] != par[v][k]) {
                u = par[u][k];
                v = par[v][k];
            }
        } 
        return par[u][0];
    }
    void f(int u, int p) {
        for(auto v : g[u]) if(v != p) {
            tot[v] = tot[u] + 1LL * (subtree[u] - subtree[v]) * cost[u];
            f(v, u);
        }
    }
    void build() {
        for(int i = 1; i <= n; i++) {
            cin >> cost[i];
        }
        vector<array<int, 3>> edges; 
        for(int i = 1; i <= m; i++) {
            int u, v;
            cin >> u >> v;
            edges.push_back({max(cost[u], cost[v]), u, v});
        }
        sort(edges.begin(), edges.end());
        for(auto [w, u, v] : edges) {
            if(find(u) != find(v)) {
                join(u, v, w);
            }
        }
        dfs(id, 0);
        f(id, 0);
        for (int i = 1; i <= n; i++) {
            tot[i] += cost[i];
        }
    }
};