var mysql = require('mysql');
var tool = require('./tool');
var common = require('../config/common_var');
var dbconfig = require('../config/database');
var connection = mysql.createConnection(dbconfig.connection);
connection.query('USE ' + dbconfig.database);
var logger = require('../server').logger('ca-server');

// expose this function to our app using module.exports
var dao = module.exports;

dao.verify_user = function (req, done) {
    connection.query("SELECT * FROM user WHERE user_name = ?", [req.body.username], function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false);
        }
        if (rows.length <= 0) {
            var reason = 'User is not valid.';
            req.flash('applyMessage', reason);
            return done(false);
        } else {
            var user = rows[0];
            if (user.user_pwd != tool.hash(req.body.username + req.body.password + common.common_salt)) {
                var reason = 'User is not valid .';
                req.flash('applyMessage', reason);
                return done(false);
            }
            req.session.user = user;
            return done(true);
        }
    });
}
dao.update_has_cert = function (req, done) {
    connection.query("UPDATE user SET has_cert = 1 WHERE user_name = ?", [req.session.user.user_name], function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false);
        }
        req.session.user.has_cert = 1;
        return done(true);
    });
}

