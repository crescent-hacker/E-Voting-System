// app/routes.js
var moment = require('moment');
var jwt = require('jwt-simple');
var dao = require('../dao_module/voting_manager_dao');
var common = require('../../config/common_var');
var logger = require('../../server').logger('normal');
var routes = require('./routes');
var sender = require('../req_sender');

module.exports = function (app) {
    //new a vote
    app.post('/propose_vote', routes.isAdminLoggedIn, function (req, res) {
        dao.propose_vote(req, function (result, title) {
            if (result) {
                logger.debug("propose_vote " + title + " successfully");
                //tell main tally server to read job
                var send_data = JSON.stringify({});
                var options = sender.createOptions(sender.options_template, "GET", "tally_" + global.sys_params.main_seq, global.sys_params.main_tally_port, "/read_job_schedule", send_data, null);
                sender.send(options,send_data,function(send_result,data){
                    if(send_result) {
                        res.write(JSON.stringify({
                            success: result,
                            notify_success: true,
                            read_success:data.success
                        }));
                    }else{
                        res.write(JSON.stringify({
                            success: result,
                            notify_success: false
                        }));
                    }
                    res.end();
                });
            }
        });
    });

    app.post('/delete_vote', routes.isAdminLoggedIn, function (req, res) {
        dao.delete_vote(req, function (result, vote_id) {
            if (result) {
                logger.debug("delete_vote " + vote_id + " successfully");
                //tell main tally server to read job
                var send_data = JSON.stringify({});
                var options = sender.createOptions(sender.options_template, "GET", "tally_" + global.sys_params.main_seq, global.sys_params.main_tally_port, "/read_job_schedule", send_data, null);
                sender.send(options,send_data,function(send_result,data){
                    if(send_result) {
                        res.write(JSON.stringify({
                            success: result,
                            notify_success: true,
                            read_success:data.success
                        }));
                    }else{
                        res.write(JSON.stringify({
                            success: result,
                            notify_success: false
                        }));
                    }
                    res.end();
                });
            }
        });
    });

    app.get('/get_vote_list_m', routes.isAdminLoggedIn, function (req, res) {
        dao.get_vote_list_m(req, function (result, vote_list) {
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

    app.get('/get_vote_m', routes.isAdminLoggedIn, function (req, res) {
        dao.get_vote_m(req, function (result, vote_detail) {
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

    app.post('/publish_vote', routes.isAdminLoggedIn, function (req, res) {
        dao.publish_vote(req, function (result) {
            var str;
            if (result) {
                logger.debug("publish_vote " + req.body.vote_id + " successfully");
                str = JSON.stringify({
                    success: result
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


};

