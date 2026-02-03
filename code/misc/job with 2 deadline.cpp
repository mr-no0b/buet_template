/*
given n workers for a company
for each worker given 3 parameter 
k, l, r
we need to assign each worker for a day i for all n days 
and no two worker can be assigned on same date 
if workder i is assigned on jth day and j <= k then the profit 'l' otherwise 'r' 
assign such that maximize profit
*/

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
const ld inf = 1e100; 
const ld eps = 1e-18;

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> front(n + 1), back(n + 1);
    ll tot = 0;
    for(int i = 1; i <= n; i++) {
        int k, l, r;
        cin >> k >> l >> r;
        tot += min(l, r);
        if(l > r) {
            front[k].push_back(l - r);
        }
        else if(l < r && (n - k) > 0) {
            back[n - k].push_back(r - l);
        }
    }
    auto add = [&](vector<vector<int>> &vec) {
        priority_queue<int> pq;
        for(int i = 1; i <= n; i++) {
            for(auto g : vec[i]) {
                pq.push(-g);
                if(pq.size() > i) pq.pop();
            }
        }
        ll res = 0;
        while(not pq.empty()) {
            res += -pq.top(); pq.pop();
        }
        return res;
    };

    cout << tot + add(front) + add(back) << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int tc = 1, cs = 1;
    cin >> tc;
    while (tc--) {
        //cout << "Case " << cs++ << ": ";
        solve();
    }
    return 0;
}
