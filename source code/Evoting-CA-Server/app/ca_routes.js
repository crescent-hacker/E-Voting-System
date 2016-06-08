var fs = require('fs');
var path = require('path');
var dao = require('./dao');
var common = require('../config/common_var');
var logger = require('../server').logger('ca-server');
var shell = require('shelljs');

module.exports = function (app) {
    app.get('/', function (req, res) {
        res.render('index.ejs'); // load the index.ejs file
    });
    app.get('/result', function (req, res) {
        res.render('result.ejs'); // load the index.ejs file
    });
    app.get('/download_cert', function (req, res) {
        res.render('download_cert.ejs', {message: req.flash('downloadMessage')});
    });
    app.get('/download', function (req, res) {
        var user = req.session.user;
        if (!user) {
            req.flash('downloadMessage', 'Permission denied.');
            res.redirect('/download_cert');
            return;
        }
        if (user.has_cert == 1) {
            req.flash('downloadMessage', 'Cert has been downloaded before.');
            res.redirect('/download_cert');
            return;
        }
        var file_name = path.resolve('./') + '/certs/gen/' + req.session.user.user_name + '.crt';
        logger.debug("download filename: " + file_name);
        fs.exists(file_name, function (result) {
            if (result) {
                dao.update_has_cert(req, function (result) {
                    res.download(file_name);
                    var file_name_key = path.resolve('./') + '/certs/gen/' + req.session.user.user_name + '.key';
                    res.download(file_name_key);
                });
            } else {
                req.flash('downloadMessage', 'File not ready yet.');
                res.redirect('/download_cert');
            }
        });
    });

    app.get('/apply_cert', function (req, res) {
        console.log(req.url);
        res.render('apply_cert.ejs', {message: req.flash('applyMessage')});
    });
    app.post('/apply_cert', function (req, res) {
        dao.verify_user(req, function (result) {
            if (result) {
                var name = req.session.user.user_name;
                var abs = path.resolve('./')
                logger.debug("User " + req.session.user.user_name + " is applying cert.")
                //TODO,如果证书在,不执行命令生成,并提示已经有之前下载过了.如果不在,执行命令生成
                var cmd = 'openssl req -nodes -newkey rsa:2048 -sha256 -keyout ' + abs + '/certs/gen/' + name + '.key -out ' + abs + '/certs/gen/' + name + '.csr -subj "/C=AU/ST=NSW/L=WOLLONGONG/O=UOW/OU=CS/CN=' + name + '"';
                shell.exec(cmd, {silent: false, async: false});
                var cmd_1 = 'openssl ca -batch -in ' + abs + '/certs/gen/' + name + '.csr -out ' + abs + '/certs/gen/' + name + '.crt -days 3650 -config  /etc/pki/tls/openssl.cnf -passin pass:XXXXXX';
                shell.exec(cmd_1, {silent: false, async: true});
                res.redirect('/result');
            } else {
                res.redirect('/apply_cert');
            }
        })
    });
};

