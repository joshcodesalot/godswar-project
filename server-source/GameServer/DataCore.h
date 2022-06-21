//========================================================
//
//    Copyright (c) 2006,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� DataCore.h
//    ժ    Ҫ �� ����������ģ��
//    
//    ��ǰ�汾 �� 1.00
//    ��    �� �� �ֵ»�
//    ������� �� 2007-01-12
//
//========================================================


#pragma once


#define PLAYER_BEGIN				0
#define NPC_BEGIN					5000
#define MONSTER_BEGIN				10000

#define MAX_REGION					24				// ��������
#define MAX_REP_REGION				1				// ��������

#define ACADEMIC_PLAYER_MEM_ID					0			    // �����������
#define ACADEMIC_NPC_MEM_ID						5000			// ��ϷNPC����
#define ACADEMIC_MONSTER_MEM_ID					10000			// ��Ϸ��������
#define ACADEMIC_BOSS_MEM_ID					100				// ��ϷBOSS����

#define ACTUAL_PLAYER_ONLINE	    1500			// �����������
#define ACTUAL_NPC_ONLINE		    3000			// ��ϷNPC����
#define ACTUAL_MONSTER_ONLINE		20000			// ��Ϸ��������
#define ACTUAL_BOSS_ONLINE			100				// ��ϷBOSS����



#define	CORPSE_LOST_TIME			5				// ʬ����ʧʱ��
#define CORPSE_ITEM_TIME			20				// ������ʬ����ʱ��

#define FIGHT_TIME					10				// ս��״̬����ʱ��(sec)
#define FIGHT_INTERVAL				1700			// �������(ms)
#define REFRESH_TIME				30				// ��ʧˢ��ʱ��(sec)
#define MONSTER_AI_TIME				20				// ����AI����ʱ������(sec)

#define MONSTER_FACTION				2				//������Ӫ

//��ɫ�����������
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
���ݲ���
*/
// ========================================================================

 //�����л��ýṹ
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
��ɫ�����������
*/
// ========================================================================


//CPlayer* JoinPlayer( void );

//////////////////////////////////////////////////////////////////////////
// ������ز���
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
���������������
*/
// ========================================================================

// CRegion* GetRegion( long lID );
// CRegion* GetRegionFromLogicID( long lID );
// CRegion* GetRegionFromName( const char* pName );
// CRepRegion* GetRepRegion( long lID );