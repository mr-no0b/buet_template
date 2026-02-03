ll dp[11][2][2];
string l,r;//l<r must
ll rec(ll ind, bool tightL, bool tightR){
    if(ind==l.size())return 0;
    if(dp[ind][tightL][tightR]!=-1)return dp[ind][tightL][tightR];
    char lo=tightL?l[ind]:'0',hi=tightR?r[ind]:'9';
    ll ans=LLONG_MAX;
    for(char i=lo;i<=hi;i++){
        //Cost change kora lagbe problemwise
        ll cost=(ll)(l[ind]==i)+(r[ind]==i);
        ans=min(ans,cost+rec(ind+1,tightL & l[ind]==i, tightR & r[ind]==i));
    }
    return dp[ind][tightL][tightR]=ans;
}
