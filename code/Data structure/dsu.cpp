/*
sometimes i need to modify the find() function
first of all add all expreinece as team[u]
then when join() make self[v] = team[v] - team[u]
so for v the ans still team[v] = self[v] + team[u]
and when we find some other parents 
then the added points add to all players of a team 
so we do self[v] += self[p] so here self[p] 
*/
struct DSU {
    vector<ll> par, rank, sz;
    int n;
    DSU(int n) : n(n), par(n + 1), rank(n + 1, 0), sz(n + 1, 1) {
        iota(par.begin(), par.end(), 0);
    }
    int find(int v) {
        return (par[v] == v ? v : (par[v] = find(par[v])));
    }
    bool same(int u, int v) {
        return find(u) == find(v);
    }
    int get_size(int v) {
        return sz[find(v)];
    }
    int count() {
        return n; 
    }
    void join(int u, int v) {
        u = find(u);
        v = find(v);
        n--;
        if (rank[v] > rank[u]) swap(u, v);
        par[v] = u;
        sz[u] += sz[v];
        if (rank[u] == rank[v]) rank[u]++;
        // u is the parent;
    }
};
struct DSU {
    vector<ll> par, rank, sz, sum, elemwhichnode;
    int n, nextId;
    DSU(int n, int m)
        : par(n + m + 1), rank(n + m + 1, 0), sz(n + m + 1, 1),
          sum(n + m + 1), elemwhichnode(n + 1), n(n), nextId(n) {
        iota(par.begin(), par.end(), 0);
        for (int i = 1; i <= n; ++i) {
            elemwhichnode[i] = i;        
            sum[i] = i;             
        }
    }
    void join(int u, int v) {
        u = find(u);
        v = find(v);
        if(u == v) return;
        n--;
        if (rank[v] > rank[u]) swap(u, v);
        par[v] = u;
        sz[u] += sz[v];
        sum[u] += sum[v];
        if (rank[u] == rank[v]) rank[u]++;
        // u is the parent;
    }

    void erase(int v) {
        int nv = elemwhichnode[v];
        if (nv == 0) return;            // already deleted
        int pv = find(nv);
        sz [pv] -= 1;
        sum[pv] -= v;
        elemwhichnode[v] = 0;               // mark deleted
    }
    /* move p -> set(q) */
    void move(int u, int v) {
        if(elemwhichnode[u] == 0) return;
        
        int pu = find(elemwhichnode[u]);
        int pv = find(elemwhichnode[v]);
        if (pu == pv) return;  

        erase(u);

        nextId++;
        par [nextId] = nextId;
        rank[nextId] = 0;
        sz  [nextId] = 1;
        sum [nextId] = u;

        elemwhichnode[u] = nextId;

        join(nextId, pv);
    }

    pair<int, ll> query(int p) {
        int r = find(elemwhichnode[p]);
        return { (int)sz[r], sum[r] };
    }
};
