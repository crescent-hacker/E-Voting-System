// server.js
//init setting
if (process.argv.length < 5) {
    console.log("tally server launch fail.");
    return;
}
var c_port = process.argv[2];
var seq = process.argv[3];
var secret = process.argv[4];
global.is_main = (process.argv.length == 6 && process.argv[5] == 'main');
global.tally_seq = seq;
global.tally_secret = secret;
global.job_schedule = [];
// set up ======================================================================
// get all the tools we need
var express = require('express');
var session = require('express-session');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var morgan = require('morgan');
var port = c_port;
var flash = require('connect-flash');
var fs = require('fs');
var https = require('https');
var log4js = require('log4js');
var jwt = require('jwt-simple');
var path = require('path');
var abs = path.resolve("./")//absolute path
//ssl setting
var options = {
    host:'tally_'+seq,
    key: fs.readFileSync(__dirname+'/certs/tally_server/tally_' + seq + '.key'),
    cert: fs.readFileSync(__dirname+'/certs/tally_server/tally_' + seq + '.crt'),
    handshakeTimeout: 500,
    ca: [fs.readFileSync(__dirname+'/certs/root/ca.cer')],                                
    dhparam: fs.readFileSync(__dirname+'/certs/tally_server/dhparam.pem'),
    // passphrase         : '123456',
    requestCert: true,
    rejectUnauthorized: true,                       
    ciphers: 'ECDHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA256:AES128-GCM-SHA256:RC4:HIGH:!MD5:!aNULL',                               
}

//log4js setting
log4js.configure({
    appenders: [
        {type: 'console'},
        {
            type: 'dateFile',
            absolute: true,
            filename: __dirname + '/logs/tally-'+seq,
            maxLogSize: 1024 * 1024,
            pattern: "-yyyy-MM-dd.log",
            alwaysIncludePattern: true,
            category: 'tally-' + seq
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
app.use(log4js.connectLogger(this.logger('tally-' + seq), {level: log4js.levels.DEBUG, format: ':method :url'}));

exports.app = function () {
    return app;
}
// routes ======================================================================
require('./app/tally_routes.js')(app); // load our routes and pass in our app and fully configured passport

// launch ======================================================================
var server = https.createServer(options, app);
server.listen(port);
console.log("tally-"+seq+" server start on port "+port+(global.is_main?" as main":""));
// app.listen(port)


process.on('uncaughtException', function (err) {
    console.log(err);
    console.log(err.stack);
});


