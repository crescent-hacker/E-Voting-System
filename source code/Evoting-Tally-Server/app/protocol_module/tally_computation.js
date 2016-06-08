var tool = require('../tool');

var tally_computation = module.exports;

tally_computation.tc = {
    XYs: [],//[{X,Y}]
    t_r: [],
    lm: [],
    Nt: 0,
    ptp: [],
    times: 0,
    res: 0
}


//private functions
var ballot_pdc = function (voters, ballots, P) {
    var XY = {X: 1, Y: 1};
    for (var i = 0; i < voters; i++) {
        XY.X = (XY.X * ballots[i].x) % P;
        XY.Y = (XY.Y * ballots[i].y) % P;
    }
    return XY;
}

//public functions
tally_computation.NumT = function (Available, Needed) {
    var Nt = tool.genRandomInt(0, Available - Needed) + Needed;
    return Nt;
}

tally_computation.PT = function (Nt, Available) {
    var ptp = [];
    ptp.length = Nt;

    for (var i = 0; i < Nt; i++) {
        ptp[i] = tool.genRandomInt(1, Available);
        var flag = true;
        if (i > 0) {
            for (var j = 0; j < i; j++) {
                if (ptp[j] == ptp[i]) {
                    flag = false;
                }
            }
        }
        if (!flag) {
            i--;
        }
    }
    return ptp;
}

tally_computation.lmta = function (Nt, Q, ptp) {
    var lm = [];
    lm.length = Nt;
    for (var i = 0; i < Nt; i++) {
        lm[i] = 1;
        for (var j = 0; j < Nt; j++) {
            if (ptp[i] != ptp[j]) {
                var temp = (ptp[j] - ptp[i]) % Q;
                if (temp < 0) {
                    temp = temp + Q;
                }
                lm[i] = (lm[i] * ptp[j] * tool.modInverse(temp, Q)) % Q;
            }
        }
    }
    return lm;
}

tally_computation.tallying = function (XY, cm, K, voters, Nt, P, Gset) {
    var t_r = [];
    var W;
    W = XY.X;
    for (var i = 0; i < Nt; i++) {
        W = (W * cm[i]) % P;
    }

    var flag = false;
    while (!flag) {
        var rp = tool.rpickup(voters, K);
        var cW = 1;
        for (var i = 0; i < K; i++) {
            cW = (cW * tool.quick_mod(Gset[i], rp[i], P)) % P;
        }
        if(cW == W){
            t_r = rp;
            flag = true;
        }else{
            rp.length = 0;
        }
    }
    return t_r;
}

tally_computation.t_msg = function(times,res,Allballots,P){
    var XYs = [];
    XYs.length = times+1;
    for(var t=0;t<times;t++){
        var voters = 4;
        var ballots = [];
        ballots.length = voters;
        for(var i=0;i<voters;i++){
            ballots[i] = {
                x:Allballots[i+t*4][0],
                y:Allballots[i+t*4][1]
            };
        }
        //tallying results
        XYs[t] = ballot_pdc(voters,ballots,P);
        //delete ballots
        ballots.length = 0;
    }

    var voters = res;
    var ballots = [];
    ballots.length = voters;
    for(var i=0;i<voters;i++){
        ballots[i] = {
            x:Allballots[i+t*4][0],
            y:Allballots[i+t*4][1]
        };
    }
    //tallying results
    XYs[times] = ballot_pdc(voters,ballots,P);
    //delete ballots
    ballots.length = 0;
    return XYs;
}
