struct Matrix {
    vector<vector<double>> a;
    int n, m;

    Matrix(int r = 0, int c = 0) : n(r), m(c), a(r, vector<double>(c)) {}

    static Matrix identity(int size) {
        Matrix I(size, size);
        for (int i = 0; i < size; ++i) I.a[i][i] = 1;
        return I;
    }

    Matrix operator*(const Matrix &B) const {
        assert(m == B.n);
        Matrix res(n, B.m);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < B.m; ++j)
                for (int k = 0; k < m; ++k)
                    res.a[i][j] += a[i][k] * B.a[k][j];
        return res;
    }

    Matrix pow(ll exp) const {
        assert(n == m);
        Matrix base = *this, res = identity(n);
        while (exp) {
            if (exp & 1) res = res * base;
            base = base * base;
            exp >>= 1;
        }
        return res;
    }

    Matrix transpose() const {
        Matrix res(m, n);
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                res.a[j][i] = a[i][j];
        return res;
    }

    double determinant() const {
        assert(n == m);
        Matrix tmp = *this;
        double det = 1;
        for (int i = 0; i < n; ++i) {
            int pivot = i;
            for (int j = i + 1; j < n; ++j)
                if (fabs(tmp.a[j][i]) > fabs(tmp.a[pivot][i]))
                    pivot = j;
            if (fabs(tmp.a[pivot][i]) < 1e-9) return 0;
            if (i != pivot) swap(tmp.a[i], tmp.a[pivot]), det *= -1;
            det *= tmp.a[i][i];
            for (int j = i + 1; j < n; ++j) {
                double f = tmp.a[j][i] / tmp.a[i][i];
                for (int k = i; k < n; ++k)
                    tmp.a[j][k] -= f * tmp.a[i][k];
            }
        }
        return det;
    }

    bool inverse(Matrix &inv) const {
        if (n != m) return false;
        inv = identity(n);
        Matrix tmp = *this;

        for (int i = 0; i < n; ++i) {
            int pivot = i;
            for (int j = i + 1; j < n; ++j)
                if (fabs(tmp.a[j][i]) > fabs(tmp.a[pivot][i]))
                    pivot = j;
            if (fabs(tmp.a[pivot][i]) < 1e-9) return false;
            swap(tmp.a[i], tmp.a[pivot]);
            swap(inv.a[i], inv.a[pivot]);

            double f = tmp.a[i][i];
            for (int j = 0; j < n; ++j) {
                tmp.a[i][j] /= f;
                inv.a[i][j] /= f;
            }

            for (int j = 0; j < n; ++j) {
                if (i == j) continue;
                double fac = tmp.a[j][i];
                for (int k = 0; k < n; ++k) {
                    tmp.a[j][k] -= fac * tmp.a[i][k];
                    inv.a[j][k] -= fac * inv.a[i][k];
                }
            }
        }
        return true;
    }
};
