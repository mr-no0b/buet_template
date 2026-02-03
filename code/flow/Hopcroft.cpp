struct HopcroftKarp {
    vector<vector<int>> g;
    vector<int> pairU, pairV, dis;
    int n, m;

    HopcroftKarp(int n, int m) : n(n), m(m) {
        g.resize(n + 1);
        dis.resize(n + 1);
        pairU.assign(n + 1, -1);
        pairV.assign(m + 1, -1);
    }

    void add_edge(int u, int v) {
        g[u].push_back(v); 
    }

    bool bfs() {
        queue<int> Q;
        for(int u = 1; u <= n; u++) {
            if(pairU[u] == -1) {
                dis[u] = 0;
                Q.push(u);
            }
            else {
                dis[u] = -1;
            }
        }
        bool flag = false;
        while(not Q.empty()) {
            int u = Q.front(); Q.pop();
            for(auto v : g[u]) {
                int u2 = pairV[v];
                if(u2 == -1) {
                    flag = true;
                }
                else if(dis[u2] == -1) {
                    dis[u2] = dis[u] + 1;
                    Q.push(u2);
                }
            }
        }
        return flag;
    }

    bool dfs(int u) {
        for(auto v : g[u]) {
            int u2 = pairV[v];
            if(u2 == -1 || (dis[u2] == dis[u] + 1 && dfs(u2))) {
                pairU[u] = v;
                pairV[v] = u;
                return true;
            }
        }
        dis[u] = -1;
        return false;
    }

    int run() {
        int match = 0;
        while(bfs()) {
            for(int u = 1; u <= n; u++) {
                if(pairU[u] == -1 && dfs(u)) {
                    match++;
                }
            }
        }
        return match;
    }
};
