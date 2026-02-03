vector<ll> segmented_sieve(ll l, ll r) {
    vector<ll> segpr;
    vector<bool> pr(r - l + 5, 1);
    if (l == 1) {
        pr[0] = false;
    }
    for (ll i = 0; svp[i] * svp[i] <= r; i++) {
        ll cur = svp[i];
        ll base = cur * cur;
        if (base < l) {
            base = ((l + cur - 1) / cur) * cur;
        }
        for (ll j = base; j <= r; j += cur) {
            pr[j - l] = false;
        }
    }
    for (ll i = 0; i <= r - l; i++) {
        if (pr[i]) {
            segpr.push_back(l + i);
        }
    }
    return segpr;
}
