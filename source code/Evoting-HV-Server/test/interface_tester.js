var https = require('https');
var fs = require('fs');
var path = require('path');
var abs = path.resolve('../');

//####################  tool method ####################
var options_template = {
    key: fs.readFileSync(abs + '/certs/client/client.key'),
    cert: fs.readFileSync(abs + '/certs/client/client.crt'),
    ca: [fs.readFileSync(abs + '/certs/root/ca.cer')],
    hostname: 'honest_verifier',
    port: 8082,
    method: 'POST',
    headers: {
        'Content-Type': 'application/json',
        connection: 'keep-alive',
        cookie: 'connect.sid=s%3Adu3I_9PNDzfU4iNqQT_RQ-uLprhzwKdU.njnttgTQfEXlCMQf45JCiCObZOdosyT49cRxMWhTvpw; Path=/; HttpOnly'
    }
};

function createOptions(method, path, send_data, cookie) {
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
    o.headers['cookie'] = (cookie);
    o.path = path;
    return o;
}

var req_sender = function (options, send_data, callback) {
    var req = https.request(options, function (res) {
        console.log("access " + options.path);
        // console.log('statusCode: ', res.statusCode);
        if (res.headers['set-cookie']) {
            console.log(res.headers['set-cookie'][0]);
        }
        var body = [];
        res.on('data', function (data) {
            body.push(data);
        });
        res.on('end', function () {
            var str = '';
            for (var i = 0; i < body.length; i++) {
                str += body.toString();
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
    req.on('error', function (e) {
        console.error(e);
        console.error(e.stack);
    });
}

//####################  white box test case definition  ####################
function test() {
    var obj = {
        xy: {x: 1, y: 2}
    };
    var send_data = JSON.stringify(obj);
    var options = createOptions("GET", "/step_3", send_data,
'connect.sid=s%3A1OJTlGMuHC5bUfdEezT-cv9pRFt_RjgC.sqPl1%2BkOH3ccVBAb5SAJ2Ik0kcB%2Fw7f12Jc4530q5ww; Path=/; HttpOnly'    );
    req_sender(options, send_data, function (data) {
        console.log(data);
    });
}

//####################  white box test case running  ####################
test();