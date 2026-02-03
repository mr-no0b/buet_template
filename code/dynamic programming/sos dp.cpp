// SUBSET SOS
// dp[mask] = sum of dp[submask] where submask ⊆ mask
void sos_subset(int M, vector<ll>& dp) {
    int N = 1 << M;
    for(int i = 0; i < M; i++)
        for(int mask = 0; mask < N; mask++)
            if(mask & (1 << i))
                dp[mask] += dp[mask ^ (1 << i)];
}

// SUPERSET SOS
// dp[mask] = sum of dp[supermask] where supermask ⊇ mask
void sos_superset(int M, vector<ll>& dp) {
    int N = 1 << M;
    for(int i = 0; i < M; i++)
        for(int mask = N-1; mask >= 0; mask--)
            if(mask & (1 << i))
                dp[mask ^ (1 << i)] += dp[mask];
}
