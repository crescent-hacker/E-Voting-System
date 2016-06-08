// server.js
//init setting
var c_port = process.argv[2];
// set up ======================================================================
// get all the tools we need
var express = require('express');
var session = require('express-session');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var morgan = require('morgan');
var port = c_port||8082;
var flash = require('connect-flash');
var fs = require('fs');
var https = require('https');
var log4js = require('log4js');
var jwt = require('jwt-simple');
var path = require('path');
//ssl setting
var options = {
    host:'localhost',
    key: fs.readFileSync(__dirname+'/certs/hv_server/honest_verifier.key'),
    cert: fs.readFileSync(__dirname+'/certs/hv_server/honest_verifier.crt'),
    handshakeTimeout: 500,
    ca: [fs.readFileSync(__dirname+'/certs/root/ca.cer')],                                
    dhparam: fs.readFileSync(__dirname+'/certs/hv_server/dhparam.pem'),
    // passphrase         : '123456',
    requestCert: true,
    rejectUnauthorized: true,
    ciphers: 'ECDHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA256:AES128-GCM-SHA256:RC4:HIGH:!MD5:!aNULL',
};

//log4js setting
log4js.configure({
    appenders: [
        {type: 'console'},
        {
            type: 'dateFile',
            absolute: true,
            filename: __dirname + '/logs/hv',
            maxLogSize: 1024 * 1024,
            pattern: "-yyyy-MM-dd.log",
            alwaysIncludePattern: true,
            category: 'hv'
        }
    ],
    replaceConsole: true
});
exports.logger = function (name) {
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
})); // session secret
app.use(flash()); // use connect-flash for flash messages stored in session
app.use(log4js.connectLogger(this.logger('hv'), {level: log4js.levels.DEBUG, format: ':method :url'}));

exports.app = function () {
    return app;
}
// routes ======================================================================
require('./app/hv_routes.js')(app); // load our routes and pass in our app and fully configured passport

// launch ======================================================================
var server = https.createServer(options, app);
server.listen(port);
console.log("hv server start on port "+port);
// app.listen(port)


process.on('uncaughtException', function (err) {
    console.log(err);
    console.log(err.stack);
});


