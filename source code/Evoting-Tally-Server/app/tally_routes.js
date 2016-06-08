var fs = require('fs');
var path = require('path');
var scheduler = require('node-schedule');
var dao = require('./dao');
var common = require('../config/common_var');
var tool = require('./tool');
var logger = require('../server').logger('tally-' + global.tally_seq);
var config = require('../config/tally_server_config');
var sender = require('./req_sender');
var tc = require('./protocol_module/tally_computation');

module.exports = function (app) {
    //init
    dao.load_sys_params(function (result, sys_params) {
        if (result) {
            // logger.debug("load_sys_params successful.");
            global.sys_params = sys_params;
            // logger.debug(sys_params);
        }
    });
    //routes define
    app.get("/compute_commitment", function (req, res) {
        logger.debug("receive commitment");

        var X = req.body.X;
        var si = global.tally_secret;
        var lamda = req.body.lamda;
        var tmp = (si * lamda) % global.sys_params.q;
        if (tmp < 0)
            tmp += global.sys_params.q;
        var commitment = tool.quick_mod(X, global.sys_params.q - tmp, global.sys_params.p);
        logger.debug("compute_commitment " + commitment + " success");
        res.write(JSON.stringify({
            w: commitment,
            seq: global.tally_seq
        }));
        res.end();
    });

    app.get("/read_job_schedule", isMain, function (req, res) {
        dao.readVoteTallyJob(function (result, voteList) {
            if (result) {
                global.job_schedule = [];
                for (var i = 0; i < voteList.length; i++) {
                    var date;
                    if (voteList[i].need_handle_now) {
                        date = new Date(Date.now() + 5000);
                    } else {
                        date = new Date(Date.parse(voteList[i].deadline_date));
                    }
                    var newJob = scheduler.scheduleJob(date, function (vote_id) {
                        var result = compute_tallying(vote_id);
                        if (result) {
                            logger.info("Finish a job on " + vote_id);
                        } else {
                            logger.error("Tally vote " + vote_id + " failed.");
                        }
                    }.bind(null, voteList[i].vote_id));
                    logger.debug("Create a tally job on vote " + voteList[i].vote_id + " at " + date.toLocaleDateString());
                    global.job_schedule.push({
                        vote_id: voteList[i].vote_id,
                        job: newJob
                    });
                }
            }

            res.write(JSON.stringify({
                success: result
            }));
            res.end();
        });
    });

    //tallying function
    function compute_tallying(vote_id) {
        var sp = global.sys_params;
        dao.get_final_ballots(vote_id, function (result, retObj) {
            if (result) {
                var times = Math.floor(retObj.final_ballots.length / 4);
                var res = retObj.final_ballots.length % 4;
                var XYs = tc.t_msg(times, res, retObj.final_ballots, sp.p);
                //init rset
                var rset = [];
                rset.length = times + 1;
                for (var i = 0; i < rset.length; i++) {
                    rset[i] = [];
                    rset[i].length = retObj.K;
                }
                //randomly choose tallying authorities
                var Nt = tc.NumT(config.tally_servers_info.length, config.threshold);
                //ptp
                var ptp = tc.PT(Nt, config.tally_servers_info.length);
                //lamda
                var lm = tc.lmta(Nt, sp.q, ptp);
                //compute all X
                for (var j = 0; j < XYs.length; j++) {
                    var Ws = [];
                    var count = 0;
                    //send x and related lamda to selected tally servers
                    for (var i = 0; i < ptp.length; i++) {
                        var selected_tally = config.tally_servers_info[ptp[i] - 1];
                        if (selected_tally.seq == global.tally_seq) {
                            //compute directly
                            var si = global.tally_secret;
                            var tmp = (si * lm[i]) % global.sys_params.q;
                            if (tmp < 0)
                                tmp += global.sys_params.q;
                            var commitment = tool.quick_mod(XYs[j].X, global.sys_params.q - tmp, global.sys_params.p);
                            Ws.push(commitment);
                            count++;
                        } else {
                            var send_json = {
                                X:XYs[j].X,
                                lamda:lm[i]
                            };
                            var send_data = JSON.stringify(send_json);
                            var options = sender.createOptions(sender.options_template, "GET", 'tally_' + selected_tally.seq, selected_tally.port, "/compute_commitment",send_data);
                            console.log(options);
                            sender.send(options,send_data);
                        }
                    }
                    //hold until all w return back
                    // console.log('count:'+count);
                    //compute tally of XYs[j].X
                    var accum = XYs[j].Y;
                    for(var i=0;i<Nt;i++){
                        accum*=Ws[i];
                    }
                    accum = accum%global.sys_params.p;
                    //brute force
                    var voters = (j==XYs.length-1)?res:4;
                    var k_arr = tc.tallying(XYs[j],accum,retObj.K,voters,Nt,global.sys_params.p,retObj.Gset);
                    rset[j] = k_arr;
                }
                //final tally
                var stat = [];
                stat.length = retObj.K;
                for(var i=0;i<rset.length;i++){
                    for(var j=0;j<retObj.K;j++){
                        stat[j]+=rset[i][j];
                    }
                }
            }
        });
    }

    function isMain(req, res, next) {
        if (global.is_main == false) {
            var reason = 'This tally server is not main tally server.';
            logger.error(reason);
            res.end(reason, 400);
        } else {
            next();
        }
    }
};

