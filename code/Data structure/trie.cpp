/*
if you need dfs on trie
at first make the trie an uniqe tree
inserting an unique id on each node
then just treat as node of a tree
*/
class Trie {
public:
 
    class node { // 1 based
    public:
        node *child[27];
        int leaf, sz, id;
        node() {
            for(int i = 0; i < 27; i++) {
                child[i] = nullptr;
            }
            sz = 0;
            leaf = 0;
            id = 0;
        }
    } *root;
    
    vector<int> dis;
    int unqid = 1;

    Trie () {
        root = new node();

    }

    void insert(string s) {
        auto cur = root; 
        for(auto x : s) {
            int i = x - 'a' + 1; // 1-based
            if(!cur->child[i]) { 
                cur->child[i] = new node(); // edge dilam
            }
            cur = cur->child[i];
            cur->sz++; // new edge e ashalam ebar increment korbo 
            cur->id = unqid++;
        }
        cur->leaf++;
    }

    int set(node *cur) {
        dis[cur->id] = cur->leaf;
        for(int i = 1; i <= 26; i++) {
            if(cur->child[i]) {
                set(cur->child[i]);
                dis[cur->id] = max(dis[cur->id], dis[cur->child[i]->id]);
            }
        }
        return dis[cur->id];
    }
};
Trie mytrie;
for(int i = 1; i <= n; i++) {
    string t; cin >> t;
    mytrie.insert(t);
}
mytrie.dis.assign(mytrie.unqid + 1, 0);
mytrie.set(mytrie.root);
/*
count how many subarray Xor < k
*/
struct Trie {
    const int B = 20;
    struct node { // 1 based
        node *child[2];
        int cnt;
        node() {
            child[0] = child[1] = 0;
            cnt = 0;
        }
    } *root;
    
    Trie () {
        root = new node();
    }

    void insert(int x) {
        auto cur = root;
        for(int i = B - 1; i >= 0; i--) {
            int id = (x >> i) & 1;
            if(!cur->child[id]) cur->child[id] = new node();
            cur = cur->child[id];
            cur->cnt++;
        }
    }

    ll countLess(int y, int k) { // to count greater n - countless
        auto cur = root;
        ll ans = 0;
        for(int i = B - 1; i >= 0 && cur; i--) {
            int yb = (y >> i) & 1;
            int kb = (k >> i) & 1;
            if(kb == 1) {
                if(cur->child[yb]) {
                    ans += cur->child[yb]->cnt;
                }
                cur = cur->child[!yb];
            }
            else {
                cur = cur->child[yb];
            }
        }
        return ans;
    }
    int occurence(int x) {
        auto cur = root;
        for(int i = B - 1; i >= 0; i--) {
            int id = (x >> i) & 1;
            if(!cur->child[id]) return 0;
            cur = cur->child[id];
        }
        return cur->leaf;
    }
 
    void del(int x) {
        stack<pair<node *, int>> stck;
        auto cur = root;
        for(int i = B - 1; i >= 0; i--) {
            int id = (x >> i) & 1;
            stck.push({cur, id});
            cur = cur->child[id];
        }   
        while(not stck.empty()) {
            auto [par, id] = stck.top();
            stck.pop();
            auto child = par->child[id];
            if(!child->child[0] && !child->child[1]) {
                delete child;
                par->child[id] = nullptr;
            }
            else {
                break;
            }
        }
    }
 
    void remove(int x) {
        if(occurence(x) == 1) {
            del(x);
            return;
        }
        auto cur = root;
        for(int i = B - 1; i >= 0; i--) {
            int id = (x >> i) & 1;
            cur = cur->child[id];
        }
        cur->leaf--;
    }
 
    int get_max(int x) {
        auto cur = root;
        int ans = 0;
        for(int i = B - 1; i >= 0; i--) {
            int id = (x >> i) & 1;
            if(cur->child[!id]) {
                //cout << i << " " << id << " " << sz << "\n";
                ans += (1 << i);
                cur = cur->child[!id];
            }
            else if(cur->child[id]){
                cur = cur->child[id];
            }
            else break;
        }
    }
    void clear(node *cur) {
        if(!cur) return;
        for(int i = 0; i < 2; i++) {
            if(cur->child[i]) clear(cur->child[i]);
        }
        delete(cur);
    }
    ~Trie() {
        clear(root);
    }
};
ll ans = 0;
int pref = 0;
Trie trie;
trie.insert(0);
for(auto x : v) {
    pref ^= x;
    ans += trie.countLess(pref, k);
    trie.insert(pref);
}