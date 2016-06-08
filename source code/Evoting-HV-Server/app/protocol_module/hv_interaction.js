var tool = require('../tool');

var hv_interaction = module.exports;

//private functions
hv_interaction.hv = {
    c1:0,
    PV1_verf:false,
    bta:0,
    w2:0,
    uv:[],
    ab:[],
    r:0,
    st:[]
};

//---PV1---
hv_interaction.PV1_2 = function(Q){
    var c1 =  tool.genRandomInt(1,Q-1);
    return c1;
};

hv_interaction.PV1_4 = function(P,g,h,Gset,K,dr,fballot,AB,Q,c1){
    // console.log(arguments);
    var flag = true;
    var sum =0;
    var sum1,sum2;
    for(var i=0;i<K;i++){
        sum = (sum+dr[i].d)%Q;
    }
    console.log("sum:"+sum);
    if(sum!=c1){
        return false;
    }
    
    for(var i=0;i<K;i++){
        sum1 = (tool.quick_mod(g,dr[i].r,P)*tool.quick_mod(fballot.x,dr[i].d,P))%P;
        // console.log("sum1:"+sum1);
        // console.log("AB[i].a:"+AB[i].a);
        if(AB[i].a!=sum1){
            flag = false;
        }
        sum2 = ((tool.quick_mod(h,dr[i].r,P))*(tool.quick_mod(fballot.y,dr[i].d,P))*(tool.quick_mod(Gset[i],(P-1-dr[i].d),P)))%P;
        // console.log("sum2:"+sum2);
        // console.log("AB[i].b:"+AB[i].b);
        if(AB[i].b!=sum2)
        {
            flag=false;
        }
    }
    return flag;
}

hv_interaction.PK_1_bta = function(Q){
    var bta = tool.genRandomInt(1,Q-1);
    return bta;
}

hv_interaction.PK_1_w2 = function(Q){
    var w2 = tool.genRandomInt(1,Q-1);
    return w2;
}

hv_interaction.PK_1_uv = function(P,g,h,bta){
    var uv = {u:0,v:0};
    uv.u = tool.quick_mod(g,bta,P);
    uv.v = tool.quick_mod(h,bta,P);
    return uv;
}

hv_interaction.PK_1_ab = function(P,g,h,w2){
    var ab = {a:0,b:0};
    ab.a = tool.quick_mod(g,w2,P);
    ab.b = tool.quick_mod(h,w2,P);
    return ab;
}

hv_interaction.PK_3_r = function(P,c2,w2,bta){
    var r;
    r = (w2+bta*c2)%(P-1);
    return r;
}

//----PV2----
//(s,t)
hv_interaction.PV2_1 = function(P,K,uv,AB,dr){
    var ST = [];
    ST.length = K;
    for(var i=0;i<K;i++){
        ST[i] = {
            s:(AB[i].a*tool.quick_mod(uv.u,dr[i].d,P))%P,
            t:(AB[i].b*tool.quick_mod(uv.v,dr[i].d,P))%P
        };
    }
    return ST;
}

