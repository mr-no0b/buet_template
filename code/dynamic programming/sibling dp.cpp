/*
  Problem:
  Divide a rooted tree into the minimum number of groups such that
  the total cost (edge weights) within each group does not exceed `k`.

  Definitions:
  - dp[u][remk]: Minimum number of groups required to cover subtree rooted at node `u`,
    where `remk` is the remaining cost capacity of the current group.
  - adj[par][idx]: The idx-th child of node `par`, along with the edge cost.
*/

ll n, k;
ll dp[mx][mx];
vector<pair<ll, ll>> adj[mx]; // adj[u] = {v, cost} edges in rooted tree

// Recursive function to compute dp[par][remk]
// par = current parent node
// idx = index of child being processed
// remk = remaining budget in the current group
ll sibling_dp(ll par, ll idx, ll remk) {
  if (remk < 0) return inf; // invalid case, over budget

  // No more children to process
  if (adj[par].size() <= idx) return 0;

  ll u = adj[par][idx].first; // current child node

  // Memoization check
  if (dp[u][remk] != -1)
    return dp[u][remk];

  ll ret = inf;
  ll under = 0, sibling = 0;

  // Option 1: Create a new group for this child (if not the root)
  if (par != 0) {
    under = 1 + dfs(u, 0, k);             // Entire subtree of `u` in a new group
    sibling = dfs(par, idx + 1, remk);    // Process next siblings with same group
    ret = min(ret, under + sibling);      // Total groups = groups under + groups from siblings
  }

  // Option 2: Try including this child in current group if cost allows
  ll temp = remk - adj[par][idx].second; // Remaining cost after adding this edge

  // Try all possible cost splits between `under` (child) and `sibling` (rest)
  for (ll chk = temp; chk >= 0; chk--) {
    ll siblingk = temp - chk;
    under = dfs(u, 0, chk);               // Child's subtree with budget `chk`
    sibling = dfs(par, idx + 1, siblingk); // Remaining siblings with remaining budget
    ret = min(ret, under + sibling);
  }

  return dp[u][remk] = ret; // Memoize and return result
}
