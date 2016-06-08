// server.js
// set up ======================================================================
// get all the tools we need
var express = require('express');
var session = require('express-session');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var morgan = require('morgan');
var port = process.argv[2] || 8080;
var passport = require('passport');
var flash = require('connect-flash');
var fs = require('fs');
var https = require('https');
var log4js = require('log4js');
var jwt = require('jwt-simple');

//ssl setting
var options = {
    key: fs.readFileSync('certs/server/server.key'),
    cert: fs.readFileSync('certs/server/server.crt'),
    handshakeTimeout: 500,
    ca: [fs.readFileSync('certs/root/ca.cer')],
    dhparam: fs.readFileSync('certs/server/dhparam.pem'),
    // passphrase         : '123456',
    requestCert: true,
    rejectUnauthorized: true,
    ciphers: 'ECDHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA256:AES128-GCM-SHA256:RC4:HIGH:!MD5:!aNULL',
    // secureProtocol : secureProtocol,
    // secureOptions  : secureOptions
}

//log4js setting
log4js.configure({
    appenders: [
        {type: 'console'},
        {
            type: 'dateFile',
            absolute: true,
            filename:  __dirname +'/logs/main-server',
            maxLogSize: 1024*1024,
            pattern: "-yyyy-MM-dd.log",
            alwaysIncludePattern: true,
            category: 'normal'
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
// connect to our database
require('./app/route_module/passport')(passport); // pass passport for configuration

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

// required for passport
app.use(session({
    secret: 'vidyapathaisalwaysrunning',
    resave: true,
    saveUninitialized: true
})); // session secret
app.use(passport.initialize());
app.use(passport.session()); // persistent login sessions
app.use(flash()); // use connect-flash for flash messages stored in session
app.use(log4js.connectLogger(this.logger('normal'), {level: log4js.levels.INFO, format: ':method :url'}));

exports.app = function () {
    return app;
}
// routes ======================================================================
require('./app/route_module/tally_manager.js')(app);
require('./app/route_module/user_manager.js')(app, passport);
require('./app/route_module/voter_handler.js')(app);
require('./app/route_module/voting_manager.js')(app);

// launch ======================================================================
var server = https.createServer(options, app);
server.listen(port);

// app.listen(port)
console.log('The main authority server started on port ' + port);

process.on('uncaughtException', function (err) {
    console.log(err);
    console.log(err.stack);
});


