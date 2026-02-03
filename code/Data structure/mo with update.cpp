const ll N = 1e6 + 9;
const ll B = 1000;

struct query {
    ll l, r, t, id;
    bool operator < (const query &x) const {
        if(l / B == x.l / B) {
            if(r / B == x.r / B) return t < x.t;
            return r / B < x.r / B;
        }
        return l / B < x.l / B;
    }
} Q[N];
struct upd {
    ll pos, old, cur;
} U[N];

ll a[N];
ll cnt[N], ans[N], l, r, t;
ll tot;
inline void add(int x) {
    ++cnt[x];
    if(cnt[x]==1)tot++;
}
inline void del(int x) {
    --cnt[x];
    if(cnt[x]==0)tot--;
}
inline void update(int pos, int x) {
    if (l <= pos && pos <= r) {
        add(x);
        del(a[pos]);
    }
    a[pos] = x;
}
map <ll, ll> mp;
ll nxt = 0;
ll get(ll x) {
    return mp.count(x) ? mp[x] : mp[x] = ++nxt;
}

void solve(){
    ll n, q;
    cin >> n >> q;
    for (ll i = 1; i <= n; i++) {
        cin >> a[i];
        a[i] = get(a[i]);
    }
    ll nq = 0, nu = 0;
    for (ll i = 1; i <= q; i++) {
        char ty;ll l, r;
        cin >> ty >> l >> r;
        if (ty == 'Q') {l++;++nq, Q[nq] = {l, r, nu, nq};}
        else{l++; ++nu, U[nu].pos = l, U[nu].old = a[l], a[l] = get(r), U[nu].cur = a[l];}
    }
    sort(Q + 1, Q + nq + 1);
    t = nu, l = 1, r = 0;
    for (ll i = 1; i <= nq; i++) {
        ll L = Q[i].l, R = Q[i].r, T = Q[i].t;
        while(t < T){ t++, update(U[t].pos, U[t].cur);}
        while(t > T) {update(U[t].pos, U[t].old), t--;}
        if(R < l) {
            while(l > L) add(a[--l]);
            while(l < L) del(a[l++]);
            while(r < R) add(a[++r]);
            while(r > R) del(a[r--]);
        } else {
            while(r < R) add(a[++r]);
            while(r > R) del(a[r--]);
            while(l > L) add(a[--l]);
            while(l < L) del(a[l++]);
        }
        ans[Q[i].id] = tot;
    }
    for (ll i = 1; i <= nq; i++) cout << ans[i] << '\n';
}
