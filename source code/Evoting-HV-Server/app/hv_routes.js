var fs = require('fs');
var path = require('path');
var dao = require('./dao');
var common = require('../config/common_var');
var tool = require('./tool');
var logger = require('../server').logger('hv');
var hi = require('./protocol_module/hv_interaction')

module.exports = function (app) {
    dao.load_sys_params(function (result, sys_params) {
        if (result) {
            logger.debug("load_sys_params successful.");
            global.sys_params = sys_params;
            logger.debug(sys_params);
        }
    });
    
    app.get('/pv1_step_3', function (req, res) {
        var sp = global.sys_params;
        req.session.commit_map = {};
        req.session.voter_map = {};
        console.log(req.body);
        req.session.voter_map.user_name = req.body.user_name;
        req.session.voter_map.vote_id = req.body.vote_id;
        var c1 = hi.PV1_2(sp.q);
        var xy = req.body.xy;
        req.session.commit_map.c1 = c1;
        req.session.voter_map.xy = xy;
        var retJson = {
            c1:c1,
            success:true
        }
        response(res,retJson);
    });

    app.get('/pv1_step_5', function (req, res) {
        if(!req.session.voter_map||!req.session.commit_map){
            logger.error("Did not run pv1_step_3");
            response(res,{success:false});
            return;
        }
        var sp = global.sys_params;
        var Gset = req.body.Gset;
        req.session.voter_map.K = Gset.length;
        var DR = req.body.DR;
        req.session.voter_map.DR = DR;
        var fballot = req.session.voter_map.xy;
        var c1 = req.session.commit_map.c1;
        var AB = req.body.AB;
        req.session.voter_map.AB = AB;

        //-----PV1-----
        var PV1 = hi.PV1_4(sp.p,sp.g,sp.h,Gset,Gset.length,DR,fballot,AB,sp.q,c1);
        if(!PV1){
            response(res,{success:false});
            return;
        }
        //-----cal (u,v) and (a,b)-----
        var bta = hi.PK_1_bta(sp.q);
        req.session.voter_map.bta = bta;
        var w2 = hi.PK_1_w2(sp.q);
        req.session.voter_map.w2 = w2;
        var uv = hi.PK_1_uv(sp.p,sp.g,sp.h,bta);
        req.session.voter_map.uv = uv;
        var ab = hi.PK_1_ab(sp.p,sp.g,sp.h,w2);

        var retJson = {
            success:true,
            uv:uv,
            ab:ab
        };
        response(res,retJson);
    });

    app.get('/pk_step_4', function (req, res) {
        if(!req.session.voter_map||!req.session.commit_map){
            logger.error("Did not run pv1_step_3");
            response(res,{success:false});
            return;
        }
        var sp = global.sys_params;
        var c2 = req.body.c2;
        var w2 = req.session.voter_map.w2;
        var bta = req.session.voter_map.bta;
        var r = hi.PK_3_r(sp.p,c2,w2,bta);

        var retJson = {
            success:true,
            r:r
        };
        response(res,retJson);
    });

    app.get('/pv2_step_1', function (req, res) {
        if(!req.session.voter_map||!req.session.commit_map){
            logger.error("Did not run step 3");
            response(res,{success:false});
            return;
        }
        var sp = global.sys_params;
        var map = req.session.voter_map;
        var ST = hi.PV2_1(sp.p,map.K,map.uv,map.AB,map.DR);
        var retJson = {
            success:true,
            ST:ST
        };
        response(res,retJson);
        dao.post_st_c1(req,ST,req.session.commit_map.c1,function(result){
            if(result){
                logger.info(map.user_name+" post st on vote "+map.vote_id+" successful.")
            }else{
                logger.info(map.user_name+" post st on vote "+map.vote_id+" fail.")
            }
        });
    });

    function response(res,json){
        console.log(json);
        res.write(JSON.stringify(json));
        res.end();
    }
};

