/*
Navicat MySQL Data Transfer

Source Server         : 127.0.0.1
Source Server Version : 50136
Source Host           : localhost:3306
Source Database       : godswar

Target Server Type    : MYSQL
Target Server Version : 50136
File Encoding         : 65001

Date: 2009-10-20 19:22:23
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `cache`
-- ----------------------------
DROP TABLE IF EXISTS `cache`;
CREATE TABLE `cache` (
  `id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '缓冲数据表',
  PRIMARY KEY (`id`),
  KEY `index` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of cache
-- ----------------------------

-- ----------------------------
-- Table structure for `character_avatar`
-- ----------------------------
DROP TABLE IF EXISTS `character_avatar`;
CREATE TABLE `character_avatar` (
  `id` int(4) NOT NULL,
  `hair_style` tinyint(1) NOT NULL COMMENT '发型',
  `hair_color` tinyint(1) NOT NULL COMMENT '发色',
  `eye_color` tinyint(1) NOT NULL COMMENT '瞳色',
  PRIMARY KEY (`id`),
  KEY `index` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of character_avatar
-- ----------------------------

-- ----------------------------
-- Table structure for `character_base`
-- ----------------------------
DROP TABLE IF EXISTS `character_base`;
CREATE TABLE `character_base` (
  `id` int(4) unsigned NOT NULL AUTO_INCREMENT COMMENT '色角ID',
  `accounts` char(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' COMMENT '家玩帐号ID',
  `name` char(32) COLLATE utf8_unicode_ci NOT NULL COMMENT '角色名称',
  `gender` enum('female','male') COLLATE utf8_unicode_ci NOT NULL DEFAULT 'male' COMMENT '色角性别',
  `GM` tinyint(1) unsigned NOT NULL DEFAULT '0' COMMENT 'GM标志',
  `camp` tinyint(1) unsigned NOT NULL COMMENT '国籍',
  `profession` tinyint(1) unsigned NOT NULL COMMENT '职业',
  `fighter_job_lv` tinyint(1) unsigned NOT NULL COMMENT '战士职业等级',
  `scholar_job_lv` tinyint(1) unsigned NOT NULL COMMENT '学者职业等级',
  `fighter_job_exp` int(4) unsigned NOT NULL COMMENT '战士职业经验',
  `scholar_job_exp` int(4) unsigned NOT NULL COMMENT '学者职业经验',
  `curHP` int(2) unsigned NOT NULL DEFAULT '150',
  `curMP` int(2) unsigned NOT NULL,
  `status` tinyint(1) NOT NULL,
  `belief` tinyint(1) unsigned NOT NULL COMMENT '当前仰信',
  `prestige` smallint(2) unsigned NOT NULL COMMENT '声望',
  `earl_rank` tinyint(1) unsigned NOT NULL COMMENT '爵位',
  `consortia` smallint(2) unsigned zerofill NOT NULL DEFAULT '00' COMMENT '公会',
  `consortia_job` tinyint(1) unsigned zerofill NOT NULL DEFAULT '0' COMMENT '6会长5副会长4理事3精英2会员1见习会员0没有',
  `consortia_contribute` int(4) unsigned NOT NULL COMMENT '公会贡献',
  `store_num` int(2) unsigned NOT NULL DEFAULT '10' COMMENT '仓库个数',
  `bag_num` int(2) unsigned NOT NULL DEFAULT '1' COMMENT '包裹个数',
  `hair_style` tinyint(4) unsigned NOT NULL COMMENT '发型 ',
  `face_shap` tinyint(4) unsigned DEFAULT NULL,
  `Map` tinyint(1) unsigned NOT NULL DEFAULT '1' COMMENT '当前地图',
  `Pos_X` float(10,5) NOT NULL DEFAULT '0.00000' COMMENT 'X坐标',
  `Pos_Z` float(10,5) NOT NULL DEFAULT '0.00000' COMMENT 'Z坐标',
  `Money` int(4) unsigned NOT NULL DEFAULT '0' COMMENT '金钱',
  `Stone` int(4) unsigned NOT NULL DEFAULT '0' COMMENT '宝石',
  `SkillPoint` int(4) unsigned zerofill NOT NULL DEFAULT '0000' COMMENT '技能点数',
  `SkillExp` int(4) unsigned zerofill NOT NULL DEFAULT '0000' COMMENT '技能经验',
  `MaxHP` int(4) unsigned NOT NULL DEFAULT '0' COMMENT '最大 HP',
  `MaxMP` int(4) unsigned NOT NULL DEFAULT '0' COMMENT '最大MP',
  `Register_time` datetime NOT NULL COMMENT '注册时间 ',
  `LastLogin_time` datetime NOT NULL,
  `mutetime` int(4) unsigned zerofill DEFAULT NULL COMMENT '总在线时长(秒计时)',
  PRIMARY KEY (`id`,`name`),
  UNIQUE KEY `RoleName` (`name`),
  KEY `Accounts` (`accounts`),
  KEY `index` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3796 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of character_base
-- ----------------------------
INSERT INTO `character_base` VALUES ('3795', 'updays', '盘古', 'male', '0', '0', '1', '3', '3', '170', '170', '1371', '391', '0', '0', '0', '0', '00', '0', '0', '10', '1', '10', '0', '51', '-16.46150', '-102.64510', '2100', '0', '0000', '0024', '1371', '391', '2009-10-20 18:10:01', '2009-10-20 19:15:02', '0008');

-- ----------------------------
-- Table structure for `character_equip`
-- ----------------------------
DROP TABLE IF EXISTS `character_equip`;
CREATE TABLE `character_equip` (
  `user_id` int(4) unsigned NOT NULL,
  `body_part_id` tinyint(1) NOT NULL COMMENT '身上14个位置',
  `prop_id` smallint(2) NOT NULL COMMENT '具道ID',
  `type1` tinyint(1) DEFAULT NULL,
  `quality1` tinyint(1) DEFAULT NULL,
  `value1` float(4,0) DEFAULT NULL,
  `type2` tinyint(1) DEFAULT NULL,
  `quality2` tinyint(1) DEFAULT NULL,
  `value2` float(4,0) DEFAULT NULL,
  `type3` tinyint(1) DEFAULT NULL,
  `quality3` tinyint(1) DEFAULT NULL,
  `value3` float(4,0) DEFAULT NULL,
  `type4` tinyint(1) DEFAULT NULL,
  `quality4` tinyint(1) DEFAULT NULL,
  `value4` float(4,0) DEFAULT NULL,
  `type5` tinyint(1) DEFAULT NULL,
  `quality5` tinyint(1) DEFAULT NULL,
  `value5` float(4,0) DEFAULT NULL,
  `isbind` tinyint(1) unsigned DEFAULT '0' COMMENT '是否绑定',
  PRIMARY KEY (`user_id`,`body_part_id`),
  KEY `index` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of character_equip
-- ----------------------------

-- ----------------------------
-- Table structure for `character_kitbag`
-- ----------------------------
DROP TABLE IF EXISTS `character_kitbag`;
CREATE TABLE `character_kitbag` (
  `user_id` int(4) unsigned NOT NULL COMMENT '玩家ID',
  `kitbag_1` varchar(4000) DEFAULT NULL COMMENT '背包1',
  `kitbag_2` varchar(4000) DEFAULT NULL COMMENT '背包2',
  `kitbag_3` varchar(4000) DEFAULT NULL COMMENT '背包3',
  `kitbag_4` varchar(4000) DEFAULT NULL COMMENT '背包4',
  `storage` varchar(4000) DEFAULT NULL COMMENT '背包5',
  `equip` varchar(2000) DEFAULT NULL COMMENT '备装',
  PRIMARY KEY (`user_id`),
  KEY `index` (`user_id`)
) ENGINE=MyISAM DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of character_kitbag
-- ----------------------------
INSERT INTO `character_kitbag` VALUES ('3795', '[4000,,,,,,0,20,1,1,0]#[4030,,,,,,0,20,1,1,0]#[2900,,,,,,1,1,1,1,0]#[]#[]#[1000,0,,,,,1,1,1,1,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#', null, null, null, '[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[0]#', '[]#[]#[2800,0,,,,,1,1,1,1,0]#[2100,,,,,,1,1,1,1,0]#[]#[]#[2900,50,,,,,1,1,1,1,0]#[]#[]#[]#[1400,,,,,,1,1,1,1,0]#[]#');

-- ----------------------------
-- Table structure for `consortia`
-- ----------------------------
DROP TABLE IF EXISTS `consortia`;
CREATE TABLE `consortia` (
  `id` int(4) unsigned NOT NULL AUTO_INCREMENT COMMENT 'ID编号',
  `name` char(64) COLLATE utf8_unicode_ci NOT NULL COMMENT '公会名称',
  `placard` char(128) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '公会公告',
  `business_name1` char(32) COLLATE utf8_unicode_ci DEFAULT '会长' COMMENT '会长名称',
  `business_name2` char(32) COLLATE utf8_unicode_ci DEFAULT '副会长' COMMENT '副会长名称',
  `business_name3` char(32) COLLATE utf8_unicode_ci DEFAULT '理事' COMMENT '理事名称',
  `business_name4` char(32) COLLATE utf8_unicode_ci DEFAULT '精英' COMMENT '精英名称',
  `business_name5` char(32) COLLATE utf8_unicode_ci DEFAULT '会员' COMMENT '会员名称',
  `business_name6` char(32) COLLATE utf8_unicode_ci DEFAULT '见习会员' COMMENT '见习会员名称',
  `found` int(4) unsigned zerofill DEFAULT '0000' COMMENT '资金',
  `bijou` int(4) unsigned zerofill DEFAULT '0000' COMMENT '宝石',
  `LV` int(4) unsigned DEFAULT '0' COMMENT '工会等级',
  `honor` int(4) unsigned zerofill DEFAULT '0000' COMMENT '荣誉',
  `altar_info` varchar(1000) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '公会创建的所有祭坛',
  PRIMARY KEY (`id`,`name`),
  UNIQUE KEY `Name_Index` (`name`),
  UNIQUE KEY `ID_Index` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1483 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of consortia
-- ----------------------------

-- ----------------------------
-- Table structure for `gm_placard`
-- ----------------------------
DROP TABLE IF EXISTS `gm_placard`;
CREATE TABLE `gm_placard` (
  `id` int(4) unsigned NOT NULL COMMENT '公告ID',
  `gm_name` char(32) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'GM操作员名字',
  `placardtype` tinyint(1) unsigned DEFAULT NULL COMMENT '公告类型',
  `sendtype` tinyint(1) unsigned DEFAULT NULL COMMENT '发送类型',
  `starttime` datetime DEFAULT NULL COMMENT '开始时间',
  `endtime` datetime DEFAULT NULL COMMENT '结束时间',
  `intervaltime` int(4) unsigned DEFAULT NULL,
  `placardinfo` varchar(255) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT '公告信息',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of gm_placard
-- ----------------------------

-- ----------------------------
-- Table structure for `gm_speakforbid`
-- ----------------------------
DROP TABLE IF EXISTS `gm_speakforbid`;
CREATE TABLE `gm_speakforbid` (
  `rolename` char(32) COLLATE utf8_unicode_ci NOT NULL COMMENT '角色名',
  `gm_name` char(32) COLLATE utf8_unicode_ci DEFAULT NULL COMMENT 'GM操作员名字',
  `starttime` datetime DEFAULT NULL COMMENT '开始时间',
  `endtime` datetime DEFAULT NULL COMMENT '束结时间',
  PRIMARY KEY (`rolename`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of gm_speakforbid
-- ----------------------------

-- ----------------------------
-- Table structure for `user_altar`
-- ----------------------------
DROP TABLE IF EXISTS `user_altar`;
CREATE TABLE `user_altar` (
  `user_id` int(4) unsigned NOT NULL DEFAULT '0' COMMENT '用户id',
  `altar1` int(4) unsigned DEFAULT '0' COMMENT '祭坛1',
  `altar2` int(4) unsigned DEFAULT '0',
  `altar3` int(4) unsigned DEFAULT '0',
  `altar4` int(4) unsigned DEFAULT '0',
  `altar5` int(4) unsigned DEFAULT '0',
  `altar6` int(4) unsigned DEFAULT '0',
  `altar7` int(4) unsigned DEFAULT '0',
  `altar8` int(4) unsigned DEFAULT '0',
  `altar9` int(4) unsigned DEFAULT '0',
  `altar10` int(4) unsigned DEFAULT '0',
  `altar11` int(4) unsigned DEFAULT '0',
  `altar12` int(4) unsigned DEFAULT '0',
  PRIMARY KEY (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of user_altar
-- ----------------------------
INSERT INTO `user_altar` VALUES ('3795', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0');

-- ----------------------------
-- Table structure for `user_magic`
-- ----------------------------
DROP TABLE IF EXISTS `user_magic`;
CREATE TABLE `user_magic` (
  `user_id` int(4) unsigned NOT NULL COMMENT '玩家ID',
  `magic_info` varchar(1024) DEFAULT NULL COMMENT '主动技能信息 ',
  PRIMARY KEY (`user_id`),
  KEY `_INDEX_USER` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of user_magic
-- ----------------------------
INSERT INTO `user_magic` VALUES ('3795', '[250,0]#[3063,0]#');

-- ----------------------------
-- Table structure for `user_quest`
-- ----------------------------
DROP TABLE IF EXISTS `user_quest`;
CREATE TABLE `user_quest` (
  `id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '角色ID',
  `allqueststatus` longtext CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '所有任务完成情况',
  `activestatus` text CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL COMMENT '当前激活的任务',
  `timequest` mediumtext NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `index` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of user_quest
-- ----------------------------
INSERT INTO `user_quest` VALUES ('3795', '31,0,0,0,', '[5,0,4,0,0,0,0,0,7,0,0,0,0,]#', '[141,0,2,1256083260,0,0,0,0,0,0,0,0,]#[143,0,2,1256083260,0,0,0,0,0,0,0,0,]#[144,0,2,1256083260,0,0,0,0,0,0,0,0,]#[145,0,2,1256083260,0,0,0,0,0,0,0,0,]#[146,0,2,1256083260,0,0,0,0,0,0,0,0,]#[147,0,2,1256083260,0,0,0,0,0,0,0,0,]#[148,0,2,1256083260,0,0,0,0,0,0,0,0,]#[149,0,2,1256083260,0,0,0,0,0,0,0,0,]#[150,0,2,1256083260,0,0,0,0,0,0,0,0,]#[167,0,2,1256083260,0,0,0,0,0,0,0,0,]#[168,0,2,1256083260,0,0,0,0,0,0,0,0,]#[169,0,2,1256083260,0,0,0,0,0,0,0,0,]#[170,0,2,1256083260,0,0,0,0,0,0,0,0,]#[171,0,2,1256083260,0,0,0,0,0,0,0,0,]#[172,0,2,1256083260,0,0,0,0,0,0,0,0,]#[173,0,2,1256083260,0,0,0,0,0,0,0,0,]#[174,0,2,1256083260,0,0,0,0,0,0,0,0,]#[175,0,2,1256083260,0,0,0,0,0,0,0,0,]#[176,0,2,1256083260,0,0,0,0,0,0,0,0,]#[177,0,2,1256083260,0,0,0,0,0,0,0,0,]#[178,0,2,1256083260,0,0,0,0,0,0,0,0,]#[192,0,2,1256083260,0,0,0,0,0,0,0,0,]#[193,0,2,1256083260,0,0,0,0,0,0,0,0,]#[194,0,2,1256083260,0,0,0,0,0,0,0,0,]#[195,0,2,1256083260,0,0,0,0,0,0,0,0,]#[207,0,1,1256083260,0,0,0,0,0,0,0,0,]#[208,0,2,1256083260,0,0,0,0,0,0,0,0,]#[209,0,2,1256083260,0,0,0,0,0,0,0,0,]#[210,0,2,1256083260,0,0,0,0,0,0,0,0,]#[211,0,2,1256083260,0,0,0,0,0,0,0,0,]#[212,0,2,1256083260,0,0,0,0,0,0,0,0,]#[223,0,2,1256083260,0,0,0,0,0,0,0,0,]#[224,0,2,1256083260,0,0,0,0,0,0,0,0,]#[225,0,2,1256083260,0,0,0,0,0,0,0,0,]#[226,0,2,1256083260,0,0,0,0,0,0,0,0,]#[227,0,2,1256083260,0,0,0,0,0,0,0,0,]#[228,0,2,1256083260,0,0,0,0,0,0,0,0,]#[229,0,2,1256083260,0,0,0,0,0,0,0,0,]#[230,0,2,1256083260,0,0,0,0,0,0,0,0,]#');

-- ----------------------------
-- Table structure for `user_relation`
-- ----------------------------
DROP TABLE IF EXISTS `user_relation`;
CREATE TABLE `user_relation` (
  `user_id` int(11) NOT NULL,
  `friend` varchar(2000) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT ' ' COMMENT '好友列表',
  `blacklist` varchar(2000) CHARACTER SET utf8 COLLATE utf8_unicode_ci DEFAULT '   ' COMMENT '黑名单 ',
  PRIMARY KEY (`user_id`),
  UNIQUE KEY `index` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of user_relation
-- ----------------------------

-- ----------------------------
-- Table structure for `user_skill`
-- ----------------------------
DROP TABLE IF EXISTS `user_skill`;
CREATE TABLE `user_skill` (
  `user_id` int(4) unsigned NOT NULL COMMENT '色角id',
  `skill_info` varchar(4000) CHARACTER SET gb2312 NOT NULL COMMENT '被动技能信息 ',
  PRIMARY KEY (`user_id`),
  UNIQUE KEY `id_index` (`user_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- ----------------------------
-- Records of user_skill
-- ----------------------------
INSERT INTO `user_skill` VALUES ('3795', '[50,1]#[51,0]#[52,0]#[53,0]#[54,0]#[55,0]#[56,0]#[57,0]#[58,0]#[59,0]#[60,0]#[61,0]#[62,0]#[63,0]#[64,0]#[65,0]#[66,0]#[67,0]#');

-- ----------------------------
-- Table structure for `user_status`
-- ----------------------------
DROP TABLE IF EXISTS `user_status`;
CREATE TABLE `user_status` (
  `id` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '玩家ID',
  `status_id` tinyint(4) NOT NULL COMMENT '态状ID',
  `time` tinyint(1) unsigned NOT NULL COMMENT '剩余时间(单位:秒)',
  PRIMARY KEY (`id`,`status_id`),
  UNIQUE KEY `index` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of user_status
-- ----------------------------

-- ----------------------------
-- View structure for `view_consortia`
-- ----------------------------
DROP VIEW IF EXISTS `view_consortia`;
CREATE ALGORITHM=UNDEFINED DEFINER=`root`@`localhost` SQL SECURITY DEFINER VIEW `view_consortia` AS select `character_base`.`name` AS `Role_Name`,`character_base`.`scholar_job_lv` AS `S_LV`,`character_base`.`fighter_job_lv` AS `F_LV`,`character_base`.`consortia_job` AS `Consortia_Job`,`character_base`.`Map` AS `Map_ID`,`character_base`.`consortia` AS `Consortia_ID`,`consortia`.`name` AS `name`,`character_base`.`profession` AS `Profession`,`character_base`.`consortia_contribute` AS `Contribute` from (`character_base` join `consortia`) where (`character_base`.`consortia` = `consortia`.`id`);

-- ----------------------------
-- Procedure structure for `CreateRole`
-- ----------------------------
DROP PROCEDURE IF EXISTS `CreateRole`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `CreateRole`()
begin
     DECLARE  Num    int  DEFAULT 0;
     DECLARE acc CHAR(32) default "";
     DECLARE role CHAR(32) default "";
    SET  Num=1001;
     myloop: LOOP
       IF  Num>2000 THEN
           LEAVE myloop;
      END IF;
     SET acc = CONCAT('gtest', CAST(  Num  as char ) );
     CALL SP_INSERT_ROLE_INFO(acc, acc ,'male',0,0,1,0,0,0,0,0,0,0,0,0);
     SET Num = Num + 1;
END  LOOP  myloop;

end;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `GM_SP_DELETE_FORBID`
-- ----------------------------
DROP PROCEDURE IF EXISTS `GM_SP_DELETE_FORBID`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GM_SP_DELETE_FORBID`(IN cIP CHAR(64),IN iType INT)
BEGIN
	DECLARE playerID  INT  default 0;
	IF iType = 0 THEN
		select GID from accounts.gamearea_info where PassName= cIP INTO playerID;
		IF playerID != 0 THEN
			DELETE FROM accounts.accounts_ban WHERE accounts.accounts_ban.id = playerID;
		END IF;
	ELSE
		DELETE FROM accounts.ip_ban WHERE accounts.ip_ban.ip = cIP;
	END IF;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `GM_SP_DELETE_PLACARD`
-- ----------------------------
DROP PROCEDURE IF EXISTS `GM_SP_DELETE_PLACARD`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GM_SP_DELETE_PLACARD`(IN PlacardID INT)
BEGIN
	DELETE FROM gm_placard WHERE PlacardID = id;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `GM_SP_INSERT_FORBID_ACCOUNT`
-- ----------------------------
DROP PROCEDURE IF EXISTS `GM_SP_INSERT_FORBID_ACCOUNT`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GM_SP_INSERT_FORBID_ACCOUNT`(IN cAccount char(32),IN StartTime char(32),IN EndTime char(32),IN cGmName char(32),IN cReason char(255))
begin
	DECLARE Num tinyint default 0;
	DECLARE Num1 tinyint default 0;
	DECLARE pID INT DEFAULT 0;
	select count(*) from accounts.gamearea_info where PassName=cAccount into Num;
IF Num != 0 THEN
	select GID from accounts.gamearea_info where PassName=cAccount into pID;
	select count(*) from accounts.accounts_ban where accounts.accounts_ban.id=pID into Num1;
	IF Num1 = 0 THEN
             INSERT INTO accounts.accounts_ban(id,bandate,unbandate,bannedby,banreason) VALUES ( pID, StartTime,EndTime,cGmName,cReason  );
        ELSE
            UPDATE accounts.accounts_ban SET bandate = StartTime ,unbandate = EndTime,bannedby = cGmName,banreason = cReason WHERE id = pID;
       END IF;
END IF;

	select Num;
end;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `GM_SP_INSERT_FORBID_IP`
-- ----------------------------
DROP PROCEDURE IF EXISTS `GM_SP_INSERT_FORBID_IP`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GM_SP_INSERT_FORBID_IP`(IN cIP char(32), IN StartTime char(32),IN EndTime char(32), IN cGmName char(32),IN cReason char(255))
begin
	DECLARE Num tinyint default 0;
	DECLARE ID INT DEFAULT 0;
	select count(*) from accounts.ip_ban where ip=cIP into Num;
	IF Num = 0 THEN
             INSERT INTO accounts.ip_ban(ip,bandate,unbandate,bannedby,banreason) VALUES (cIP, StartTime,EndTime,cGmName,cReason  );
        ELSE
            UPDATE accounts. ip_ban SET bandate = StartTime,unbandate = EndTime, bannedby = cGmName,banreason = cReason WHERE ip=cIP;
       END IF;

	select count(*) from accounts.ip_ban where ip=cIP into Num;
	select Num;
end;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `GM_SP_INSERT_SEND_OBJECT`
-- ----------------------------
DROP PROCEDURE IF EXISTS `GM_SP_INSERT_SEND_OBJECT`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GM_SP_INSERT_SEND_OBJECT`(IN cName char(32), IN cItem char(100))
begin
	DECLARE  cKitBag  text  default "";
	DECLARE cTemp    text  default "";
	DECLARE  iKitBagLen    int  DEFAULT 0;
	DECLARE SumLen int DEFAULT 0;
	DECLARE CurLen int DEFAULT 0;
	DECLARE StrLen int DEFAULT 0;
	DECLARE iCanInsert int DEFAULT 0;
	DECLARE Num int default 0;

	select count(*) from character_kitbag,character_base where character_base.name=cName and character_base.id=character_kitbag.user_id into Num;
	if Num=1 then
	select character_kitbag.kitbag_1 from character_kitbag,character_base where character_base.name=cName and character_base.id=character_kitbag.user_id into cKitBag;
	end if;

	set iKitBagLen=CHAR_LENGTH(cKitBag);
myloop:LOOP
	IF  iKitBagLen=0 OR SumLen>=iKitBagLen THEN
	LEAVE myloop;
	END IF;
	set CurLen=LOCATE('#',cKitBag,SumLen+1);
	set Curlen= Curlen-SumLen;
	set cTemp=MID(cKitBag,SumLen+1,CurLen);
	set StrLen=CHAR_LENGTH(cTemp);
	set SumLen=SumLen+CurLen;
	IF StrLen=3 THEN
	set iCanInsert=1;
	set cKitBag=INSERT(cKitBag,SumLen+1-3,CurLen-1,cItem);
	LEAVE myloop;
	END IF;
END LOOP myloop;

	IF iCanInsert = 1 THEN
	 UPDATE character_kitbag,character_base SET  kitbag_1=cKitBag
     	WHERE character_base.name=cName and character_base.id=character_kitbag.user_id ;
	END IF;
	select iCanInsert;
end;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `GM_SP_SELECT_FORBID_ACCOUNT_LIST`
-- ----------------------------
DROP PROCEDURE IF EXISTS `GM_SP_SELECT_FORBID_ACCOUNT_LIST`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GM_SP_SELECT_FORBID_ACCOUNT_LIST`()
begin
	delete from accounts.accounts_ban where accounts.accounts_ban.unbandate < NOW();

	select accounts.gamearea_info.PassName, accounts.accounts_ban.*
	from accounts.accounts_ban ,accounts.gamearea_info
	where accounts.accounts_ban.unbandate > NOW() and accounts.accounts_ban.id = accounts.gamearea_info.GID;
end;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `GM_SP_SELECT_FORBID_IP_LIST`
-- ----------------------------
DROP PROCEDURE IF EXISTS `GM_SP_SELECT_FORBID_IP_LIST`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GM_SP_SELECT_FORBID_IP_LIST`()
BEGIN
	delete from accounts.ip_ban where accounts.ip_ban.unbandate < NOW();

	select * from accounts.ip_ban where accounts.ip_ban.unbandate > NOW();
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `GM_SP_SELECT_FORBID_TIME`
-- ----------------------------
DROP PROCEDURE IF EXISTS `GM_SP_SELECT_FORBID_TIME`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GM_SP_SELECT_FORBID_TIME`(IN cName CHAR(32), IN iType INT)
BEGIN
	DECLARE  playername  varchar(32) default "";
	DECLARE  Num tinyint default 0;

	DELETE FROM gm_speakforbid WHERE endtime< NOW();

	IF iType=0 THEN
		SELECT godswar.character_base.name
	 	FROM   godswar.character_base ,accounts.gamearea_info
		WHERE godswar.character_base.accounts=cName and  godswar.character_base.accounts = accounts.gamearea_info.PassName  INTO playername;

		SELECT ((DAY(endtime)-DAY(NOW()))*86400 + (TIME_TO_SEC(TIME(endtime)) - TIME_TO_SEC(CURTIME())))  AS IntervalTime
		FROM   gm_speakforbid
		WHERE rolename = playername;
	ELSEIF iType = 1 THEN
		SELECT ((DAY(endtime)-DAY(NOW()))*86400 + (TIME_TO_SEC(TIME(endtime)) - TIME_TO_SEC(CURTIME())))  AS IntervalTime
		FROM   gm_speakforbid
		WHERE rolename = cName;
	END IF;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `GM_SP_SELECT_PLACARD`
-- ----------------------------
DROP PROCEDURE IF EXISTS `GM_SP_SELECT_PLACARD`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GM_SP_SELECT_PLACARD`()
BEGIN
	SELECT * FROM gm_placard WHERE endtime > NOW() AND placardtype != 0; -- 0:SEND IMMEDIATELY
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `GM_SP_SELECT_PLAYER_INFO_FROM_ACCOUNT`
-- ----------------------------
DROP PROCEDURE IF EXISTS `GM_SP_SELECT_PLAYER_INFO_FROM_ACCOUNT`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GM_SP_SELECT_PLAYER_INFO_FROM_ACCOUNT`(IN cAccounts char(32))
begin
        DECLARE  IP CHAR(32) DEFAULT "192.168.6.168";
        DECLARE  MAC CHAR(32) DEFAULT "123456EWREW";
	select godswar.character_base.*,IP,MAC
	 from godswar.character_base ,accounts.gamearea_info
	where godswar.character_base.accounts=cAccounts and  godswar.character_base.accounts = accounts.gamearea_info.PassName ;
end;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `GM_SP_SELECT_PLAYER_INFO_FROM_NAME`
-- ----------------------------
DROP PROCEDURE IF EXISTS `GM_SP_SELECT_PLAYER_INFO_FROM_NAME`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GM_SP_SELECT_PLAYER_INFO_FROM_NAME`(IN cName char(32))
begin
	 DECLARE  IP CHAR(32) DEFAULT "192.168.6.168";
     	 DECLARE  MAC CHAR(32) DEFAULT "123456EWREW";
	 select character_base.*, IP,MAC
	 from character_base ,accounts.gamearea_info
	 where  character_base.name=cName and character_base.accounts = accounts.gamearea_info.PassName ;
end;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `GM_SP_SELECT_PLAYER_OBJECT`
-- ----------------------------
DROP PROCEDURE IF EXISTS `GM_SP_SELECT_PLAYER_OBJECT`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GM_SP_SELECT_PLAYER_OBJECT`(IN cName char(32))
begin
	select character_kitbag.kitbag_1,character_kitbag.storage,character_kitbag.equip
	from character_base,character_kitbag
	where character_base.name=cName and character_base.id=character_kitbag.user_id;
end;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `GM_SP_UPDATE_PLACARD`
-- ----------------------------
DROP PROCEDURE IF EXISTS `GM_SP_UPDATE_PLACARD`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GM_SP_UPDATE_PLACARD`(IN pID  INT, IN GmName  CHAR(32),IN PlacardType  INT, IN SendType INT,IN StartTime CHAR(32),IN EndTime CHAR(32),IN Intervaltime INT,IN PlacardInfo CHAR(255))
BEGIN
	DECLARE Num tinyint default 0;
	select count(*) from gm_placard where gm_placard.id = pID into Num;

	IF Num = 1 THEN
		UPDATE gm_placard SET gm_name = GmName,placardtype=PlacardType,sendtype=SendType,starttime=StartTime,endtime=EndTime,intervaltime=Intervaltime,placardinfo=PlacardInfo
		WHERE gm_placard.id=pID;
	ELSEIF Num = 0 THEN
		INSERT INTO gm_placard VALUES (pID, GmName  , PlacardType, SendType ,StartTime ,EndTime,Intervaltime ,PlacardInfo);
	END IF;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `GM_SP_UPDATE_PLAYER_POSIT`
-- ----------------------------
DROP PROCEDURE IF EXISTS `GM_SP_UPDATE_PLAYER_POSIT`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GM_SP_UPDATE_PLAYER_POSIT`(IN cName char(32))
begin
	DECLARE  Num    int  DEFAULT 0;
	DECLARE icamp   int DEFAULT 0;
	DECLARE imap	    int DEFAULT 0;
	DECLARE X  float DEFAULT 0.00;
   	DECLARE Z  float  DEFAULT 0.00;

	select camp from character_base where  name=cName into icamp;
	IF  icamp  = 1 THEN
        SET  imap = 1, X =  98.0,Z = -178.0;     -- athen
  	 ELSEIF  icamp = 0 THEN
        SET  imap = 51 , X =  114.0, Z = -20.0;     -- sparta
   	END IF;

	UPDATE character_base  SET Map  = imap, Pos_X=X,Pos_Z=Z  WHERE  name=cName ;

	select count(*) from character_base where name=cName;

end;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `GM_SP_UPDATE_SPEAKFORBID`
-- ----------------------------
DROP PROCEDURE IF EXISTS `GM_SP_UPDATE_SPEAKFORBID`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GM_SP_UPDATE_SPEAKFORBID`(IN Role_Name  CHAR(32), IN Gm_Name  CHAR(32),IN TimeDefer INT)
BEGIN
	DECLARE Num tinyint DEFAULT 0;
	SELECT COUNT(*) FROM gm_speakforbid
	WHERE rolename=Role_Name  INTO Num;

	IF Num = 1 THEN
		UPDATE gm_speakforbid SET gm_name=Gm_Name,starttime=NOW(),endtime= ADDTIME(NOW(),SEC_TO_TIME(TimeDefer))
		WHERE  rolename=Role_Name;
	ELSEIF Num = 0 THEN
		INSERT INTO gm_speakforbid(rolename,gm_name,starttime,endtime) VALUES(Role_Name,Gm_Name,NOW(),ADDTIME(NOW(),SEC_TO_TIME(TimeDefer)));
	END IF;

	DELETE FROM gm_speakforbid WHERE endtime< NOW();

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `OFFLINE_SQL_UPDATE_ALTAR_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `OFFLINE_SQL_UPDATE_ALTAR_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `OFFLINE_SQL_UPDATE_ALTAR_INFO`(IN cName CHAR(32))
BEGIN
   DECLARE Num  INT DEFAULT 0;
   DECLARE UserID INT DEFAULT 0;
   SELECT COUNT(*),character_base.id
   FROM character_base,user_altar
   where character_base.name=cName AND character_base.id=user_altar.user_id GROUP BY  character_base.id INTO Num,UserID;

  IF Num=1 THEN
  	UPDATE user_altar SET altar1=0, altar2=0,altar3=0,altar4=0,altar5=0,altar6=0,altar7=0,altar8=0,altar9=0,altar10=0,altar11=0,altar12=0  WHERE  user_id=UserID ;
  END IF;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `OFFLINE_SQL_UPDATE_ONE_ALTAR_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `OFFLINE_SQL_UPDATE_ONE_ALTAR_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `OFFLINE_SQL_UPDATE_ONE_ALTAR_INFO`(IN cName CHAR(32),IN AltarID INT)
BEGIN
   DECLARE Num  INT DEFAULT 0;
   DECLARE UserID INT DEFAULT 0;
  DECLARE  altar  varchar(32) default "altar";
   SELECT COUNT(*),character_base.id
   FROM character_base,user_altar
   where character_base.name=cName AND character_base.id=user_altar.user_id 
   Group by character_base.id
   INTO Num,UserID;

  IF Num=1 THEN
	IF AltarID=1 THEN
		UPDATE user_altar SET altar1=0 WHERE  user_id=UserID ;
	ELSEIF AltarID=2 THEN
		UPDATE user_altar SET altar2=0 WHERE  user_id=UserID ;
	ELSEIF AltarID=3 THEN
		UPDATE user_altar SET altar3=0 WHERE  user_id=UserID ;
	ELSEIF AltarID=4 THEN
		UPDATE user_altar SET altar4=0 WHERE  user_id=UserID ;
	ELSEIF AltarID=5 THEN
		UPDATE user_altar SET altar5=0 WHERE  user_id=UserID ;
	ELSEIF AltarID=6 THEN
		UPDATE user_altar SET altar6=0 WHERE  user_id=UserID ;
	ELSEIF AltarID=7 THEN
		UPDATE user_altar SET altar7=0 WHERE  user_id=UserID ;
	ELSEIF AltarID=8 THEN
		UPDATE user_altar SET altar8=0 WHERE  user_id=UserID ;
	ELSEIF AltarID=9 THEN
		UPDATE user_altar SET altar9=0 WHERE  user_id=UserID ;
	ELSEIF AltarID=10 THEN
		UPDATE user_altar SET altar10=0 WHERE  user_id=UserID ;
	ELSEIF AltarID=11 THEN
		UPDATE user_altar SET altar11=0 WHERE  user_id=UserID ;
	ELSEIF AltarID=12 THEN
		UPDATE user_altar SET altar12=0 WHERE  user_id=UserID ;
	END IF;
  END IF;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `OFFLINE_SQL_UPDATE_UNION`
-- ----------------------------
DROP PROCEDURE IF EXISTS `OFFLINE_SQL_UPDATE_UNION`;
DELIMITER ;;
CREATE DEFINER=`user`@`localhost` PROCEDURE `OFFLINE_SQL_UPDATE_UNION`(IN cName CHAR(32), IN iUnion INT UNSIGNED, IN iUnionDuty TINYINT UNSIGNED,IN iUnionDutyA TINYINT UNSIGNED)
BEGIN
    DECLARE  Num  TINYINT DEFAULT 0;
    DECLARE  OldDuty  TINYINT DEFAULT 0;
    DECLARE  NewDuty  TINYINT DEFAULT 0;
    DECLARE  RoleName  CHAR(32) DEFAULT '';
    SELECT COUNT(*)
    FROM character_base
    WHERE name=cName AND  iUnionDutyA > consortia_job
    INTO Num;

    IF Num=1 THEN

        SELECT name,consortia_job FROM character_base WHERE name=cName INTO RoleName,OldDuty;

        UPDATE character_base
        SET consortia = iUnion,
            consortia_job =  iUnionDuty
        WHERE   name =  cName;
    END IF;

    SELECT Num,RoleName,OldDuty,iUnionDuty,iUnion;


END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_OFFLINE_SQL_UPDATE_UNION`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_OFFLINE_SQL_UPDATE_UNION`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_OFFLINE_SQL_UPDATE_UNION`(IN cName CHAR(32), IN iUnion INT UNSIGNED, IN iUnionDuty TINYINT UNSIGNED,IN iUnionDutyA TINYINT UNSIGNED)
BEGIN
    DECLARE  Num  TINYINT DEFAULT 0;
    DECLARE  OldDuty  TINYINT DEFAULT 0;
    DECLARE  NewDuty  TINYINT DEFAULT 0;
    DECLARE  RoleName  CHAR(32) DEFAULT '';
    SELECT COUNT(*)
    FROM character_base
    WHERE name=cName AND  iUnionDutyA > consortia_job
    INTO Num;

    IF Num=1 THEN

        SELECT name,consortia_job FROM character_base WHERE name=cName INTO RoleName,OldDuty;

        UPDATE character_base
        SET consortia = iUnion,
            consortia_job =  iUnionDuty
        WHERE   name =  cName;
    END IF;

    SELECT Num,RoleName,OldDuty,iUnionDuty,iUnion;


END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_DELETE_ROLE_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_DELETE_ROLE_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_DELETE_ROLE_INFO`(IN RoleID INT)
BEGIN
   DELETE FROM character_base WHERE id = RoleID;
   DELETE FROM character_kitbag WHERE user_id = RoleID;
   DELETE FROM user_magic WHERE user_id = RoleID;
   DELETE FROM user_skill WHERE user_id = RoleID;
   DELETE FROM user_quest WHERE id = RoleID;
   DELETE FROM user_relation WHERE user_id = RoleID;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_INSERT_ROLE`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_INSERT_ROLE`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_INSERT_ROLE`(IN  cAccounts CHAR(32) , IN RoleName  CHAR(32),IN Gender  CHAR(16),IN iCamp INT,IN iProfession INT, IN F_LV INT, IN S_LV INT,  IN F_Exp INT, IN S_Exp INT, IN Cur_HP INT , IN Cur_MP INT, IN Staus INT,  IN iBelief INT, IN Hair_style INT,IN Face_shap INT)
BEGIN
   DECLARE Last_ID INT DEFAULT 0;
   DECLARE  Num    INT  DEFAULT 0;
   DECLARE Map INT DEFAULT 0;
   DECLARE Pos_X  FLOAT DEFAULT 0.00;
   DECLARE Pos_Z  FLOAT  DEFAULT 0.00;
   DECLARE GodSkillID INT DEFAULT 0;
   DECLARE  GodSkill  VARCHAR(64) default "";
   DECLARE  ExistID  INT UNSIGNED DEFAULT 0;
   DECLARE  FLV  INT DEFAULT 0;
   DECLARE  SLV  INT DEFAULT 0;
   DECLARE  iPrestige  INT DEFAULT 0;
   DECLARE  iMaxLV INT DEFAULT 0;
   DECLARE  iIsDraw TINYINT DEFAULT 0;

   IF  iCamp  = 1 THEN
       SET  Map = 1, Pos_X =  98.0,Pos_Z = -178.0;     -- athen
   ELSEIF  iCamp = 0 THEN
       SET  Map = 51 , Pos_X =  -108.0, Pos_Z = -122.0;     -- sparta
   END IF;
  INSERT INTO character_base  VALUES ( NULL , cAccounts ,RoleName  , Gender , 0,  iCamp, iProfession,  F_LV, S_LV,  F_Exp,S_Exp, Cur_HP, Cur_MP, Staus,  iBelief, 0, 0, 0, 0,0, Hair_style,Face_shap,Map,Pos_X,Pos_Z,100,0,0,0,Cur_HP,Cur_MP, NOW(),'00-00-00 00:00:00 ',0);
  SELECT LAST_INSERT_ID() from  character_base limit 1 INTO Last_ID;
  INSERT INTO character_kitbag( user_id ) VALUES ( Last_ID );
  INSERT INTO user_quest values ( LAST_ID,'','','');

  SET GodSkill = CAST( 5000+iBelief  as char );
  SET GodSkill = CONCAT( '[',GodSkill,']#' );
-- default  Prop
   IF  iCamp  = 1 THEN -- athen
      SET GodSkill = CONCAT( GodSkill, '[3003,0]#' );
       -- CALL SP_UPDATE_KITBAG_INFO(Last_ID,1,'[4000,,,,,,0,5,1]#[4100,,,,,,0,5,1]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
   ELSEIF  iCamp = 0 THEN -- sparta
       SET GodSkill = CONCAT( GodSkill, '[3103,0]#' );
       -- CALL SP_UPDATE_KITBAG_INFO(Last_ID,1,'[4000,,,,,,0,5,1]#[4100,,,,,,0,5,1]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
   END IF;

  /*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
  /* 2008-01-15
  /* ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
   SELECT COUNT(*) FROM gwo_backup.character_base WHERE gwo_backup.character_base .accounts = cAccounts INTO ExistID;
   -- SELECT IsDraw FROM  accounts.gamearea_info WHERE PassName=cAccounts  INTO iIsDraw ;

   IF ExistID >= 1 /*AND  iIsDraw = 0 */ THEN

       SELECT MAX(prestige)  FROM gwo_backup.character_base  WHERE   gwo_backup.character_base .accounts = cAccounts INTO iPrestige;

       SELECT MAX(fighter_job_lv ), MAX(scholar_job_lv) FROM gwo_backup.character_base  WHERE   gwo_backup.character_base .accounts = cAccounts INTO FLV,SLV;
       IF FLV>SLV THEN
           SET iMaxLV = FLV;
       ELSE
           SET iMaxLV = SLV;
       END IF;

        IF ( iMaxLV >= 10  AND iMaxLV <= 20 )   THEN
           CALL SP_UPDATE_KITBAG_INFO(Last_ID,1,'[4000,,,,,,0,5,1,1,0]#[4100,,,,,,0,5,1,1,0]#[4200,,,,,,1,10,1,0,0]#[4201,,,,,,1,20,1,0,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
        ELSEIF  ( iMaxLV >= 21 AND iMaxLV <= 30 )  THEN
           CALL SP_UPDATE_KITBAG_INFO(Last_ID,1,'[4000,,,,,,0,5,1,1,0]#[4100,,,,,,0,5,1,1,0]#[4200,,,,,,1,15,1,0,0]#[4201,,,,,,1,30,1,0,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
        ELSEIF ( iMaxLV >= 31 AND iMaxLV <=35  ) THEN
           CALL SP_UPDATE_KITBAG_INFO(Last_ID,1,'[4000,,,,,,0,5,1,1,0]#[4100,,,,,,0,5,1,1,0]#[4200,,,,,,1,20,1,0,0]#[4201,,,,,,1,40,1,0,0]#[4202,,,,,,1,3,1,0,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
        ELSEIF ( iMaxLV >= 36 AND iMaxLV <= 40  )   THEN
           CALL SP_UPDATE_KITBAG_INFO(Last_ID,1,'[4000,,,,,,0,5,1,1,0]#[4100,,,,,,0,5,1,1,0]#[4200,,,,,,1,25,1,0,0]#[4201,,,,,,1,50,1,0,0]#[4202,,,,,,1,6,1,0,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
        ELSEIF  ( iMaxLV >= 41 AND iMaxLV <= 45 )  THEN
           CALL SP_UPDATE_KITBAG_INFO(Last_ID,1,'[4000,,,,,,0,5,1,1,0]#[4100,,,,,,0,5,1,1,0]#[4200,,,,,,1,30,1,0,0]#[4201,,,,,,1,60,1,0,0]#[4202,,,,,,1,9,1,0,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
        ELSEIF ( iMaxLV >=46  AND iMaxLV <= 50 ) THEN
           CALL SP_UPDATE_KITBAG_INFO(Last_ID,1,'[4000,,,,,,0,5,1,1,0]#[4100,,,,,,0,5,1,1,0]#[4200,,,,,,1,35,1,0,0]#[4201,,,,,,1,70,1,0,0]#[4202,,,,,,1,12,1,0,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
        ELSEIF  iMaxLV >50    THEN
           CALL SP_UPDATE_KITBAG_INFO(Last_ID,1,'[4000,,,,,,0,5,1,1,0]#[4100,,,,,,0,5,1,1,0]#[4200,,,,,,1,35,1,0,0]#[4201,,,,,,1,70,1,0,0]#[4202,,,,,,1,15,1,0,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
        ELSE
           CALL SP_UPDATE_KITBAG_INFO(Last_ID,1,'[4000,,,,,,0,5,1,1,0]#[4100,,,,,,0,5,1,1,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
        END IF;
        UPDATE character_base SET prestige = iPrestige  WHERE id = Last_ID;
        UPDATE accounts.gamearea_info SET  IsDraw = 1 WHERE  PassName=cAccounts;

   ELSE
        CALL SP_UPDATE_KITBAG_INFO(Last_ID,1,'[4000,,,,,,0,5,1,1]#[4100,,,,,,0,5,1,1]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
   END IF;

-- default skill and magic
  IF iProfession = 0 THEN
     -- SET GodSkill = CONCAT( GodSkill, '[0,0]#[200,0]#' );
     IF  iCamp  = 0 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[0,0]#[3063,0]#' );
     ELSEIF  iCamp = 1 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[0,0]#[3061,0]#' );
     END IF;

     CALL SP_UPDATE_PASSIVESKILL_INFO( Last_ID,  '[0,0]#[1,0]#[2,0]#[3,0]#[4,0]#[5,0]#[6,0]#[7,0]#[8,0]#[9,0]#[10,0]#[11,0]#[12,0]#[13,0]#[14,0]#[15,0]#[16,0]#[17,0]#'  );
     CALL SP_UPDATE_KITBAG_INFO(Last_ID,6,'[]#[]#[]#[2100,,,,,,1,1,1,1,0]#[]#[]#[2900,,,,,,1,1,1,1,0]#[]#[]#[]#[1000,,,,,,1,1,1,1,0]#[2000,,,,,,1,1,1,1,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
     CALL SP_UPDATE_ALTAR_INFO(Last_ID,0,0,0,0,0,0,0,0,0,0,0,0);
  ELSEIF iProfession=1 THEN
     -- SET GodSkill = CONCAT( GodSkill, '[1000,0]#[1200,0]#' );
     IF  iCamp  = 0 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[250,0]#[3063,0]#' );
     ELSEIF  iCamp = 1 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[250,0]#[3061,0]#' );
     END IF;

     CALL SP_UPDATE_PASSIVESKILL_INFO( Last_ID,  '[50,0]#[51,0]#[52,0]#[53,0]#[54,0]#[55,0]#[56,0]#[57,0]#[58,0]#[59,0]#[60,0]#[61,0]#[62,0]#[63,0]#[64,0]#[65,0]#[66,0]#[67,0]#' );
     CALL SP_UPDATE_KITBAG_INFO(Last_ID,6,'[]#[]#[]#[2100,,,,,,1,1,1,1,0]#[]#[]#[2900,,,,,,1,1,1,1,0]#[]#[]#[]#[1400,,,,,,1,1,1,1,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
     CALL SP_UPDATE_ALTAR_INFO(Last_ID,0,0,0,0,0,0,0,0,0,0,0,0);
 ELSEIF iProfession=2 THEN
     -- SET GodSkill = CONCAT( GodSkill, '[1000,0]#[1200,0]#' );
     IF  iCamp  = 0 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[800,0]#[3063,0]#' );
     ELSEIF  iCamp = 1 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[800,0]#[3061,0]#' );
     END IF;

     CALL SP_UPDATE_PASSIVESKILL_INFO( Last_ID,   '[150,0]#[151,0]#[152,0]#[153,0]#[154,0]#[155,0]#[156,0]#[157,0]#[158,0]#[159,0]#[160,0]#[161,0]#[162,0]#[163,0]#[164,0]#[165,0]#[166,0]#[167,0]#' );
     CALL SP_UPDATE_KITBAG_INFO(Last_ID,6,'[]#[]#[]#[2200,,,,,,1,1,1,1,0]#[]#[]#[2900,,,,,,1,1,1,1,0]#[]#[]#[]#[1700,,,,,,1,1,1,1,0]#[2000,,,,,,1,1,1,1,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
     CALL SP_UPDATE_ALTAR_INFO(Last_ID,0,0,0,0,0,0,0,0,0,0,0,0);
 ELSEIF  iProfession=3 THEN
     -- SET GodSkill = CONCAT( GodSkill, '[1000,0]#[1200,0]#' );
     IF  iCamp  = 0 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[500,0]#[3063,0]#' );
     ELSEIF  iCamp = 1 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[500,0]#[3061,0]#' );
     END IF;

     CALL SP_UPDATE_PASSIVESKILL_INFO( Last_ID,  '[100,0]#[101,0]#[102,0]#[103,0]#[104,0]#[105,0]#[106,0]#[107,0]#[108,0]#[109,0]#[110,0]#[111,0]#[112,0]#[113,0]#[114,0]#[115,0]#[116,0]#[117,0]#' );
     CALL SP_UPDATE_KITBAG_INFO(Last_ID,6,'[]#[]#[]#[2200,,,,,,1,1,1,1,0]#[]#[]#[2900,,,,,,1,1,1,1,0]#[]#[]#[]#[1800,,,,,,1,1,1,1,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
     CALL SP_UPDATE_ALTAR_INFO(Last_ID,0,0,0,0,0,0,0,0,0,0,0,0);
  END IF;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_INSERT_ROLE_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_INSERT_ROLE_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_INSERT_ROLE_INFO`(IN  Accounts CHAR(32) , IN RoleName  CHAR(32),IN Gender  CHAR(16),IN iCamp INT,IN iProfession INT, IN F_LV INT, IN S_LV INT,  IN F_Exp INT, IN S_Exp INT, IN Cur_HP INT , IN Cur_MP INT, IN Staus INT,  IN iBelief INT, IN Hair_style INT,IN Face_shap INT)
    DETERMINISTIC
BEGIN
   DECLARE  Num    int  DEFAULT 0;
   DECLARE  Last_ID    int  DEFAULT 0;
   DECLARE ReturnName CHAR(32)  DEFAULT  RoleName ;
   SELECT  count(*)   FROM  character_base  WHERE  name = RoleName  into   Num;
   IF Num = 0 THEN
      CALL  PLOY_SP_INSERT_ROLE(Accounts , RoleName , Gender, iCamp ,iProfession ,F_LV ,S_LV ,F_Exp ,S_Exp ,Cur_HP ,Cur_MP ,Staus ,iBelief,Hair_style,Face_shap);
     SELECT max(id) from  character_base limit 1 INTO Last_ID;
  END IF;
   SELECT  Num,ReturnName,Last_ID,Num;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_SELECT_ACCOUNT_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_SELECT_ACCOUNT_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_SELECT_ACCOUNT_INFO`(IN  Name CHAR(32) , IN Password  CHAR(32))
BEGIN
     DECLARE  num  TINYINT DEFAULT 0;
    SELECT count(*)  FROM    accounts_info  WHERE  accounts = Name  AND  password = Password  INTO  num;
    SELECT num;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_SELECT_ACTIVESKILL_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_SELECT_ACTIVESKILL_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_SELECT_ACTIVESKILL_INFO`(IN  pra  INT)
BEGIN
   SELECT magic_info FROM user_magic WHERE user_id = pra;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_SELECT_CONSORTIA_BASE_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_SELECT_CONSORTIA_BASE_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_SELECT_CONSORTIA_BASE_INFO`()
BEGIN
    -- SELECT id,name,placard,business_name1,business_name2,business_name3,business_name4,attribute1,attribute2,attribute3,attribute4 from consortia where id = 1;
    select *  from consortia;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_SELECT_CONSORTIA_MEMBER_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_SELECT_CONSORTIA_MEMBER_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_SELECT_CONSORTIA_MEMBER_INFO`(IN  pra  CHAR(64))
BEGIN
    SELECT * 
    FROM VIEW_CONSORTIA 
    WHERE  Consortia_ID IN 
               ( SELECT id FROM consortia WHERE name = pra   ) ;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_SELECT_KITBAG_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_SELECT_KITBAG_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_SELECT_KITBAG_INFO`(IN userid INT)
BEGIN
   DECLARE  bag1 VARCHAR(4000);
   DECLARE  bag2 VARCHAR(4000);
   DECLARE  bag3 VARCHAR(4000);
   DECLARE  bag4 VARCHAR(4000);
   DECLARE  bag5 VARCHAR(4000);
   DECLARE  item  VARCHAR(2000);


   DECLARE  TmpEquip   BLOB;
   DECLARE  i  INT DEFAULT 0;
   DECLARE Num INT DEFAULT 0;
  CREATE TEMPORARY TABLE IF NOT EXISTS bag
  (
         bag   BLOB  NULL
  )  ;

  SET   Num = 0;

  SELECT COUNT(equip),kitbag_1, kitbag_2, kitbag_3, kitbag_4, storage
  FROM  character_kitbag
  WHERE user_id = userid
  GROUP BY equip
  INTO  Num,bag1,bag2,bag3,bag4,bag5;

#SELECT * FROM  bag;
SELECT kitbag_1, kitbag_2, kitbag_3, kitbag_4, storage
FROM  character_kitbag
WHERE user_id = userid;


DROP TABLE  bag;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_SELECT_PASSIVESKILL_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_SELECT_PASSIVESKILL_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_SELECT_PASSIVESKILL_INFO`(IN  pra  INT)
BEGIN
   SELECT skill_info FROM user_skill WHERE user_id = pra;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_SELECT_PLAYER_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_SELECT_PLAYER_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_SELECT_PLAYER_INFO`(IN  pra  INT)
BEGIN
    SELECT character_base.*,character_kitbag.equip  FROM character_base,character_kitbag WHERE character_base.id = pra and character_kitbag.user_id = pra;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_SELECT_QUEST_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_SELECT_QUEST_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_SELECT_QUEST_INFO`(IN  pra  INT)
BEGIN
    SELECT  user_quest.*,character_base.profession, character_base.camp, character_base.fighter_job_lv,character_base.scholar_job_lv,character_base.belief
    from user_quest,character_base
    where user_quest.id = character_base.id and character_base.id  = pra ;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_SELECT_RELATION_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_SELECT_RELATION_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_SELECT_RELATION_INFO`(IN  pra  INT)
BEGIN

   DECLARE Num INT DEFAULT 0;
   DECLARE Friend_List VARCHAR(2000) DEFAULT '';
   DECLARE BlackList_List  VARCHAR(2000) DEFAULT '';
   DECLARE  Leftstr CHAR(32) default "";
   DECLARE  Temp  CHAR(64) default "";

   CREATE TEMPORARY TABLE IF NOT EXISTS RelationInfo
   (
         Name CHAR(32)  NOT  NULL,
         Business TINYINT UNSIGNED,
         Camp TINYINT UNSIGNED ,
         City  TINYINT UNSIGNED ,
         F_LV  TINYINT UNSIGNED ,
         S_LV   TINYINT UNSIGNED ,
         Style  TINYINT UNSIGNED
   )  ;
  SELECT COUNT(*) FROM user_relation WHERE user_id = pra INTO  Num;
  IF NUM=1 THEN
    SELECT friend,blacklist FROM user_relation WHERE user_id = pra  INTO  Friend_List,BlackList_List;
 END IF;
 -- 好友
    FL:REPEAT
       SET  Leftstr = SUBSTRING_INDEX( Friend_List,']#',1);
       SET Temp = concat( Leftstr,']#' );
       SET  Friend_List = TRIM( Temp FROM  Friend_List );
       SET Leftstr = SUBSTRING_INDEX( Leftstr,'[',-1);
       INSERT INTO RelationInfo  SELECT name,profession,camp,Map,fighter_job_lv,scholar_job_lv,0  FROM character_base WHERE name = Leftstr;
    UNTIL ( Friend_List = '' or ISNULL(Friend_List)  )
  END REPEAT FL;

-- 黑名单
  BL:REPEAT
       SET  Leftstr = SUBSTRING_INDEX( BlackList_List,']#',1);
       SET Temp = concat( Leftstr,']#' );
       SET  BlackList_List = TRIM( Temp FROM  BlackList_List );
       SET Leftstr = SUBSTRING_INDEX( Leftstr,'[',-1);
       INSERT INTO RelationInfo  SELECT name,profession,camp,Map,fighter_job_lv,scholar_job_lv,1  FROM character_base WHERE name = Leftstr;
      UNTIL ( BlackList_List = ''  or ISNULL(BlackList_List)   )
  END REPEAT BL;
--  select Friend_List,BlackList_List;
  SELECT * FROM  RelationInfo;
  DROP TABLE RelationInfo;
 -- SELECT friend,blacklist FROM  user_relation WHERE user_id =  pra;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_SELECT_ROLE_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_SELECT_ROLE_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_SELECT_ROLE_INFO`(IN  InAccounts CHAR(32))
BEGIN
    -- SELECT  name,gender,camp,fighter_job_lv,scholar_job_lv,hair_style, id   FROM  character_base  WHERE  accounts = InAccounts     LIMIT  4;
     SELECT  name,gender,camp,fighter_job_lv,profession,hair_style,face_shap, id ,SF_GET_EQUIP(b.equip,0),SF_GET_EQUIP(b.equip,1),SF_GET_EQUIP(b.equip,2),SF_GET_EQUIP(b.equip,3),SF_GET_EQUIP(b.equip,4),SF_GET_EQUIP(b.equip,5),SF_GET_EQUIP(b.equip,6),SF_GET_EQUIP(b.equip,7),
                  SF_GET_EQUIP(b.equip,8),SF_GET_EQUIP(b.equip,9),SF_GET_EQUIP(b.equip,10),SF_GET_EQUIP(b.equip,11),SF_GET_EQUIP(b.equip,12),SF_GET_EQUIP(b.equip,13)
     FROM  character_base  a
     LEFT JOIN character_kitbag  b
     ON  a.id = b.user_id
     WHERE a.accounts = InAccounts
     LIMIT  4;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_SELECT_SAME_ROLE`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_SELECT_SAME_ROLE`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_SELECT_SAME_ROLE`(IN  RoleName CHAR(32) )
BEGIN
   DECLARE  Num    int  DEFAULT 0;
   DECLARE ReturnName CHAR(32)  DEFAULT  RoleName ;
  SELECT  count(name),name   FROM  character_base  WHERE  name = RoleName  group by name into  Num, ReturnName;
  SELECT  Num,ReturnName;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_UPDATE_ACTIVESKILL_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_UPDATE_ACTIVESKILL_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_UPDATE_ACTIVESKILL_INFO`(IN userid INT, IN INFO VARCHAR(2000))
BEGIN
  DECLARE a INT;
  SET a = 0;
  SELECT count(*)  FROM  user_magic  WHERE user_id=userid  INTO a;
   IF (a = 0) THEN
       INSERT INTO user_magic VALUES (userid, INFO );
  ELSE
      UPDATE user_magic SET magic_info  = INFO  WHERE  user_id = userid ;
 END IF;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_UPDATE_CONSORTIA_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_UPDATE_CONSORTIA_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_UPDATE_CONSORTIA_INFO`(IN iID INT, IN cname CHAR(64), IN cplacard CHAR(255),IN business1 CHAR(32),IN business2 CHAR(32),IN business3 CHAR(32) ,IN business4 CHAR(32),IN business5 CHAR(32),IN business6 CHAR(32),IN ifound INT,IN ibijou INT,IN  iLV INT,IN  ihonor INT,IN caltar_info VARCHAR(1000))
BEGIN
             DECLARE  Last_ID   INT UNSIGNED  DEFAULT 0;
            SET  @num = 0;
            SET @Last_ID = 0;
            SELECT COUNT(*) FROM consortia  WHERE  name = cname INTO @num;
            IF @num=0 THEN
                   INSERT INTO consortia
                   VALUES ( NULL, cname, cplacard, business1, business2, business3, business4, business5, business6, ifound, ibijou, iLV, ihonor,NULL);
                   SELECT LAST_INSERT_ID() from  consortia limit 1 INTO Last_ID;
                   SELECT cname ,Last_ID;
            ELSEIF @num  = 1 THEN
                 IF iID = -1 THEN
                   SELECT cname AS Name , Last_ID AS isOK;
                 ELSE
                  UPDATE consortia
                 SET name = cname,
                  placard = cplacard,
                  business_name1= business1,
                  business_name2= business2,
                  business_name3= business3,
                  business_name4= business4,
                  business_name5= business5,
                  business_name6= business6,
                  found   =  ifound,
                  bijou    =  ibijou,
                  LV         =  iLV,
                  honor   =  ihonor,
		 altar_info = caltar_info
                 WHERE id =  iID;
               END IF;

            END IF;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_UPDATE_KITBAG_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_UPDATE_KITBAG_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_UPDATE_KITBAG_INFO`(IN userid INT,IN Colum INT, IN Equip VARCHAR(4000))
BEGIN
 DECLARE compare TEXT DEFAULT '';
IF Colum = 1 THEN
-- 如果背包1激活,更新背包
        SELECT  kitbag_1 FROM  character_kitbag WHERE user_id = userid   INTO  compare;
        UPDATE character_kitbag SET  kitbag_1=Equip
     WHERE user_id = userid ;

    ELSEIF Colum = 2 THEN
 -- 如果背包2激活,更新背包
       SELECT  kitbag_2 FROM  character_kitbag WHERE user_id = userid   INTO  compare;
       IF  NOT ISNULL(compare) THEN
            UPDATE character_kitbag SET  kitbag_2=Equip
   WHERE user_id = userid ;
       END IF;
    ELSEIF Colum = 3  THEN
-- 如果背包3激活,更新背包
        SELECT  kitbag_3 FROM  character_kitbag WHERE user_id = userid  INTO  compare;
        IF  NOT ISNULL(compare) THEN
            UPDATE character_kitbag SET  kitbag_3=Equip
           WHERE user_id = userid ;
        END IF;
    ELSEIF Colum = 4 THEN
  -- 如果背包4激活,更新背包
        SELECT  kitbag_4 FROM  character_kitbag  WHERE user_id = userid  INTO  compare;
        IF  NOT ISNULL(compare) THEN
             UPDATE character_kitbag SET  kitbag_4=Equip
             WHERE user_id = userid ;
        END IF;
    ELSEIF  Colum = 5 THEN
  -- 如果背包5激活,更新背包
         --  SELECT  kitbag_5 FROM  character_kitbag  WHERE user_id = userid  INTO  compare;
        --  IF  NOT ISNULL(compare) THEN
             UPDATE character_kitbag SET  storage=Equip
              WHERE user_id = userid ;
       --   END IF;
   ELSE
 -- 更新装备
       UPDATE character_kitbag  SET  equip=Equip
       WHERE user_id = userid ;
   END IF;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_UPDATE_PASSIVESKILL_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_UPDATE_PASSIVESKILL_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_UPDATE_PASSIVESKILL_INFO`(IN userid INT, IN INFO VARCHAR(2000))
BEGIN
  DECLARE a INT;
  SET a = 0;
  SELECT count(*)  FROM  user_skill  WHERE user_id=userid  INTO a;
   IF (a = 0) THEN
       INSERT INTO user_skill  VALUES (userid, INFO );
  ELSE
      UPDATE user_skill SET skill_info  = INFO  WHERE  user_id = userid ;
 END IF;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_UPDATE_QUEST_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_UPDATE_QUEST_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_UPDATE_QUEST_INFO`(IN iID INT, IN cAllquest  LONGTEXT, IN  cActivestatus TEXT, IN cTimequest TEXT)
BEGIN
  DECLARE a INT;
  DECLARE  IniQuest LONGTEXT default "";
  SET a = 0;
  SELECT count(*)  FROM  user_quest  WHERE id=iID  INTO a;
   IF (a = 0) THEN
           INSERT INTO user_quest VALUES (iID, cAllquest, cActivestatus, cTimequest  );
  ELSE
         update user_quest
         set allqueststatus=cAllquest,activestatus=cActivestatus,timequest = cTimequest
         where id=iID;
 END IF;


END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_UPDATE_RELATION_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_UPDATE_RELATION_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_UPDATE_RELATION_INFO`(IN userid INT,IN Colum INT, IN Relation VARCHAR(2000))
BEGIN
    DECLARE NUM TINYINT DEFAULT 0;
    SELECT COUNT(*)  FROM  user_relation WHERE  user_id = userid INTO NUM;
     IF Colum =1 THEN
       IF NUM = 0 THEN
           INSERT INTO user_relation(user_id,friend) VALUES ( userid,Relation );
      ELSE
           UPDATE user_relation SET friend = Relation  WHERE user_id = userid;
       END IF;
    ELSEIF Colum=2 THEN
       IF NUM = 0 THEN
             INSERT INTO user_relation(user_id,blacklist) VALUES ( userid, Relation  );
        ELSE
            UPDATE user_relation SET blacklist = Relation  WHERE user_id = userid;
       END IF;

  END IF;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `PLOY_SP_UPDATE_ROLE_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `PLOY_SP_UPDATE_ROLE_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `PLOY_SP_UPDATE_ROLE_INFO`(IN UserID INT,IN iGender  CHAR(16),IN iCamp TINYINT,IN iProfession TINYINT, IN F_LV INT, IN S_LV INT,  IN F_Exp INT, IN S_Exp INT, IN Cur_HP INT , IN Cur_MP INT, IN Staus INT,  IN MapID TINYINT UNSIGNED, IN X FLOAT, IN Z FLOAT,IN iMoney INT,IN iStone INT,IN iConsortia INT, IN iConsortia_job INT,IN iSkillPoint INT,IN iSkillExp INT,IN iMaxHP INT,IN iMaxMP INT ,IN iPre INT,IN  tLastLoginTime  DATETIME, IN tLastUpdatetime TIME)
BEGIN
    UPDATE character_base
    SET  gender=iGender, camp=iCamp, profession=iProfession , fighter_job_lv=F_LV, scholar_job_lv=S_LV, fighter_job_exp=F_Exp, scholar_job_exp=S_Exp, curHP=Cur_HP,  curMP=Cur_MP,
           status=Staus,Map=MapID ,Pos_X=X,Pos_Z=Z,Money =iMoney ,Stone = iStone, consortia = iConsortia, consortia_job = iConsortia_job, SkillPoint = iSkillPoint, SkillExp = iSkillExp,MaxHP = iMaxHP,
          MaxMP= iMaxMP,prestige=iPre,LastLogin_time= tLastLoginTime, mutetime=  MINUTE(TIMEDIFF(CURRENT_TIME() ,tLastUpdatetime ))   +  HOUR(TIMEDIFF(CURRENT_TIME()  ,tLastUpdatetime)) *60 + mutetime
    WHERE    id = UserID ;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SET_GM`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SET_GM`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SET_GM`(IN iRoleName CHAR(32))
BEGIN
   UPDATE CHARACTER_BASE
   SET  gender = 'female',GM=1
   WHERE NAME = iRoleName;

    UPDATE character_kitbag
    SET Equip='[2590,,,,,,1,1,0]#[3190,,,,,,1,1,0]#[]#[]#[]#[]#[]#[2190,,,,,,1,1,0]#[2690,,,,,,1,1,0]#[3090,,,,,,1,1,0]#[2990,,,,,,1,1,0]#[2790,,,,,,1,1,0]#[3290,,,,,,1,1,0]#[3290,,,,,,1,1,0]#'
    WHERE user_id  = (SELECT ID FROM CHARACTER_BASE WHERE NAME= iRoleName);
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_DELETE_CONSORTIA_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_DELETE_CONSORTIA_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_DELETE_CONSORTIA_INFO`(IN ConsortiaID INT)
BEGIN
   DELETE FROM consortia WHERE id = ConsortiaID;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_DELETE_ROLE_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_DELETE_ROLE_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_DELETE_ROLE_INFO`(IN RoleID INT)
BEGIN
   DELETE FROM character_base WHERE id = RoleID;
   DELETE FROM character_kitbag WHERE user_id = RoleID;
   DELETE FROM user_magic WHERE user_id = RoleID;
   DELETE FROM user_skill WHERE user_id = RoleID;
   DELETE FROM user_quest WHERE id = RoleID;
   DELETE FROM user_relation WHERE user_id = RoleID;
   DELETE FROM user_altar WHERE user_id = RoleID;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_INSERT_ROLE`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_INSERT_ROLE`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_INSERT_ROLE`(IN  Accounts CHAR(32) , IN RoleName  CHAR(32),IN Gender  CHAR(16),IN iCamp INT,IN iProfession INT, IN F_LV INT, IN S_LV INT,  IN F_Exp INT, IN S_Exp INT, IN Cur_HP INT , IN Cur_MP INT, IN Staus INT,  IN iBelief INT, IN Hair_style INT,IN Face_shap INT)
BEGIN
   DECLARE Last_ID INT DEFAULT 0;
   DECLARE  Num    int  DEFAULT 0;
   DECLARE Map int DEFAULT 0;
   DECLARE Pos_X  float DEFAULT 0.00;
   DECLARE Pos_Z  float  DEFAULT 0.00;
   DECLARE GodSkillID int DEFAULT 0;
   DECLARE  GodSkill  varchar(32) default "";
   IF  iCamp  = 1 THEN
       SET  Map = 1, Pos_X =  98.0,Pos_Z = -178.0;     -- athen
   ELSEIF  iCamp = 0 THEN
       SET  Map = 51 , Pos_X =  -108.0, Pos_Z = -122.0;     -- sparta
   END IF;
  INSERT INTO character_base  VALUES ( NULL , Accounts ,RoleName  , Gender , 0,  iCamp, iProfession,  F_LV, S_LV,  F_Exp,S_Exp, Cur_HP, Cur_MP, Staus,  iBelief, 0, 0, 0, 0,0, 10,1,Hair_style,Face_shap,Map,Pos_X,Pos_Z,100,0,0,0,Cur_HP,Cur_MP, NOW(),'00-00-00 00:00:00 ',0);
  SELECT LAST_INSERT_ID() from  character_base limit 1 INTO Last_ID;
  INSERT INTO character_kitbag( user_id ) VALUES ( Last_ID );
  INSERT INTO user_quest values ( LAST_ID,'','','');
 -- default  Prop
   IF  iCamp  = 1 THEN -- athen
      CALL SP_UPDATE_KITBAG_INFO(Last_ID,1,'[4000,,,,,,0,10,1,1,0]#[4030,,,,,,0,10,1,1,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
   ELSEIF  iCamp = 0 THEN -- sparta
       CALL SP_UPDATE_KITBAG_INFO(Last_ID,1,'[4000,,,,,,0,10,1,1,0]#[4030,,,,,,0,10,1,1,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
   END IF;

  -- SET GodSkill = CAST( 5000+iBelief  as char );
  -- SET GodSkill = CONCAT( '[',GodSkill,']#' );


-- default skill and magic
  IF iProfession = 0 THEN
     -- SET GodSkill = CONCAT( GodSkill, '[0,0]#[200,0]#' );
     IF  iCamp  = 0 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[0,0]#[3063,0]#' );
     ELSEIF  iCamp = 1 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[0,0]#[3061,0]#' );
     END IF;

     CALL SP_UPDATE_PASSIVESKILL_INFO( Last_ID,  '[0,0]#[1,0]#[2,0]#[3,0]#[4,0]#[5,0]#[6,0]#[7,0]#[8,0]#[9,0]#[10,0]#[11,0]#[12,0]#[13,0]#[14,0]#[15,0]#[16,0]#[17,0]#'  );
     CALL SP_UPDATE_KITBAG_INFO(Last_ID,6,'[]#[]#[]#[2100,,,,,,1,1,1,1,0]#[]#[]#[2900,,,,,,1,1,1,1,0]#[]#[]#[]#[1000,,,,,,1,1,1,1,0]#[2000,,,,,,1,1,1,1,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
     CALL SP_UPDATE_ALTAR_INFO(Last_ID,0,0,0,0,0,0,0,0,0,0,0,0);
  ELSEIF iProfession=1 THEN
     -- SET GodSkill = CONCAT( GodSkill, '[1000,0]#[1200,0]#' );
     IF  iCamp  = 0 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[250,0]#[3063,0]#' );
     ELSEIF  iCamp = 1 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[250,0]#[3061,0]#' );
     END IF;

     CALL SP_UPDATE_PASSIVESKILL_INFO( Last_ID,  '[50,0]#[51,0]#[52,0]#[53,0]#[54,0]#[55,0]#[56,0]#[57,0]#[58,0]#[59,0]#[60,0]#[61,0]#[62,0]#[63,0]#[64,0]#[65,0]#[66,0]#[67,0]#' );
     CALL SP_UPDATE_KITBAG_INFO(Last_ID,6,'[]#[]#[]#[2100,,,,,,1,1,1,1,0]#[]#[]#[2900,,,,,,1,1,1,1,0]#[]#[]#[]#[1400,,,,,,1,1,1,1,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
     CALL SP_UPDATE_ALTAR_INFO(Last_ID,0,0,0,0,0,0,0,0,0,0,0,0);
 ELSEIF iProfession=2 THEN
     -- SET GodSkill = CONCAT( GodSkill, '[1000,0]#[1200,0]#' );
     IF  iCamp  = 0 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[800,0]#[3063,0]#' );
     ELSEIF  iCamp = 1 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[800,0]#[3061,0]#' );
     END IF;

     CALL SP_UPDATE_PASSIVESKILL_INFO( Last_ID,   '[150,0]#[151,0]#[152,0]#[153,0]#[154,0]#[155,0]#[156,0]#[157,0]#[158,0]#[159,0]#[160,0]#[161,0]#[162,0]#[163,0]#[164,0]#[165,0]#[166,0]#[167,0]#' );
     CALL SP_UPDATE_KITBAG_INFO(Last_ID,6,'[]#[]#[]#[2200,,,,,,1,1,1,1,0]#[]#[]#[2900,,,,,,1,1,1,1,0]#[]#[]#[]#[1700,,,,,,1,1,1,1,0]#[2000,,,,,,1,1,1,1,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
     CALL SP_UPDATE_ALTAR_INFO(Last_ID,0,0,0,0,0,0,0,0,0,0,0,0);
 ELSEIF  iProfession=3 THEN
     -- SET GodSkill = CONCAT( GodSkill, '[1000,0]#[1200,0]#' );
     IF  iCamp  = 0 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[500,0]#[3063,0]#' );
     ELSEIF  iCamp = 1 THEN
         CALL SP_UPDATE_ACTIVESKILL_INFO( Last_ID, '[500,0]#[3061,0]#' );
     END IF;

     CALL SP_UPDATE_PASSIVESKILL_INFO( Last_ID,  '[100,0]#[101,0]#[102,0]#[103,0]#[104,0]#[105,0]#[106,0]#[107,0]#[108,0]#[109,0]#[110,0]#[111,0]#[112,0]#[113,0]#[114,0]#[115,0]#[116,0]#[117,0]#' );
     CALL SP_UPDATE_KITBAG_INFO(Last_ID,6,'[]#[]#[]#[2200,,,,,,1,1,1,1,0]#[]#[]#[2900,,,,,,1,1,1,1,0]#[]#[]#[]#[1800,,,,,,1,1,1,1,0]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#[]#');
     CALL SP_UPDATE_ALTAR_INFO(Last_ID,0,0,0,0,0,0,0,0,0,0,0,0);
  END IF;


END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_INSERT_ROLE_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_INSERT_ROLE_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_INSERT_ROLE_INFO`(IN  Accounts CHAR(32) , IN RoleName  CHAR(32),IN Gender  CHAR(16),IN iCamp INT,IN iProfession INT, IN F_LV INT, IN S_LV INT,  IN F_Exp INT, IN S_Exp INT, IN Cur_HP INT , IN Cur_MP INT, IN Staus INT,  IN iBelief INT, IN Hair_style INT,IN Face_shap INT)
    DETERMINISTIC
BEGIN
   DECLARE  Num    int  DEFAULT 0;
   DECLARE  Last_ID    int  DEFAULT 0;
   DECLARE ReturnName CHAR(32)  DEFAULT  RoleName ;
   SELECT  count(*)   FROM  character_base  WHERE  name = RoleName  into   Num;
   IF Num = 0 THEN
      CALL  SP_INSERT_ROLE(Accounts , RoleName , Gender, iCamp ,iProfession ,F_LV ,S_LV ,F_Exp ,S_Exp ,Cur_HP ,Cur_MP ,Staus ,iBelief,Hair_style,Face_shap);
     SELECT max(id) from  character_base limit 1 INTO Last_ID;
  END IF;
   SELECT  Num,ReturnName,Last_ID,Num;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_SELECT_ACCOUNT_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_SELECT_ACCOUNT_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_SELECT_ACCOUNT_INFO`(IN  Name CHAR(32) , IN Password  CHAR(32))
BEGIN
     DECLARE  num  TINYINT DEFAULT 0;
    SELECT count(*)  FROM    accounts_info  WHERE  accounts = Name  AND  password = Password  INTO  num;
    SELECT num;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_SELECT_ACTIVESKILL_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_SELECT_ACTIVESKILL_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_SELECT_ACTIVESKILL_INFO`(IN  pra  INT)
BEGIN
   SELECT magic_info FROM user_magic WHERE user_id = pra;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_SELECT_ALTAR_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_SELECT_ALTAR_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_SELECT_ALTAR_INFO`(IN id INT)
BEGIN
	SELECT * FROM user_altar WHERE user_id = id;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_SELECT_CONSORTIA_BASE_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_SELECT_CONSORTIA_BASE_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_SELECT_CONSORTIA_BASE_INFO`()
BEGIN
    -- SELECT id,name,placard,business_name1,business_name2,business_name3,business_name4,attribute1,attribute2,attribute3,attribute4 from consortia where id = 1;
     select *  from consortia;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_SELECT_CONSORTIA_MEMBER_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_SELECT_CONSORTIA_MEMBER_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_SELECT_CONSORTIA_MEMBER_INFO`(IN  pra  CHAR(64))
BEGIN
    SELECT * 
    FROM VIEW_CONSORTIA 
    WHERE  Consortia_ID IN 
               ( SELECT id FROM consortia WHERE name = pra   ) ;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_SELECT_KITBAG_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_SELECT_KITBAG_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_SELECT_KITBAG_INFO`(IN userid INT)
BEGIN
   DECLARE  bag1 VARCHAR(4000);
   DECLARE  bag2 VARCHAR(4000);
   DECLARE  bag3 VARCHAR(4000);
   DECLARE  bag4 VARCHAR(4000);
   DECLARE  bag5 VARCHAR(4000);
   DECLARE  item  VARCHAR(2000);


   DECLARE  TmpEquip   BLOB;
   DECLARE  i  INT DEFAULT 0;
   DECLARE Num INT DEFAULT 0;
  CREATE TEMPORARY TABLE IF NOT EXISTS bag
  (
         bag   BLOB  NULL
  )  ;

  SET   Num = 0;

  SELECT COUNT(equip),kitbag_1, kitbag_2, kitbag_3, kitbag_4, storage
  FROM  character_kitbag
  WHERE user_id = userid
  GROUP BY equip
  INTO  Num,bag1,bag2,bag3,bag4,bag5;

#SELECT * FROM  bag;
SELECT kitbag_1, kitbag_2, kitbag_3, kitbag_4, storage
FROM  character_kitbag
WHERE user_id = userid;


DROP TABLE  bag;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_SELECT_PASSIVESKILL_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_SELECT_PASSIVESKILL_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_SELECT_PASSIVESKILL_INFO`(IN  pra  INT)
BEGIN
   SELECT skill_info FROM user_skill WHERE user_id = pra;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_SELECT_PLACARD`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_SELECT_PLACARD`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_SELECT_PLACARD`()
BEGIN
	SELECT placardtype,sendtype,((DAY(starttime)-DAY(NOW()))*86400 + (TIME_TO_SEC(TIME(starttime)) - TIME_TO_SEC(CURTIME())))  AS StartTime ,((DAY(endtime)-DAY(NOW()))*86400 + (TIME_TO_SEC(TIME(endtime)) - TIME_TO_SEC(CURTIME()))) AS EndTime ,intervaltime,placardinfo
	FROM   gm_placard;

	DELETE FROM gm_placard WHERE endtime < NOW() OR gm_placard.placardtype = 0;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_SELECT_PLAYER_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_SELECT_PLAYER_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_SELECT_PLAYER_INFO`(IN  pra  INT)
BEGIN
    SELECT character_base.*,character_kitbag.equip  FROM character_base,character_kitbag WHERE character_base.id = pra and character_kitbag.user_id = pra;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_SELECT_QUEST_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_SELECT_QUEST_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_SELECT_QUEST_INFO`(IN  pra  INT)
BEGIN
    SELECT  user_quest.*,character_base.profession, character_base.camp, character_base.fighter_job_lv,character_base.scholar_job_lv,character_base.belief
    from user_quest,character_base
    where user_quest.id = character_base.id and character_base.id  = pra ;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_SELECT_RELATION_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_SELECT_RELATION_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_SELECT_RELATION_INFO`(IN  pra  INT)
BEGIN

   DECLARE Num INT DEFAULT 0;
   DECLARE Friend_List VARCHAR(2000) DEFAULT '';
   DECLARE BlackList_List  VARCHAR(2000) DEFAULT '';
   DECLARE  Leftstr CHAR(32) default "";
   DECLARE  Temp  CHAR(64) default "";

   CREATE TEMPORARY TABLE IF NOT EXISTS RelationInfo
   (
         Name CHAR(32)  NOT  NULL,
         Business TINYINT UNSIGNED,
         Camp TINYINT UNSIGNED ,
         City  TINYINT UNSIGNED ,
         F_LV  TINYINT UNSIGNED ,
         S_LV   TINYINT UNSIGNED ,
         Style  TINYINT UNSIGNED
   )  ;
  SELECT COUNT(*) FROM user_relation WHERE user_id = pra INTO  Num;
  IF NUM=1 THEN
    SELECT friend,blacklist FROM user_relation WHERE user_id = pra  INTO  Friend_List,BlackList_List;
 END IF;
 -- 好友
    FL:REPEAT
       SET  Leftstr = SUBSTRING_INDEX( Friend_List,']#',1);
       SET Temp = concat( Leftstr,']#' );
       SET  Friend_List = TRIM( Temp FROM  Friend_List );
       SET Leftstr = SUBSTRING_INDEX( Leftstr,'[',-1);
       INSERT INTO RelationInfo  SELECT name,profession,camp,Map,fighter_job_lv,scholar_job_lv,0  FROM character_base WHERE name = Leftstr;
    UNTIL ( Friend_List = '' or ISNULL(Friend_List)  )
  END REPEAT FL;

-- 黑名单
  BL:REPEAT
       SET  Leftstr = SUBSTRING_INDEX( BlackList_List,']#',1);
       SET Temp = concat( Leftstr,']#' );
       SET  BlackList_List = TRIM( Temp FROM  BlackList_List );
       SET Leftstr = SUBSTRING_INDEX( Leftstr,'[',-1);
       INSERT INTO RelationInfo  SELECT name,profession,camp,Map,fighter_job_lv,scholar_job_lv,1  FROM character_base WHERE name = Leftstr;
      UNTIL ( BlackList_List = ''  or ISNULL(BlackList_List)   )
  END REPEAT BL;
--  select Friend_List,BlackList_List;
  SELECT * FROM  RelationInfo;
  DROP TABLE RelationInfo;
 -- SELECT friend,blacklist FROM  user_relation WHERE user_id =  pra;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_SELECT_ROLE_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_SELECT_ROLE_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_SELECT_ROLE_INFO`(IN  InAccounts CHAR(32))
BEGIN
    -- SELECT  name,gender,camp,fighter_job_lv,scholar_job_lv,hair_style, id   FROM  character_base  WHERE  accounts = InAccounts     LIMIT  4;
     SELECT  name,gender,camp,fighter_job_lv,profession,hair_style,face_shap, id ,SF_GET_EQUIP(b.equip,0),SF_GET_EQUIP(b.equip,1),SF_GET_EQUIP(b.equip,2),SF_GET_EQUIP(b.equip,3),SF_GET_EQUIP(b.equip,4),SF_GET_EQUIP(b.equip,5),SF_GET_EQUIP(b.equip,6),SF_GET_EQUIP(b.equip,7),
                  SF_GET_EQUIP(b.equip,8),SF_GET_EQUIP(b.equip,9),SF_GET_EQUIP(b.equip,10),SF_GET_EQUIP(b.equip,11),SF_GET_EQUIP(b.equip,12),SF_GET_EQUIP(b.equip,13)
     FROM  character_base  a
     LEFT JOIN character_kitbag  b
     ON  a.id = b.user_id
     WHERE a.accounts = InAccounts
     LIMIT  4;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_SELECT_SAME_ROLE`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_SELECT_SAME_ROLE`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_SELECT_SAME_ROLE`(IN  RoleName CHAR(32) )
BEGIN
   DECLARE  Num    int  DEFAULT 0;
   DECLARE ReturnName CHAR(32)  DEFAULT  RoleName ;
  SELECT  count(name),name   FROM  character_base  WHERE  name = RoleName  group by name into  Num, ReturnName;
  SELECT  Num,ReturnName;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_UPDATE_ACTIVESKILL_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_UPDATE_ACTIVESKILL_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_UPDATE_ACTIVESKILL_INFO`(IN userid INT, IN INFO VARCHAR(2000))
BEGIN
  DECLARE a INT;
  SET a = 0;
  SELECT count(*)  FROM  user_magic  WHERE user_id=userid  INTO a;
   IF (a = 0) THEN
       INSERT INTO user_magic VALUES (userid, INFO );
  ELSE
      UPDATE user_magic SET magic_info  = INFO  WHERE  user_id = userid ;
 END IF;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_UPDATE_ALTAR_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_UPDATE_ALTAR_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_UPDATE_ALTAR_INFO`(IN id INT, IN altar_1 INT, IN altar_2 INT, IN altar_3 INT, IN altar_4  INT, IN altar_5 INT, IN altar_6 INT, IN altar_7 INT, IN altar_8 INT, IN altar_9 INT, IN altar_10 INT, IN altar_11  INT, IN altar_12  INT)
BEGIN
  DECLARE a INT;
  SET a = 0;
  SELECT count(*)  FROM  user_altar  WHERE user_id=id  INTO a;
   IF (a = 0) THEN
       INSERT INTO user_altar  VALUES (id, 0,0,0,0,0,0,0,0,0,0,0,0 );
  ELSE
      UPDATE user_altar SET altar1=altar_1, altar2=altar_2,altar3=altar_3,altar4=altar_4,altar5=altar_5,altar6=altar_6,altar7=altar_7,altar8=altar_8,altar9=altar_9,altar10=altar_10,altar11=altar_11,altar12=altar_12  WHERE  user_id=id ;
 END IF;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_UPDATE_CONSORTIA_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_UPDATE_CONSORTIA_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_UPDATE_CONSORTIA_INFO`(IN iID INT, IN cname CHAR(64), IN cplacard CHAR(255),IN business1 CHAR(32),IN business2 CHAR(32),IN business3 CHAR(32) ,IN business4 CHAR(32),IN business5 CHAR(32),IN business6 CHAR(32),IN ifound INT,IN ibijou INT,IN  iLV INT,IN  ihonor INT,IN caltar_info  VARCHAR(1000))
BEGIN
             DECLARE  Last_ID   INT UNSIGNED  DEFAULT 0;
            SET  @num = 0;
            SET @Last_ID = 0;
            SELECT COUNT(*) FROM consortia  WHERE  name = cname INTO @num;
            IF @num=0 THEN
                   INSERT INTO consortia
                   VALUES ( NULL, cname, cplacard, business1, business2, business3, business4, business5, business6, ifound, ibijou, iLV, ihonor,NULL);
                   SELECT LAST_INSERT_ID() from  consortia limit 1 INTO Last_ID;
                   SELECT cname ,Last_ID;
            ELSEIF @num  = 1 THEN
                 IF iID = -1 THEN
                   SELECT cname AS Name , Last_ID AS isOK;
                 ELSE
                  UPDATE consortia
                 SET name = cname,
                  placard = cplacard,
                  business_name1= business1,
                  business_name2= business2,
                  business_name3= business3,
                  business_name4= business4,
                  business_name5= business5,
                  business_name6= business6,
                  found   =  ifound,
                  bijou    =  ibijou,
                  LV         =  iLV,
                  honor   =  ihonor,
		 altar_info = caltar_info
                 WHERE id =  iID;
               END IF;

            END IF;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_UPDATE_KITBAG_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_UPDATE_KITBAG_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_UPDATE_KITBAG_INFO`(IN userid INT,IN Colum INT, IN Equip VARCHAR(4000))
BEGIN
 DECLARE compare TEXT DEFAULT '';
IF Colum = 1 THEN
-- 如果背包1激活,更新背包
        SELECT  kitbag_1 FROM  character_kitbag WHERE user_id = userid   INTO  compare;
        UPDATE character_kitbag SET  kitbag_1=Equip
     WHERE user_id = userid ;

    ELSEIF Colum = 2 THEN
 -- 如果背包2激活,更新背包
       SELECT  kitbag_2 FROM  character_kitbag WHERE user_id = userid   INTO  compare;
       IF  NOT ISNULL(compare) THEN
            UPDATE character_kitbag SET  kitbag_2=Equip
   WHERE user_id = userid ;
       END IF;
    ELSEIF Colum = 3  THEN
-- 如果背包3激活,更新背包
        SELECT  kitbag_3 FROM  character_kitbag WHERE user_id = userid  INTO  compare;
        IF  NOT ISNULL(compare) THEN
            UPDATE character_kitbag SET  kitbag_3=Equip
           WHERE user_id = userid ;
        END IF;
    ELSEIF Colum = 4 THEN
  -- 如果背包4激活,更新背包
        SELECT  kitbag_4 FROM  character_kitbag  WHERE user_id = userid  INTO  compare;
        IF  NOT ISNULL(compare) THEN
             UPDATE character_kitbag SET  kitbag_4=Equip
             WHERE user_id = userid ;
        END IF;
    ELSEIF  Colum = 5 THEN
  -- 如果背包5激活,更新背包
         --  SELECT  kitbag_5 FROM  character_kitbag  WHERE user_id = userid  INTO  compare;
        --  IF  NOT ISNULL(compare) THEN
             UPDATE character_kitbag SET  storage=Equip
              WHERE user_id = userid ;
       --   END IF;
   ELSE
 -- 更新装备
       UPDATE character_kitbag  SET  equip=Equip
       WHERE user_id = userid ;
   END IF;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_UPDATE_PASSIVESKILL_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_UPDATE_PASSIVESKILL_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_UPDATE_PASSIVESKILL_INFO`(IN userid INT, IN INFO VARCHAR(2000))
BEGIN
  DECLARE a INT;
  SET a = 0;
  SELECT count(*)  FROM  user_skill  WHERE user_id=userid  INTO a;
   IF (a = 0) THEN
       INSERT INTO user_skill  VALUES (userid, INFO );
  ELSE
      UPDATE user_skill SET skill_info  = INFO  WHERE  user_id = userid ;
 END IF;

END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_UPDATE_QUEST_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_UPDATE_QUEST_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_UPDATE_QUEST_INFO`(IN iID INT, IN cAllquest  LONGTEXT, IN  cActivestatus TEXT, IN cTimequest TEXT)
BEGIN
  DECLARE a INT;
  DECLARE  IniQuest LONGTEXT default "";
  SET a = 0;
  SELECT count(*)  FROM  user_quest  WHERE id=iID  INTO a;
   IF (a = 0) THEN
           INSERT INTO user_quest VALUES (iID, cAllquest, cActivestatus, cTimequest  );
  ELSE
         update user_quest
         set allqueststatus=cAllquest,activestatus=cActivestatus,timequest = cTimequest
         where id=iID;
 END IF;


END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_UPDATE_RELATION_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_UPDATE_RELATION_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_UPDATE_RELATION_INFO`(IN userid INT,IN Colum INT, IN Relation VARCHAR(2000))
BEGIN
    DECLARE NUM TINYINT DEFAULT 0;
    SELECT COUNT(*)  FROM  user_relation WHERE  user_id = userid INTO NUM;
     IF Colum =1 THEN
       IF NUM = 0 THEN
           INSERT INTO user_relation(user_id,friend) VALUES ( userid,Relation );
      ELSE
           UPDATE user_relation SET friend = Relation  WHERE user_id = userid;
       END IF;
    ELSEIF Colum=2 THEN
       IF NUM = 0 THEN
             INSERT INTO user_relation(user_id,blacklist) VALUES ( userid, Relation  );
        ELSE
            UPDATE user_relation SET blacklist = Relation  WHERE user_id = userid;
       END IF;

  END IF;
END;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_UPDATE_ROLE_INFO`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_UPDATE_ROLE_INFO`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `SP_UPDATE_ROLE_INFO`(IN UserID INT,IN iGender  CHAR(16),IN iCamp TINYINT,IN iProfession TINYINT, IN F_LV INT, IN S_LV INT,  IN F_Exp INT, IN S_Exp INT, IN Cur_HP INT , IN Cur_MP INT, IN Staus INT,  IN MapID TINYINT UNSIGNED, IN X FLOAT, IN Z FLOAT,IN iMoney INT,IN iStone INT,IN iConsortia INT, IN iConsortia_job INT,IN iConsortiaContribute INT,IN iStoreNum INT,IN iBagNum INT,IN iSkillPoint INT,IN iSkillExp INT,IN iMaxHP INT,IN iMaxMP INT ,IN iPre INT,IN  tLastLoginTime  DATETIME, IN tLastUpdatetime TIME)
BEGIN
    UPDATE character_base
    SET  gender=iGender, camp=iCamp, profession=iProfession , fighter_job_lv=F_LV, scholar_job_lv=S_LV, fighter_job_exp=F_Exp, scholar_job_exp=S_Exp, curHP=Cur_HP,  curMP=Cur_MP,
           status=Staus,Map=MapID ,Pos_X=X,Pos_Z=Z,Money =iMoney ,Stone = iStone, consortia = iConsortia, consortia_job = iConsortia_job, consortia_contribute = iConsortiaContribute ,store_num = iStoreNum ,bag_num = iBagNum ,SkillPoint = iSkillPoint, SkillExp = iSkillExp,MaxHP = iMaxHP,
          MaxMP= iMaxMP,prestige=iPre,LastLogin_time= tLastLoginTime, mutetime=  MINUTE(TIMEDIFF(CURRENT_TIME() ,tLastUpdatetime ))   +  HOUR(TIMEDIFF(CURRENT_TIME()  ,tLastUpdatetime)) *60 + mutetime
    WHERE    id = UserID ;
END;;
DELIMITER ;

-- ----------------------------
-- Function structure for `SF_GET_EQUIP`
-- ----------------------------
DROP FUNCTION IF EXISTS `SF_GET_EQUIP`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `SF_GET_EQUIP`(MyEquip VARCHAR(2000), Pos INT ) RETURNS int(4)
BEGIN
    DECLARE  Leftstr VARCHAR(2000) default "";
    IF NOT ISNULL(MyEquip) THEN
      SET  Leftstr = SUBSTRING_INDEX(MyEquip,'#',Pos+1);
      IF Pos > 0 THEN
         SET  Leftstr = SUBSTRING_INDEX(Leftstr,'#',-1);
     END IF;
  IF LOCATE(',',Leftstr) >0 THEN
SET  Leftstr = SUBSTRING_INDEX(Leftstr,',',1);
SET Leftstr = SUBSTRING_INDEX(Leftstr,'[',-1);
 RETURN  CAST( Leftstr as SIGNED );
 ELSE
            RETURN -1;
END IF;
ELSE
       RETURN -1;
END IF;
END;;
DELIMITER ;
