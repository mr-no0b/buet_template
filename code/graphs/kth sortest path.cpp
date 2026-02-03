Time Complexity = O(m * K * log(n * K))

int m, n, deg[MM], source, sink, K;
int val[MM][12]; // val[u][k] = k-th shortest path distance to node u (0-based k)

// Edge representation: destination node and weight
struct edge {
    int v, w;
} adj[MM][500]; // Adjacency list as a 2D array: adj[u][i] = i-th neighbor of u

// Info stored in priority queue for Dijkstra
struct info {
    int v, w, k; // v = current node, w = total weight, k = current path rank
    bool operator<(const info &b) const {
        return w > b.w; // Min-heap by weight
    }
};

priority_queue<info, vector<info>> Q;

void kthBestShortestPath() {
    // Step 1: Initialize all distances to INF
    for (int i = 0; i < n; i++)
        for (int j = 0; j < K; j++)
            val[i][j] = inf;

    // Step 2: Start from source
    Q.push({source, 0, 0});
    val[source][0] = 0;

    // Step 3: Modified Dijkstra
    while (!Q.empty()) {
        info u = Q.top(); Q.pop();

        // For each neighbor of u
        for (int i = 0; i < deg[u.v]; i++) {
            int to = adj[u.v][i].v;
            int cost = adj[u.v][i].w + u.w;

            // Try to insert this new cost into val[to][0...K-1]
            for (int k = 0; k < K; k++) {
                if (cost < val[to][k]) {
                    swap(cost, val[to][k]); // Insert and shift worse value forward
                    Q.push({to, val[to][k], k});
                }
            }
        }
    }
}
