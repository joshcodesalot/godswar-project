#ifndef _ERRORINFO_H_
#define _ERRORINFO_H_

#include "stdafx.h"

enum eError
{
	NO_MSG_ERRO	= 0,

    //_MSG_CREATE_ROLE error  
	MSG_ERRO_0001	= 0x0001,    		//成功
	MSG_ERRO_0002	,    		        //字符串长度限制
	MSG_ERRO_0003	,    		        //包含非法字符
	MSG_ERRO_0004	,    		        //角色个数到达上限
	MSG_ERRO_0005	,    		        //重复注册
	MSG_ERRO_0006	,					//你是会长，为了你的会员着想 ，请删除公会后在删除角色
	MSG_ERRO_0007	,					//删除角色失败
	MSG_ERRO_0008	,					//删除角色成功

    //_MSG_DELETE_ROLE error
	MSG_ERRO_0011	= 0x0011,

    //_MSG_WALK_BEGIN error
	MSG_ERRO_0020	= 0x0020,          //摆摊不能移动

	//_MSG_WALK_END error
	MSG_ERRO_002F	= 0x002F,    

	//_MSG_AVATAR_INFO error
	MSG_ERRO_003E	= 0x003E,

	//_MSG_FIGHT error
	MSG_ERRO_004D	= 0x004D, 

	//_MSG_ATTACK error
	MSG_ERRO_005C = 0x005C,        //目标不存在

	//_MSG_SKILL
	MSG_ERRO_006A	= 0x006A,		         //摆摊中
	MSG_ERRO_006B	= 0x006B,		 //距离太远
	MSG_ERRO_006C	,		         //魔法不足
	MSG_ERRO_006D	,		         //冷却中
	MSG_ERRO_006E	,		         //装备不配
	MSG_ERRO_006F	,		         //无此技能
	

	//_MSG_LEARN
	MSG_ERRO_007A	= 0x007A,		

	//_MSG_SKILLBACKUP
	MSG_ERRO_0089	= 0x0089,

	//_MSG_SKILL_UPGRADE
	MSG_ERRO_0098	= 0x0098,       //不满足升级条件

	//_MSG_SKILL_INTERRUPT
	MSG_ERRO_00A7	= 0x00A7,

	//_MSG_EFFECT
	MSG_ERRO_00B6	= 0x00B6,

	//_MSG_TALK
	MSG_ERRO_00C5	= 0x00C5,

	//_MSG_PICKUPDROPS
	MSG_ERRO_00D4	= 0x00D4,    

	//_MSG_USEOREQUIP
	MSG_ERRO_00E3	= 0x00E3,      

	//_MSG_MOVEITEM
	MSG_ERRO_00F2	= 0x00F2,

	//_MSG_SELL
	MSG_ERRO_0101	= 0x0101,

    //_MSG_STORAGEITEM
	MSG_ERRO_0110	= 0x0110,

	//_MSG_STALL
	MSG_ERRO_011F	= 0x011F,

	//_MSG_STALLITEM
	MSG_ERRO_012E	= 0x012E,

	//_MSG_STALLADDITEM
	MSG_ERRO_013D	= 0x013D,

	//_MSG_STALLDELITEM
	MSG_ERRO_014C	= 0x014C,

	//_MSG_STALLBUYITEM
	MSG_ERRO_015A	= 0x015A,		//物品不存在 goods->item == NULL
	MSG_ERRO_015B	= 0x015B,		//摆摊成功但是不返回消息
	MSG_ERRO_015C	,				//该物品已经扯回或被人买走，请向摊主咨询
	MSG_ERRO_015D	,				//金额不足

	MSG_ERRO_015F	,				//非法处理摆摊物品，疑似作弊

	//_MSG_TALKNPC
	MSG_ERRO_016A	= 0x016A,       //阵营不符
	MSG_ERRO_016B	= 0x016B,		//检查通过
	MSG_ERRO_016C	= 0x016C,		//不是会长，无法接
	MSG_ERRO_016D	= 0x016D,		//没有组队

	//_MSG_NPCDATA
	MSG_ERRO_0179	= 0x0179,

	//_MSG_NPCSELL
	MSG_ERRO_0188	= 0x0188,		//正在更新最新的物品列表

	//_MSG_MALLSELL
	MSG_ERRO_0197	= 0x0197,		//正在交易中，不能购买物品
	MSG_ERRO_0198	= 0x0198,		//你没有足够的货币
	MSG_ERRO_0199	= 0x0199,		//包裹没有足够的空间
	MSG_ERRO_019A	= 0x019A,

	//_MSG_ASSOCIATIONSELL
	MSG_ERRO_01A6	= 0x01A6,

	//_MSG_MALLITEMDATA
	MSG_ERRO_01B5	= 0x01B5,

	//_MSG_ASSOCIATIONITEMDATA
	MSG_ERRO_01C4	= 0x01C4,

	//_MSG_FINDQUEST
	MSG_ERRO_01D3	= 0x01D3,

	//_MSG_NPCACCEPTQUEST
	MSG_ERRO_01E2	= 0x01E2,

	//_MSG_NPCCANCELQUEST
	MSG_ERRO_01F1	= 0x01F1,

	//_MSG_NPCCOMPLETEQUEST
	MSG_ERRO_0200	= 0x0200,		//没有物品
	MSG_ERRO_0201	,		        //物品已满

	//_MSG_NPCQUESTVIEW
	MSG_ERRO_020F	= 0x020F,

    //_MSG_RELATION_REQUEST, 
	MSG_ERRO_021E	= 0x021E,      //玩家不在线
	MSG_ERRO_021F   ,              //不同阵营
	MSG_ERRO_0220,				   //该玩家已经是你的好友
	MSG_ERRO_0221,				   //该玩家已经在你的黑名单列表中
	MSG_ERRO_0222,				   //好友列表已满
	MSG_ERRO_0223,				   //黑名单列表已满
	MSG_ERRO_O224,				   //你已经把该玩家加入黑名单中
	MSG_ERRO_0225,				   //该玩家已经在你的好友列表中
	
    //_MSG_RELATION_RESPONSE,
    MSG_ERRO_022D	= 0x022D,

	//_MSG_RELATION_DELETE,
	MSG_ERRO_023C	= 0x023C,

	//_MSG_TRADE,  
	MSG_ERRO_024B	= 0x024B,		//玩家忙
	MSG_ERRO_024C	= 0x024C,		//道具栏不够,交易结束
	MSG_ERRO_024D	= 0x024D,		//没有足够的金钱
	MSG_ERRO_024E	= 0x024E,		//没有足够的宝石
	MSG_ERRO_024F	= 0x024F,		//交易取消
	

	//_MSG_TRADE_MONEY,             	
	MSG_ERRO_025A	= 0x025A,

	//_MSG_TRADE_ADDITEM,           	
	MSG_ERRO_0269	= 0x0269,

	//_MSG_TRADE_REMOVEITEM,        	
	MSG_ERRO_0278	= 0x0278,

	//_MSG_EQUIPFORGE,              	
	MSG_ERRO_0287	= 0x0287,

	//_MSG_EQUIPFORGE_EQUIP,        	
	MSG_ERRO_0296	= 0x0296,

	//_MSG_EQUIPFORGE_MATERIAL,     	
	MSG_ERRO_02A5	= 0x02A5,

	//_MSG_EQUIPFORGE_EQUIPCANCEL,  	
	MSG_ERRO_02B4	= 0x02B4,

	//_MSG_EQUIPFORGE_MATERIALCANCEL,   
	MSG_ERRO_02C3	= 0x02C3,

	//_MSG_EQUIPFORGE_CANCEL,           
	MSG_ERRO_02D2	= 0x02D2,

	//_MSG_TEAM_INVITE,                 
	MSG_ERRO_02E1	= 0x02E1,		//

	//_MSG_TEAM_REJECT,                 
	MSG_ERRO_02F0	= 0x02F0,

	//_MSG_TEAM_ADD,                    
	MSG_ERRO_02FF	= 0x02FF,

	//_MSG_TEAM_DELETE,                 
	MSG_ERRO_030E	= 0x030E,

	//_MSG_TEAM_REPLACELEADER,          
	MSG_ERRO_031D	= 0x031D,

	//_MSG_TEAM_DISSOLVE,               
	MSG_ERRO_032C	= 0x032C,

	//_MSG_TEAM_LEAVE,                  
	MSG_ERRO_033B	= 0x033B,

	//_MSG_BACKHOME,                    
	MSG_ERRO_034A	= 0x034A,

	//_MSG_CONSORTIA_CREATE,            
	MSG_ERRO_0359	= 0x0359,       //已有公会
	MSG_ERRO_035A	,               //资源不足
	MSG_ERRO_035B	,               //等级不够
	MSG_ERRO_035C	,               //该公会已存在
	MSG_ERRO_035D	,               //公会名字为空
	MSG_ERRO_035E   ,               //没有创建公会的道具 <天堂令>

	//_MSG_CONSORTIA_INVITE,            
	MSG_ERRO_0368	= 0x0368,       //被邀请者已有公会
	MSG_ERRO_0369	,               //不同阵营
	MSG_ERRO_036A	,               //没有权限
	MSG_ERRO_036B	,               //公会满员
	MSG_ERRO_036C	,               //已经是公会成员

	//_MSG_CONSORTIA_DISMISS,           
	MSG_ERRO_0377	= 0x0377,       //非会长

	//_MSG_CONSORTIA_RESPONSE,          
	MSG_ERRO_0386	= 0x0386,

	//_MSG_CONSORTIA_EXIT,              
	MSG_ERRO_0395	= 0x0395,       //非自己一人成员

	//_MSG_CONSORTIA_TEXT, 
	MSG_ERRO_03A4	= 0x03A4,      //非正副会长

	//_MSG_CONSORTIA_DUTY,              
	MSG_ERRO_03B3	= 0x03B3,

	//_MSG_CONSORTIA_MEMBER_DEL,        
	MSG_ERRO_03C2	= 0x03C2,     //理事以下职务没有权限
    MSG_ERRO_03C3	,             //不能删除自己平级或者高于自己的玩家
	MSG_ERRO_O3C4	,			  //公会已经最大等级
	MSG_ERRO_03C5	,			  //你还没有建立公会
	MSG_ERRO_03C6	,			  //公会升级成功
	MSG_ERRO_03C7	,			  //升级公会的资金或宝石不足

	//_MSG_CONSORTIA_LVUP,				
	MSG_ERRO_03D1	= 0x03D1,

	//_MSG_ALTAR_CREATE,				
	MSG_ERRO_03E0	= 0x03E0,

	//_MSG_ALTAR_LVUP,					
	MSG_ERRO_03EF	= 0x03EF,

	//_MSG_ALTAR_OBLATION,				
	MSG_ERRO_03FE	= 0x03FE,

	//_MSG_CRETIT_EXCHANGE,				
	MSG_ERO_040D	= 0x040D,

	//_MSG_GOLD_BOX
	MSG_ERRO_041C	= 0x041C,		//重复操作
	MSG_ERRO_041D	,		        //物品不全
	MSG_ERRO_041E	,		        //空格不足
	MSG_ERRO_041F	,		        //宝箱不匹配
	MSG_ERRO_0420	,		        //宝箱尚未开启

	//_MSG_EXPLORER_QUEST
	MSG_ERRO_41	= 0x042B,
};

#endif