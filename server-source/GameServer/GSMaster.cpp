#include "stdafx.h"
#include "GSMaster.h"

#include "World.h"
#include "DataCore.h"
#include "ServiceLoader.h"
#include "./GameCore/GMMgr.h"
#include "./GameCore/Backup.h"
#include "./GameCore/QuestManager.h"
#include "./GameCore/SkillManager.h"
#include "./GameCore/ItemManager.h"
#include "./GameCore/Mall.h"
#include "./GameCore/QuestManager.h"
#include "./GameCore/KeyWords.h"
#include "../Common/Include/PyScript/ScriptSystem.h"
#include "..\Common/Include/Base\Inc\IPCChannel.h"

#include "ChatCommand.h"

bool InitGameScript();


std::string g_OutPut ="";


//sbase::CTimer timerLogin;		// 帐号服务器超时
//sbase::CTimer timerDB;			// 数据库服务器超时

sbase::CTimer timerCommand;       // 读取网络数据超时
sbase::CTimer timerOutNet;       // 读取网络数据超时

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow ();

void   GSMaster::GOutPut()
{
	char a;
	static char buf[100];
	static char Load[100];
	memset( buf, 0L, sizeof(buf) );
	memset( Load, 0L, sizeof(Load) );
	for ( int i = 0; i < 50; i++ )
	{ 
		switch(i%4) 
		{
		case 0:
			a='-';
			break;
		case 1:
			a='\\';
			break;
		case 2:
			a='|';
			break;
		case 3:
			a='/';
			break;
		default:
			a='*';
		}
		Load[i]='='; 
		printf("%-50s   %c【%d%%】",Load,a,i*2);

		if(i!=49)
		{
			sprintf(buf,"%-50s   %c【%d%%】",Load,a,i*2);
			for(size_t j=0;j<strlen(buf);j++)
			{
				printf("%c",'\b');
			}
		}
		else
		{
			sprintf(buf,"%-50s   %c【%d%%】",Load,'*',100);
			for(size_t j=0;j<strlen(buf);j++)
			{
				printf("%c",'\b');
			}
			printf("%s",buf);
			g_OutPut.clear();
		}

		Sleep(50);
	}
	printf("\n");
}

void   GSMaster::GOutPut(  int a  )
{
	if ( ( a >> 1 ) > 50 )
	{
		return;
	}

	int Index = (int)g_OutPut.find_first_not_of("=");

	string buf="";
	buf.assign( g_OutPut,0, Index);

	int temp = ( a >> 1 ) - 1 ;
	if ( temp <= Index )
	{
		return ;
	}

	for ( int i = 0; i <=  temp - Index ; i++ )
	{
		buf += '=';
	}

	while ( buf.length() < 53 )
	{
		buf += ' ';
	}

	char kkk[32];
	sprintf(kkk,"%c【%d%%】",'*',a);
	buf += kkk;

	for(size_t j=0;j<g_OutPut.length();j++)
	{
		printf("%c",'\b');
	}
	printf("%s",buf.c_str());
	g_OutPut = buf;

}


GSMaster::GSMaster():m_pWorld(NULL)
{
	for (int i=0; i<SERVICE_COUNT; i++)
	  m_Service[i] = NULL;

}

GSMaster::~GSMaster()
{
	for (int i=0; i<SERVICE_COUNT; i++)
       SAFE_RELEASE(m_Service[i]);
}

bool GSMaster::Init()
{

	sbase::ConsoleSetup();
	sbase::SetConsoleTitle("GodsWar GameServer"); 
	sbase::SetConsoleFontColor();
	//屏蔽关闭按钮（ 禁止非法退出 ）
	HWND  hWnd = GetConsoleWindow();   
	if ( hWnd )   
	{   
		HMENU  hMenu = GetSystemMenu(hWnd,   FALSE);   
		DeleteMenu(hMenu,SC_MAXIMIZE  , MF_GRAYED | MF_DISABLED);
		DeleteMenu(hMenu,SC_SIZE, MF_GRAYED | MF_DISABLED);
		DeleteMenu(hMenu, SC_CLOSE , MF_GRAYED | MF_DISABLED); 
 	}   
 
 	cout<<"               ★★★★★★★★★★★★★★★★★★★★★★★★★"<<endl;
 	cout<<"               ★                                              ★"<<endl;
 	cout<<"               ★             GodsWar Game Server              ★"<<endl;
 	cout<<"               ★       Copyright (c) 2007,欢乐连线工作室      ★"<<endl;
 	cout<<"               ★             All rights reserved.             ★"<<endl;
 	cout<<"               ★                                              ★"<<endl;
 	cout<<"               ★★★★★★★★★★★★★★★★★★★★★★★★★\n"<<endl;
 	cout<<"\n★★★Initialize★★★"<<endl<<endl;

// 		 cout<<"                 GGGGG 	  GG   GG    GGGG 	   "<<endl;
// 		 cout<<"                GGG GGG	  GG   GG   GG  GG	  "<<endl;
// 		 cout<<"                GGG GGG	  GG G GG   GG  GG	  "<<endl;
// 		 cout<<"                GGG    	  GG G GG   GG  GG	  "<<endl;
// 		 cout<<"                GGG       GG G GG   GG  GG	  "<<endl;
// 		 cout<<"                GGGGGGG	  GG G GG   GG  GG	  "<<endl;
// 		 cout<<"                GG  GGG	  GGG GGG   GG  GG	  "<<endl;
// 		 cout<<"                GGG GGG	  GGG GGG   GG  GG	   http://www.9458.com"<<endl;
// 		 cout<<"                 GGGGGG	  GGG GGG    GGGG 	  GodsWar Game Server "<<endl;



	srand( (unsigned)time( NULL ) );

#ifdef WIN32
	{
		HANDLE hProcess = GetCurrentProcess();
		UINT curAff = 3;             
		SetThreadAffinityMask( GetCurrentThread(),curAff );
		SetPriorityClass(hProcess,REALTIME_PRIORITY_CLASS);
	}
#endif

	sbase::SetConsoleFontColor( FOREGROUND_GREEN  | 0x0009 );

	m_pServerSocketEventHandle = new ServerSocketEventHandle();
	m_pWorld = new CWorld(this, m_pServerSocketEventHandle);
	if (!m_pWorld)
	{
		return false;
	}

// 	m_pNetClient = new	cnet::CIOCP();
// 	if ( !m_pNetClient )
// 	{
// 		return false;
// 	}

	m_Service[SERVICE_DB]       =  new  DBService(m_pWorld);
	//m_Service[SERVICE_PIPE]     =  new  PipeService(m_pWorld);
	m_Service[SRVICE_COMMAND]   =  new  CommandService(m_pWorld);
	m_Service[SERVICE_REGISTER] =  new  RegisterService(m_pWorld, NULL);
	m_Service[SERVICE_QUEUE]    =  new  QueueService(m_pWorld);
	m_Service[SERVICE_BACKUP]   =  new	BackUpService(m_pWorld, NULL);
									   
 	for (int i=0; i<SERVICE_COUNT; i++)
	{
		if (  NULL != m_Service[i] )
		{
			m_Service[i]->Init();
		}
	} 		

	m_pWorld->Init();

	CWorld::m_startEvent = true;

	m_pThread = sbase::CThread::CreateNew(*this, sbase::CThread::RUN, 11 );
	if (!m_pThread)
	{
		::MessageBox(NULL, "Create NetWorker thread fail.", "Error", MB_OK|MB_ICONERROR);
	}
	m_pThread->SetThreadAMask( 12 );
  	cout<<"\n★★★Initialize successfully!★★★"<<endl;
  	sbase::SetConsoleFontColor();

  	cout<<"\n\nGodsWar>";

 	return true;

}

void GSMaster::Release()
{
	SAFE_DELETE(m_pThread);
	SAFE_DELETE(m_pServerSocketEventHandle);

	for (int i=0; i<SERVICE_COUNT; i++)
	{
		if (  NULL != m_Service[i] )
		{
			m_Service[i]->Release();
		}
	} 

	m_pWorld->Release();
}

void GSMaster::Run()
{
	m_pWorld->Run();
}

int	GSMaster::OnThreadProcess(void)
{
	for ( int i = 0; i<SERVICE_COUNT; i++)
	{
		if ( m_Service[i] != NULL )	
		  m_Service[i]->Run();
	}

	Sleep(1);

   return 0;
}

