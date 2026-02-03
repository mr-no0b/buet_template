//Multiply returns ans[k]=∑ ai*bj % MOD [such that i+j==k]
const double PI = acos(-1);
struct cd {
    double x, y;
    cd(double x = 0, double y = 0): x(x), y(y) {}
    cd operator+(cd o) const { return cd(x + o.x, y + o.y); }
    cd operator-(cd o) const { return cd(x - o.x, y - o.y); }
    cd operator*(cd o) const { return cd(x*o.x - y*o.y, x*o.y + y*o.x); }
    cd operator*(double d) const { return cd(x * d, y * d); }
    cd conj() const { return cd(x, -y); }
};
void fft(vector<cd> &a, bool invert) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        while (j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        if (i < j)
            swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1); 
        cd wlen(cos(ang), sin(ang)); 
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; ++j) {
                cd u = a[i + j];
                cd v = a[i + j + len/2] * w;
                a[i + j] = u + v;
                a[i + j + len/2] = u - v;
                w = w * wlen;
            }
        }
    }
    if (invert) {
        for (cd &x : a)
            x = x * (1.0 / n);
    }
}
vector<long long> multiply(const vector<int> &a, const vector<int> &b) {
    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    int n = 1;
    while (n < a.size() + b.size())
        n <<= 1;
    fa.resize(n);
    fb.resize(n);
    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < n; ++i)
        fa[i] = fa[i] * fb[i];
    fft(fa, true);
    vector<long long> result(n);
    for (int i = 0; i < n; ++i)
        result[i] = round(fa[i].x);
    return result;
}
