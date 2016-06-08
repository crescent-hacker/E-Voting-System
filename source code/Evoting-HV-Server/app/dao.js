var mysql = require('mysql');
var tool = require('./tool');
var common = require('../config/common_var');
var dbconfig = require('../config/database');
var connection = mysql.createConnection(dbconfig.connection);
connection.query('USE ' + dbconfig.database);
var logger = require('../server').logger('hv');

// expose this function to our app using module.exports
var dao = module.exports;

dao.load_sys_params = function (done) {
    var querySql = "SELECT * from sys_param order by sp_id DESC limit 1";
    connection.query(querySql, [], function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false,null);
        }
        if(rows.length<=0){
            logger.error("System has not been initialized yet.");
            return done(false,null);
        }
        var sys_params = {
            p: rows[0].p,
            g: rows[0].g,
            h: rows[0].h,
            gn:rows[0].gn,
            q: rows[0].q
        };
        return done(true,sys_params);
    });
}

dao.post_st_c1 = function(req,ST,c1,done){
    var obj = [JSON.stringify(ST),c1,req.session.voter_map.vote_id,req.session.voter_map.user_name];
    var updateSql = "UPDATE voter SET proof_field = ?,challenge_field = ? WHERE vote_id = ? and user_name = ?";
    connection.query(updateSql,obj,function(err,rows) {
        if(err){
            logger.error(err);
            return done(false);
        }
        if(rows.affectedRows<=0){
            logger.error("not such vote.");
            return done(false);
        }
        return done(true);
    });
}