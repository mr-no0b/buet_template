ll NoOfDigitInNFactInBaseB(ll N, ll B) {
    ll i;
    double ans = 0;

    // Sum of logarithms: log(N!) = log(1) + log(2) + ... + log(N)
    for (i = 1; i <= N; i++) ans += log(i);

    // Convert log from base e to base B, then add 1 to get digit count
    ans = ans / log(B);
    ans = ans + 1;

    // Return the result as an integer (floor value)
    return (ll)ans;
}
