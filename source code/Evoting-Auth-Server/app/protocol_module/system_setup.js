var tool = require('../tool');

var system_setup = module.exports;

//private functions
var sp = {
    NN:100001,
    prime :[],
    p:[],
    pr:[],
    k:0,
    c:0,
    s:0,
    Gs_P:[],//generators
    Gs_Q:[],//generators
    GN:0,
    P:0,
    Q:0,
    g:0,
    h:0,
    K:0,
    Gset:[],
    Nv:0,
    Nb:0,
    Needed:0,
    Available:0
};
var gen_s = function(Q){
    return tool.genRandomInt(1,Q-1);
};
var is_prime = function(){
    var i,j;
    for(i=2;i<sp.NN;i++){
        if(sp.prime[i]){
            sp.p[sp.k++] = i;
            for(j=i+i;j<sp.NN;j+=i){
                sp.prime[j]=false;
            }
        }
    }
};
var cal = function(n){
    for(var i=0;sp.p[i]*sp.p[i]<=n;i++){
        if(n%sp.p[i]==0){
            sp.pr[sp.c] = sp.p[i];
            while((n%sp.p[i])==0){
                n/=sp.p[i];
            }
            sp.c++;
        }
    }
    if(n>1){
        sp.pr[sp.c]=n;
        sp.c++;
    }
};
var quick_mod = function(a,b,m){
    var ans = 1;
    a%=m;
    while(b){
        if(b&1){
            ans=ans*a%m;
            b--;
        }
        b>>=1;
        a=a*a%m;
    }
    return ans;
};
//public functions
var gen_generator_list_p = function(P){
    var GN = 0;
    var t,gg,root;
    var root_arr = [];
    for(var i=0;i<sp.NN;i++){
        root_arr.push(1);
    }
    is_prime();
    cal(P-1);
    for(gg=2;gg<P;gg++){
        var flag = true;
        for(i=0;i<sp.c;i++){
            t = (P-1)/sp.pr[i];
            if(quick_mod(gg,t,P)==1){
                flag = false;
                break;
            }
        }
        if(flag){
            root = gg;
            root_arr[GN] = gg;
            GN++;
        }
    }
    
    for(var j=0;j<GN;j++){
        sp.Gs_P.push(root_arr[j]);
    }
};
var gen_generator_list_q = function(Q){
    var GN = 0;
    var t,gg,root;
    var root_arr = [];
    for(var i=0;i<sp.NN;i++){
        root_arr.push(1);
    }
    is_prime();
    cal(Q-1);
    for(gg=2;gg<Q;gg++){
        var flag = true;
        for(i=0;i<sp.c;i++){
            t = (Q-1)/sp.pr[i];
            if(quick_mod(gg,t,Q)==1){
                flag = false;
                break;
            }
        }
        if(flag){
            root = gg;
            root_arr[GN] = gg;
            GN++;
        }
    }

    for(var j=0;j<GN;j++){
        sp.Gs_Q.push(root_arr[j]);
    }
};

var gen_g_s_h = function(Q,P){
    var usable_gs = [];
    for(var i=0;i<sp.Gs_Q.length;i++){
        if(quick_mod(sp.Gs_Q[i],Q,P)==1){
            usable_gs.push(sp.Gs_Q[i]);
        }
    }
    if(usable_gs.length <= 0){
        return {success:false};
    }
    var g = usable_gs[tool.genRandomInt(0,usable_gs.length-1)];
    var s = gen_s(Q);
    var h = quick_mod(g,s,P);
    return {success:true,g:g,h:h,s:s};
}


var shamir_split = function(Available,Needed,Q,s){
    var avb=Available;
    var sh = [];
    for(var i=0;i<avb;i++){
        sh.push([0,0]);
    }
    var a1 = tool.genRandomInt(1,Q);
    var a2;
    do{
        a2 = tool.genRandomInt(1,Q);
    }while(a1==a2);

    var coef = [s,a1,a2];
    for(var i=1;i<=avb;i++){
        var accum = coef[0];
        for(var j=1;j<Needed;j++){
            accum = (accum + coef[j]*quick_mod(i,j,Q))%(Q);
        }
        sh[i-1][0] = i;
        sh[i-1][1] = accum;
    }
    return sh;
};

//export interface 883,8831
system_setup.genSysParam = function(Q,P,Needed,Available){
    //init params
    sp.prime.length = sp.NN;
    sp.p.length = sp.NN;
    sp.pr.length = sp.NN;
    for(var i=0;i<sp.NN;i++){
        sp.prime[i] = true;
        sp.p[i] = 0;
        sp.pr[i] = 0;
    }
    var retObj = {};
    gen_generator_list_q(Q);
    gen_generator_list_p(P);
    var g_s_h = gen_g_s_h(Q,P);
    if(!g_s_h.success){
        retObj.success = false;
        return retObj;
    }

    var sh = shamir_split(Available,Needed,Q,g_s_h.s);
    retObj.secrets = [];
    for(var i=0;i<sh.length;i++){
        retObj.secrets.push(sh[i][1]);
    }
    retObj.g = g_s_h.g;
    retObj.h = g_s_h.h;
    retObj.p = P;
    retObj.q = Q;
    retObj.gn = sp.Gs_P.length;
    retObj.generator_list = sp.Gs_P;
    retObj.success = true;
    return retObj;
};

// console.log(system_setup.genSysParam(888,8831,2,5));