class HLD : public SegTlazy {
public:
    vector<vector<pair<int, int>>> g;
    vector<vector<int>> par;
    vector<int> heavy, head, pos, depth, val;
    int timer, n, height;
 
    HLD(int n) : n(n), SegTlazy(n) {
        timer = 0;
        height = 31 - __builtin_clz(n);
        heavy.assign(n + 1, 0);
        head.assign(n + 1, 0);
        pos.assign(n + 1, 0);
        par.assign(n + 1, vector<int>(height + 1));
        depth.assign(n + 1, 0);
        val.assign(n + 1, 0);
        g.resize(n + 1);
    } 
 
    void add_edge(int u, int v, int w) {
        g[u].push_back({v, w});
        g[v].push_back({u, w});
    }
 
    int dfs(int u = 1, int p = 0) {
        int sub = 1, big = 0;
        par[u][0] = p;
        for(int j = 1; j <= height; j++) {
            par[u][j] = par[par[u][j - 1]][j - 1];
        }
        for (auto [v, w] : g[u]) if(v != p) {
            depth[v] = depth[u] + 1;
            val[v] = w;
            int subsize = dfs(v, u);
            if (subsize > big) big = subsize, heavy[u] = v;
            sub += subsize;
        }
        return sub;
    }
    void decompose(int u, int h) {
        head[u] = h, 
        pos[u] = ++timer;
        if (heavy[u]) decompose(heavy[u], h);
        for (auto [v, w] : g[u]) {
            if (v != par[u][0] && v != heavy[u]) decompose(v, v);
        }
    }
    void makeHLD(int root = 1) {
        dfs(root);
        decompose(root, root);
        for(int i = 1; i <= n; i++) {
            SegTlazy::a[pos[i]] = val[i];
        }
        SegTlazy::build(1, 1, n);
    }
    // if value on edge then call isEdge = 1
    ll Query(int u, int v, bool isEdge = 1) {
        node ret;
        for (; head[u] != head[v]; v = par[head[v]][0]) {
            if (depth[head[u]] > depth[head[v]]) swap(u, v);
            node tmp = SegTlazy::query(1, 1, n, pos[head[v]], pos[v]);
            SegTlazy::merge(ret, ret, tmp);
        }
        if (depth[u] > depth[v]) swap(u, v);
        node tmp = SegTlazy::query(1, 1, n, pos[u] + isEdge, pos[v]);
        SegTlazy::merge(ret, ret, tmp);
        return ret.val;
    }
    void Update(int u, int v, int val, bool isEdge = 0) {
        for (; head[u] != head[v]; v = par[head[v]][0]) {
            if (depth[head[u]] > depth[head[v]]) swap(u, v);
          //  cout<<"Updating:"<<v<<' '<<head[v]<<' '<<val<<endl;
            SegTlazy::upd(1, 1, n, pos[head[v]], pos[v], val);
        }
        if (depth[u] > depth[v]) swap(u, v);
        //cout<<"Updating:"<<v<<' '<<u<<' '<<val<<endl;
        SegTlazy::upd(1, 1, n, pos[u] + isEdge, pos[v], val);
    }
};
 
void solve() { 
    int n;
    cin>> n;
    HLD hld(n);
    // for vertex cin >> v[i] 
    vector<pair<int, int>> edge;
    for(int i=1;i<n;i++){
        int u, v, w;
        cin >> u >> v >> w;
        hld.add_edge(u, v, w);
        edge.push_back({u, v});
    }
    hld.makeHLD();
    int q; cin >> q;
    while(q--){
        int ty; cin >> ty;
        if(ty == 1) {
            int id, val;
            cin >> id >> val;
            auto [u, v] = edge[id - 1];
            hld.Update(u, v, val, 1);
        }
        else {
            int u, v;
            cin >> u >> v;
            cout << hld.Query(u, v, 1) << "\n";
        }
    }
}   