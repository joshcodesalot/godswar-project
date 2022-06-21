/*
Navicat MySQL Data Transfer

Source Server         : 127.0.0.1
Source Server Version : 50136
Source Host           : localhost:3306
Source Database       : accounts

Target Server Type    : MYSQL
Target Server Version : 50136
File Encoding         : 65001

Date: 2009-10-20 19:22:10
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `accounts_ban`
-- ----------------------------
DROP TABLE IF EXISTS `accounts_ban`;
CREATE TABLE `accounts_ban` (
  `id` int(4) unsigned zerofill NOT NULL COMMENT 'ID',
  `bandate` datetime NOT NULL COMMENT 'BANDATE',
  `unbandate` datetime NOT NULL COMMENT 'UNBANDATE',
  `bannedby` varchar(50) COLLATE utf8_unicode_ci NOT NULL COMMENT 'GM',
  `banreason` varchar(255) COLLATE utf8_unicode_ci NOT NULL COMMENT 'BANREASON',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of accounts_ban
-- ----------------------------

-- ----------------------------
-- Table structure for `accounts_info`
-- ----------------------------
DROP TABLE IF EXISTS `accounts_info`;
CREATE TABLE `accounts_info` (
  `id` int(4) unsigned NOT NULL AUTO_INCREMENT COMMENT 'ID',
  `accounts` varchar(32) COLLATE utf8_unicode_ci NOT NULL COMMENT 'ACCOUNTS',
  `password` char(32) COLLATE utf8_unicode_ci NOT NULL COMMENT 'PASSWORD',
  `login_status` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT 'LOGINSTATUS',
  `last_login_time` datetime NOT NULL COMMENT 'LASTLOGIN',
  `last_logout_time` datetime NOT NULL COMMENT 'LASTLOGOUT',
  `last_login_IP` char(20) CHARACTER SET gb2312 NOT NULL COMMENT 'LASTLOGINIP',
  `last_login_mac` varchar(50) CHARACTER SET gb2312 NOT NULL COMMENT 'LASTLOGINMAC',
  `total_online_time` bigint(8) unsigned NOT NULL COMMENT 'TOTALONLINETIME',
  `accounts_status` tinyint(1) unsigned NOT NULL COMMENT 'ACCOUNTSTATUS',
  PRIMARY KEY (`id`),
  UNIQUE KEY `index` (`accounts`)
) ENGINE=InnoDB AUTO_INCREMENT=132 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of accounts_info
-- ----------------------------
INSERT INTO `accounts_info` VALUES ('125', 'updays', 'updays', '0', '2009-10-19 22:42:16', '2009-10-19 22:42:21', '127.0.0.1', 'updays', '44', '11');
INSERT INTO `accounts_info` VALUES ('126', '1110', '1110', '1', '2007-07-31 11:23:15', '2007-07-31 11:23:19', '192.168.1.101', '23565', '2545', '1');
INSERT INTO `accounts_info` VALUES ('127', '1111', '1111', '1', '2007-07-31 11:23:15', '2007-07-31 11:23:19', '192.168.1.101', '23565', '2545', '1');
INSERT INTO `accounts_info` VALUES ('128', '1112', '1112', '1', '2007-07-31 11:23:15', '2007-07-31 11:23:19', '192.168.1.101', '23565', '2545', '1');
INSERT INTO `accounts_info` VALUES ('129', '1113', '1113', '1', '2007-07-31 11:23:15', '2007-07-31 11:23:19', '192.168.1.101', '23565', '2545', '1');
INSERT INTO `accounts_info` VALUES ('130', '1114', '1114', '1', '2007-07-31 11:23:15', '2007-07-31 11:23:19', '192.168.1.101', '23565', '2545', '1');
INSERT INTO `accounts_info` VALUES ('131', '1115', '1115', '1', '2007-07-31 11:23:15', '2007-07-31 11:23:19', '192.168.1.101', '23565', '2545', '1');

-- ----------------------------
-- Table structure for `accounts_register`
-- ----------------------------
DROP TABLE IF EXISTS `accounts_register`;
CREATE TABLE `accounts_register` (
  `AccID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `PassName` varchar(32) COLLATE utf8_unicode_ci DEFAULT NULL,
  `PassPwd` varchar(32) COLLATE utf8_unicode_ci DEFAULT NULL,
  `IdentityID` varchar(255) CHARACTER SET gb2312 DEFAULT NULL,
  `RealName` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `Email` varchar(255) CHARACTER SET gb2312 DEFAULT NULL,
  `Question` varchar(255) CHARACTER SET gb2312 DEFAULT NULL,
  `Answer` varchar(255) CHARACTER SET gb2312 DEFAULT NULL,
  `RegeditTime` datetime DEFAULT NULL,
  `AreaID` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `ActiveNumber` varchar(50) COLLATE utf8_unicode_ci DEFAULT NULL,
  `IsActive` tinyint(1) DEFAULT NULL,
  `IsDel` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`AccID`)
) ENGINE=InnoDB AUTO_INCREMENT=126 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of accounts_register
-- ----------------------------
INSERT INTO `accounts_register` VALUES ('125', 'updays', 'updays', '1', 'updays', 'updays', 'updays', 'updays', '2009-10-19 22:43:16', '222', '12', null, null);

-- ----------------------------
-- Table structure for `gamearea_info`
-- ----------------------------
DROP TABLE IF EXISTS `gamearea_info`;
CREATE TABLE `gamearea_info` (
  `GID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `GameArea` int(11) unsigned NOT NULL,
  `AccID` int(11) unsigned NOT NULL,
  `PassName` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `PassPwd` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `Balance` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL,
  `IsDel` tinyint(1) NOT NULL DEFAULT '0',
  `IsDraw` tinyint(1) DEFAULT '0' COMMENT 'ISDRAW',
  PRIMARY KEY (`GID`),
  UNIQUE KEY `index` (`PassName`)
) ENGINE=InnoDB AUTO_INCREMENT=8233 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of gamearea_info
-- ----------------------------
INSERT INTO `gamearea_info` VALUES ('8226', '1', '125', 'updays', 'updays', null, '0', '0');
INSERT INTO `gamearea_info` VALUES ('8227', '1', '0', '1110', '1110', null, '0', '0');
INSERT INTO `gamearea_info` VALUES ('8228', '1', '0', '1111', '1111', null, '0', '0');
INSERT INTO `gamearea_info` VALUES ('8229', '1', '0', '1112', '1112', null, '0', '0');
INSERT INTO `gamearea_info` VALUES ('8230', '1', '0', '1113', '1113', null, '0', '0');
INSERT INTO `gamearea_info` VALUES ('8231', '1', '0', '1114', '1114', null, '0', '0');
INSERT INTO `gamearea_info` VALUES ('8232', '1', '0', '1115', '1115', null, '0', '0');

-- ----------------------------
-- Table structure for `ip_ban`
-- ----------------------------
DROP TABLE IF EXISTS `ip_ban`;
CREATE TABLE `ip_ban` (
  `ip` varchar(32) COLLATE utf8_unicode_ci NOT NULL COMMENT 'IP',
  `bandate` datetime NOT NULL COMMENT 'BANDATE',
  `unbandate` datetime NOT NULL COMMENT 'UNBANDATE',
  `bannedby` varchar(50) COLLATE utf8_unicode_ci NOT NULL COMMENT 'GM',
  `banreason` varchar(255) COLLATE utf8_unicode_ci NOT NULL COMMENT 'BANREASON',
  PRIMARY KEY (`ip`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of ip_ban
-- ----------------------------

-- ----------------------------
-- View structure for `view_web_select_lv`
-- ----------------------------
DROP VIEW IF EXISTS `view_web_select_lv`;
CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `view_web_select_lv` AS select distinct `accounts`.`accounts_info`.`accounts` AS `accounts`,`godswar`.`character_base`.`name` AS `RoleName`,`godswar`.`character_base`.`fighter_job_lv` AS `F_LV`,`godswar`.`character_base`.`scholar_job_lv` AS `S_LV` from (`accounts`.`accounts_info` join `godswar`.`character_base`) where ((`accounts`.`accounts_info`.`accounts` = `godswar`.`character_base`.`accounts`) and (_utf8'' = _utf8''));

-- ----------------------------
-- Procedure structure for `SP_CREATE_ACCOUNTS`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_CREATE_ACCOUNTS`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_CREATE_ACCOUNTS`(IN Str CHAR(32), IN TheStrat INT, IN TheEnd INT)
BEGIN
     DECLARE  Num    int  DEFAULT 0;
     DECLARE acc CHAR(32) default "";
    SET  Num=TheStrat;
myloop: LOOP
      IF  Num>TheEnd  THEN
           LEAVE myloop;
      END IF;
     SET  acc = CAST( Num AS CHAR(32) );
     SET  acc=CONCAT(Str, acc );
      INSERT INTO accounts_info VALUES (NULL, acc, acc, '1', '2007-07-31 11:23:15', '2007-07-31 11:23:19', '192.168.1.101', '23565', '2545', '1');
      INSERT INTO gamearea_info  VALUES (NULL, 1, 0, acc, acc, null, 0,0);
      SET Num = Num + 1;
END  LOOP  myloop;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_SELECT_ACCOUNTS_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_SELECT_ACCOUNTS_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_SELECT_ACCOUNTS_INFO`(IN  Name CHAR(32) , IN cPassword  CHAR(32),IN cIP  CHAR(32))
BEGIN
     DECLARE  num  TINYINT DEFAULT 0;
     DECLARE  ban_account  TINYINT DEFAULT 0;
     DECLARE  ban_ip  TINYINT DEFAULT 0;

     SELECT count(*)
     FROM    gamearea_info
     WHERE   PassName = Name  AND  PassPwd = cPassword
     INTO     num;

     IF num = 1 THEN
          SELECT COUNT(*)
          FROM   accounts_ban
          WHERE  id = ( SELECT GID  FROM  gamearea_info WHERE PassName = Name  ) AND NOW()<unbandate
          INTO  ban_account;

          SELECT COUNT(*)
          FROM   ip_ban
          WHERE ip = cIP AND NOW()<unbandate
          INTO  ban_ip;
     END IF;
   -- SELECT 1,0,0;
     SELECT num,ban_account,ban_ip;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_WEB_SELECT_LV`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_WEB_SELECT_LV`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_WEB_SELECT_LV`(IN F_LV INT, IN S_LV INT)
BEGIN
    SELECT accounts,name,fighter_job_lv,scholar_job_lv
    FROM   godswar.character_base
    WHERE fighter_job_lv >= F_LV
    AND      scholar_job_lv >= S_LV
    ORDER BY accounts;
END;;
DELIMITER ;
