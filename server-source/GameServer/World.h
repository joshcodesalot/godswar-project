//========================================================
//
//    Copyright (c) 2008,�������˹�����
//    All rights reserved.
//
//    �ļ����� �� World.h 
//    ժ    Ҫ �� ��Ϸ���߼�
//    
//    ��ǰ�汾 �� 1.0
//    ��    �� �� ����
//    ������� �� 2008-6-16
//    ˵    �� �� 
// 
//========================================================
#pragma once
//#include "StdAfx.h"
#include "GSMaster.h"
#include "ServiceLoader.h"
#include "DataCore.h"

//#include "..\Common/Include/Base\Inc\IPCChannel.h"
#include "../Common/Include/sNet/IOCP.h"
//#include "../Lib_Pipe/PipeClient.h"
//#include "../Common/Include/cNet/IOCP.h"

typedef std::vector<CPlayer*>	VECTOR_PLAYER;	
enum eTimerIndex	//��ʱ��
{
	TIMER_LOGIC = 0,			// ��Ϸ���߼�
	TIMER_GM,                   // GMͳ��
	TIMER_COUNT					   
};

class CRegion;
class CRepRegion;
class CGameObject;
class CPlayer;
class CNPC;
class CMonster;
class CObstacle;
class CSkillManager;
class CStatusManager;
class CConsortiaMgr;

//*****************************************
//�� �֣�CWorld
//�� �棺		
//*****************************************
class CWorld
{
					/*--------------------------------------*/
					/*				��Ա������				*/
					/*--------------------------------------*/
	typedef map<CPlayer*, bool> MAP_PLAYER;
	friend  class  CPlayer;
	struct SpeakTime
	{
		time_t OriginalTime;
		int    IntervalTime;
	};
public:
	static volatile bool m_stopEvent;
	static volatile bool m_startEvent;
	CWorld(GSMaster* pGSMaster,ServerSocketEventHandle* pSocketHandle );
	~CWorld();

public:

	bool			Init( void );
	void			Run( void );
	void			Release( void );
	GSMaster*		GetGSMaster();
	long            BackUpPlayer( void );
	long            BackUpConsortia(void);
	long			GameLogic( void );
	void            OnNewEnterGameWorld();
	void			OnReadlyForGame();

	void            SendMsgToClient( void* buf, snet::CSocket *pSocket);
	dbtool::IDatabase*  GetDBAddr( DB_TYPE dbType );
	void            CloseSocket(CPlayer* pPlayer);
	unsigned long   GetAllowedOnline();
	void            SetAllowedOnline(unsigned int Num );

	//����_MSG_LOGIN_GAMESERVER
	void			NewSocketProc(CPlayer* pPlayer);
	
	// ========================================================================
	//���ݲ���
	// ========================================================================
	void InitDataCore( void );
	void ReleaseDataCore( void );
	long GetNowPlayer( void );
	long GetOnlinePlayer( void );

	// ========================================================================
	//��ɫ�����������
	// ========================================================================
	CPlayer* JoinPlayer( void );
	long RemovePlayer( CPlayer* pPlayer );

	CPlayer* GetPlayer	( long lID );

	CPlayer* GetPlayerFromAccounts( string Accounts);
	CPlayer* GetPlayerFromName( string strName,char &Result );
	CPlayer* GetPlayerFromSocket( void *pSocket );

	CNPC* GetNPC( long lID );
	CMonster* GetMonster( long lID );
	CGameObject* GetObject( long lID );

	bool KickPlayer( CPlayer* pPlayer );
	void Add_SpeakForbidPlayer( const char* name,time_t OrlTime,int IntervalTime);
	void Del_SpeakForbidPlayer( const char* name);
	bool HasSpeakForbid( const char* name );
	// ========================================================================
	//���������������
	// ========================================================================
	CRegion* GetRegion( long lID );
	CRegion* GetRegionFromLogicID( long lID );
	CRegion* GetRegionFromName( const char* pName );
	CRepRegion* GetRepRegion( long lID );

	//ɾ����ʱplayer����
	void DelPlayer(CPlayer* pPlayer);

	void SetPlayer(CPlayer* pPlayer, bool bState = true);

	bool CheckPlayer(CPlayer* pPlayer);

	snet::CIOCP* GetIOCPServer(){ return m_pSrvIocp;}
	void    UpdateGameLoginQueue();
	void    EndQueue()
	{
		sbase::CSingleLock xLock1(&m_xLock);
	}
	void   StartQueue( CPlayer * pPlayer )
	{
		sbase::CSingleLock xLock1(&m_xLock);
		g_QueuePlayerVec.push_back( pPlayer );
	}

	//���ݻ���������

	bool	WriteToBackBuff( char *pMsg, size_t iLen );

	bool	ReadBackBuff( char **Paket );

	void	RemoveBackBuff( size_t nLen );	

//	void	SetBackupSck( )


protected:
private:
	void            OnSceneChange();

	void            OnTradeDangerousPlayer();
	void            GMStat();
	void            EntityAI();

					/*--------------------------------------*/
					/*				��Ա������				*/
					/*--------------------------------------*/
public:

	//SocketEventHandle*	g_pLoader;
	CSkillManager*		g_pSkillManager;
	CStatusManager*		g_pStatusManager;
	CConsortiaMgr*		g_pConsortiaManager;
	VECTOR_PLAYER		g_QueuePlayerVec;
//	ipc_pipe::CPipeClient *	g_pBackUpMgr;
	ipc_np::CPipe *		g_pBackUpMgr;

	CRegion*			g_pRegion;
	CRepRegion*			g_pRepRegion;
	CPlayer*			g_pPlayer;
	CNPC*				g_pNPC;
	CMonster*			g_pMonster;
	CObstacle*			g_pObstacle;

	long				g_lPlayerCount;
	long				g_lMainCity;
	long				g_lCity;
	long				g_lField;
	long				g_lDuplicate;
	long				g_lAllScene;

	int nAIFPS;
	int nFPS;


	list<tarSceneChange*> g_listSceneChange;
	list<CPlayer*> g_listKickPlayer;		// ���������ר��
	map<string,SpeakTime> m_SpeakForbidPlayer;     // ��ֹ�������
	map<string , int> g_ScriptNPC;
	MAP_PLAYER		  g_setPlayer;

	sbase::CCriticalSection	m_xLock;
	sbase::CCriticalSection	m_xLock_SpeakTalk;

protected:
private:
	GSMaster*		m_pGSMaster;
	ServerSocketEventHandle* m_pSocketHandle;
	sbase::CTimerMS	m_TimerArray[TIMER_COUNT];

	snet::CIOCP*	m_pSrvIocp;

	snet::CircularBuffer m_BackBuff;

//	cnet::CSocket*	m_pClientSocket;


}; // end class CWorld

