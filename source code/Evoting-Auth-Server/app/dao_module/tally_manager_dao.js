var mysql = require('mysql');
var tool = require('./../tool');
var dbconfig = require('../../config/database');
var common = require('../../config/common_var');
var connection = mysql.createConnection(dbconfig.connection);
connection.query('USE ' + dbconfig.database);
var logger = require('../../server').logger('normal');

// expose this function to our app using module.exports
var dao = module.exports;

dao.system_init = function (sys_params,done) {
    var insertSql = "INSERT INTO sys_param (p, g, h, gn, q) values (?,?,?,?,?)";
    var insertObj = [
        sys_params.p,
        sys_params.g,
        sys_params.h,
        sys_params.gn,
        sys_params.q
    ];
    connection.query(insertSql,insertObj,function(err,result){
        if(err){
            logger.error(err);
            return done(false);
        }
        var sp_id = result.insertId;
        var insertShareSql = "INSERT INTO key_share (sp_id,tally_seq,secret) values ?";
        var insertShareObj = [];
        for(var i=0;i<sys_params.secrets.length;i++){
            var obj = [
                sp_id,
                i+1,
                sys_params.secrets[i]
            ];
            insertShareObj.push(obj);
        }
        connection.query(insertShareSql,[insertShareObj],function(err,rows){
            if(err){
                logger.error(err);
                return done(false);
            }
            var insertGenSql = "INSERT INTO generator(gen_seq,gen_value,sp_id) values ?";
            var insertGenObj = [];
            for(var i=0;i<sys_params.generator_list.length;i++){
                var obj = [
                    i+1,
                    sys_params.generator_list[i],
                    sp_id
                ];
                insertGenObj.push(obj);
            }
            connection.query(insertGenSql,[insertGenObj],function(err,rows){
                if(err){
                    logger.error(err);
                    return done(false);
                }
                return done(true);
            });
        });
    });
}

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
            q: rows[0].q,
            sp_id:rows[0].sp_id
        };
        var secretQuerySql = "SELECT * FROM key_share WHERE sp_id = ?";
        connection.query(secretQuerySql,[rows[0].sp_id],function(err,rows){
            if (err) {
                logger.error(err);
                return done(false,null);
            }
            sys_params.secret_shares = rows;
            return done(true,sys_params);
        });
    });
}

