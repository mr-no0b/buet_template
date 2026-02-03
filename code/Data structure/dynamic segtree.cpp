class SegTlazy {
public:
    struct node {
        ll mn, mncnt, lazy;
        bool haslazy;
        node *l, *r;      
        node() : mn(inf), mncnt(0), lazy(0), haslazy(0), l(nullptr), r(nullptr) {}
        node(ll mn, ll mncnt) : mn(mn), mncnt(mncnt), lazy(0), haslazy(0), l(nullptr), r(nullptr) {}
    };
 
    node *root; 
    ll n, m; // max and min range      
     // default 1 to max
    explicit SegTlazy(ll _n, ll _m = 1) : n(_n), m(_m) { root = new node(); }
    inline void merge(node *nd) {
        nd->mncnt = 0;
        if(nd->l && nd->r) {
            nd->mn = min(nd->l->mn, nd->r->mn);
            if(nd->mn == nd->l->mn) nd->mncnt += nd->l->mncnt;
            if(nd->mn == nd->r->mn) nd->mncnt += nd->r->mncnt;
        }
        else if(nd -> l) {
            nd->mn = nd->l->mn;
            if(nd->mn == nd->l->mn) nd->mncnt += nd->l->mncnt;
        }
        else if(nd -> r) {
            nd->mn = nd->r->mn;
            if(nd->mn == nd->r->mn) nd->mncnt += nd->r->mncnt;
        }
    }
    void apply(node *nd, ll b, ll e, ll lazy) {
        nd->mn += lazy;
    }
    inline void push(node *nd, ll b, ll e) {
        if (!nd->haslazy) return;
        apply(nd, b, e, nd->lazy);
        if (b != e) {    
            if (!nd->l) nd->l = new node();
            if (!nd->r) nd->r = new node();
            nd->l->lazy += nd->lazy;
            nd->r->lazy += nd->lazy;
            nd->l->haslazy = 1;
            nd->r->haslazy = 1;
        }
        nd->lazy = 0;
        nd->haslazy = 0;
    }
    void build(node *nd, ll b, ll e) {
        if(b == e) {
            nd->mn = 0;
            nd->mncnt = 1;
            return;
        }
        ll mid = (b + e) >> 1;
        if (!nd->l) nd->l = new node();
        if (!nd->r) nd->r = new node();
        build(nd->l, b, mid);
        build(nd->r, mid + 1, e);
        merge(nd);
    }
    void build() { build(root, m, n); }
    void upd(node *nd, ll b, ll e, ll i, ll j, ll v) {
        push(nd, b, e);
        if (j < b || e < i) return;       
        if (i <= b && e <= j) {          
            nd->lazy += v;
            nd->haslazy = 1;
            push(nd, b, e);
            return;
        }
        ll mid = (b + e) >> 1;
        if (!nd->l) nd->l = new node();
        if (!nd->r) nd->r = new node();
        upd(nd->l, b, mid, i, j, v);
        upd(nd->r, mid + 1, e, i, j, v);
        merge(nd);
    }
    void upd(ll l, ll r, ll v) { upd(root, m, n, l, r, v); }
    node query(node *nd, ll b, ll e, ll i, ll j) {
        if (j < b || e < i) return node();     
        if(!nd) return node();
        if (i <= b && e <= j) return *nd;
        push(nd, b, e);
        ll mid = (b + e) >> 1;
        node left  = query(nd->l, b, mid, i, j);
        node right = query(nd->r, mid + 1, e, i, j);
        node res;
        res.mn = min(left.mn, right.mn);
        if(res.mn == left.mn) res.mncnt += left.mncnt;
        if(res.mn == right.mn) res.mncnt += right.mncnt;
        return res;
    }
    node query(ll l, ll r) { return query(root, m, n, l, r); }
};
 
