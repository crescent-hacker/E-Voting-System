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
    hostname: 'localhost',
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
/****************** admin test functions ***************/
function test_pre_login(user_info,callback) {
    var obj = {}
    var send_data = JSON.stringify(obj);
    var options = createOptions("GET", "/pre_login", send_data, null);
    req_sender(options, send_data, function(data){
        test_login(user_info,data,callback);
    });
}
function test_login(user_info,token_info,callback) {
    var obj = {
        username: user_info.username,
        password: user_info.password,
        t_token:token_info.t_token
    };
    var plaintext = obj.username+obj.password+common.common_salt;
    obj.password = tool.hash(tool.hash(plaintext)+token_info.t);

    var send_data = JSON.stringify(obj);
    var options = createOptions("POST", "/login", send_data);
    var req = https.request(options, function (res) {
        console.log("access /login");
        var body = [];
        res.on('data', function (data) {
            body.push(data);
        });
        res.on('end', function () {
            var str = '';
            for(var i=0;i<body.length;i++){
                str+=body.toString();
            }
            // console.log(str);
            var token = JSON.parse(str).token;
            callback(token);
        });
    });
    req.write(send_data);
    req.end();
    req.on('error', function (e) {
        console.error(e);
        console.error(e.stack);
    });
}
function test_add_user(token) {
    var send_data = JSON.stringify({
        username: 'difan.chen',
        email: 'difan.chen@gmail.com'
    });
    var options = createOptions("POST", "/add_user", send_data, token);
    req_sender(options, send_data);
}
function test_get_user_list(token) {
    var obj = {}
    var send_data = JSON.stringify(obj);
    var options = createOptions("GET", "/get_user_list", send_data, token);
    req_sender(options, send_data);
}
function test_add_users(token) {
    var obj = {
        user_list: [
            {
                username: 'xiaoming',
                email: 'xiaoming@qq.com'
            },
            {
                username: 'xiaohong',
                email: 'xiaohong@qq.com'
            },
            {
                username: 'xiaofan',
                email: 'xioafan@qq.com'
            }
        ]
    };
    var send_data = JSON.stringify(obj);
    var options = createOptions("POST", "/add_users", send_data, token);
    req_sender(options, send_data);
}
function test_activate_user(token) {
    var obj = {
        username: 'difan.chen'
    };
    var send_data = JSON.stringify(obj);
    var options = createOptions("POST", "/activate_user", send_data, token);
    req_sender(options, send_data);
}
function test_disable_user(token) {
    var obj = {
        username: 'difan.chen'
    };
    var send_data = JSON.stringify(obj);
    var options = createOptions("POST", "/disable_user", send_data, token);
    req_sender(options, send_data);
}
function test_has_system_init(token){
    var obj = {};
    var send_data = JSON.stringify(obj);
    var options = createOptions("GET", "/has_system_init", send_data, token);
    req_sender(options, send_data);
}
function test_system_init(token){
    var obj = {};
    var send_data = JSON.stringify(obj);
    var options = createOptions("GET", "/system_init", send_data, token);
    req_sender(options, send_data);
}
function test_delete_vote(token){
    var obj = {
        vote_id:17
    };
    var send_data = JSON.stringify(obj);
    var options = createOptions("POST", "/delete_vote", send_data, token);
    req_sender(options, send_data);
}
function test_get_vote_list_m(token) {
    var obj = {};
    var send_data = JSON.stringify(obj);
    var options = createOptions("GET", "/get_vote_list_m", send_data, token);
    req_sender(options, send_data);
}
function test_get_vote_m(token) {
    var obj = {
        vote_id : 20
    };
    var send_data = JSON.stringify(obj);
    var options = createOptions("GET", "/get_vote_m", send_data, token);
    req_sender(options, send_data);
}
function test_publish_vote(token) {
    var obj = {
        vote_id : 15
    };
    var send_data = JSON.stringify(obj);
    var options = createOptions("POST", "/publish_vote", send_data, token);
    req_sender(options, send_data);
}
function test_launch_tally_servers(token) {
    var obj = {};
    var send_data = JSON.stringify(obj);
    var options = createOptions("GET", "/launch_tally_servers", send_data, token);
    req_sender(options, send_data,function(data){
        console.log(data);
    });
}
function test_propose_vote(token){
    var obj = {
        title: 'let\'s voting',
        content:'this is a test vote',
        deadline:'2016/06/19 14:03:56',
        publish_date:'2016/06/05 14:03:56',
        candidates:[
            {
                candname:'c1'
            },
            {
                candname:'c2'
            },
            {
                candname:'c3'
            }
        ],
        eligible_voters:[
            {
                username:'laolv'
            },
            {
                username:'difan.chen'
            }
        ]
    };
    var send_data = JSON.stringify(obj);
    var options = createOptions("POST", "/propose_vote", send_data, token);
    req_sender(options, send_data);
}

/****************** voter test functions ***************/
function test_modify_pwd(token) {
    var obj = {
        username: 'difan.chen',
        password: '123Abc..'
    };
    // obj.password = tool.hash(username+obj.password+common.common_salt);
    var send_data = JSON.stringify(obj);
    var options = createOptions("POST", "/modify_pwd", send_data, token);
    req_sender(options, send_data);
}
function test_get_vote_list(token) {
    var obj = {};
    var send_data = JSON.stringify(obj);
    var options = createOptions("GET", "/get_vote_list", send_data, token);
    req_sender(options, send_data);
}
function test_get_vote(token) {
    var obj = {
        vote_id : 18
    };
    var send_data = JSON.stringify(obj);
    var options = createOptions("GET", "/get_vote", send_data, token);
    req_sender(options, send_data);
}
function test_is_user_eligible(token) {
    var obj = {
        vote_id : 40
    };
    var send_data = JSON.stringify(obj);
    var options = createOptions("GET", "/is_user_eligible", send_data, token);
    req_sender(options, send_data);
}
function test_post_final_ballot(token) {
    var obj = {
        vote_id : 40,
        dr:{d:1,r:2},
        final_ballot:{x:1,y:2}
    };
    var send_data = JSON.stringify(obj);
    var options = createOptions("POST", "/post_final_ballot", send_data, token);
    req_sender(options, send_data);
}


//####################  test actions differed from roles after login  ####################
function after_admin_login(token) {
    test_launch_tally_servers(token);
    // test_get_user_list(token);
    // test_add_user(token);
    // test_add_users(token);
    // test_activate_user(token);
    // test_disable_user(token);
    // test_has_system_init(token);
    // test_system_init(token);
    // test_delete_vote(token);
    // test_get_vote_list_m(token);
    // test_get_vote_m(token);
    // test_publish_vote(token);
    // test_propose_vote(token);
}
function after_voter_login(token) {
    // test_modify_pwd(token);
    // test_has_system_init(token);
    // test_get_vote_list(token);
    // test_get_vote(token);
    // test_is_user_eligible(token);
    test_post_final_ballot(token);

}
//####################  user info  ####################
var voter_info = {
    username:'difan.chen',
    password:'123Abc..'
}
var admin_info = {
    username:'stefan',
    password:'stefan'
}

//####################  login and test ####################

test_pre_login(admin_info,after_admin_login);
// test_pre_login(voter_info,after_voter_login);