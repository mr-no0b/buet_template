int ans = 0;
map<int, bool> has;
for(int i = 1; i <= n; i++) {
    int cur = 0, next = i, cnt = 0;
    while(next <= n) {
        cur |= tree.query(1, 1, n, i, next).val;
        if(!has.count(cur)) cnt++;
        has[cur] = 1;
        int mn = n + 2;
        for(int j = 0; j < 30; j++) {
            if(!(cur & (1 << j))) {
                if(bitv[j].size() > 0) {
                    auto it = lower_bound(bitv[j].begin(), bitv[j].end(), i);
                    if(it != bitv[j].end()) {
                        mn = min(mn, *it);
                    }
                }
            }
        }
        next = mn;
    }
}