var https = require('https');
var fs = require('fs');
var path = require('path');
var abs = path.resolve('../Evoting-Tally-Server');
var tool = require('./tool');
var common = require('../config/common_var');

var req_sender = module.exports;
req_sender.options_template = {
    key: fs.readFileSync(abs + '/certs/tally_server/tally_'+global.tally_seq+'.key'),
    cert: fs.readFileSync(abs + '/certs/tally_server/tally_'+global.tally_seq+'.crt'),
    ca: [fs.readFileSync(abs + '/certs/root/ca.cer')],
    hostname: 'localhost',
    port: 8080,
    method: 'POST',
    headers: {
        'Content-Type': 'application/json',
        connection: 'keep-alive'
    }
};

req_sender.createOptions = function(options_template,method,hostname,port, path, send_data, token) {
    var o = new Object();
    o.key = options_template.key;
    o.cert = options_template.cert;
    o.ca = options_template.ca;
    o.hostname = hostname||options_template.hostname;
    o.port = port||options_template.port;
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

req_sender.send = function (options, send_data, callback) {
    var req = https.request(options, function (res) {
        console.log("access "+options.path);
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
                callback(true,json);
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
        callback(false);
    });
}