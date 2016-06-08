// app/routes.js
var moment = require('moment');
var jwt = require('jwt-simple');
var dao = require('../dao_module/tally_manager_dao');
var tallyconfig = require('../../config/tally_server_config');
var tool = require('./../tool');
var shell = require("shelljs");
var common = require('../../config/common_var');
var logger = require('../../server').logger('normal');
var routes = require('./routes');
var setup = require('../protocol_module/system_setup');
var sender = require('../req_sender');

module.exports = function (app) {
    dao.load_sys_params(function (result, sys_params) {
        if (result) {
            logger.debug("load_sys_params successful.");
            global.sys_params = sys_params;
            logger.debug(sys_params);
        }
    });
    app.get("/has_system_init", routes.isLoggedIn, function (req, res) {
        //read the sys params into global
        var str;
        if (global.sys_params) {
            var params = {
                p: global.sys_params.p,
                g: global.sys_params.g,
                h: global.sys_params.h,
                gn: global.sys_params.gn,
                q: global.sys_params.q
            }
            logger.debug("has_system_init successful.");
            str = JSON.stringify({
                success: true,
                sys_params: params
            });
        } else {
            str = JSON.stringify({
                success: false
            });
        }
        res.write(str);
        res.end();
    });

    app.get("/system_init", routes.isAdminLoggedIn, function (req, res) {
        var setupObj = setup.genSysParam(tallyconfig.Q, tallyconfig.P, tallyconfig.threshold, tallyconfig.tally_servers_info.length);
        var str;//return msg
        if (setupObj.success) {
            dao.system_init(setupObj, function (result) {
                if (result) {
                    var params = {
                        p: setupObj.p,
                        g: setupObj.g,
                        h: setupObj.h,
                        gn: setupObj.gn,
                        q: setupObj.q
                    }
                    logger.debug("system_init successful.")
                    str = JSON.stringify({
                        success: true,
                        sys_params: params
                    });
                } else {
                    str = JSON.stringify({
                        success: false
                    });
                }
                res.write(str);
                res.end();
            });

            dao.load_sys_params(function (result, sys_params) {
                if (result) {
                    logger.debug("load_sys_params successful.");
                    global.sys_params = sys_params;
                    logger.debug(sys_params);
                }
            });
        } else {
            logger.error("Init system params fail, inproper Q and P.")
            str = JSON.stringify({
                success: false
            });
            res.write(str);
            res.end();
        }
    });

    //launch tally server from config file
    app.get("/launch_tally_servers", routes.isAdminLoggedIn, function (req, res) {
        if (!global.sys_params.main_seq) {
            var tally_amount = tallyconfig.tally_servers_info.length;
            var main_seq = tool.genRandomInt(1, tally_amount);
            global.sys_params.main_seq = main_seq;
            for (var i = 0; i < tally_amount; i++) {
                var cmd = "node "
                    + tallyconfig.tally_server_path
                    + "server.js "
                    + tallyconfig.tally_servers_info[i].port
                    + " " + tallyconfig.tally_servers_info[i].seq + " "
                    + sys_params.secret_shares[i].secret + " ";
                if (tallyconfig.tally_servers_info[i].seq == main_seq) {
                    cmd += "main";
                    global.sys_params.main_tally_port = tallyconfig.tally_servers_info[i].port;
                    logger.debug("main tally server is tally_" + main_seq);
                }
                shell.exec(cmd, {silent: false, async: true});
            }
            res.write(JSON.stringify({
                success: true,
                main_seq: main_seq
            }));

            setTimeout(function(){var send_data = JSON.stringify({});
                var options = sender.createOptions(sender.options_template, "GET", "tally_" + global.sys_params.main_seq, global.sys_params.main_tally_port, "/read_job_schedule", send_data, null);
                sender.send(options, send_data, function (send_result) {
                    if (send_result) {
                        logger.debug("Notify main tally read jobs successfully.")
                    } else {
                        logger.debug("Notify main tally read jobs fail.")
                    }
                });},5000);

        } else {
            res.write(JSON.stringify({
                success: false,
                reason: "Tally servers have been launched."
            }));
        }
        res.end();
    });

    

   
};

