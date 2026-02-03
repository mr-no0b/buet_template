deque<int> dq;
for(int i = 1; i <= n; i++) {
    while(!dq.empty() && v[dq.back()] < v[i]) {
        dq.pop_back();
    }
    dq.push_back(i);
    if(dq.front() <= i - k) {
        dq.pop_front();
    }
    if(i >= k) {
        cout << v[dq.front()] << " ";
    }
}