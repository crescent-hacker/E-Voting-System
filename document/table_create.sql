# ************************************************************
# Sequel Pro SQL dump
# Version 4096
#
# http://www.sequelpro.com/
# http://code.google.com/p/sequel-pro/
#
# Host: 23.105.210.117 (MySQL 5.1.73)
# Database: csci969
# Generation Time: 2016-06-07 16:34:35 +0000
# ************************************************************


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;


# Dump of table candidate
# ------------------------------------------------------------

DROP TABLE IF EXISTS `candidate`;

CREATE TABLE `candidate` (
  `cand_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `vote_id` int(11) NOT NULL,
  `cand_seq` int(11) NOT NULL,
  `cand_generator` int(11) NOT NULL,
  `cand_content` varchar(150) NOT NULL DEFAULT '',
  `cand_result` int(11) NOT NULL DEFAULT '-1',
  PRIMARY KEY (`cand_id`),
  UNIQUE KEY `vote_id` (`vote_id`,`cand_seq`)
) ENGINE=MyISAM AUTO_INCREMENT=110 DEFAULT CHARSET=utf8;



# Dump of table generator
# ------------------------------------------------------------

DROP TABLE IF EXISTS `generator`;

CREATE TABLE `generator` (
  `gen_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `gen_seq` int(11) NOT NULL,
  `gen_value` int(11) NOT NULL,
  `sp_id` int(11) NOT NULL,
  PRIMARY KEY (`gen_id`)
) ENGINE=MyISAM AUTO_INCREMENT=52921 DEFAULT CHARSET=utf8;



# Dump of table key_share
# ------------------------------------------------------------

DROP TABLE IF EXISTS `key_share`;

CREATE TABLE `key_share` (
  `ks_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `sp_id` int(11) NOT NULL,
  `tally_seq` int(11) NOT NULL,
  `secret` int(11) DEFAULT NULL,
  PRIMARY KEY (`ks_id`)
) ENGINE=MyISAM AUTO_INCREMENT=91 DEFAULT CHARSET=utf8;



# Dump of table role
# ------------------------------------------------------------

DROP TABLE IF EXISTS `role`;

CREATE TABLE `role` (
  `role_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `role_name` varchar(20) NOT NULL DEFAULT '',
  PRIMARY KEY (`role_id`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;



# Dump of table sys_param
# ------------------------------------------------------------

DROP TABLE IF EXISTS `sys_param`;

CREATE TABLE `sys_param` (
  `sp_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `p` int(11) NOT NULL COMMENT 'prime order',
  `g` int(11) NOT NULL COMMENT 'generator of Zp',
  `h` int(11) NOT NULL COMMENT 'public key',
  `gn` int(11) NOT NULL COMMENT 'amount of generators',
  `q` int(11) DEFAULT NULL COMMENT 'q|p-1',
  PRIMARY KEY (`sp_id`)
) ENGINE=MyISAM AUTO_INCREMENT=16 DEFAULT CHARSET=utf8;



# Dump of table user
# ------------------------------------------------------------

DROP TABLE IF EXISTS `user`;

CREATE TABLE `user` (
  `user_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `user_name` varchar(100) NOT NULL DEFAULT '',
  `user_pwd` varchar(100) NOT NULL DEFAULT '',
  `email_addr` varchar(100) DEFAULT '',
  `last_change_time` timestamp NULL DEFAULT NULL,
  `max_pwd_age` int(11) DEFAULT NULL,
  `role_id` int(11) NOT NULL DEFAULT '2',
  `is_activated` int(11) NOT NULL DEFAULT '0',
  `is_pwd_modified` int(11) NOT NULL DEFAULT '0',
  `plain_pwd` varchar(100) DEFAULT NULL,
  `token` varchar(200) DEFAULT NULL,
  `cert_seq` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`user_id`),
  UNIQUE KEY `user_name` (`user_name`),
  UNIQUE KEY `email_addr` (`email_addr`)
) ENGINE=MyISAM AUTO_INCREMENT=49 DEFAULT CHARSET=utf8;



# Dump of table vote
# ------------------------------------------------------------

DROP TABLE IF EXISTS `vote`;

CREATE TABLE `vote` (
  `vote_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `vote_title` varchar(100) NOT NULL DEFAULT '',
  `vote_content` varchar(300) NOT NULL DEFAULT '',
  `is_published` int(2) NOT NULL DEFAULT '0',
  `deadline_date` varchar(30) NOT NULL DEFAULT '',
  `publish_date` varchar(30) NOT NULL DEFAULT '',
  `is_close` int(2) NOT NULL DEFAULT '0',
  `sp_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`vote_id`)
) ENGINE=MyISAM AUTO_INCREMENT=50 DEFAULT CHARSET=utf8;



# Dump of table voter
# ------------------------------------------------------------

DROP TABLE IF EXISTS `voter`;

CREATE TABLE `voter` (
  `vu_id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `vote_id` int(11) NOT NULL,
  `user_name` varchar(100) NOT NULL DEFAULT '',
  `challenge_field` varchar(11) NOT NULL DEFAULT '',
  `response_field` varchar(11) NOT NULL DEFAULT '',
  `proof_field` varchar(250) NOT NULL DEFAULT '',
  `final_ballot` varchar(100) NOT NULL DEFAULT '',
  PRIMARY KEY (`vu_id`)
) ENGINE=MyISAM AUTO_INCREMENT=72 DEFAULT CHARSET=utf8;




/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
