var tool = module.exports;
var crypto = require('crypto');

tool.randPassword = function () {
    var text = ['abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', '1234567890', '~!@#$%^&*()_+";",./?<>'];
    var rand = function (min, max) {
        return Math.floor(Math.max(min, Math.random() * (max + 1)));
    }
    var len = rand(8, 16);
    var pw = '';
    for (i = 0; i < len; ++i) {
        var strpos = rand(0, 3);
        pw += text[strpos].charAt(rand(0, text[strpos].length));
    }
    return pw;
}

tool.hash = function(pwd) {
    var hash = crypto.createHash('sha256').update(pwd).digest('base64');
    return hash;
}

tool.isPwdSimple = function(pwd){
    if (pwd.length < 8) {
        return true;
    }
    var ls = 0;
    if (pwd.match(/([a-z])+/)) {
        ls++;
    }
    if (pwd.match(/([0-9])+/)) {
        ls++;
    }
    if (pwd.match(/([A-Z])+/)) {
        ls++;
    }
    if (pwd.match(/[^a-zA-Z0-9]+/)) {
        ls++;
    }
    if(ls<4){
        return true;
    }
    return false;
}

tool.genRandomInt = function(min,max){
    return Math.round(Math.random()*(max-min)+min);
}

tool.quick_mod = function(a,b,m){
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

