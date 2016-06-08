// server.js
// set up ======================================================================
// get all the tools we need
var express  = require('express');
var session  = require('express-session');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var morgan = require('morgan');
var port     = process.argv[2] || 8081;
var flash    = require('connect-flash');
var fs       = require('fs');
var https = require('https');
var log4js = require('log4js');
var jwt = require('jwt-simple');

//ssl setting
var options = {
    key: fs.readFileSync('certs/ca_server/ca_server.key'),
    cert: fs.readFileSync('certs/ca_server/ca_server.crt'),
    handshakeTimeout   : 500,                               // 握手超时时间，default=120s
    ca                 : [fs.readFileSync('certs/root/ca.cer')],                                // 有效证书认证机构
    dhparam            : fs.readFileSync('certs/ca_server/dhparam.pem'),
    // passphrase         : '123456',                          // 服务器密钥解析密码
    // requestCert        : true,                              // 客户端需要发送签名证书
    rejectUnauthorized : true,                               // 客户端发送的证书必须是有效认证机构签名
    ciphers          : 'ECDHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA256:AES128-GCM-SHA256:RC4:HIGH:!MD5:!aNULL',                               // 加密组件
}

//log4js setting
log4js.configure({
    appenders: [
        {type: 'console'},
        {
            type: 'dateFile',
            absolute: true,
            filename:  __dirname +'/logs/ca-server',
            maxLogSize: 1024*1024,
            pattern: "-yyyy-MM-dd.log",
            alwaysIncludePattern: true,
            category: 'ca-server'
        }
    ],
    replaceConsole: true
});
exports.logger=function(name){
    var logger = log4js.getLogger(name);
    logger.setLevel('DEBUG');
    return logger;
}

// configuration ===============================================================
// set up our express application
var app = express(options);
// var app      = express();
app.use(morgan('dev')); // log every request to the console
app.use(cookieParser()); // read cookies (needed for auth)
app.use(bodyParser.urlencoded({
	extended: true
}));
app.use(bodyParser.json());

app.set('view engine', 'ejs'); // set up ejs for templating
app.set('jwtTokenSecret', 'ThisTheSecr123etStrin5gOfToken$%^&*');

app.use(session({
	secret: 'vidyapathaisalwaysrunning',
	resave: true,
	saveUninitialized: true
 } )); // session secret
app.use(flash()); // use connect-flash for flash messages stored in session
app.use(log4js.connectLogger(this.logger('ca-server'), {level:log4js.levels.DEBUG, format:':method :url'}));

exports.app = function(){
    return app;
}
// routes ======================================================================
require('./app/ca_routes.js')(app); // load our routes and pass in our app and fully configured passport

// launch ======================================================================
var server = https.createServer(options,app);
server.listen(port);

// app.listen(port)
console.log('The magic happens on port ' + port);

process.on('uncaughtException', function (err) {
	console.log(err);
	console.log(err.stack);
});


