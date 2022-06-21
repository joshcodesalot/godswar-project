//========================================================
//
//    Copyright (c) 2006,欢乐连线工作室
//    All rights reserved.
//
//    文件名称 ： DataCore.h
//    摘    要 ： 核心数据区模块
//    
//    当前版本 ： 1.00
//    作    者 ： 林德辉
//    完成日期 ： 2007-01-12
//
//========================================================


#pragma once


#define PLAYER_BEGIN				0
#define NPC_BEGIN					5000
#define MONSTER_BEGIN				10000

#define MAX_REGION					24				// 场景数量
#define MAX_REP_REGION				1				// 副本数量

#define ACADEMIC_PLAYER_MEM_ID					0			    // 在线玩家数量
#define ACADEMIC_NPC_MEM_ID						5000			// 游戏NPC数量
#define ACADEMIC_MONSTER_MEM_ID					10000			// 游戏怪物数量
#define ACADEMIC_BOSS_MEM_ID					100				// 游戏BOSS数量

#define ACTUAL_PLAYER_ONLINE	    1500			// 在线玩家数量
#define ACTUAL_NPC_ONLINE		    3000			// 游戏NPC数量
#define ACTUAL_MONSTER_ONLINE		20000			// 游戏怪物数量
#define ACTUAL_BOSS_ONLINE			100				// 游戏BOSS数量



#define	CORPSE_LOST_TIME			5				// 尸体消失时间
#define CORPSE_ITEM_TIME			20				// 道具在尸体上时间

#define FIGHT_TIME					10				// 战斗状态持续时间(sec)
#define FIGHT_INTERVAL				1700			// 攻击间隔(ms)
#define REFRESH_TIME				30				// 消失刷新时间(sec)
#define MONSTER_AI_TIME				20				// 怪物AI触发时间周期(sec)

#define MONSTER_FACTION				2				//怪物阵营

//角色对象操作部分
#define ERROR_JOIN_PLAYER_FULL				2
#define ERROR_JOIN_PLAYER_OUT				3
#define ERROR_REMOVE_PLAYER_OUT				3

class CRegion;
class CRepRegion;
class CGameObject;
class CPlayer;
class CNPC;
class CMonster;
class CObstacle;

// ========================================================================
/*
数据部分
*/
// ========================================================================

 //场景切换用结构
 struct tarSceneChange
 {
 	CPlayer* pPlayer;
 	long lFromSceneID;
 	long lToSceneID;
 	float x, y, z;
 };

//delelte by czq
//long GetNowPlayer( void );
//long GetOnlinePlayer( void );


// ========================================================================
/*
角色对象操作部分
*/
// ========================================================================


//CPlayer* JoinPlayer( void );

//////////////////////////////////////////////////////////////////////////
// 对象相关操作
//////////////////////////////////////////////////////////////////////////
//long RemovePlayer( CPlayer* pPlayer );

//CPlayer* GetPlayer	( long lID );

//  CPlayer* GetPlayerFromAccounts( string Accounts);
// CPlayer* GetPlayerFromName( string strName,char &Result );
//  CPlayer* GetPlayerFromSocket( void *pSocket );

//CNPC* GetNPC( long lID );
//CMonster* GetMonster( long lID );
//CGameObject* GetObject( long lID );

//bool KickPlayer( CPlayer* pPlayer );

// ========================================================================
/*
场景区域操作部分
*/
// ========================================================================

// CRegion* GetRegion( long lID );
// CRegion* GetRegionFromLogicID( long lID );
// CRegion* GetRegionFromName( const char* pName );
// CRepRegion* GetRepRegion( long lID );