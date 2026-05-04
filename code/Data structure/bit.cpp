struct FenwickTree {
    int n;
    int LOGN;
    vector<ll> bit;
    // all is 1 based
    FenwickTree(int _n) : n(_n), bit(_n + 1, 0) {
        LOGN = 31 - __builtin_clz(n);
    }
    // make A[i] += v
    // for making A[i] = v, use +(v - A[i])
    void update(int id, ll v) {
        for (; id <= n; id += id & -id) {
            bit[id] += v;
        }
    }
    // Return sum of elements from 1 to idx
    ll query(int id) {
        ll sum = 0;
        for (; id > 0; id -= id & -id) {
            sum += bit[id];
        }
        return sum;
    }
    // Return sum of elements from l to r
    ll query(int l, int r) {
        return query(r) - query(l - 1);
    }
    // This is equivalent to calculating lower_bound on prefix sums array
    // Finds the smallest index `idx` such that query(idx) >= v
    int bit_search(ll val) {
        ll sum = 0;
        int pos = 0;
        for (int i = LOGN; i >= 0; i--) {
            if (pos + (1 << i) <= n && sum + bit[pos + (1 << i)] < val) {
                sum += bit[pos + (1 << i)];
                pos += (1 << i);
            }
        }
        return pos + 1; // +1 because 'pos' will have position of largest value less than 'v'
    }
};