//========================================================
//
//    Copyright (c) 2006,欢乐连线工作室
//    All rights reserved.
//
//    文件名称 ： Region.h
//    摘    要 ： 场景管理模块
//    
//    当前版本 ： 1.00
//    作    者 ： 林德辉
//    完成日期 ： 2007-01-12
//
//========================================================

#pragma once

class CGameObject;
class CPlayer;
class CNPC;
class CMonster;

#include "Obj.h"
#include "..\..\Common\MsgDefine.h"
#include "..\errormessage.h"

// 传送点数据结构
struct tarSceneConvey
{
	float rect_left;
	float rect_top;
	float rect_right;
	float rect_bottom;
	string toScene;
	float toX, toZ;
};

#define		MAX_MAGIC_REGION		2048
#define  NOTE  "因服务器紧急维护,即将关闭,给您带来的不便请谅解!"
#define MAX_LEVEL_LIMIT 30      //等级30以下不能PK

struct MagicRegion {
	int		playerID;			// magic caster
	int		magicID;			// magic id
	float	vCenter[3];			// magic center position
	float	fRadius;			// radius
	int		flags;				// target info
	int		lastTime;			// last time
	int		iCount;				// count for magic
	int     targetID;           // target id
};

typedef list< tarSceneConvey* > SceneConveyList;
typedef list< tarSceneConvey* >::iterator SceneConveyIterator;


typedef list< CPlayer* > PlayerList;
typedef list< CPlayer* >::iterator PlayerIterator;

typedef list< CNPC* > NPCList;
typedef list< CNPC* >::iterator NPCIterator;

typedef list< CMonster* > MonsterList;
typedef list< CMonster* >::iterator MonsterIterator;

typedef list< CGameObject* > ObjectList;
typedef list< CGameObject* >::iterator ObjectIterator;

typedef list< MagicRegion* > MgcRgnList;
typedef list< MagicRegion* >::iterator MgcRgnIterator;

// 坐标转换
#define REGION_WIDTH						64.0f
#define REGION_HEIGHT						64.0f
#define REGION_SIZE							8
#define CONVERT_REGION_WIDTH_POS(x)			(x+REGION_WIDTH/2.0f)/8.0f
#define CONVERT_REGION_HEIGHT_POS(x)		(x+REGION_HEIGHT/2.0f)/8.0f

#define MAX_REGION_MONSTER					1000

class CCell;
class CMap;
class CWorld;
// 场景区域管理
class CRegion : private IObj
{	
	struct Revival
	{
		long  MapID;
		float X;
		float Z;
	};
public:
	CRegion();
	~CRegion(void);

public:
	long GetID( void ) { return m_lID; }
	void SetID( long lID ) { m_lID = lID; }
	long GetLogicID(){ return m_LogicID;}
	void SetLogicID( long lID ){ m_LogicID = lID; }
	CMap *GetMap(){ return m_pMap; };

public:		// 逻辑
	long Init(  CWorld * pWorld );
	long AI( void );
	long Release( void ) { delete this; return 0; }
	//得到对象所在的地图ID
	BYTE GetCellID( CGameObject * pObj);

	bool CreateMonsterFromIni( void );

	//	long Walk( CPlayer* pPlayer, float x, float y, float z );
	eError WalkBegin( CPlayer* pPlayer, float x, float y, float z, float offset_x, float offset_y, float offset_z, unsigned int tick );
	long WalkEnd( CPlayer* pPlayer, float x, float y, float z, float atan2, unsigned int tick ,unsigned int skillflag = 0);

	long Walk( CPlayer* pPlayer, float x, float z, float fatan, long lState );

	long ChangeRegion( CPlayer* pPlayer, float x, float y, float z );

	eError Attack( CPlayer* pPlayer, CGameObject* pTarget, float x, float y, float z );
	eError Skill( CGameObject* pPlayer, CGameObject* pTarget, float x, float y, float z ,UINT magicID, float x1, float y1, float z1);
	long Effects( CPlayer* pPlayer, void* Msg);
	long LearnSkill( CPlayer* pPlayer, unsigned char cType );
	long LearnMagic( CPlayer* pPlayer, unsigned char cType, BYTE Flag);

	eError PickupDrops(CPlayer* pPlayer , CMonster* pMonster , int index);

	eError UseOrEquip(CPlayer* pPlayer , int index , int num);
	long AddRelation( CPlayer* pPlayer, CPlayer* pOpsite);

	void StartFight(CGameObject* obj , CGameObject* target);
	void EndFight(CGameObject* obj);

	//给同一区域内的其他玩家发送自己的同步消息
	void SendAreaMsgOneToOther( CCell *pCell,  void* buf , CPlayer* pMySelf = NULL );
	void SendAreaMsgOneToOther( std::vector<CCell*> VecCell, void *buf );
	void SendCellMsg( CCell *pCell,  void* buf , CPlayer* pMySelf = NULL);

	//主动怪
	void RefreshInitiativeMonster( CPlayer* player ) ;

	bool WriteDataToDB();

	bool SendNoteToClient( const char * Note );
	bool SendSysNoteToClient( const char * Note );
	bool SendMidNoteToClient( const char * Note );

	void AddGMControl( ActionElem &elem );
	void DelActionCtr(  GM_MSG_TYPE eType );

	//GM控制指令
	static bool GM_Handler_placard( ActionElem &elem );
	static bool GM_Handler_ShutDown( ActionElem &elem );

public:		// 数据操作
	long Join( CPlayer* pPlayer );
	long RemovePlayer( CPlayer* pPlayer );

	long GetOnline( void );
	void RemoveObject( long lID );
	void RemoveMonster( CMonster* pMonster );
	void RemoveNPC( CNPC* pNPC );
	void ResetMonster( void );

	//聊天专用
	eError Talk_Local( CPlayer* _pPlayer, MSG_TALK* _msg );
	eError Talk_Faction( CPlayer* _pPlayer, MSG_TALK* _msg );
	eError Talk_Union( CPlayer* _pPlayer, MSG_TALK* _msg );
	eError Talk_GM(CPlayer* _pPlayer, MSG_TALK* _msg);
	eError Talk_World(CPlayer* _pPlayer, MSG_TALK* _msg);
	eError Talk_Team(CPlayer* _pPlayer, MSG_TALK* _msg);
	//
 
	long Avatar( CPlayer* pPlayer, int nPos, int nAvatarID );

	CGameObject* GetNearestPartner(CGameObject* obj);

	// magic region handle
	void AddMagicRegion( int playerID, int magicID, float vCenter[3], float radius, int flags , int targetID = -1);
	void FreeMagicRegion( MgcRgnIterator Iter );
	void UpdateMagicRegion( void );
	//挑选目标
    CGameObject* CollectTarget( CGameObject *pCaster, CGameObject *pTarget, UINT flags,const MagicData* magicData );
	// collect gameobject within sphere//mod by zwx,修改第一个参数CPlayer为CGameObject，支持怪物释放群体伤害技能
	void CollectGameObjectSphere( CGameObject* caster, ObjectList* objlst, float vCenter[3], 
									const MagicData* magicData, int max = 100 );
	// collection gameobject within axis align boudning box
	void CollectGameObjectAABB( CPlayer* player, ObjectList* objlst, float vMin[3], float vMax[3], int flags, int max = 100 );

	PlayerList& GetPlayerList(){  return m_listPlayer ; };
public:		// 数据发送协助操作
	long SendMsgToPlayer( void* buf );
	void BackUpPlayerData();

private:	// 协助操作
	long JoinRegionSizeOperate( int gx, int gy, CPlayer* pPlayer );
	long WalkInRegionSizeOperate( int gx, int gy, CPlayer* pPlayer );
	long WalkOutRegionSizeOperate( int gx, int gy, CPlayer* pPlayer );
	long WalkMsgRegionSizeOperate( int gx, int gy, CPlayer* pPlayer );

	long WalkBeginMsgRegionSizeOperate( int gx, int gy, CPlayer* pPlayer );
	long WalkEndMsgRegionSizeOperate( int gx, int gy, CPlayer* pPlayer );

private:	// 数据区
    sbase::CCriticalSection	m_xLock;
	// 链表方式
	PlayerList			m_listPlayer;
	NPCList				m_listNPC;
	MonsterList			m_listMonster;
	ObjectList			m_listObject;

	// 数组方式，用于优化处理
	CMonster*			m_pMonsterQueue[MAX_REGION_MONSTER];
	long				m_lMonsterQueue;
	long				m_lMonsterCount;

	MgcRgnList			m_lstMgcRgnActive;
	MgcRgnList			m_lstMgcRgnFree;

	SceneConveyList		m_listConvey;

	MagicRegion			m_MagicRegion[MAX_MAGIC_REGION];
	PlayerList			m_Player[REGION_SIZE][REGION_SIZE];
	NPCList				m_NPC[REGION_SIZE][REGION_SIZE];
	MonsterList			m_Monster[REGION_SIZE][REGION_SIZE];

	long				m_lID;
	long                m_LogicID;
	string				m_strName;
	string				m_strNameCH;
	string				m_strNumenName;		// Numen/Boss

	CMap                *m_pMap;   
	Revival             m_Athens,m_Sparta;
	CWorld	            * m_pWorld;
public:
	void				SetName( const char* pName ) { m_strName = pName; }
	void				SetNameCH( const char* pName ) { m_strNameCH = pName; }
	void				SetNumenName( const char* pName ) { m_strNumenName = pName; }

	void                SetAthensRevival( long lMapID, float X, float Z ) { m_Athens.MapID = lMapID; m_Athens.X = X; m_Athens.Z = Z;  }
	void                SetSpartaRevival( long lMapID, float X, float Z ) { m_Sparta.MapID = lMapID; m_Sparta.X = X; m_Sparta.Z = Z; }
	Revival             *GetAthensRevival(){ return &m_Athens; }
	Revival             *GetSpartaRevival(){ return &m_Sparta; }

	string				GetName( void ) { return m_strName; }
	string				GetNameCH( void ) { return m_strNameCH; }
	string				GetNumenName( void ) { return m_strNumenName; }

};
