const int N = 1505;
bitset<N> bit[N];        // input vectors (1-indexed)
int basis[N];            // basis[i] = vector responsible for bit i
vector<int> g[N];        // for reconstruction: which vectors XORed to form current
int pivot[N];            // pivot[j] = leading bit index of row j

void build_linear_basis(int n, int maxBit) {
    fill(pivot, pivot + n + 1, -1);
    fill(basis, basis + maxBit + 1, -1);

    for(int i = maxBit; i >= 0; i--) {
        int pivotRow = -1;
        for(int j = 1; j <= n; j++) {
            if(pivot[j] == -1) continue;
            if(pivotRow != -1 && bit[j][i]) {
                bit[j] ^= bit[pivotRow];  // eliminate bit i
                g[j].push_back(pivotRow); // record operation
            }
            else if(bit[j][i]) {
                pivotRow = j;             // new pivot
                pivot[j] = i;
                basis[i] = j;
            }
        }
    }
}

// Example: maximum XOR of any subset
int maxXOR(int maxBit) {
    bitset<N> res;
    for(int i = maxBit; i >= 0; i--) {
        if(basis[i] != -1 && !res[i]) res ^= bit[basis[i]];
    }
    return (int)(res.to_ullong()); // works if N <= 64, else handle manually
}
