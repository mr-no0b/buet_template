#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct LazySegTree {
    vector<ll> tree, lazyAdd, assignVal;
    vector<char> assignFlag; // 0 = no assign pending, 1 = assign pending
    ll n;

    LazySegTree(ll arr[], ll sz) {
        n = sz;
        tree.assign(4*n, 0);
        lazyAdd.assign(4*n, 0);
        assignVal.assign(4*n, 0);
        assignFlag.assign(4*n, 0);
        build(arr, 0, 0, n-1);
    }

    void build(ll arr[], ll ind, ll st, ll ed) {
        if (st == ed) { tree[ind] = arr[st]; return; }
        ll mid = (st + ed) / 2;
        build(arr, 2*ind+1, st, mid);
        build(arr, 2*ind+2, mid+1, ed);
        tree[ind] = tree[2*ind+1] + tree[2*ind+2];
    }

    // Push pending operations at node ind to children and apply to node's tree
    void push(ll ind, ll st, ll ed) {
        if (st > ed) return;
        // First handle assignment (it overrides additions)
        if (assignFlag[ind]) {
            tree[ind] = assignVal[ind] * (ed - st + 1);
            if (st != ed) {
                ll L = 2*ind+1, R = 2*ind+2;
                // set children to this assignment, clear their adds
                assignFlag[L] = assignFlag[R] = 1;
                assignVal[L] = assignVal[R] = assignVal[ind];
                lazyAdd[L] = lazyAdd[R] = 0;
            }
            assignFlag[ind] = 0; // consumed at this node (tree already updated)
            assignVal[ind] = 0;
        }
        // Then handle addition
        if (lazyAdd[ind] != 0) {
            tree[ind] += lazyAdd[ind] * (ed - st + 1);
            if (st != ed) {
                ll L = 2*ind+1, R = 2*ind+2;
                // if child has assign pending, adding modifies assignVal
                if (assignFlag[L]) assignVal[L] += lazyAdd[ind];
                else lazyAdd[L] += lazyAdd[ind];
                if (assignFlag[R]) assignVal[R] += lazyAdd[ind];
                else lazyAdd[R] += lazyAdd[ind];
            }
            lazyAdd[ind] = 0;
        }
    }

    // Range add: add val to every element in [l,r]
    void updateAdd(ll val, ll ind, ll l, ll r, ll st, ll ed) {
        push(ind, st, ed);
        if (r < st || ed < l) return;
        if (l <= st && ed <= r) {
            lazyAdd[ind] += val;
            push(ind, st, ed);
            return;
        }
        ll mid = (st + ed) / 2;
        updateAdd(val, 2*ind+1, l, r, st, mid);
        updateAdd(val, 2*ind+2, l, r, mid+1, ed);
        tree[ind] = tree[2*ind+1] + tree[2*ind+2];
    }

    // Range assign: set every element in [l,r] to val
    void updateAssign(ll val, ll ind, ll l, ll r, ll st, ll ed) {
        push(ind, st, ed);
        if (r < st || ed < l) return;
        if (l <= st && ed <= r) {
            // mark assignment on this node and apply immediately
            assignFlag[ind] = 1;
            assignVal[ind] = val;
            lazyAdd[ind] = 0; // assignment overrides previous adds
            push(ind, st, ed);
            return;
        }
        ll mid = (st + ed) / 2;
        updateAssign(val, 2*ind+1, l, r, st, mid);
        updateAssign(val, 2*ind+2, l, r, mid+1, ed);
        tree[ind] = tree[2*ind+1] + tree[2*ind+2];
    }

    // Range sum query [l,r]
    ll sum(ll ind, ll l, ll r, ll st, ll ed) {
        push(ind, st, ed);
        if (r < st || ed < l) return 0;
        if (l <= st && ed <= r) return tree[ind];
        ll mid = (st + ed) / 2;
        return sum(2*ind+1, l, r, st, mid) + sum(2*ind+2, l, r, mid+1, ed);
    }

    // convenient wrappers:
    void rangeAdd(ll l, ll r, ll val) { updateAdd(val, 0, l, r, 0, n-1); }
    void rangeAssign(ll l, ll r, ll val) { updateAssign(val, 0, l, r, 0, n-1); }
    ll rangeSum(ll l, ll r) { return sum(0, l, r, 0, n-1); }
};

// Example usage
int main() {
    ll a[] = {1,2,3,4,5};
    ll sz = 5;
    LazySegTree seg(a, sz);

    // add 10 to range [1,3]
    seg.rangeAdd(1,3,10); // array -> {1,12,13,14,5}

    // assign range [2,4] to 7
    seg.rangeAssign(2,4,7); // array -> {1,12,7,7,7}

    // add 5 to [0,2]
    seg.rangeAdd(0,2,5); // array -> {6,17,12,7,7}

    cout << seg.rangeSum(0,4) << "\n"; // prints 6+17+12+7+7 = 49
    cout << seg.rangeSum(1,2) << "\n"; // prints 17+12 = 29
    return 0;
}
