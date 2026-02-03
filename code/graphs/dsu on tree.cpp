struct DSUTree {
    int n; // Number of nodes in the tree

    // Graph structure and metadata
    vector<vector<int>> adj;           // Original tree (adjacency list)
    vector<int> color, sz, res;        // color[i]: color of node i, sz[i]: subtree size, res[i]: result for node i
    vector<map<int, int>*> cnt;        // cnt[i]: pointer to map storing color frequencies in subtree of node i

    // Constructor: initialize vectors and process input + solve
    DSUTree(int n) : n(n) {
        adj.resize(n + 1);
        color.resize(n + 1);
        sz.resize(n + 1);
        res.resize(n + 1);
        cnt.resize(n + 1, nullptr);
        take_input(); // Read input
        build();      // Run DSU on tree
    }

    // Read color input and edges
    void take_input() {
        for (int i = 1; i <= n; i++) {
            cin >> color[i];
        }
        for (int i = 1; i < n; i++) {
            int u, v;
            cin >> u >> v;
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }

    // Step 1: Compute sizes of all subtrees
    void compute_subtree_sizes(int u, int p) {
        sz[u] = 1;
        for (int v : adj[u]) {
            if (v != p) {
                compute_subtree_sizes(v, u);
                sz[u] += sz[v];
            }
        }
    }

    // Step 2: Perform DSU on Tree to count distinct colors
    void dfs(int u, int p) {
        int big = -1, max_size = -1;

        // Find the heavy child (largest subtree)
        for (int v : adj[u]) {
            if (v != p && sz[v] > max_size) {
                max_size = sz[v];
                big = v;
            }
        }

        // Process all light children first (their data will be deleted)
        for (int v : adj[u]) {
            if (v != p && v != big) dfs(v, u);
        }

        // Process heavy child and reuse its color map
        if (big != -1) {
            dfs(big, u);
            cnt[u] = cnt[big]; // Inherit heavy child's map
        } else {
            cnt[u] = new map<int, int>(); // Create new map for leaf
        }

        // Add current node’s color
        (*cnt[u])[color[u]]++;

        // Merge all light child maps into current node's map
        for (int v : adj[u]) {
            if (v == p || v == big) continue;
            for (auto &[col, freq] : *cnt[v]) {
                (*cnt[u])[col] += freq;
            }
        }

        // Store the number of distinct colors in the subtree of u
        res[u] = cnt[u]->size();
    }

    // Entry point: preprocess and call DFS
    void build() {
        compute_subtree_sizes(1, 0); // Root the tree at node 1
        dfs(1, 0);                   // Start DFS from root
    }

    // Output the result
    void print() {
        for (int i = 1; i <= n; i++) {
            cout << res[i] << " "; // Output result for node i
        }
        cout << '\n';
    }
};
