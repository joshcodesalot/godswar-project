#include "stdafx.h"
#include "World.h"
#include "./GameCore/GameObject.h"

#include "./GameCore/Mall.h"
#include "./GameCore/SkillManager.h"
#include "./GameCore/QuestManager.h"
#include "./GameCore/Region.h"
#include "./GameCore\RepRegion.h"
#include "./GameCore\Player.h"
#include "./GameCore\NPC.h"
#include "./GameCore\Monster.h"
#include "./GameCore\Obstacle.h"
#include "./GameCore/GMMgr.h"
#include "./GameCore/Backup.h"
#include "./GameCore/ItemManager.h"
#include "./GameCore/KeyWords.h"
#include "./GameCore/GameData.h"
#include "../Common/Include/PyScript/ScriptSystem.h"
#include "./GameCore/RoleRelation.h"
#include "ChatCommand.h"

#include "../Common/Include/Base/Inc/Ini.h"

bool InitGameScript();

volatile bool CWorld::m_stopEvent = false;
volatile bool CWorld::m_startEvent = false;

CWorld::CWorld( GSMaster* pGSMaster,ServerSocketEventHandle* pSocketHandle):nFPS(0),nAIFPS(0),g_pBackUpMgr(NULL),
m_pGSMaster(pGSMaster),m_pSocketHandle(pSocketHandle),m_pSrvIocp(new snet::CIOCP()),m_BackBuff(1024*1024*2)
{
	g_lPlayerCount = 0;
	g_lMainCity = 0;
	g_lCity = 0;
	g_lField = 0;
	g_lDuplicate = 0;
	g_lAllScene = 0;
	g_QueuePlayerVec.reserve( 500 );
	m_SpeakForbidPlayer.clear();
}

CWorld::~CWorld()
{
   SAFE_DELETE(m_pSrvIocp);
}

bool CWorld::Init()
{

	CGameObject::s_World = this;

	char szPort[12] = {0};
	char szIP[32]	= {0};
	long MaxCon = 0;

	sbase::CIni _ini("config.ini", false);
	strcpy(szIP, _ini.GetString("System", "IP"));
	strcpy(szPort,_ini.GetString("System", "ListenPort"));
	MaxCon = _ini.GetData("System","MaxOnline");

	sbase::ConsoleWriteColorText(FOREGROUND_BLUE ,">>Initialize Net...\n");
// 	sbase::ConsoleWriteColorText( FOREGROUND_GREEN  ,">>Initialize Net...\n");
// 	sbase::ConsoleWriteColorText(FOREGROUND_BLUE | FOREGROUND_GREEN  ,">>Initialize Net...\n");
// 	sbase::ConsoleWriteColorText( FOREGROUND_RED ,">>Initialize Net...\n");
// 	sbase::ConsoleWriteColorText(FOREGROUND_BLUE  | FOREGROUND_RED ,">>Initialize Net...\n");
// 	sbase::ConsoleWriteColorText( FOREGROUND_GREEN | FOREGROUND_RED ,">>Initialize Net...\n");
// 	sbase::ConsoleWriteColorText(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED ,">>Initialize Net...\n");
// 	sbase::ConsoleWriteColorText(FOREGROUND_INTENSITY,">>Initialize Net...\n");
// 	sbase::ConsoleWriteColorText(FOREGROUND_GREEN |FOREGROUND_INTENSITY ,">>Initialize Net...\n");
// 	sbase::ConsoleWriteColorText(FOREGROUND_BLUE | FOREGROUND_GREEN| FOREGROUND_INTENSITY ,">>Initialize Net...\n");
// 	sbase::ConsoleWriteColorText(FOREGROUND_RED | FOREGROUND_INTENSITY ,">>Initialize Net...\n");

	sbase::ConsoleWriteColorText(7 ,">>Initialize Net...\n");

	if (!m_pSrvIocp->Init(
		                 szIP, 
						 szPort,
						 0, 
						 1, 
						 6, 
						 MaxCon ))
	{
		printf( "Initialize Net Failed!\n");
		//return false;
	}
	m_pGSMaster->GOutPut();

	printf( ">>Initialize ChatHandler...\n");
	ChatHandler::Instance()->Init();
	m_pGSMaster->GOutPut();

	printf( ">>Initialize ScriptSystem...\n");
	ScriptSystem::Instance()->Init();
	InitGameScript();
	m_pGSMaster->GOutPut();

	printf( ">>Initialize SkillManager...\n");
	g_pSkillManager = new CSkillManager(this);
	g_pSkillManager->Init();
	m_pGSMaster->GOutPut();

	printf( ">>Initialize StatusManager...\n");
	g_pStatusManager = new CStatusManager(this);
	g_pStatusManager->Init();
	m_pGSMaster->GOutPut();

	printf( ">>Initialize ConsortiaMgr...\n");
	g_pConsortiaManager = new CConsortiaMgr(this);
	g_pConsortiaManager->Init();
	m_pGSMaster->GOutPut();

	printf( ">>Initialize QuestManager...\n");
	QuestManager::Instance()->Init(this);
	m_pGSMaster->GOutPut();

	printf( ">>Initialize ItemManager...\n");
	ItemManager::Instance()->Init();
	CMall::Instance()->Init();
	m_pGSMaster->GOutPut();

	printf( ">>Initialize Memory...\n");
	m_pGSMaster->GOutPut( sbase::RandGet(10) );
	InitDataCore();
	//m_pGSMaster->GOutPut( 100 );
	printf("\n");

	printf( ">>Initialize QuestManager...\n");
	QuestManager::Instance()->Check();
	m_pGSMaster->GOutPut();
/*
	printf( ">>Initialize GM service...\n");
	CGMMgr::Instance()->Init( this );
	m_pGSMaster->GOutPut();*/

	printf( ">>Initialize Key Filter...\n");
	CKeyWords::Instance()->Init();
	m_pGSMaster->GOutPut();

	m_TimerArray[TIMER_LOGIC].Startup( HINT_TIMEOUT );
//	m_TimerArray[TIMER_GM].Startup( GM_TIMEOUT  );

	m_BackBuff.Initnalize();

	return true;
}



void CWorld::Run()
{
	//登入
	OnNewEnterGameWorld();

	//准备进入游戏
	OnReadlyForGame();

	//新连接

	//游戏中逻辑
	GameLogic();




	//统计在线人数
	// GMStat();

	snet::CSocket* ClosedSocket = NULL;

	while( ( ClosedSocket = m_pSrvIocp->PopCloseConnect() )!= NULL)
	{
		if ( NULL != ClosedSocket )
			m_pSrvIocp->FreeSocketObj( ClosedSocket );
	}

	//退出玩家处理
}

void CWorld::Release()
{
	SAFE_RELEASE( g_pConsortiaManager );
	SAFE_RELEASE( g_pStatusManager );
	SAFE_RELEASE( g_pSkillManager );
	CKeyWords::Instance()->Release();
	printf( "release NetModule ...\n" );
	ReleaseDataCore();
	printf( "release DataCore...\n" );
	ItemManager::Instance()->Release();
	QuestManager::Instance()->Release();
	ChatHandler::Instance()->Release();
	ScriptSystem::Instance()->Release();

//	CGMMgr::Instance()->Release();
	CMall::Instance()->Release();
}


GSMaster* CWorld::GetGSMaster()
{
	return m_pGSMaster;
}

void  CWorld::SendMsgToClient( void* buf, snet::CSocket *pSocket)
{
	MsgHead* pHead = (MsgHead*)buf;

	if ( NULL != pSocket)
     pSocket->PackMsg((char*)buf, pHead->usSize);

}
dbtool::IDatabase* CWorld::GetDBAddr( DB_TYPE dbType )
{
	dbtool::IDatabase* pDb = NULL;
// 	if ( dbType == DB_NONE )
// 	{
// 		int Min = 1;
// 		
		pDb = m_pGSMaster->GetDatabase();

//		size_t Num = pDb->GetRequestSize();

// 		for ( int a = 0; a < MAX_THREAD_DB; a++ )
// 		{
// 			if ( pDb[a]->GetRequestSize() < Num  && a != DB_PRI )
// 			{
// 				Min = a;
// 				Num = pDb[a]->GetRequestSize();
// 			}
// 		}
		// cout<<"DB线程ID"<<Min<<":[In]"<<m_pDB[Min]->GetRequestSize()<<";[Out]"<<m_pDB[Min]->GetResultSize()<<endl;
// 		return pDb[Min];
// 	}
// 	else
// 	{
// 		return pDb[dbType];
// 	}

	return pDb;

}

void CWorld::InitDataCore( void )
{
	g_pPlayer = new CPlayer[ACTUAL_PLAYER_ONLINE];
	for( int i=0; i<ACTUAL_PLAYER_ONLINE; i++ )
	{
		g_pPlayer[i].SetID( ACADEMIC_PLAYER_MEM_ID + i );
		g_pPlayer[i].SetActive(false);
	}
	
	// 功能NPC
	g_pNPC = new CNPC[ACTUAL_NPC_ONLINE];
	for( int i=0; i<ACTUAL_NPC_ONLINE; i++ )
		g_pNPC[i].SetID( ACADEMIC_NPC_MEM_ID + i );

	//怪
	g_pMonster = new CMonster[ACTUAL_MONSTER_ONLINE];
	for( int i=0; i<ACTUAL_MONSTER_ONLINE; i++ )
		g_pMonster[i].SetID( ACADEMIC_MONSTER_MEM_ID + i );

	// BOSS
	sbase::CIni ini( "./scene/SceneDefine.ini", false );
	g_lMainCity = ini.GetData( "default", "main" );
	g_lCity = ini.GetData( "default", "city" );
	g_lField = ini.GetData( "default", "field" );
	g_lDuplicate = ini.GetData( "default", "duplicate" );

	char buf[256];

	// 主场景/野外
	g_pRegion = new CRegion[ g_lMainCity+g_lCity+g_lField ];
	for( int i=0, j=1; i<g_lMainCity; i++, j++ )				// Athens/Sparta
	{
		g_pRegion[i].SetID( i );
		g_pRegion[i].Init(this);

		memset( buf, 0, 256 );
		sprintf( buf, "main_%02d", j );
		g_pRegion[i].SetName( ini.GetString( buf, "name" ) );
		g_pRegion[i].SetNameCH( ini.GetString( buf, "ch" ) );
		g_pRegion[i].SetNumenName( ini.GetString( buf, "numen" ) );
		g_pRegion[i].SetLogicID( ini.GetData( buf, "id" ) );
		g_pRegion[i].SetAthensRevival( (long)ini.GetData( buf, "AthensRevivalMap" ), (float) ini.GetFloatData( buf, "AthensX" ),(float)ini.GetFloatData( buf, "AthensZ" ) );
		g_pRegion[i].SetSpartaRevival( (long)ini.GetData( buf, "SpartaRevivalMap" ), (float) ini.GetFloatData( buf, "SpartaX" ),(float)ini.GetFloatData( buf, "SpartaZ" ) );

		printf( "name=%s/%s \n\t", ini.GetString( buf, "ch" ), ini.GetString( buf, "name" ) );
		g_pRegion[i].CreateMonsterFromIni( );
		printf( "\n\n" );
	}

	for( int i=g_lMainCity, j=1; i<g_lMainCity+g_lCity; i++, j++ )
	{
		g_pRegion[i].SetID( i );
		g_pRegion[i].Init(this);

		memset( buf, 0, 256 );
		sprintf( buf, "city_%02d", j );
		g_pRegion[i].SetName( ini.GetString( buf, "name" ) );
		g_pRegion[i].SetNameCH( ini.GetString( buf, "ch" ) );
		g_pRegion[i].SetNumenName( ini.GetString( buf, "numen" ) );
		g_pRegion[i].SetLogicID( ini.GetData( buf, "id" ) );
		g_pRegion[i].SetAthensRevival( (long)ini.GetData( buf, "AthensRevivalMap" ), (float) ini.GetFloatData( buf, "AthensX" ),(float)ini.GetFloatData( buf, "AthensZ" ) );
		g_pRegion[i].SetSpartaRevival( (long)ini.GetData( buf, "SpartaRevivalMap" ), (float) ini.GetFloatData( buf, "SpartaX" ),(float)ini.GetFloatData( buf, "SpartaZ" ) );
		printf( "name=%s/%s \n\t", ini.GetString( buf, "ch" ), ini.GetString( buf, "name" ) );
		g_pRegion[i].CreateMonsterFromIni( );
		printf( "\n\n" );
	}

	for( int i=g_lMainCity+g_lCity, j=1; i<g_lMainCity+g_lCity+g_lField; i++, j++ )
	{
		g_pRegion[i].SetID( i );
		g_pRegion[i].Init(this);

		memset( buf, 0, 256 );
		sprintf( buf, "field_%02d", j );
		g_pRegion[i].SetName( ini.GetString( buf, "name" ) );
		g_pRegion[i].SetNameCH( ini.GetString( buf, "ch" ) );
		g_pRegion[i].SetNumenName( ini.GetString( buf, "boss" ) );
		g_pRegion[i].SetLogicID( ini.GetData( buf, "id" ) );
		g_pRegion[i].SetAthensRevival( (long)ini.GetData( buf, "AthensRevivalMap" ), (float) ini.GetFloatData( buf, "AthensX" ),(float)ini.GetFloatData( buf, "AthensZ" ) );
		g_pRegion[i].SetSpartaRevival( (long)ini.GetData( buf, "SpartaRevivalMap" ), (float) ini.GetFloatData( buf, "SpartaX" ),(float)ini.GetFloatData( buf, "SpartaZ" ) );
		printf( "name=%s/%s \n\t", ini.GetString( buf, "ch" ), ini.GetString( buf, "name" ) );
		g_pRegion[i].CreateMonsterFromIni( );
		printf( "\n\n" );
	}


	g_lAllScene = g_lMainCity+g_lCity+g_lField;		// 所有场景数量(副本除外)

	g_pRepRegion = new CRepRegion[g_lDuplicate];
	for( int i=0, j=1; i<g_lDuplicate; i++, j++ )
	{
		// load from ini
		g_pRepRegion[i].SetID( g_lMainCity+g_lCity+g_lField + i );
		g_pRepRegion[i].Init(this);
		memset( buf, 0, 256 );
		sprintf( buf, "duplicate_%02d", j );
		g_pRepRegion[i].SetName( ini.GetString( buf, "name" ) );
		g_pRepRegion[i].SetNameCH( ini.GetString( buf, "ch" ) );
		g_pRepRegion[i].SetNumenName( ini.GetString( buf, "boss" ) );
		g_pRepRegion[i].SetLogicID( ini.GetData( buf, "id" ) );
	}
	printf( "Create RepRegion Data Core\t\t\t\tReady!\n" );

	//场景档格数据
	g_pObstacle = new CObstacle;
	g_pObstacle->Init();
	printf( "Create Obstacle Data Core\t\t\t\tReady!\n" );

}
void CWorld::ReleaseDataCore( void )
{
	SAFE_DELETE_ARRAY( g_pPlayer );
	printf( "release player data...\n" );
	SAFE_DELETE_ARRAY( g_pNPC );
	printf( "release NPC data...\n" );
	SAFE_DELETE_ARRAY( g_pMonster );
	printf( "release Monster data...\n" );
	SAFE_DELETE_ARRAY( g_pRegion );
	printf( "release Region data...\n" );
	SAFE_DELETE_ARRAY( g_pRepRegion );
	printf( "release Rep Region data...\n" );
	SAFE_DELETE(g_pObstacle);
	printf( "release Obstacle data...\n" );
}
long CWorld::GetNowPlayer( void )
{
	return g_lPlayerCount;
}
long CWorld::GetOnlinePlayer( void )
{
	int Online = 0;
	for( int i=0; i<g_lMainCity+g_lCity+g_lField; i++ )
	{
		Online += g_pRegion[i].GetOnline();
	}
	return Online;
}

CPlayer* CWorld::JoinPlayer( void )
{
	if( g_lPlayerCount >= ACTUAL_PLAYER_ONLINE )
		return NULL;

	for( int i=0; i<ACTUAL_PLAYER_ONLINE; i++ )
	{
		if( !g_pPlayer[i].IsActive() )
		{
			g_lPlayerCount++;
			g_pPlayer[i].Activate();
			return &g_pPlayer[i];
		}
	}
	return NULL;
}

long CWorld::RemovePlayer( CPlayer* pPlayer )
{
	for( int i=0; i<ACTUAL_PLAYER_ONLINE; i++ )
	{
		if( g_pPlayer[i].IsActive() )
		{
			if( pPlayer == &g_pPlayer[i] )
			{
				g_lPlayerCount--;
				g_pPlayer[i].Release();
				return 0;
			}
		}
	}

	return ERROR_REMOVE_PLAYER_OUT;
}

CPlayer* CWorld::GetPlayer( long lID )
{
	// 排除益处下标
	if( lID < 0 || lID >= ACTUAL_PLAYER_ONLINE )
		return NULL;

	if( g_pPlayer[lID].IsActive() )
		return &g_pPlayer[lID];

	return NULL;
}


//------------------------------------------------
//根据帐号查询
//------------------------------------------------
CPlayer* CWorld::GetPlayerFromAccounts( string Accounts)
{
	return CPlayer::GetPlayerFromAccounts( Accounts );
}
//根据角色名字查询角色
//------------------------------------------------
CPlayer* CWorld::GetPlayerFromName( string strName,char &Result )
{
	CPlayer *pPlayer =  CPlayer::GetPlayerFromRoleName( strName );
	if ( pPlayer == NULL )
	{
		Result = -1;
	}
	return  pPlayer;
}

CPlayer* CWorld::GetPlayerFromSocket( void *pSocket )
{
	snet::CSocket *pConSocket = static_cast<snet::CSocket*>(pSocket);
	return CPlayer::GetPlayerFromSocket( pConSocket );
}

CNPC* CWorld::GetNPC( long lID )
{
	// 排除益处下标
	if( lID < ACADEMIC_NPC_MEM_ID  || lID >= ACADEMIC_NPC_MEM_ID+ACTUAL_NPC_ONLINE )
		return NULL;

	return &g_pNPC[lID - ACADEMIC_NPC_MEM_ID];
}

CMonster* CWorld::GetMonster( long lID )
{
	// 排除益处下标
	if( lID < ACADEMIC_MONSTER_MEM_ID|| lID >= ACADEMIC_MONSTER_MEM_ID+ACTUAL_MONSTER_ONLINE )
		return NULL;

	return &g_pMonster[lID - ACADEMIC_MONSTER_MEM_ID];
}



CGameObject* CWorld::GetObject( long lID )
{
	// 排除益处下标
	if( lID < ACADEMIC_PLAYER_MEM_ID || lID >= ACADEMIC_MONSTER_MEM_ID + ACTUAL_MONSTER_ONLINE  )
		return NULL;

	if( lID < ACADEMIC_NPC_MEM_ID )	   
		return GetPlayer( lID );

	if( lID < ACADEMIC_MONSTER_MEM_ID )
		return GetNPC( lID );

	if( lID < ACADEMIC_MONSTER_MEM_ID + ACTUAL_MONSTER_ONLINE )
		return GetMonster( lID );

	return NULL;
}


bool CWorld::KickPlayer( CPlayer* pPlayer )
{
	pPlayer->GetSocket()->Refresh();
	GetIOCPServer()->PushNewClose(pPlayer->GetSocket());
	pPlayer->DelayExit(0);
	return true;
}

void CWorld::Add_SpeakForbidPlayer( const char* name,time_t OrlTime,int IntervalTime)
{
	sbase::CSingleLock xLock(&m_xLock_SpeakTalk);
	SpeakTime speaktime;
	speaktime.OriginalTime = OrlTime;
	speaktime.IntervalTime = IntervalTime;
	m_SpeakForbidPlayer[name] = speaktime;
}
void CWorld::Del_SpeakForbidPlayer( const char* name)
{
	sbase::CSingleLock xLock(&m_xLock_SpeakTalk);
	std::map<string,SpeakTime>::iterator itor = m_SpeakForbidPlayer.find(name);
	if ( itor != m_SpeakForbidPlayer.end())
	{
		m_SpeakForbidPlayer.erase(itor);
	}
}
bool CWorld::HasSpeakForbid( const char* name )
{
	sbase::CSingleLock xLock(&m_xLock_SpeakTalk);
	std::map<string,SpeakTime>::iterator itor = m_SpeakForbidPlayer.find(name);
	if ( itor != m_SpeakForbidPlayer.end())
	{
		if (itor->second.OriginalTime + itor->second.IntervalTime < time(NULL))
		{
			m_SpeakForbidPlayer.erase(itor);
			return false;
		}
		return true;
	}
	return false;
}

CRegion* CWorld::GetRegionFromLogicID( long lID )
{
	for( int i=0; i<g_lMainCity+g_lCity+g_lField; i++ )
	{
		if(  g_pRegion[i].GetLogicID() == lID )
			return &g_pRegion[i];
	}

	return NULL;
}

CRegion* CWorld::GetRegion( long lID )
{
	// 场景id号安全区间
	if( lID >= 0 && lID < MAX_REGION )
		return &g_pRegion[lID];

	return NULL;
}

CRegion* CWorld::GetRegionFromName( const char* pName )
{
	for( int i=0; i<g_lMainCity+g_lCity+g_lField; i++ )
	{
		if( strcmp( g_pRegion[i].GetName().c_str(), pName ) == 0 )
			return &g_pRegion[i];
	}

	return NULL;
}

CRepRegion* CWorld::GetRepRegion( long lID )
{
	// 副本id号安全区间
	if( lID >= 0 && lID < MAX_REP_REGION )
		return &g_pRepRegion[lID];

	return NULL;
}

void CWorld::CloseSocket( CPlayer *pPlayer )
{
	pPlayer->GetSocket()->Refresh();
	pPlayer->s_World->GetIOCPServer()->PushNewClose(pPlayer->GetSocket() );
	pPlayer->DelayExit(0);
}
unsigned long CWorld::GetAllowedOnline()
{
	return 0;
}
void CWorld::SetAllowedOnline(unsigned int Num )
{

}

void CWorld::DelPlayer( CPlayer* pPlayer )
{
	MAP_PLAYER::iterator itor = g_setPlayer.find(pPlayer);

	if (itor != g_setPlayer.end())
	{

		g_setPlayer.erase(itor);

	}
}
			   

void CWorld::SetPlayer( CPlayer* pPlayer, bool bState /*= true*/ )
{
	MAP_PLAYER::iterator itor = g_setPlayer.find(pPlayer);

	if (itor != g_setPlayer.end())
	{

		itor->second = bState;

	}		
}

bool CWorld::CheckPlayer( CPlayer* pPlayer )
{
	MAP_PLAYER::iterator itor = g_setPlayer.find(pPlayer);

	if (itor != g_setPlayer.end())
	{

		if (itor->second)
		{
			return true;
		}			

	}
	return false;
}

bool CWorld::WriteToBackBuff( char *pMsg, size_t iLen )
{

	m_BackBuff.Write(pMsg, iLen);

	return true;
}

bool CWorld::ReadBackBuff( char **Paket )
{
	size_t nLen =  m_BackBuff.GetLength();
	if ( nLen > sizeof( sbase::MsgHead ) && m_BackBuff.GetStart() )
	{
		sbase::MsgHead *pHead = ( sbase::MsgHead *)m_BackBuff.GetStart();
		if ( nLen >= pHead->usSize )
		{
			*Paket = m_BackBuff.GetStart();
			return true;
		}
		else
		{		 
			*Paket	= NULL;
			return false;
		}
	}
	else 
	{
		*Paket = NULL;
		return false;
	}

	return false;
}

void CWorld::RemoveBackBuff( size_t nLen )
{
	m_BackBuff.Remove( nLen ); 
}
