var mysql = require('mysql');
var tool = require('./../tool');
var dbconfig = require('../../config/database');
var common = require('../../config/common_var');
var connection = mysql.createConnection(dbconfig.connection);
connection.query('USE ' + dbconfig.database);
var logger = require('../../server').logger('normal');

// expose this function to our app using module.exports
var dao = module.exports;

//modify password
dao.modify_pwd = function (req, done) {
    if (tool.isPwdSimple(req.body.password)) {
        var reason = 'modify_pwd: Password too simple.';
        req.flash('daoMessage', reason);
        return done(false);
    }
    connection.query("SELECT * FROM user WHERE role_id<>1 and user_name = ?", [req.body.username], function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false);
        }
        if (rows.length <= 0) {
            var reason = 'modify_pwd: User does not exist.';
            req.flash('daoMessage', reason);
            return done(false);
        } else {
            var user = rows[0];
            // update the user password
            var pt = req.body.username + req.body.password + common.common_salt;
            user.user_pwd = tool.hash(pt);
            var updateQuery = "UPDATE user SET user_pwd = ?,is_pwd_modified = 1,plain_pwd = '' where user_name = ?";

            connection.query(updateQuery, [user.user_pwd, user.user_name], function (err, rows) {
                if (err) {
                    logger.error(err);
                    return done(false);
                }
                return done(true, user);
            });
        }
    });
}

dao.get_vote_list = function (req, done) {
    var queryVoteListSql = "SELECT vote_id,vote_title,is_close,deadline_date FROM vote WHERE is_published = 1";
    connection.query(queryVoteListSql, [], function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false);
        }
        return done(true, rows);
    });
}

dao.get_vote = function (req, done) {
    var vote_id = req.body.vote_id;
    var retObj = {};
    var queryVoteSql = "SELECT * FROM vote WHERE vote_id = ? and is_published = 1";
    var queryCandsSql = "SELECT cand_seq,cand_generator,cand_content,cand_result FROM candidate WHERE vote_id = ?";
    var queryVoterSql = "SELECT user_name,challenge_field,response_field,proof_field,final_ballot FROM voter WHERE vote_id = ?";

    connection.query(queryVoteSql, [vote_id], function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false);
        }
        if (rows.length <= 0) {
            logger.error("Vote " + vote_id + " is not exist.");
            return done(false);
        }
        retObj = rows[0];
        connection.query(queryCandsSql, [vote_id], function (err, rows) {
            if (err) {
                logger.error(err);
                return done(false);
            }
            retObj.candidates = [];
            for (var i = 0; i < rows.length; i++) {
                retObj.candidates.push(rows[i]);
            }
            connection.query(queryVoterSql, [vote_id], function (err, rows) {
                if (err) {
                    logger.error(err);
                    return done(false);
                }
                retObj.eligible_voters = [];
                for (var i = 0; i < rows.length; i++) {
                    retObj.eligible_voters.push(rows[i]);
                }
                return done(true, retObj);
            })
        });
    });
}

dao.is_user_eligible = function (req, done) {
    var user_name = req.user.user_name;
    var vote_id = req.body.vote_id;
    var sql = "SELECT 1 FROM vote t1,voter t2 WHERE user_name = ? and t1.vote_id = ? and t1.vote_id = t2.vote_id and t1.is_close = 0 and t1.is_published = 1";
    connection.query(sql, [user_name, vote_id], function (err, rows) {
        if (err) {
            logger.error(err);
            return done(false);
        }
        if (rows.length <= 0) {
            logger.debug(rows);
            logger.error("User " + user_name + " is not eligible to vote " + vote_id);
            return done(false);
        }
        return done(true);
    });
}

dao.post_final_ballot = function (req, done) {
    var obj = [JSON.stringify(req.body.dr), JSON.stringify(req.body.final_ballot), req.body.vote_id, req.user.user_name];
    var querySql = "SELECT * FROM voter WHERE vote_id = ? and user_name = ? and final_ballot<>'' ";
    connection.query(querySql, [req.body.vote_id, req.user.user_name], function (err, rows) {

        if (err) {
            logger.error(err);
            return done(false);
        }
        if (rows.length > 0) {
            logger.error("cannot vote repeatly on vote " + req.body.vote_id);
            return done(false);
        }
        var updateSql = "UPDATE voter SET response_field = ? , final_ballot = ? WHERE vote_id = ? and user_name = ?";
        connection.query(updateSql, obj, function (err, rows) {
            if (err) {
                logger.error(err);
                return done(false);
            }
            if (rows.affectedRows <= 0) {
                logger.error("No such vote.");
                return done(false);
            }
            return done(true);
        });
    });
}