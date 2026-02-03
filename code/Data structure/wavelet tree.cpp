// Wavelet Tree structure used for answering range queries efficiently
struct wavelet_tree {
    int lo, hi;            // range of values represented at this node
    wavelet_tree *l, *r;   // left and right children
    int *b;
    ll *c;            // b: bitmap of counts going left, c: prefix sums
    int bsz, csz;          // sizes of b and c arrays

    // Constructor: initializes members
    wavelet_tree() : lo(1), hi(0), l(nullptr), r(nullptr),
                     b(nullptr), bsz(0), c(nullptr), csz(0) {}  

    // Initializes wavelet tree on range [from, to) with values in [x, y]
    void init(int *from, int *to, int x, int y) {
        lo = x, hi = y;
        if(from >= to) return;

        int mid = (lo + hi) >> 1;
        auto f = [mid](int x) {
            return x <= mid;
        };

        int n = to - from;
        b = (int*)malloc((n + 2) * sizeof(int));
        c = (ll*)malloc((n + 2) * sizeof(ll));
        bsz = csz = 0;
        b[bsz++] = 0;
        c[csz++] = 0;

        for(auto it = from; it != to; it++) {
            b[bsz] = b[bsz - 1] + f(*it);     // Count of elements ≤ mid
            c[csz] = c[csz - 1] + (*it);      // Prefix sum of elements
            bsz++;
            csz++;
        }

        if(hi == lo) return;

        auto pivot = stable_partition(from, to, f);  // Separate elements
        l = new wavelet_tree(); l->init(from, pivot, lo, mid);       // Left subtree
        r = new wavelet_tree(); r->init(pivot, to, mid + 1, hi);     // Right subtree
    }

    // Returns count of numbers ≤ k in range [l, r]
    int LTE(int l, int r, int k) {
        if(l > r || k < lo) return 0;
        if(hi <= k) return r - l + 1;

        int lb = b[l - 1], rb = b[r];
        return this->l->LTE(lb + 1, rb, k) + this->r->LTE(l - lb, r - rb, k);
    }

    // Returns sum of elements ≤ k in range [l, r]
    ll sum(int l, int r, int k) {
        if(l > r || k < lo) return 0;
        if(hi <= k) return c[r] - c[l - 1];

        int lb = b[l - 1], rb = b[r];
        return this->l->sum(lb + 1, rb, k) + this->r->sum(l - lb, r - rb, k);
    }
    //kth smallest element in [l, r]
    //for array [1,2,1,3,5] 2nd smallest is 1 and 3rd smallest is 2
    int kth(int l, int r, int k) {
        if(l > r) return 0;
        if(lo == hi) return lo;
        int inLeft = b[r] - b[l - 1], lb = b[l - 1], rb = b[r];
        if(k <= inLeft) return this->l->kth(lb + 1, rb, k);
        return this->r->kth(l - lb, r - rb, k - inLeft);
    }
    // Destructor to free memory
    ~wavelet_tree() {
        delete l;
        delete r;
        if(b) free(b);
        if(c) free(c);
    }
};
