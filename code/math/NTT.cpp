//Multiply returns ans[k]=∑ ai*bj % MOD [such that i+j==k]
//Frequency arrays are often used
const int MOD=998244353, ROOT=3;
int lim, inv_lim;
vector<int> rev, wn, w;
int modexp(int x,int e){
  int r=1;
  while(e){
    if(e&1) r=(long long)r*x%MOD;
    x=(long long)x*x%MOD;
    e>>=1;
  }
  return r;
}
void precompute(int n){
  lim=1; int L=0;
  while(lim<n) lim<<=1, ++L;
  rev.assign(lim,0);
  for(int i=0;i<lim;i++)
    rev[i]=(rev[i>>1]>>1)|((i&1)<<(L-1));
  wn.assign(lim,1);
  int g=modexp(ROOT,(MOD-1)/lim);
  for(int i=1;i<lim;i++) wn[i]=(long long)wn[i-1]*g%MOD;
  inv_lim=modexp(lim,MOD-2);
  w.resize(lim);
}
void ntt(vector<int>&a,bool invert){
  for(int i=0;i<lim;i++) if(i<rev[i]) swap(a[i],a[rev[i]]);
  for(int len=1;len<lim;len<<=1){
    int step=lim/(len<<1);
    for(int i=0;i<len;i++) w[i]=wn[i*step];
    for(int i=0;i<lim;i+=len<<1)
      for(int j=0;j<len;j++){
        int u=a[i+j];
        int v=(long long)a[i+j+len]*w[j]%MOD;
        a[i+j]=u+v<MOD?u+v:u+v-MOD;
        a[i+j+len]=u-v>=0?u-v:u-v+MOD;
      }
  }
  if(invert){
    reverse(a.begin()+1,a.begin()+lim);
    for(int&i:a) i=(long long)i*inv_lim%MOD;
  }
}
vector<int> multiply(vector<int>a,vector<int>b){
  if(a.empty()||b.empty()) return {};
  int need=a.size()+b.size()-1;
  precompute(need);
  a.resize(lim); b.resize(lim);
  ntt(a,false); ntt(b,false);
  for(int i=0;i<lim;i++) a[i]=(long long)a[i]*b[i]%MOD;
  ntt(a,true);
  a.resize(need);
  return a;
}
