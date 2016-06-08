var email = require("emailjs/email");
var logger = require('../server').logger('routes');

var server = email.server.connect({
    user: "csci969.2016@gmail.com",
    password: "csci969google",
    host: "smtp.gmail.com",
    ssl: true
    // port        : 993
});

var mail_sender = module.exports;

// send the message and get a callback with an error or details of the message that was sent
mail_sender.send = function (account,pwd,email) {
    server.send({
        text: "Your account name is:   "+account+"\nYour temporary password:   "+pwd+"\nPlease modify your password ASAP.",
        from: "Administrator <difan.chen@gmail.com>",
        to: account+" <"+email+">",
        // cc:      "else <else@your-email.com>",
        subject: "Your account has been activated."
    }, function (err, message) {
        if(err){
            logger.error(err);
        }else {
            logger.info(message);
        }
    });
}

mail_sender.send_pwd_modify_info = function (account,email) {
    server.send({
        text: "The password of your account "+account+" has changed.\nIf you did not perform this action, please contact the administrator.",
        from: "Administrator <difan.chen@gmail.com>",
        to: account+" <"+email+">",
        // cc:      "else <else@your-email.com>",
        subject: "Your password has changed."
    }, function (err, message) {
        if(err){
            logger.error(err);
        }else {
            logger.info(message);
        }
    });
}

