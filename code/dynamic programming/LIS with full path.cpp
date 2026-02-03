// Function to compute and return LIS as a vector
vector<int> LIS(const vector<int>& num) {
    int n = num.size();

    vector<int> array(n + 1, INF);    // array[k] = min ending value of LIS of length k
    vector<int> mem(n + 1);           // mem[k] = index in `num` where LIS of length k ends
    vector<int> prev_idx(n, -1);      // prev_idx[i] = previous element's index in LIS
    vector<int> res;                  // result vector to store LIS

    array[0] = -INF;                  // Sentinel

    int maxlen = 1;

    for (int i = 0; i < n; i++) {
        // Binary search for the first position k where array[k] >= num[i]
        int k = lower_bound(array.begin(), array.begin() + maxlen + 1, num[i]) - array.begin();

        array[k] = num[i];     // Update LIS ending at length k
        mem[k] = i;            // Store current index

        if (k > 1)
            prev_idx[i] = mem[k - 1]; // Link to previous index in LIS

        if (k > maxlen)
            maxlen = k;        // Update LIS length
    }

    // Reconstruct LIS
    for (int i = mem[maxlen]; i != -1; i = prev_idx[i])
        res.push_back(num[i]);

    reverse(res.begin(), res.end()); // To get correct order

    return res;
}
