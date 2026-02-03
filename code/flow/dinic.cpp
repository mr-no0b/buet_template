// Dinic's Algorithm for Maximum Flow
// Complexity: O(V^2 * E) in general graphs
// Usage:
//   1. Initialize with Dinic dinic(num_nodes);
//   2. Add edges with addEdge(u, v, capacity);
//   3. Compute maximum flow with maxFlow(source, sink);

#define eb emplace_back

struct Dinic {
  // Structure to represent an edge in the flow graph
  struct Edge {
    int u, v;        // from node u to node v
    ll cap, flow = 0; // capacity and current flow

    Edge() {}
    Edge(int u, int v, ll cap) : u(u), v(v), cap(cap) {}
  };

  int N;  // Number of nodes in the graph
  vector<Edge> edge;              // List of all edges
  vector<vector<int>> adj;       // Adjacency list of edge indices
  vector<int> d, pt;             // Distance (level) and pointer arrays

  // Constructor: Initializes flow network for N nodes
  Dinic(int N) : N(N), edge(0), adj(N), d(N), pt(N) {}

  // Adds a directed edge from u to v with given capacity
  // Adds reverse edge with 0 capacity for residual graph
  void addEdge(int u, int v, ll cap) {
    if (u == v) return;  // No self-loops
    edge.eb(u, v, cap);
    adj[u].eb(edge.size() - 1);
    edge.eb(v, u, 0);    // Reverse edge
    adj[v].eb(edge.size() - 1);
  }

  // Constructs level graph using BFS from source s
  // Returns true if sink t is reachable from s
  bool bfs(int s, int t) {
    queue<int> q({s});
    fill(d.begin(), d.end(), N + 1);  // Initialize all distances to "infinity"
    d[s] = 0;

    while (!q.empty()) {
      int u = q.front(); q.pop();
      if (u == t) break;  // Early stop if sink is reached
      for (int k : adj[u]) {
        Edge& e = edge[k];
        if (e.flow < e.cap && d[e.v] > d[u] + 1) {
          d[e.v] = d[u] + 1;
          q.emplace(e.v);
        }
      }
    }
    return d[t] != N + 1;  // If sink is reachable
  }

  // DFS to find augmenting path from u to T with available flow
  // Returns amount of flow pushed through the path
  ll dfs(int u, int T, ll flow = -1) {
    if (u == T || flow == 0) return flow;
    for (int& i = pt[u]; i < adj[u].size(); i++) {
      Edge& e = edge[adj[u][i]];
      Edge& oe = edge[adj[u][i] ^ 1];  // Reverse edge
      if (d[e.v] == d[u] + 1) {        // Valid level edge
        ll amt = e.cap - e.flow;       // Residual capacity
        if (flow != -1 && amt > flow) amt = flow; // Limit to available flow
        if (ll pushed = dfs(e.v, T, amt)) {
          e.flow += pushed;
          oe.flow -= pushed;          // Adjust reverse flow
          return pushed;
        }
      }
    }
    return 0;  // No augmenting path found
  }

  // Computes the maximum flow from source s to sink t
  // Returns total flow value
  ll maxFlow(int s, int t) {
    ll total = 0;
    while (bfs(s, t)) {              // While there exists an augmenting path
      fill(pt.begin(), pt.end(), 0); // Reset current edge pointers
      while (ll flow = dfs(s, t)) {  // Push as much flow as possible
        total += flow;
      }
    }
    return total;
  }
};
