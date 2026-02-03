// Finds Maximum Matching in a General Graph using Edmonds' Blossom Algorithm
// Returns: vector<int> mate where mate[i] = j means node i is matched with node j
// Complexity: O(N * M), where N = number of nodes, M = number of edges
vector<int> Blossom(vector<vector<int>>& graph) {
  int n = graph.size();           // Number of nodes
  int timer = -1;                 // Used for tracking during LCA search

  // mate[i] = j means i is matched with j (or -1 if unmatched)
  vector<int> mate(n, -1);
  vector<int> label(n);          // Labels: -1 = unused, 0 = even level, 1 = odd level
  vector<int> parent(n);         // BFS tree parent
  vector<int> orig(n);           // Original base of blossom for each node
  vector<int> aux(n, -1);        // Aux array used in LCA finding
  vector<int> q;                 // BFS queue

  // Finds the Lowest Common Ancestor (LCA) of x and y in the alternating tree
  auto lca = [&](int x, int y) {
    timer++;  // Increase visit timestamp
    for (; ; swap(x, y)) {
      if (x == -1) continue;
      if (aux[x] == timer) return x;  // Found LCA
      aux[x] = timer;
      x = (mate[x] == -1 ? -1 : orig[parent[mate[x]]]);  // Move up the alternating tree
    }
  };

  // Contracts a blossom (odd-length cycle) found during BFS
  auto blossom = [&](int v, int w, int a) {
    // v and w are two endpoints; a is the LCA
    while (orig[v] != a) {
      parent[v] = w;
      w = mate[v];
      if (label[w] == 1) {
        label[w] = 0;
        q.push_back(w);  // Re-add to BFS queue
      }
      orig[v] = orig[w] = a;  // Update base of blossom
      v = parent[w];
    }
  };

  // Augments along the alternating path ending at v to increase the matching
  auto augment = [&](int v) {
    while (v != -1) {
      int pv = parent[v], nv = mate[pv];
      mate[v] = pv;      // Match v with its parent
      mate[pv] = v;
      v = nv;            // Continue on the unmatched end
    }
  };

  // Breadth-First Search to find an augmenting path starting from root
  auto bfs = [&](int root) {
    fill(label.begin(), label.end(), -1);    // Reset labels
    iota(orig.begin(), orig.end(), 0);       // Each node is its own blossom initially
    q.clear();                               // Reset BFS queue

    label[root] = 0;                          // Start root at even level
    q.push_back(root);

    for (int i = 0; i < (int)q.size(); ++i) {
      int v = q[i];
      for (auto x : graph[v]) {
        if (label[x] == -1) {
          // If x is unvisited
          label[x] = 1;       // Mark as odd level
          parent[x] = v;
          if (mate[x] == -1)  // Found an augmenting path
            return augment(x), 1;
          label[mate[x]] = 0;  // Next level (even)
          q.push_back(mate[x]);
        }
        else if (label[x] == 0 && orig[v] != orig[x]) {
          // Found a blossom (x and v have the same label and different bases)
          int a = lca(orig[v], orig[x]);
          blossom(x, v, a);
          blossom(v, x, a);
        }
      }
    }
    return 0;  // No augmenting path found
  };

  // Try to find augmenting paths starting from unmatched nodes
  for (int i = 0; i < n; i++) {
    if (mate[i] == -1)
