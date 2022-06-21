//---------------------------------------------------------------------------
// Desc:	�����н�����Ӧ����
// Createor:�ֵ»�(David Lin)
// Date:	2004-10-11
// 
// Last Update: �ֵ»�(David Lin)
// Update Data: 2004-10-11
//---------------------------------------------------------------------------

#pragma once

#include "stdafx.h"
using namespace std;

#include "../../Common/Include/Base/Inc/Timer.h"


namespace login
{
	class CLogin;
}

namespace loginserver
{

	//��Ϸ����������
	class  IService
	{
		enum eServiceStatus
		{
			SERVICE_CLOSE,
			SERVICE_OPEN
		};
	public:
		IService():m_Status(SERVICE_CLOSE){}
		virtual ~IService(){}
		virtual bool  Init()    =0;
		virtual void  Start(){ m_Status = SERVICE_OPEN; };
		virtual void  Stop(){ m_Status = SERVICE_CLOSE; };	 
		virtual void  Release() =0;
		virtual bool  Run()     =0;
		bool IsOpen(){ return m_Status == SERVICE_OPEN; }
		virtual void* GetValue(){return NULL;};

	protected:
		sbase::CTimerMS   m_Timer;
		eServiceStatus m_Status;
	public:
		login::CLogin* m_pLoginSrv;
	};

	class CommandService;
	typedef int(* pFunPrintf)(const char*,...);
	typedef void(CommandService::*pKeyFunc)(char *,pFunPrintf);

	struct KeyCommand
	{
		const char * cmd;
		unsigned char cmdLen;
		pKeyFunc pfun;
		const char * Note;
		long lID;
	};

	//�����з���
	class  CommandService :public IService
	{	
#define  COMMAND_TIMER	50
	public:
		CommandService(login::CLogin* pLoginSrv);
		bool  Init();

		void  Release(){delete this;};
		bool  Run();

	protected:
	private:
		long Command( void );

		long ParseCommand( char* Argcommand );

		KeyCommand* GetConmandFunTable();

public:
		void Key_Help( char *,pFunPrintf );           //����
		void Key_Exit( char *,pFunPrintf );           //�����رշ�����
		void Key_Quit( char *,pFunPrintf );           //ָ����������رշ�����
		void Key_List( char *,pFunPrintf );           //����б�


};

#define CMD_FORMAT(a) a,(sizeof(a)-1)
static KeyCommand Commands[]=
{
	{CMD_FORMAT("help"),          &CommandService::Key_Help,            "Display this help",             			0},
	{CMD_FORMAT("exit"),          &CommandService::Key_Exit,         	"Shutdown immediately ",         			1},
	{CMD_FORMAT("quit"),          &CommandService::Key_Quit,         	"Shutdown several minutes later",			2},
	{CMD_FORMAT("list"),          &CommandService::Key_List,         	"Display the GS information",				3},

}; 

#define  AllCmds sizeof(Commands)/sizeof(KeyCommand)

}
