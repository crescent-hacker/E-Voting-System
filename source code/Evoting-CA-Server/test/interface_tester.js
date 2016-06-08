var https = require('https');
var fs = require('fs');
var abs = '/Users/dick/Dropbox/Workspace/NODEJS_PROJECT/Evoting-Auth-Server/';
var tool = require('./test_tool');
var common = require('../config/common_var');

//####################  tool method ####################
var options_template = {
    key: fs.readFileSync(abs + 'certs/client/client.key'),
    cert: fs.readFileSync(abs + 'certs/client/client.crt'),
    ca: [fs.readFileSync(abs + 'certs/root/ca.cer')],
    hostname: 'localhost',
    // hostname: 'csci969server',
    // hostname: '23.105.210.117',
    port: 8080,
    method: 'POST',
    headers: {
        // 'Content-Type': 'application/x-www-form-urlencoded',
        'Content-Type': 'application/json',
        // 'Content-Length': Buffer.byteLength(send_data),
        connection: 'keep-alive'
        // 'x-access-token':'eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOjE1LCJleHAiOjE0NjM5MzcyMzYwMjJ9.iTboxJJ70UqbOChSFcbhXmI3ykxtFZs-7yFESei36w0'
        // cookie:'connect.sid=s%3AuR2BTJjNH8j5VpxBBtLyv98Khq9tKVW0.lSvR422KQVLVhQAazbDDMgNkBQSepwfK%2Fm8HpbnAkdA'
    }
};

function createOptions(method, path, send_data, token) {
    var o = new Object();
    o.key = options_template.key;
    o.cert = options_template.cert;
    o.ca = options_template.ca;
    o.hostname = options_template.hostname;
    o.port = options_template.port;
    o.method = method;
    o.headers = options_template.headers;
    if (!(!send_data)) {
        o.headers['Content-Length'] = Buffer.byteLength(send_data);
    }
    if (!(!token)) {
        o.headers['x-access-token'] = token;
    }
    o.path = path;
    return o;
}

var req_sender = function (options, send_data, callback) {
    var req = https.request(options, function (res) {
        console.log("access "+options.path);
        // console.log('statusCode: ', res.statusCode);
        // console.log('headers: ', res.headers);
        var body = [];
        res.on('data', function (data) {
            body.push(data);
        });
        res.on('end', function () {
            var str = '';
            for(var i=0;i<body.length;i++){
                str+=body.toString();
            }
            if (!callback) {
                console.log(str)
            } else {
                var json = JSON.parse(str);
                callback(json);
            }
        });
    });
    if (!(!send_data)) {
        req.write(send_data);
    }
    req.end();
    req.on('error', function (e) {
        console.error(e);
        console.error(e.stack);
    });
}

//####################  white box test case   ####################
/**
 * -----------PV1 BEGIN--------------
 * TODO,1.getGi
 * input:
 *      -item number,
 *      -set Gset array 1
 * outout:
 *      Gi
 * C++:
 *      GetGi
 * TODO,2.getAlpha     
 * input:
 *      -P
 * output:
 *      alpha
 * C++:
 *      PV1_1_arf
 * TODO,3.genFirstBallot
 * input:
 *      -P from sys,
 *      -g from sys,
 *      -h from sys
 *      -Gi
 *      -alpha
 * output:
 *      xy array 1
 * C++:
 *      PV1_1
 * TODO,4.sendFirstBallotToHV
 * input:
 *      -xy
 * output:
 *      success bool
 * TODO,5.receiveC1fromHV
 * input:
 * output:
 *      c1
 * TODO,6.genW1
 * input:
 *      -P from sys
 * output:
 *      w1
 * C++:
 *      PV1_3_w1
 * TODO,7.genDR
 * input:
 *      -c1,
 *      -w1,
 *      -P from sys,
 *      -K(candidate amount)
 *      -item number
 * output:
 *      dr array 2 Kx2
 * C++:
 *      PV1_3_dr
 * TODO,8.genAB
 * input:
 *      -P from sys,
 *      -K(candidate amount)
 *      -item number,
 *      -g from sys,
 *      -h from sys,
 *      -xy
 *      -Gset
 *      -w1
 *      -dr
 * output:
 *      -AB array 2 Kx2
 * C++:
 *      PV1_3_AB
 * TODO,9.sendDRandABtoHV
 * input:
 *      -dr
 *      -AB
 * output:
 *      success bool
 * TODO,10.sendDRtoServer
 * input:
 *      -dr
 * output:
 *      success bool
 * -----------PV1 END--------------
 * -----------PK BEGIN--------------
 * TODO,1.receiveabandUV
 * input:
 * output:
 *      -ab array 1
 *      -uv array 1
 * TODO,2.genC2
 * input:
 *      -P from sys
 * output:
 *      -c2
 * C++:
 *      PK_2_c2
 * TODO,3.sendC2toHV
 * input:
 *      -c2
 * output:
 *      success bool
 * TODO,4.receiveRfromHV
 * input:
 * output:
 *      -r(response)
 * TODO,5.verifyPK
 * input:
 *      -P from sys,
 *      -ab,
 *      -uv,
 *      -r,
 *      -c2,
 *      -g from sys,
 *      -h from sys
 * output:
 *      success bool
 * C++:
 *      PK_4
 * -----------PK END--------------
 * -----------PV2 BEGIN--------------
 * TODO,1.receiveSTfromServer
 * input:
 * output:
 *      -st
 * TODO,2.genFinalBallot
 * input:
 *      -P from sys,
 *      -uv,
 *      -xy
 * output:
 *      Fballot
 * C++:
 *      PV2_2
 * TODO,3.verifyFinalBallot
 * input:
 *      -P from sys,
 *      -g from sys,
 *      -h from sys,
 *      -dr,
 *      -Fballot,
 *      -Gset,
 *      -st
 * output:
 *      success bool
 * C++:
 *      PV2_3
 * TODO,4.sendFBallotToServer(if verification passed)
 * input:
 *      -Fballot
 * output:
 *      success bool
 */