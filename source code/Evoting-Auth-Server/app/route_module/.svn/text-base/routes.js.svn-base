var jwt = require('jwt-simple');
var dao = require('../dao_module/user_manager_dao');
var logger = require('../../server').logger('normal');

var app =require("../../server").app();

//define routes utils
var routes = module.exports;

routes.isLoggedIn = function(req, res, next) {
    var req_token = (req.body && req.body.access_token) || (req.query && req.query.access_token) || req.headers['x-access-token'];
    if (req_token) {
        try {
            var decoded = jwt.decode(req_token, app.get('jwtTokenSecret'));
            if (decoded.exp <= Date.now()) {
                res.end('Access token has expired', 400);
            } else {
                dao.get_user(req, decoded.iss,function (result, user) {
                    if (result) {
                        if (user.is_activated != 1) {
                            var reason = 'User ' + user.user_name + ' is not activated.';
                            logger.error(reason)
                            res.end(reason, 400);
                        } else if(user.token!=req_token){
                            var reason = 'Token not valid .';
                            logger.error(reason)
                            res.end(reason, 400);
                        }else if(user.is_pwd_modified != 1 && "/modify_pwd" != req.path){
                            var reason = 'User ' + user.user_name + ' has not modified password.';
                            logger.error(reason)
                            res.end(reason, 400);
                        }else{
                            req.user = user;
                            logger.debug("Normal User "+user.user_name+" is accessing.");
                            next();
                        }

                    } else {
                        logger.error(req.flash('daoMessage'));
                        next();
                    }
                });
            }
        } catch (err) {
            logger.error(err);
            res.end('Permission denied', 400);
        }
    } else {
        res.end('Permission denied', 400);
    }
}

routes.isAdminLoggedIn = function(req, res, next) {
    var req_token = (req.body && req.body.access_token) || (req.query && req.query.access_token) || req.headers['x-access-token'];
    if (req_token) {
        try {
            var decoded = jwt.decode(req_token, app.get('jwtTokenSecret'));

            if (decoded.exp <= Date.now()) {
                res.end('Access token has expired', 400);
            } else {
                dao.get_user(req, decoded.iss,function (result, user) {
                    if (result) {
                        if (user.is_activated != 1) {
                            var reason = 'User ' + user.user_name + ' is not activated.';
                            logger.error(reason)
                            res.end(reason, 400);
                        } else if (user.role_id != 1) {
                            var reason = 'Permission denied.';
                            logger.error(reason)
                            res.end(reason, 400);

                        } else if(user.token!=req_token){
                            var reason = 'Token not valid.';
                            logger.error(reason)
                            res.end(reason, 400);
                        }else {
                            req.user = user;
                            logger.debug("Admin User "+user.user_name+" is accessing.");
                            next();
                        }
                    } else {
                        logger.error(req.flash('daoMessage'));
                        next();
                    }
                });
            }
        } catch (err) {
            logger.error(err);
            res.end('Permission denied', 400);
        }
    } else {
        res.end('Permission denied', 400);
    }
}