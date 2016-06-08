var mysql = require('mysql');
var tool = require('./../tool');
var common = require('../../config/common_var');
var dbconfig = require('../../config/database');
var connection = mysql.createConnection(dbconfig.connection);
connection.query('USE ' + dbconfig.database);
var logger = require('../../server').logger('normal');

// expose this function to our app using module.exports
var dao = module.exports;

//update token
dao.update_token = function(req,token,done){
    var updateTokenSql = "UPDATE user SET token = ? where user_id = ?";
    connection.query(updateTokenSql,[token,req.user.user_id],function(err,rows){
        if(err){
            logger.error(err);
            return done(false);
        }
        return done(true);
    });
}

//add_user
dao.add_user = function (req, done) {
    connection.query("SELECT * FROM user WHERE user_name = ?", [req.body.username], function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false);
        }
        if (rows.length) {
            var reason = 'add_user: That username is already taken.';
            req.flash('daoMessage', reason);
            return done(false);
        } else {
            // if there is no user with that username
            // create the user
            var plain_pwd = tool.randPassword();
            var newUserMysql = {
                username: req.body.username,
                // password: bcrypt.hashSync(password, null, null),  // use the generateHash function in our user model
                password: tool.hash(req.body.username + plain_pwd + common.common_salt),
                email: req.body.email,
                plaintext: plain_pwd
            };

            var insertQuery = "INSERT INTO user ( user_name, user_pwd, email_addr ,role_id,plain_pwd) values (?,?,?,2,?)";

            connection.query(insertQuery, [newUserMysql.username, newUserMysql.password, newUserMysql.email, newUserMysql.plaintext], function (err, rows) {
                if (err) {
                    logger.error(err);
                    return done(false);
                }
                newUserMysql.id = rows.insertId;
                return done(true, newUserMysql);
            });
        }
    });
}

//add_users
dao.add_users = function (req, done) {
    var user_list = req.body.user_list;
    var sql = 'SELECT * FROM user WHERE 1=0 ';
    var username_list = [];
    for (var i = 0; i < user_list.length; i++) {
        sql += ' or user_name = ? ';
        username_list.push(user_list[i].username);
    }
    connection.query(sql, username_list, function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false);
        }
        if (rows.length) {
            var reason = 'add_users: Some usernames are already taken.';
            req.flash('daoMessage', reason);
            return done(false);
        } else {
            // if there is no user with that username
            // create the users
            var newUserObjs = [];
            for (var i = 0; i < user_list.length; i++) {
                var plain_pwd = tool.randPassword();
                var sqlObj = [
                    user_list[i].username,
                    // password: bcrypt.hashSync(password, null, null),  // use the generateHash function in our user model
                    tool.hash(user_list[i].username + plain_pwd + common.common_salt),
                    user_list[i].email,
                    plain_pwd
                ];
                newUserObjs.push(sqlObj);
            }
            var insertQuery = "INSERT INTO user ( user_name, user_pwd, email_addr,plain_pwd ) values ?";
            connection.query(insertQuery, [newUserObjs], function (err, rows) {
                if (err) {
                    logger.error(err);
                    return done(false);
                }
                return done(true);
            });
        }
    });
}

//activate a user
dao.activate_user = function (req, done) {
    connection.query("SELECT * FROM user WHERE user_name = ?", [req.body.username], function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false);
        }
        if (rows.length <= 0) {
            var reason = 'activate_user: No such user.';
            req.flash('daoMessage', reason);
            return done(false);
        }
        var user = rows[0];
        if (user.is_activated == 1) {
            var reason = 'activate_user: That user has been activated.';
            req.flash('daoMessage', reason);
            return done(false);
        }

        var updateQuery = "UPDATE user SET is_activated = 1 WHERE user_name = ?";
        connection.query(updateQuery, [user.user_name], function (err, update_rows) {
            if (err) {
                logger.error(err);
                return done(false);
            }
            return done(true, user);
        });
    });
}

//disable a user
dao.disable_user = function (req, done) {
    connection.query("SELECT * FROM user WHERE user_name = ?", [req.body.username], function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false);
        }
        if (rows.length <= 0) {
            var reason = 'disable_user: No such user.';
            req.flash('daoMessage', reason);
            return done(false);
        }
        var user = rows[0];
        if (user.is_activated == 0) {
            var reason = 'disable_user: That user has been disabled.';
            req.flash('daoMessage', reason);
            return done(false);
        }

        //update
        user.plain_pwd = tool.randPassword();
        user.user_pwd = tool.hash(user.user_name+user.plain_pwd+common.common_salt);
        var updateQuery = "UPDATE user SET is_activated = 0,user_pwd = ?,plain_pwd = ?,is_pwd_modified = 0 WHERE user_name = ?";
        connection.query(updateQuery, [user.user_pwd,user.plain_pwd, user.user_name], function (err) {
            if (err) {
                logger.error(err);
                return done(false);
            }
            return done(true, user);
        });
    });
}

//get one user
dao.get_user = function (req, user_id, done) {
    connection.query("SELECT * FROM user WHERE user_id = ?", [user_id], function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false);
        }
        if (!rows.length) {
            req.flash('daoMessage', 'get_user: This user not exists or token error.');
            return done(false);
        }
        return done(true, rows[0]);
    });
};

//get user list
dao.get_user_list = function (req,done) {
    connection.query("SELECT * FROM user WHERE role_id <> 1", function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false);
        }
        if (!rows.length) {
            req.flash('daoMessage', 'get_user_list: No user at all.');
            return done(false);
        }
        var retObj = [];
        for (var i = 0; i < rows.length; i++) {
            retObj[i] = {username: rows[i].user_name, is_activated: rows[i].is_activated};
        }
        return done(true, retObj);
    });
}

