struct SuffixArray {
    string s;
    int n;
    vector<int> sa;
    vector<int> lcp, lg;
    vector<vector<int>> st;

    // Constructor: builds SA, LCP, and Sparse Table
    SuffixArray(const string& str) : s(str), n(str.size()) {
        buildSA();
        buildLCP();
        buildSparse();
    }

    // Builds the suffix array using the doubling method in O(n log n)
    void buildSA() {
        sa.resize(n);
        vector<int> rank(n), tmp(n);
        iota(sa.begin(), sa.end(), 0);
        for (int i = 0; i < n; i++) rank[i] = s[i];

        for (int k = 1; k < n; k *= 2) {
            auto cmp = [&](int i, int j) {
                if (rank[i] != rank[j]) return rank[i] < rank[j];
                int ri = (i + k < n) ? rank[i + k] : -1;
                int rj = (j + k < n) ? rank[j + k] : -1;
                return ri < rj;
            };
            sort(sa.begin(), sa.end(), cmp);
            tmp[sa[0]] = 0;
            for (int i = 1; i < n; i++)
                tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
            rank = tmp;
        }
    }

    // Builds the LCP (Longest Common Prefix) array in O(n)
    void buildLCP() {
        lcp.resize(n);
        vector<int> inv(n);
        for (int i = 0; i < n; i++) inv[sa[i]] = i;

        int k = 0;
        lcp[n - 1] = 0;
        for (int i = 0; i < n; i++) {
            if (inv[i] == n - 1) {
                k = 0;
                continue;
            }
            int j = sa[inv[i] + 1];
            while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
            lcp[inv[i]] = k;
            if (k) k--;
        }
    }

    // Builds sparse table over sa for rangeMin queries
    void buildSparse() {
        lg.resize(n + 1); lg[0] = -1;
        for (int i = 1; i <= n; ++i) lg[i] = lg[i >> 1] + 1;

        int K = lg[n] + 1;
        st.assign(K, vector<int>(n));
        st[0] = sa;
        for (int k = 1; k < K; ++k)
            for (int i = 0; i + (1 << k) <= n; ++i)
                st[k][i] = min(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
    }

    // Range minimum over sa[l..r] using sparse table
    int rangeMin(int l, int r) {
        int k = lg[r - l + 1];
        return min(st[k][l], st[k][r - (1 << k) + 1]);
    }

    // Binary search for pattern range in suffix array
    pair<int, int> rangeSearch(const string& pat, int &L, int &R, int pos) {
        //cout << L << " " << R << "\n";
        int lo = L, hi = R, mid, ans = -1;
 
        while(lo <= hi) {
            mid = (lo + hi) >> 1;
            if(sa[mid] + pos >= n || s[sa[mid] + pos] < pat[pos]) {
                lo = mid + 1;
            }
            else {
                ans = mid;
                hi = mid - 1;
            }
        }
 
        if(ans == -1) return {-1, -1};
 
        int left = ans;
        lo = ans, hi = R, ans = -1;
        while(lo <= hi) {
            mid = (lo + hi) >> 1;
            if(sa[mid] + pos >= n || s[sa[mid] + pos] <= pat[pos]) {
                ans = mid;
                lo = mid + 1;
            }
            else {
                hi = mid - 1;
            }
        }
        if(ans == -1) return {-1, -1};
        int right = ans;
        return {left, right};
    }
 
    ll query(const string &p, int &L, int &R, int pos) {
        auto [l, r] = rangeSearch(p, L, R, pos);
        if(l == -1) return -1;
        L = l, R = r;
        //cout << p << " " << l << " " << r << "\n";
        return (r - l + 1);
    }
 
    ll occurence_as_substring(const string &t) {
        string p;
        int ans = 0;
        int l = 0, r = n - 1;
        for(int i = 0; i < t.size(); i++) {
            p.push_back(t[i]);
            ans = query(p, l, r, i);
            if(ans == -1) break;
        }
        if(ans == -1) ans = 0;
        return ans;
    }

    // Returns the number of distinct substrings of each length
    vector<int> countDistinctByLength() {
        vector<int> diff(n + 2, 0);
        for (int i = 0; i < n; i++) {
            int len = n - sa[i];
            int lcp_prev = (i == 0 ? 0 : lcp[i - 1]);
            int low = lcp_prev + 1;
            int high = len;
            if (low <= high) {
                diff[low]++;
                diff[high + 1]--;
            }
        }
        vector<int> result(n + 1);
        for (int i = 1; i <= n; i++)
            result[i] = result[i - 1] + diff[i];
        return result;
    }

    // Returns the k-th lexicographically smallest distinct substring
    string getKthDistinctSubstring(long long k) {
        for (int i = 0; i < n; i++) {
            int start = sa[i];
            int lcp_prev = (i == 0 ? 0 : lcp[i - 1]);
            int total = n - start - lcp_prev;
            if (k <= total)
                return s.substr(start, lcp_prev + k);
            k -= total;
        }
        return "";
    }

    // Returns the longest substring that appears more than once
    string longestRepeatingSubstring() {
        int max_len = 0, index = -1;
        for (int i = 1; i < n; i++) {
            if (lcp[i] > max_len) {
                max_len = lcp[i];
                index = sa[i];
            }
        }
        if (max_len == 0) return "-1";
        return s.substr(index, max_len);
    }

    // Returns the longest common substring between s and another string s2
    pair<int, string> longestCommonSubstring(const string& s2) {
        string combined = s + '#' + s2;
        int len1 = s.size();
        SuffixArray combinedSA(combined);
        int max_len = 0, index = -1;
        for (int i = 1; i < combinedSA.n; i++) {
            int a = combinedSA.sa[i], b = combinedSA.sa[i - 1];
            if ((a < len1) != (b < len1)) {
                if (combinedSA.lcp[i - 1] > max_len) {
                    max_len = combinedSA.lcp[i - 1];
                    index = combinedSA.sa[i];
                }
            }
        }
        if (max_len == 0) return {0, ""};
        return {max_len, combined.substr(index, max_len)};
    }

    // Returns the longest palindromic substring in s
    string longestPalindromicSubstring() {
        string rev = s;
        reverse(rev.begin(), rev.end());
        string joined = s + '#' + rev;
        SuffixArray pal(joined);
        int max_len = 0, index = -1;
        for (int i = 1; i < pal.n; i++) {
            int a = pal.sa[i], b = pal.sa[i - 1];
            bool in_s = (a < n), in_rev = (b > n);
            bool in_s2 = (b < n), in_rev2 = (a > n);
            if ((in_s && in_rev) || (in_s2 && in_rev2)) {
                int lcp_len = pal.lcp[i - 1];
                int pos = in_s ? a : b;
                if (lcp_len > max_len && pos + lcp_len <= n) {
                    max_len = lcp_len;
                    index = pos;
                }
            }
        }
        if (max_len == 0) return "";
        return s.substr(index, max_len);
    }

    // Lexicographically compares s[l1..r1] and s[l2..r2]
    int compareSubstrings(int l1, int r1, int l2, int r2) {
        int len1 = r1 - l1 + 1, len2 = r2 - l2 + 1;
        int min_len = min(len1, len2);
        for (int i = 0; i < min_len; i++) {
            if (s[l1 + i] != s[l2 + i])
                return (s[l1 + i] < s[l2 + i]) ? -1 : 1;
        }
        if (len1 == len2) return 0;
        return (len1 < len2) ? -1 : 1;
    }
};
