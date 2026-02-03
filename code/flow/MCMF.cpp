/* Minimum Cost Maximum Flow (MCMF)
 * Works for both directed and undirected graphs, and supports negative edge costs.
 * Does NOT work with negative cost cycles.
 * For undirected edges, set `directed = false`.
 * Complexity: O(min(E^2 * V log V, E log V * flow))
 * Usage:
 *   MCMF mcmf(n);                     // Initialize with n nodes (0-indexed)
 *   mcmf.add_edge(u, v, cap, cost);  // Add edge from u to v with capacity and cost
 *   auto [maxFlow, minCost] = mcmf.solve(source, sink);
 */

using T = long long;
const T inf = 1LL << 61;

struct MCMF {
  struct edge {
    int u, v;
    T cap, cost;
    int id;
    edge(int _u, int _v, T _cap, T _cost, int _id)
      : u(_u), v(_v), cap(_cap), cost(_cost), id(_id) {}
  };

  int n, s, t, mxid;
  T flow, cost;
  bool neg;

  vector<vector<int>> g;    // Adjacency list: g[u] = indices of edges
  vector<edge> e;           // Edge list
  vector<T> d, potential;   // d = distances in Dijkstra; potential = reduced costs
  vector<T> flow_through;   // Stores flow pushed through original edges by ID
  vector<int> par;          // Parent edge index in path

  // Default constructor
  MCMF() {}

  // Initialize graph with n nodes (0-based indexing)
  MCMF(int _n) {
    n = _n + 10;
    g.assign(n, vector<int>());
    neg = false;
    mxid = 0;
  }

  // Adds an edge from u to v with given capacity and cost
  // If directed = false, adds edge in both directions
  void add_edge(int u, int v, T cap, T cost, int id = -1, bool directed = true) {
    if (cost < 0) neg = true;
    g[u].push_back(e.size());
    e.emplace_back(u, v, cap, cost, id);
    g[v].push_back(e.size());
    e.emplace_back(v, u, 0, -cost, -1);  // Reverse edge
    mxid = max(mxid, id);
    if (!directed) add_edge(v, u, cap, cost, -1, true); // Add reverse only once
  }

  // Dijkstra with potentials (Johnson's algorithm) to find shortest path
  bool dijkstra() {
    par.assign(n, -1);
    d.assign(n, inf);
    priority_queue<pair<T, T>, vector<pair<T, T>>, greater<>> q;

    d[s] = 0;
    q.emplace(0, s);

    while (!q.empty()) {
      int u = q.top().second;
      T nw = q.top().first;
      q.pop();
      if (nw != d[u]) continue;

      for (int i : g[u]) {
        int v = e[i].v;
        T cap = e[i].cap;
        T w = e[i].cost + potential[u] - potential[v]; // Reduced cost
        if (cap > 0 && d[u] + w < d[v]) {
          d[v] = d[u] + w;
          par[v] = i;
          q.emplace(d[v], v);
        }
      }
    }

    for (int i = 0; i < n; i++) {
      if (d[i] < inf) potential[i] += d[i]; // Update potentials
    }

    return d[t] != inf;  // True if sink is reachable
  }

  // DFS-style recursive flow sending function
  T send_flow(int v, T cur) {
    if (par[v] == -1) return cur;
    int id = par[v];
    int u = e[id].u;
    T w = e[id].cost;
    T f = send_flow(u, min(cur, e[id].cap));
    cost += f * w;
    e[id].cap -= f;
    e[id ^ 1].cap += f;  // Update reverse edge
    return f;
  }

  // Main function to compute minimum cost max flow from s to t
  // Optionally limit total flow with `goal`
  // Returns {maximum flow, minimum cost}
  pair<T, T> solve(int _s, int _t, T goal = inf) {
    s = _s; t = _t;
    flow = 0; cost = 0;
    potential.assign(n, 0);

    // If negative costs exist, use Bellman-Ford to initialize potentials
    if (neg) {
      d.assign(n, inf);
      d[s] = 0;
      for (int i = 0, relax = true; i < n && relax; ++i) {
        relax = false;
        for (int u = 0; u < n; ++u) {
          if (d[u] == inf) continue;
          for (int k : g[u]) {
            int v = e[k].v;
            T cap = e[k].cap, w = e[k].cost;
            if (cap > 0 && d[v] > d[u] + w) {
              d[v] = d[u] + w;
              relax = true;
            }
          }
        }
      }
      for (int i = 0; i < n; ++i) if (d[i] < inf) potential[i] = d[i];
    }

    // Repeatedly find augmenting paths using Dijkstra + potential
    while (flow < goal && dijkstra()) {
      flow += send_flow(t, goal - flow);
    }

    // Track flow through original edges (by ID)
    flow_through.assign(mxid + 10, 0);
    for (int u = 0; u < n; ++u) {
      for (int idx : g[u]) {
        if (e[idx].id >= 0) {
          flow_through[e[idx].id] = e[idx ^ 1].cap;
        }
      }
    }

    return {flow, cost};
  }
};
