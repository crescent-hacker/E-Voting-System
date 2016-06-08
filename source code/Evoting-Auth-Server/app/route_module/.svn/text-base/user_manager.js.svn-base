// app/routes.js
var moment = require('moment');
var jwt = require('jwt-simple');
var dao = require('../dao_module/user_manager_dao');
var common = require('../../config/common_var');
var logger = require('../../server').logger('normal');
var mailsender = require('../mail_sender');
var routes = require('./routes');

module.exports = function (app, passport) {
    app.get('/pre_login', function (req, res) {
        var timestamp = Date.now();
        var t_token = jwt.encode({
            timestamp: timestamp,
        }, app.get('jwtTokenSecret'));
        //return to admin client
        res.write(JSON.stringify({
            t_token: t_token,
            t: timestamp,
            t_str: '' + timestamp
        }));
        res.end();

    });
    // process the login form
    app.post('/login', passport.authenticate('local-login', {
            // successRedirect : '/profile', // redirect to the secure profile section
            failureRedirect: '/login', // redirect back to the signup page if there is an error
            failureFlash: true // allow flash messages
        }),
        function (req, res) {
            logger.debug("User " + req.user.user_name + " has logined");
            req.session.is_admin = (req.user.role_id == common.admin_role);
            //generate token and send it to client
            var expires = moment().add(common.token_expire, 'days').valueOf();
            var token = jwt.encode({
                iss: req.user.user_id,
                exp: expires
            }, app.get('jwtTokenSecret'));
            dao.update_token(req,token,function(result){
                if(result){
                    logger.debug("update_token for "+req.user.user_name+" success.");
                }
            });
            var token_info = {
                token: token,
                expires: expires,
                success: true,
                is_admin: req.session.is_admin,
                is_pwd_modified: req.user.is_pwd_modified
            };
            res.write(JSON.stringify(token_info));
            res.end();
        });

    // logout
    app.post('/logout',
        function (req, res) {
            req.logout();
            res.json({
                success: true
            });
        });

    //add a new user
    app.post('/add_user', routes.isAdminLoggedIn, function (req, res) {
        dao.add_user(req, function (result, added_user) {
            if (result) {
                logger.debug("add_user " + added_user.username + " successfully; pwd is " + added_user.password);
            } else {
                logger.error(req.flash('daoMessage'));
            }
            res.write(JSON.stringify({
                success: result
            }));
            res.end();
        });
    });

    //add users
    app.post('/add_users', routes.isAdminLoggedIn, function (req, res) {
        dao.add_users(req, function (result) {
            if (result) {
                logger.info("add_users successfully");
            } else {
                logger.error(req.flash('daoMessage'));
            }
            res.write(JSON.stringify({
                success: result
            }));
            res.end();
        });
    });

    //get user list
    app.get('/get_user_list', routes.isAdminLoggedIn, function (req, res) {
        dao.get_user_list(req,function (result, user_list) {
            var retObj = {};
            if (result) {
                logger.debug("get_user_list successfully");
                retObj['user_list'] = user_list;
            } else {
                logger.error(req.flash('daoMessage'));
            }
            retObj['success'] = result;
            res.write(JSON.stringify(retObj));
            res.end();
        });
    });

    //activate a user
    app.post('/activate_user', routes.isAdminLoggedIn, function (req, res) {
        dao.activate_user(req, function (result, activated_user) {
            if (result) {
                logger.debug("activate_user " + activated_user.user_name + " successfully");
                //send mail to the user
                mailsender.send(activated_user.user_name, activated_user.plain_pwd, activated_user.email_addr);
            } else {
                logger.error(req.flash('daoMessage'));
            }

            //return to admin client
            res.write(JSON.stringify({
                success: result
            }));
            res.end();
        });
    });

    //disable a user
    app.post('/disable_user', routes.isAdminLoggedIn, function (req, res) {
        dao.disable_user(req, function (result, disabled_user) {
            if (result) {
                logger.debug("disable_user " + disabled_user.user_name + " successfully");
            } else {
                logger.error(req.flash('daoMessage'));
            }

            //return to admin client
            res.write(JSON.stringify({
                success: result
            }));
            res.end();
        });
    });

};

