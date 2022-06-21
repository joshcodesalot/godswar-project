#include "stdafx.h"
#include "./GMMgr.h"
#include "../ServiceLoader.h"
#include "../../GameServer/GSMaster.h"
#include "../../GameServer/DataCore.h"
#include "../../GameServer/GameCore/Player.h"
#include "../../GameServer/World.h"

#pragma warning(disable:4702)

#define SERVER2SERVER_TIMEOUT	5

//CPlayer* GetPlayerFromName( string strName,char &Result );

CGMMgr *CGMMgr::m_pGM = NULL;

CGMMgr::CGMMgr( ):m_pThread(NULL),m_pPipe( NULL )
{

}

CGMMgr::~CGMMgr()
{
	SAFE_DELETE( m_pThread );
	m_pPipe->Release();
	FreeLibrary(m_PipeDll);
}

bool CGMMgr::Init(CWorld * pWorld)
{
	m_pWorld = pWorld;

	///////////////////////////////
	printf("Initialize GM PipeServer...\n");
	m_PipeDll = LoadLibrary("Pipe.dll"); 
	if ( NULL == m_PipeDll)
	{
		return 0;
	}

	//加载服务端网络组建 
	PipeServerCreate PipeServ =(PipeServerCreate)::GetProcAddress(m_PipeDll, "PipeServCreate");
	if (!PipeServ)
	{
		FreeLibrary(m_PipeDll);
		return false;
	}

	m_pPipe = PipeServ("IPC_GAME");

	if(m_pPipe == NULL)
		return false;
	///////////

	if ( !m_pPipe->Init() )
	{
		printf("Initialize GM PipeServer Failed!\n");
		FreeLibrary(m_PipeDll);
		return false;
	}

	m_pThread = sbase::CThread::CreateNew(*this, sbase::CThread::RUN, 11 );
	
	if (!m_pThread)
	{
		::MessageBox(NULL, "Create NetWorker thread fail.", "Error", MB_OK|MB_ICONERROR);
	}
	m_pThread->SetThreadAMask( 12 );
	timerLogin.Startup( 10 );

	//Load_Placard();
	return true;
}

CGMMgr *CGMMgr::Instance()
{
	if (   NULL == m_pGM )
	{
		m_pGM = new CGMMgr;
	}
	return m_pGM;
}

int	CGMMgr::OnThreadProcess(void)
{

    int ret = 0;

	ActionElem* pPipeMsg = NULL;
	ActionElem msg;
 	for (;;)
 	{
		ret = m_pPipe->Recv(NULL);
		if ( -1 == ret ) // 还没有断开
		{
			sbase::ConsoleWriteColorText( FOREGROUND_RED,"client exit..." );
			return 1;
		}
		else
		{
			//sbase::ConsoleWriteColorText( FOREGROUND_RED,"GM Login..." );
		}


		string strRecv;


		pPipeMsg = NULL;

		while (m_pPipe->ReadBuf((char**)&pPipeMsg))
		{
			memset(&msg,0L,sizeof(msg));
			memcpy(&msg,pPipeMsg,pPipeMsg->Head.usSize);

			switch( msg.Head.usType )
			{
			case ACTION_KICK_PLAYER:
				{
					msg.callback = CPlayer::GM_Handler_kick;
					ActionElem   ControlAct(msg);
					char a = 0;
					CPlayer *pPlayer = m_pWorld->GetPlayerFromName( msg.ObjName, a  );
					if ( NULL != pPlayer )
					{
						ControlAct.Obj = static_cast<void*>(pPlayer);
						pPlayer->AddGMControl( ControlAct );	
					}
				}
				break;
			case ACTION_BAN_SPEAK:
				{
					msg.callback = CPlayer::GM_Handler_speak;
					msg.OriginalTime = time(NULL);
					ActionElem   ControlAct(msg);
					char a = 0;
					bool Update_Ban_Speak = false;
					
					//每次有新的禁言消息， 清除之前的禁言 （相当与有 修改 删除 添加禁言的功能） 【处理玩家不在线】
					m_pWorld->Del_SpeakForbidPlayer(msg.ObjName);
					m_pWorld->Add_SpeakForbidPlayer(msg.ObjName,ControlAct.OriginalTime,ControlAct.IntervalTime);

					CPlayer *pPlayer = m_pWorld->GetPlayerFromName( msg.ObjName, a  );
					if ( NULL != pPlayer )
					{
						//删除之前的禁言设置，重新设置禁言【处理玩家在线】
						pPlayer->DelTalkBan();

						ControlAct.Obj = static_cast<void*>(pPlayer);
						pPlayer->AddGMControl( ControlAct );
					}
				}
				break;
			case ACTION_SYS_PLACARD:
				{
					Load_Placard();
				}
				break;
			case ACTION_SYS_SHUTDOWN:
				{
					


					////////计算时间////////////////////
					int year,month,day,hour,minute,second;
					struct tm *t;
					time_t currenttime = time(NULL);
					t = localtime(&currenttime);

					sscanf(msg.PlacardInfo,"%d-%d-%d %d:%d:%d",&year,&month,&day,&hour,&minute,&second);
					time_t StartTime = ( day - t->tm_mday )*86400 + ( hour - t->tm_hour )*3600 + ( minute - t->tm_min )*60 + ( second - t->tm_sec );
					////////计算时间////////////////////

					if (StartTime < 0)
					{
						break;
					}

					//step1:清除所有的场景公告
					for( int i=0; i<m_pWorld->g_lMainCity+m_pWorld->g_lCity+m_pWorld->g_lField; i++ )
					{
						m_pWorld->g_pRegion[i].DelActionCtr(ACTION_SYS_SHUTDOWN);
					}

					ActionElem   ControlAct;
					ControlAct.Head.usType = ACTION_SYS_SHUTDOWN;
					ControlAct.callback = CRegion::GM_Handler_ShutDown;
					ControlAct.OriginalTime =  time(NULL);
					ControlAct.StartTime =  StartTime;
					ControlAct.IntervalTime = SERVER_NOTE_REFRESH;
					if (StartTime > SERVER_SHOWDOWN_START) //900  关机的15分钟前提示， 每两分钟提示一次
					{
						ControlAct.Repeat = SERVER_SHOWDOWN_START / SERVER_NOTE_REFRESH;
					}
					else
					{
						ControlAct.Repeat = ControlAct.StartTime / SERVER_NOTE_REFRESH;
					}
					ControlAct.HaveRepeat = 0;


					for( int i=0; i<m_pWorld->g_lMainCity+m_pWorld->g_lCity+m_pWorld->g_lField; i++ )
					{
						ControlAct.Obj = static_cast<void*>(&m_pWorld->g_pRegion[i]);
						m_pWorld->g_pRegion[i].AddGMControl( ControlAct );
					}
				}
				break;
			case ACTION_PLAYER_ITEM:
				{
					ActionElem   ControlAct(msg);
					char a = 0;
					CPlayer *pPlayer = m_pWorld->GetPlayerFromName( msg.ObjName, a  );
					if ( NULL != pPlayer )
					{
						CPlayer *pPlayer = m_pWorld->GetPlayerFromName( msg.ObjName, a  );
						pPlayer->AddGMControl( ControlAct );
					}

				}
				break;
			default:
				break;
			}

			m_pPipe->Remove(msg.Head.usSize);
		}	
		Sleep( 10 );
	}	

	return 1;
}


void CGMMgr::Load_Placard()
{
	//读取数据库，清理之前容器中的数据，重新把记录压入容器中

	//step1:清除所有的场景公告
	for( int i=0; i<m_pWorld->g_lMainCity+m_pWorld->g_lCity+m_pWorld->g_lField; i++ )
	{
		m_pWorld->g_pRegion[i].DelActionCtr(ACTION_SYS_PLACARD);
	}

	//step2:从数据库中读取公告，重新设置各个场景的公告
	dbtool::IDatabase* p_DB = m_pWorld->GetDBAddr( DB_NONE );
	if ( NULL != p_DB)
	{
		//step 1:get all the consortia base information from DB
		dbtool::IRecordset *pRecordSet = p_DB->CreateRecordset( SQL_SELECT_PLACARD, dbtool::MODE_EDIT );
		if ( NULL != pRecordSet)
		{
			UINT Count = pRecordSet->CountRecord();
			for ( UINT i = 0; i < Count; i++ )
			{
				pRecordSet->Move(i);

				dbtool::IRecord  *pRec = pRecordSet->GetRecord();
				
				ActionElem   ControlAct;
				ControlAct.Head.usType = ACTION_SYS_PLACARD;
				ControlAct.callback = CRegion::GM_Handler_placard;
				strcpy( ControlAct.PlacardInfo, pRec->Field( UINT(5) ) );
				ControlAct.IntervalTime =  (UINT)pRec->Field( UINT(4) );
				ControlAct.StartTime =  (LONGLONG)pRec->Field( UINT(2) );
				ControlAct.EndTime =  (LONGLONG)pRec->Field( UINT(3) );
				ControlAct.PlacardType =  (UINT)pRec->Field( UINT(0) );
				ControlAct.SendType =  (UINT)pRec->Field( UINT(1) );
				if (ControlAct.PlacardType == 1)
					ControlAct.Repeat =  (ControlAct.EndTime - ControlAct.StartTime)/ControlAct.IntervalTime;
				else
					ControlAct.Repeat =  0;
				ControlAct.HaveRepeat =  0;
				ControlAct.OriginalTime =  time(NULL);

				for( int i=0; i< m_pWorld->g_lMainCity; i++ )
				{
					ControlAct.Obj = static_cast<void*>(&m_pWorld->g_pRegion[i]);
					m_pWorld->g_pRegion[i].AddGMControl( ControlAct );
				}

				for( int i=m_pWorld->g_lMainCity; i<m_pWorld->g_lMainCity+m_pWorld->g_lCity; i++ )
				{
					ControlAct.Obj = static_cast<void*>(&m_pWorld->g_pRegion[i]);
					m_pWorld->g_pRegion[i].AddGMControl( ControlAct );
				}

				for( int i=m_pWorld->g_lMainCity+m_pWorld->g_lCity; i<m_pWorld->g_lMainCity+m_pWorld->g_lCity+m_pWorld->g_lField; i++ )
				{
					ControlAct.Obj = static_cast<void*>(&m_pWorld->g_pRegion[i]);
					m_pWorld->g_pRegion[i].AddGMControl( ControlAct );
				}

				pRec->Release();
			}
		}
		pRecordSet->Release();
	}
}