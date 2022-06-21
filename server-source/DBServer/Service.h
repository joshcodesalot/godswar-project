//========================================================
//
//    Copyright (c) 2006,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� Service.h
//    ժ    Ҫ �� ���ݿ������������
//    
//    ��ǰ�汾 �� 1.01
//    ��    �� �� ����
//    ������� �� 2007-01-18
//
//========================================================

#ifndef _FILE_SERVICE_H_
#define _FILE_SERVICE_H_ 

#include "../Common/Include/Base/Inc/Timer.h"
#include "../Common/Include/Base/inc/Thread.h"
#include "../Common/Include/DB/Inc/IDB.h"

#include "../Common/Include/Pipe/IPipe.h"
#include "..\Common\MsgDefine.h"
#include <deque>


namespace snet
{
	class CIOCP;
	class CSocket;
	
}
// class snet::CIOCP;
// class snet::CIOCP;
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
	virtual void  Stop() { m_Status = SERVICE_CLOSE; }; 
	virtual void  Release() =0;
	virtual void  Run()     =0;
	bool IsOpen(){ return m_Status == SERVICE_OPEN; }
protected:
	sbase::CTimerMS   m_Timer;
	eServiceStatus m_Status;
public:
};


//�ܵ�����
class DBService;
class  PipeService : public IService , private sbase::IThreadEvent 
{
	typedef std::deque<string> SQL_DEQUE;
	enum SQL_STYLE
	{
		TYPE_ROLE_ATTRIBUTE = 10,
		TYPE_ROLE_ACTIVESKILL,
		TYPE_ROLE_PASSIVESKILL,
		TYPE_ROLE_KITBAG1,
		TYPE_ROLE_KITBAG2,
		TYPE_ROLE_KITBAG3,
		TYPE_ROLE_KITBAG4,
		TYPE_ROLE_EQUIP,
		TYPE_ROLE_STORAGE,
		TYPE_ROLE_QUEST,
		TYPE_ROLE_FRIEND,
		TYPE_ROLE_BLACKLIST,
		TYPE_ROLE_CONSORTIA,
		TYPE_MGR_CONSORTIA,
		RYPE_ROLE_COUNT,
	};

	struct  MsgHead
	{
		USHORT	usSize;
		USHORT	usType;
	};

	struct BACKUP_MSG 
	{
		MsgHead Head;

		char  SQL[4000];
	};

     #define PIPE_TIMER	5000
	 #define SQL_MAXSIZE  4096
public:
	PipeService( DBService *DB );
	~PipeService();
	bool  Init();      
	void  Stop();    
	void  Release();
	void  Run( );

	bool  OnRead();
	
	virtual int		OnThreadCreate	(void){ return 0; }		
	virtual int		OnThreadDestroy	(void){ return 0; }	
	virtual int		OnThreadEvent	(void){ return 0; }          
	virtual int		OnThreadProcess	(void);

	const string	PopQueSql();
	void			PushQueSql(string strSql);

protected:
private:
	CRITICAL_SECTION	m_CriSec;
	DBService*			m_DBMgr;
	sbase::CThread*		m_pThread;
	ipc_np::CPipe*		m_Pipe;
	SQL_DEQUE			m_QueSql;

	HMODULE				m_PipeDll;

	
};

//���ݿ����
class  DBService : public IService
{
    #define  DB_TIMER	60000
    #define MAX_THREAD_DB  1
public:
	DBService();
	~DBService();
	bool  Init();      
	void  Stop();    
	void  Release();
	void  Run();
	bool  WriteToDB( const char * SQL );

protected:
private:
	dbtool::IDatabase*  GetDBAddr( );
private:
	dbtool::IDatabase*       m_pDB[MAX_THREAD_DB];
};

//*****************************************
//�� �֣�CBackService
//�� �棺		
//*****************************************
class CBackService : public IService, sbase::IThreadEvent
{
	typedef std::vector<void*>	SESSION_VEC;

	typedef std::deque<string> SQL_DEQUE;

	enum SQL_STYLE
	{
		TYPE_ROLE_ATTRIBUTE = 10,
		TYPE_ROLE_ACTIVESKILL,
		TYPE_ROLE_PASSIVESKILL,
		TYPE_ROLE_KITBAG1,
		TYPE_ROLE_KITBAG2,
		TYPE_ROLE_KITBAG3,
		TYPE_ROLE_KITBAG4,
		TYPE_ROLE_EQUIP,
		TYPE_ROLE_STORAGE,
		TYPE_ROLE_QUEST,
		TYPE_ROLE_FRIEND,
		TYPE_ROLE_BLACKLIST,
		TYPE_ROLE_CONSORTIA,
		TYPE_ROLE_ALTAR,
		TYPE_MGR_CONSORTIA,

	};

	struct  MsgHead
	{
		USHORT	usSize;
		USHORT	usType;
	};

	struct BACKUP_MSG 
	{
		MsgHead Head;

		char  SQL[4000];
	};
					/*--------------------------------------*/
					/*				��Ա������				*/
					/*--------------------------------------*/
public:
	CBackService(DBService *DB);
	~CBackService();

public:

	bool  Init();      
	void  Stop();    
	void  Release();
	void  Run();

	//��������������
	bool                    LoadNetService();
	//�߳�ִ����,�������Ϣ������ȡ������
	int                     Process();
	//����������
	void					NewSocketProc(snet::CSocket *pSocket);
	void					OnRead();
	void					OnWrite();
	//������Ϣ
	void					ReslovePacket(const void * pPacket, snet::CSocket *pSocket);

	void	SendMsg(void* buf, void * pSocket, int nLen) const;

	//����ͻ���socket
	void                    CacheAccounts( void * socket );

	bool					FindAccount( void * socket);

	void					ClearAccount( void * socket );

	const string			PopQueSql();
	void					PushQueSql(string strSql);

protected:
	virtual    int		    OnThreadCreate	(void)	{ return 0; }
	virtual    int		    OnThreadDestroy	(void)	{ return 0; }
	virtual    int		    OnThreadEvent	(void)	{ return 0; }
	virtual    int		    OnThreadProcess	(void)	{ return Process(); }
private:

					/*--------------------------------------*/
					/*				��Ա������				*/
					/*--------------------------------------*/
public:
protected:
private:

	snet::CIOCP*		m_pSrvIocp;

	SESSION_VEC			m_AccountsSet;

	CRITICAL_SECTION	m_CriSec;
	DBService*			m_DBMgr;
	sbase::CThread*		m_pThread;
	SQL_DEQUE			m_QueSql;


}; // end class CBackService


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

 class CMaster;
//�����з���
class  CommandService : public IService 
{	
	#define  COMMAND_TIMER	50
public:
	CommandService( CMaster* pMaster );
	~CommandService();			   
	bool  Init();      
	void  Stop();    
	void  Release();
	void  Run();
protected:
private:
	long Command( void );
	long ParseCommand( char* Argcommand );
	KeyCommand* GetConmandFunTable();
public:
	void Key_Help( char *,pFunPrintf );           //����
	void Key_ShutdownServ( char *,pFunPrintf );   //�ر�
private:
	CMaster *m_pMaster;
	char strKeyBuffer[256];
	long lKeyCount;

};

#define CMD_FORMAT(a) a,(sizeof(a)-1)
static KeyCommand Commands[]=
{
	{CMD_FORMAT("help"),          &CommandService::Key_Help,            "Display this help",             			                0},
	{CMD_FORMAT("quit"),		  &CommandService::Key_ShutdownServ,    "ShutDown the DB server in several times",             	    1},
	{CMD_FORMAT("exit"),          &CommandService::Key_ShutdownServ,    "ShutDown the DB server immediately",             	        2},
}; 

#define  AllCmds sizeof(Commands)/sizeof(KeyCommand)

#endif