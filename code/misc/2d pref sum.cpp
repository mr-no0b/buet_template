struct PrefixSum2D {
    vector<vector<int>> prefix; 
    int n, m;                  
    PrefixSum2D(const vector<vector<int>>& grid) {
        n = grid.size();
        m = grid[0].size();
        prefix.assign(n, vector<int>(m, 0));

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                prefix[i][j] = grid[i][j];

                if (i > 0) prefix[i][j] += prefix[i - 1][j];      // Top
                if (j > 0) prefix[i][j] += prefix[i][j - 1];      // Left
                if (i > 0 && j > 0) prefix[i][j] -= prefix[i - 1][j - 1]; // Top-left overlap
            }
        }
    }
    int getSum(int x1, int y1, int x2, int y2) {
        int sum = prefix[x2][y2];
        if (x1 > 0) sum -= prefix[x1 - 1][y2];
        if (y1 > 0) sum -= prefix[x2][y1 - 1];
        if (x1 > 0 && y1 > 0) sum += prefix[x1 - 1][y1 - 1];
        return sum;
    }
};
