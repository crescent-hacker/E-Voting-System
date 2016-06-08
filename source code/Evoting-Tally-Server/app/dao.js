var mysql = require('mysql');
var tool = require('./tool');
var common = require('../config/common_var');
var dbconfig = require('../config/database');
var connection = mysql.createConnection(dbconfig.connection);
connection.query('USE ' + dbconfig.database);
var logger = require('../server').logger('tally-' + global.tally_seq);

// expose this function to our app using module.exports
var dao = module.exports;

dao.load_sys_params = function (done) {
    var querySql = "SELECT * from sys_param order by sp_id DESC limit 1";
    connection.query(querySql, [], function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false, null);
        }
        if (rows.length <= 0) {
            logger.error("System has not been initialized yet.");
            return done(false, null);
        }
        var sys_params = {
            p: rows[0].p,
            g: rows[0].g,
            h: rows[0].h,
            gn: rows[0].gn,
            q: rows[0].q
        };
        return done(true, sys_params);
    });
}

dao.readVoteTallyJob = function (done) {
    var querySql = "SELECT vote_id,deadline_date FROM vote WHERE is_published = 1 and is_close = 0";
    connection.query(querySql, [], function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false);
        }
        var closedVotes = [];
        for (var i = 0; i < rows.length; i++) {
            var deadline_date = Date.parse(rows[i].deadline_date);
            if (deadline_date <= Date.now()) {
                closedVotes.push(rows[i].vote_id);
                rows[i].need_handle_now = true;
            } else {
                rows[i].need_handle_now = false;
            }
        }
        var updateSql = "UPDATE vote SET is_close = 1 WHERE 1=0 ";
        for (var i = 0; i < closedVotes.length; i++) {
            updateSql += " or vote_id = ? ";
        }
        connection.query(updateSql, closedVotes, function (err) {
            if (err) {
                logger.error(err);
                return done(false);
            }
            return done(true, rows);
        });
    });
}

dao.get_final_ballots = function (vote_id, done) {
    var sql = "SELECT final_ballot FROM voter WHERE vote_id = ? and final_ballot <> '' ";
    connection.query(sql, [vote_id], function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false);
        }
        if (rows.length<=0) {
            logger.error("No final ballot on vote "+vote_id+" at all.");
            return done(false);
        }
        var retObj = {};
        var final_ballots = [];
        for (var i = 0; i < rows.length; i++) {
            logger.info(rows[0].final_ballot);
            final_ballots.push(JSON.parse(rows[0].final_ballot));
        }
        retObj.final_ballots = final_ballots;
        var sql_1 = "SELECT cand_generator FROM candidate WHERE vote_id = ?";
        connection.query(sql_1,[vote_id],function(err,rows_1){
            if (err) {
                logger.error(err);
                return done(false);
            }
            retObj.K = rows_1.length;
            retObj.Gset = rows;
            return done(true,retObj);
        });

    });
}

