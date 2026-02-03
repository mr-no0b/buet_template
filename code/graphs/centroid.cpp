#include <bits/stdc++.h>
using namespace std;

/* ----------  Centroid-decomposition with fast depth counting  ---------- */
struct CentroidTree {
    int n;
    vector<vector<int>> adj, ctree;
    vector<int>        subsize, cpar;
    vector<char>       blocked;          // faster than vector<bool>

    /* reusable frequency table */
    vector<int> cnt;                     // size = K+1, filled on demand
    vector<int> touched;                 // indices of cnt we modified

    CentroidTree(int nodes) : n(nodes) {
        adj.resize(n + 1);
        ctree.resize(n + 1);
        subsize.resize(n + 1);
        cpar.assign(n + 1, -1);
        blocked.assign(n + 1, 0);
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    /* -------------  centroid decomposition boiler-plate  ------------- */
    void calculate_size(int u, int p) {
        subsize[u] = 1;
        for (int v : adj[u])
            if (v != p && !blocked[v]) {
                calculate_size(v, u);
                subsize[u] += subsize[v];
            }
    }
    int get_centroid(int u, int p, int sz) {
        for (int v : adj[u])
            if (v != p && !blocked[v] && subsize[v] > sz / 2)
                return get_centroid(v, u, sz);
        return u;
    }

    /* ----------------  fast depth-multiset helpers  ---------------- */
    inline void add_depth(int d) {
        if (cnt[d] == 0) touched.push_back(d);
        ++cnt[d];
    }
    inline void clear_touched() {
        for (int d : touched) cnt[d] = 0;
        touched.clear();
    }

    void collect_depths(int u, int p, int d, vector<int>& depths, int K) {
        if (d > K) return;
        depths.push_back(d);
        for (int v : adj[u])
            if (v != p && !blocked[v])
                collect_depths(v, u, d + 1, depths, K);
    }

    /* count all paths of exact length K that go through centroid c */
    void process_centroid(int c, int K, long long& ans) {
        add_depth(0);                                  // centroid itself
        for (int v : adj[c]) if (!blocked[v]) {
            vector<int> depths;
            collect_depths(v, c, 1, depths, K);

            for (int d : depths)          // pairs: one in this subtree,
                if (d <= K)               // other in previous ones / root
                    ans += cnt[K - d];

            for (int d : depths)          // merge current subtree depths
                if (d <= K) add_depth(d);
        }
        clear_touched();                                  // reset counter
    }

    void decompose(int u, int p, int K, long long& ans) {
        calculate_size(u, -1);
        int c = get_centroid(u, -1, subsize[u]);
        blocked[c] = 1;
        cpar[c] = p;

        process_centroid(c, K, ans);

        if (p != -1) {
            ctree[p].push_back(c);
            ctree[c].push_back(p);
        }
        for (int v : adj[c])
            if (!blocked[v])
                decompose(v, c, K, ans);
    }

    long long count_paths_of_length_k(int K) {
        cnt.assign(K + 1, 0);             // initialise once
        touched.clear();
        long long ans = 0;
        decompose(1, -1, K, ans);
        return ans;
    }

    /* optional helpers kept intact */
    int get_centroid_parent(int u) { return cpar[u]; }
    vector<int>& get_centroid_children(int u) { return ctree[u]; }
};

/* -------------------------------  main  ------------------------------- */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    CentroidTree ct(n);
    for (int i = 0; i < n - 1; ++i) {
        int u, v; cin >> u >> v;
        ct.add_edge(u, v);
    }
    cout << ct.count_paths_of_length_k(k) << '\n';
    return 0;
}
