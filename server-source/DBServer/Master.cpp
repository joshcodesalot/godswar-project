#include "stdafx.h"
#include "Master.h"
#include <./iostream>
using namespace std;
extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow ();

volatile bool CMaster::m_stopEvent = false;

CMaster::CMaster()
{

}

CMaster::~CMaster()
{

}

bool CMaster::Init()
{
	sbase::ConsoleSetup();
	sbase::SetConsoleTitle("GodsWar DataBase Server"); 
	sbase::SetConsoleFontColor();

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
	cout<<"               ★             GodsWar DataBase Server          ★"<<endl;
	cout<<"               ★       Copyright (c) 2007,欢乐连线工作室      ★"<<endl;
	cout<<"               ★             All rights reserved.             ★"<<endl;
	cout<<"               ★                                              ★"<<endl;
	cout<<"               ★★★★★★★★★★★★★★★★★★★★★★★★★\n\n\n"<<endl;

	cout<<"★★★GodsWar database server Initialize...★★★"<<endl<<endl;

	srand( (unsigned)time( NULL ) );

#ifdef WIN32
	{
		HANDLE hProcess = GetCurrentProcess();
		UINT curAff = 3;             
		SetProcessAffinityMask( GetCurrentProcess(),curAff);
		SetThreadAffinityMask( GetCurrentThread(),curAff );
		SetPriorityClass(hProcess,REALTIME_PRIORITY_CLASS);
	}
#endif

	sbase::SetConsoleFontColor( FOREGROUND_GREEN  | 0x0009 );
	
	m_Service[SERVICE_DB]      =  new DBService();
	//m_Service[SERVICE_PIPE]    =  new PipeService((DBService*)m_Service[SERVICE_DB]);
	m_Service[SERVICE_COMAND]  =  new CommandService(this);
	m_Service[SERVICE_BACKUP]  =  new CBackService((DBService*)m_Service[SERVICE_DB]);



   for ( int i=0; i<SERVICE_COUNT; i++ )
   {
	  if ( !m_Service[i]->Init() )
	  {	 
		  printf("Init DBServer failed!!\n");
		  return false;	  
	  }
   }
	return true;
}

void CMaster::Run()
{
	for ( int i=0; i<SERVICE_COUNT; i++ )
	{
		m_Service[i]->Run();
	}

	UpdateTime();
}

void CMaster::Release()
{
    for ( int i=0; i<SERVICE_COUNT; i++ )
	{
		delete	m_Service[i];
	}
}

void CMaster::UpdateTime()
{
    if ( m_ShutdownTimer.IsActive() && m_ShutdownTimer.IsExpire() )
    {
		CMaster::m_stopEvent = true;
    }
}

void CMaster::ShutDownServ( time_t time )
{
	if ( time != 0)
	{
		m_ShutdownTimer.Clear();
		m_ShutdownTimer.Startup(time);
	}
	else
	  m_stopEvent = true;
}

