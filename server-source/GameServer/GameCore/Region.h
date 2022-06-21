//========================================================
//
//    Copyright (c) 2006,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� Region.h
//    ժ    Ҫ �� ��������ģ��
//    
//    ��ǰ�汾 �� 1.00
//    ��    �� �� �ֵ»�
//    ������� �� 2007-01-12
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

// ���͵����ݽṹ
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
#define  NOTE  "�����������ά��,�����ر�,���������Ĳ������½�!"
#define MAX_LEVEL_LIMIT 30      //�ȼ�30���²���PK

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

// ����ת��
#define REGION_WIDTH						64.0f
#define REGION_HEIGHT						64.0f
#define REGION_SIZE							8
#define CONVERT_REGION_WIDTH_POS(x)			(x+REGION_WIDTH/2.0f)/8.0f
#define CONVERT_REGION_HEIGHT_POS(x)		(x+REGION_HEIGHT/2.0f)/8.0f

#define MAX_REGION_MONSTER					1000

class CCell;
class CMap;
class CWorld;
// �����������
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

public:		// �߼�
	long Init(  CWorld * pWorld );
	long AI( void );
	long Release( void ) { delete this; return 0; }
	//�õ��������ڵĵ�ͼID
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

	//��ͬһ�����ڵ�������ҷ����Լ���ͬ����Ϣ
	void SendAreaMsgOneToOther( CCell *pCell,  void* buf , CPlayer* pMySelf = NULL );
	void SendAreaMsgOneToOther( std::vector<CCell*> VecCell, void *buf );
	void SendCellMsg( CCell *pCell,  void* buf , CPlayer* pMySelf = NULL);

	//������
	void RefreshInitiativeMonster( CPlayer* player ) ;

	bool WriteDataToDB();

	bool SendNoteToClient( const char * Note );
	bool SendSysNoteToClient( const char * Note );
	bool SendMidNoteToClient( const char * Note );

	void AddGMControl( ActionElem &elem );
	void DelActionCtr(  GM_MSG_TYPE eType );

	//GM����ָ��
	static bool GM_Handler_placard( ActionElem &elem );
	static bool GM_Handler_ShutDown( ActionElem &elem );

public:		// ���ݲ���
	long Join( CPlayer* pPlayer );
	long RemovePlayer( CPlayer* pPlayer );

	long GetOnline( void );
	void RemoveObject( long lID );
	void RemoveMonster( CMonster* pMonster );
	void RemoveNPC( CNPC* pNPC );
	void ResetMonster( void );

	//����ר��
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
	//��ѡĿ��
    CGameObject* CollectTarget( CGameObject *pCaster, CGameObject *pTarget, UINT flags,const MagicData* magicData );
	// collect gameobject within sphere//mod by zwx,�޸ĵ�һ������CPlayerΪCGameObject��֧�ֹ����ͷ�Ⱥ���˺�����
	void CollectGameObjectSphere( CGameObject* caster, ObjectList* objlst, float vCenter[3], 
									const MagicData* magicData, int max = 100 );
	// collection gameobject within axis align boudning box
	void CollectGameObjectAABB( CPlayer* player, ObjectList* objlst, float vMin[3], float vMax[3], int flags, int max = 100 );

	PlayerList& GetPlayerList(){  return m_listPlayer ; };
public:		// ���ݷ���Э������
	long SendMsgToPlayer( void* buf );
	void BackUpPlayerData();

private:	// Э������
	long JoinRegionSizeOperate( int gx, int gy, CPlayer* pPlayer );
	long WalkInRegionSizeOperate( int gx, int gy, CPlayer* pPlayer );
	long WalkOutRegionSizeOperate( int gx, int gy, CPlayer* pPlayer );
	long WalkMsgRegionSizeOperate( int gx, int gy, CPlayer* pPlayer );

	long WalkBeginMsgRegionSizeOperate( int gx, int gy, CPlayer* pPlayer );
	long WalkEndMsgRegionSizeOperate( int gx, int gy, CPlayer* pPlayer );

private:	// ������
    sbase::CCriticalSection	m_xLock;
	// ����ʽ
	PlayerList			m_listPlayer;
	NPCList				m_listNPC;
	MonsterList			m_listMonster;
	ObjectList			m_listObject;

	// ���鷽ʽ�������Ż�����
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
