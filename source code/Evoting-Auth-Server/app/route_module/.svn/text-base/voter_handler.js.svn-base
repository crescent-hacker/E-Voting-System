// app/routes.js
var moment = require('moment');
var jwt = require('jwt-simple');
var dao = require('../dao_module/voter_handler_dao');
var common = require('../../config/common_var');
var logger = require('../../server').logger('normal');
var mailsender = require('../mail_sender');
var routes = require('./routes');

module.exports = function (app) {
    //new a vote
    app.post('/get_vote_list', routes.isLoggedIn, function (req, res) {
        logger.info(JSON.stringify(req.body));
        res.write(JSON.stringify({
            'fuck': 'ohyeah'
        }));
        res.end();
    });

    //voter modify password
    app.post('/modify_pwd', routes.isLoggedIn, function (req, res) {
        dao.modify_pwd(req, function (result, modified_user) {
            if (result) {
                logger.debug("modify_pwd " + modified_user.user_name + " successfully; pwd is " + modified_user.user_pwd);
                mailsender.send_pwd_modify_info(modified_user.user_name, modified_user.email_addr);
            } else {
                logger.error(req.flash('daoMessage'));
            }
            res.write(JSON.stringify({
                success: result
            }));
            res.end();
        });
    });

    app.get('/get_vote_list', routes.isLoggedIn, function (req, res) {
        dao.get_vote_list(req, function (result, vote_list) {
            var str;
            if (result) {
                logger.debug("get_vote_list successfully");
                str = JSON.stringify({
                    success: result,
                    vote_list: vote_list
                });
            } else {
                str = JSON.stringify({
                    success: result
                });
            }
            res.write(str);
            res.end();
        });
    });

    app.get('/get_vote', routes.isLoggedIn, function (req, res) {
        dao.get_vote(req, function (result, vote_detail) {
            var str;
            if (result) {
                logger.debug("get_vote_list successfully");
                str = JSON.stringify({
                    success: result,
                    vote_detail: vote_detail
                });
            } else {
                str = JSON.stringify({
                    success: result
                });
            }
            res.write(str);
            res.end();
        });
    });

    app.get('/is_user_eligible', routes.isLoggedIn, function (req, res) {
        dao.is_user_eligible(req, function (result, vote_detail) {
            var str;
            if (result) {
                logger.debug("is_user_eligible successfully");
            }
            str = JSON.stringify({
                success: result
            });
            res.write(str);
            res.end();
        });
    });

    app.post("/post_final_ballot",routes.isLoggedIn,function(req,res){
        dao.post_final_ballot(req,function(result){
            var str;
            if (result) {
                logger.debug("post_final_ballot successful.")
                str = JSON.stringify({
                    success: true
                });
            } else {
                str = JSON.stringify({
                    success: false
                });
            }
            res.write(str);
            res.end();
        });
    });
};

