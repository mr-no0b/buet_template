string t, p;
string s = p + "#" + t;
int n = s.size();
// pi[0...i] returns longest suffix which is prefix
vector<int> pi(n, 0);
for(int i = 1, j = 0; i < n; i++) {
    while(j >= 0 && s[i] != s[j]) {
        if(j >= 1) j = pi[j - 1];
        else j = -1; 
    }
    j++;
    pi[i] = j;
}
