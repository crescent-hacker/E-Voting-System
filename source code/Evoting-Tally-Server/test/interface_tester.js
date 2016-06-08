var https = require('https');
var fs = require('fs');
var path = require('path');
var abs = path.resolve('../');
var tool = require('./test_tool');
var common = require('../config/common_var');

//####################  tool method ####################
var options_template = {
    key: fs.readFileSync(abs + '/certs/client/client.key'),
    cert: fs.readFileSync(abs + '/certs/client/client.crt'),
    ca: [fs.readFileSync(abs + '/certs/root/ca.cer')],
    hostname: 'tally_5',
    port: 10005,
    method: 'POST',
    headers: {
        'Content-Type': 'application/json',
        connection: 'keep-alive'
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

//####################  white box test case definition  ####################
function test_compute_commitment() {
    var obj = {
        X:136,
        lamda:2
    };
    var send_data = JSON.stringify(obj);
    var options = createOptions("GET", "/compute_commitment", send_data);
    req_sender(options, send_data);
}
function test_read_job_schedule() {
    var obj = {};
    var send_data = JSON.stringify(obj);
    var options = createOptions("GET", "/read_job_schedule", send_data);
    req_sender(options, send_data);
}

//####################  white box test case running  ####################
test_compute_commitment();
// test_read_job_schedule();