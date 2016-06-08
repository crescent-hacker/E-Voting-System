var mysql = require('mysql');
var tool = require('./../tool');
var common = require('../../config/common_var');
var dbconfig = require('../../config/database');
var connection = mysql.createConnection(dbconfig.connection);
connection.query('USE ' + dbconfig.database);
var logger = require('../../server').logger('normal');

// expose this function to our app using module.exports
var dao = module.exports;

dao.propose_vote = function (req, done) {
    connection.beginTransaction(function (err) {
        if (err) {
            logger.error(err);
            return done(false);
        }
        var source_data = req.body;

        //insert vote
        var insertVoteObj = [
            source_data.title,
            source_data.content,
            source_data.publish_date,
            source_data.deadline,
            global.sys_params.sp_id
        ];
        var insertVote = "INSERT INTO vote(vote_title,vote_content,publish_date,deadline_date,sp_id) values(?,?,?,?,?)";
        connection.query(insertVote, insertVoteObj, function (err, result) {
            if (err) {
                logger.error(err);
                return connection.rollback(function () {
                    done(false);
                });
            }
            var vote_id = result.insertId;

            //2.query Gi for candidates
            var queryGiSql = "SELECT gen_value FROM generator WHERE 1=0 ";
            var K = source_data.candidates.length;
            var Gi_seqs = [];
            for (var i = 0; i < K; i++) {
                Gi_seqs.push(tool.genRandomInt(1, GLOBAL.sys_params.gn));
                queryGiSql += " or gen_seq = ? ";
            }
            connection.query(queryGiSql, Gi_seqs, function (err, rows) {
                if (err) {
                    logger.error(err);
                    return connection.rollback(function () {
                        done(false);
                    });
                }

                //3.insert candidates
                var insertCandsSql = "INSERT INTO candidate(vote_id,cand_seq,cand_generator,cand_content) values ? ";
                var candsObj = [];
                for (var i = 0; i < K; i++) {
                    var obj = [
                        vote_id,
                        i + 1,
                        rows[i].gen_value,
                        source_data.candidates[i].candname
                    ];
                    candsObj.push(obj);
                }
                connection.query(insertCandsSql, [candsObj], function (err, rows) {
                    if (err) {
                        logger.error(err);
                        return connection.rollback(function () {
                            done(false);
                        });
                    }

                    //3.insert voters
                    var insertVotersSql = "INSERT INTO voter(vote_id,user_name) values ?";
                    var votersObj = [];
                    for (var i = 0; i < source_data.eligible_voters.length; i++) {
                        var obj = [
                            vote_id,
                            source_data.eligible_voters[i].username
                        ];
                        votersObj.push(obj);
                    }
                    connection.query(insertVotersSql, [votersObj], function (err, rows) {
                        if (err) {
                            logger.error(err);
                            return connection.rollback(function () {
                                done(false);
                            });
                        }
                        //commit transaction
                        connection.commit(function (err) {
                            if (err) {
                                logger.error(err);
                                return connection.rollback(function () {
                                    done(false);
                                });
                            }
                            return done(true,source_data.title);
                        });
                    });
                });
            });
        });
    });
}

dao.delete_vote = function (req, done) {
    var vote_id = req.body.vote_id;
    connection.beginTransaction(function (err) {
        var deleteVotersSql = "DELETE FROM voter WHERE vote_id = ?";
        var deleteCandsSql = "DELETE FROM candidate WHERE vote_id = ?";
        var deleteVoteSql = "DELETE FROM vote WHERE vote_id = ?";
        if (err) {
            logger.error(err);
            return connection.rollback(function () {
                done(false);
            });
        }
        connection.query(deleteVotersSql,[vote_id],function(err,rows){
            if (err) {
                logger.error(err);
                return connection.rollback(function () {
                    done(false);
                });
            }
            connection.query(deleteCandsSql,[vote_id],function(err,rows){
                if (err) {
                    logger.error(err);
                    return connection.rollback(function () {
                        done(false);
                    });
                }
                connection.query(deleteVoteSql,[vote_id],function(err,rows){
                    if (err) {
                        logger.error(err);
                        return connection.rollback(function () {
                            done(false);
                        });
                    }
                    if(rows.affectedRows<=0){
                        logger.error("Not that vote exist.");
                        return done(false);
                    }
                    //commit transaction
                    connection.commit(function (err) {
                        if (err) {
                            logger.error(err);
                            return connection.rollback(function () {
                                done(false);
                            });
                        }
                        return done(true,vote_id);
                    });
                });
            });
        });

    });
}

dao.get_vote_list_m = function(req,done){
    var queryVoteListSql = "SELECT vote_id,vote_title,is_published,is_close,deadline_date FROM vote";
    connection.query(queryVoteListSql,[],function(err,rows){
        if(err){
            logger.error(err);
            return done(false);
        }
        return done(true,rows);
    });
}

dao.get_vote_m = function(req,done){
    var vote_id = req.body.vote_id;
    var retObj = {};
    var queryVoteSql = "SELECT * FROM vote WHERE vote_id = ?";
    var queryCandsSql = "SELECT cand_seq,cand_generator,cand_content,cand_result FROM candidate WHERE vote_id = ?";
    var queryVoterSql = "SELECT user_name,challenge_field,response_field,proof_field,final_ballot FROM voter WHERE vote_id = ?";

    connection.query(queryVoteSql,[vote_id],function (err, rows) {
        if(err){
            logger.error(err);
            return done(false);
        }
        if(rows.length<=0){
            logger.error("Vote "+vote_id+" is not exist.");
            return done(false);
        }
        retObj = rows[0];
        connection.query(queryCandsSql,[vote_id],function(err,rows){
            if(err){
                logger.error(err);
                return done(false);
            }
            retObj.candidates = [];
            for(var i=0;i<rows.length;i++){
                retObj.candidates.push(rows[i]);
            }
            connection.query(queryVoterSql,[vote_id],function(err,rows){
                if(err){
                    logger.error(err);
                    return done(false);
                }
                retObj.eligible_voters = [];
                for(var i=0;i<rows.length;i++){
                    retObj.eligible_voters.push(rows[i]);
                }
                return done(true,retObj);
            })
        });
    });
}

dao.publish_vote = function(req,done){
    var updateSql = "UPDATE vote SET is_published = 1 WHERE is_published = 0 and vote_id = ?";
    connection.query(updateSql,[req.body.vote_id],function(err,rows){
        if(err){
            logger.error(err);
            return done(false);
        }
        if(rows.changedRows<=0){
            logger.error("The vote has been published before.");
            return done(false);
        }
        return done(true);
    });
}