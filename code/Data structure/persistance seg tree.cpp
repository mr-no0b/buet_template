class SegTlazy {
public:
    struct node {
        int cursum, premn, premx;
        node *left, *right;
        node(int _cur = 0, int mn = 1e9, int mx = -1e9)
            : cursum(_cur), premn(mn), premx(mx), left(nullptr), right(nullptr) {}
    };
    vector<int> a;
    vector<node*> roots; // persistent versions
    int n;
    SegTlazy() {
        n = 0;
        a.clear();
        roots.clear();
    }
    void init(int _n) {
        n = _n;
        a.assign(n + 1, 1); // 1-based intitially all 1
        roots.clear();
    }
    void merge(node* nd, node* l, node* r) {
        nd->cursum = l->cursum + r->cursum;
        nd->premn = min(l->premn, l->cursum + r->premn);
        nd->premx = max(l->premx, l->cursum + r->premx);
    }
    node* build(int b, int e) {
        if (b == e) {
            return new node(a[b], a[b], a[b]);
        }
        int mid = (b + e) >> 1;
        node* cur = new node();
        cur->left = build(b, mid);
        cur->right = build(mid + 1, e);
        merge(cur, cur->left, cur->right);
        return cur;
    }
    node* update(node* prev, int b, int e, int pos, int val) {
        if (b == e) {
            return new node(val, val, val);
        }
        int mid = (b + e) >> 1;
        node* cur = new node();
        if (pos <= mid) {
            cur->left = update(prev->left, b, mid, pos, val);
            cur->right = prev->right;
        } else {
            cur->left = prev->left;
            cur->right = update(prev->right, mid + 1, e, pos, val);
        }
        merge(cur, cur->left, cur->right);
        return cur;
    }
    node query(node* nd, int b, int e, int i, int j) {
        if (!nd || j < b || e < i) {
            return node(0, 1e9, -1e9);
        }
        if (i <= b && e <= j) {
            return *nd;
        }
        int mid = (b + e) >> 1;
        node l = query(nd->left, b, mid, i, j);
        node r = query(nd->right, mid + 1, e, i, j);
        node res;
        merge(&res, &l, &r);
        return res;
    }
};
