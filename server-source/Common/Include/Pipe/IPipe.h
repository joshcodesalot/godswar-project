#include "stdafx.h"
#include "CirBuf.h"

#pragma once

#define  PIPE_CLIENT_LOGIN  "IPC_Login"
#define  PIPE_CLIENT_GM		"IPC_GAME" 
#define  PIPE_CLIENT_DB		"IPC_BACKUP"

#define  PIPE_SERVER_LOGIN  "\\\\.\\Pipe\\IPC_Login"
#define  PIPE_SERVER_GM		"\\\\.\\Pipe\\IPC_GAME" 
#define  PIPE_SERVER_DB		"\\\\.\\Pipe\\IPC_BACKUP"

#define  DEFAULT_TIME_OUT	2000

#ifdef PIPE_EXPORTS
#define PIPE_API extern "C" __declspec(dllexport)
#else
#define PIPE_API extern "C" __declspec(dllimport)
#endif


#ifdef PIPE_EXPORTS
#define PIP_CLASS __declspec(dllexport)
#else
#define PIP_CLASS 
#endif

#define NUM_PIPES		10 

#define	BUFFER_SIZE		4096

struct PIPE_INFO
{
	char	szPipeName[32];
	char	szIP[32];
	char	szLoginName[16];
	char	szPassWord[16];			
};

namespace ipc_np
{
	enum STATE{ PIPE_OPEN, PIPE_CLOSE };
	enum TYPE{ PIPE_CLIENT,PIPE_SERVER };
	
    class  PIP_CLASS CPipe
	{

	public:

	    CPipe( const char *Name, TYPE Type);
	    virtual ~CPipe();

	public:

		virtual bool  Init()			 = 0 ;
		virtual void  Release()          = 0;
		virtual void  Close()            = 0;

        virtual bool  Trade();
		bool			Write( void* Buf, int nLen, int& iBytesSend );
		int				Recv(char * RecvBuf );

		bool			ReadBuf( char **Paket );

		void			Remove( size_t nLen );

		HANDLE			GetPipeHandle(){ return m_PipeHandle;}

		bool			GetStatus(){ return m_Start;}

		void			SetStatus(bool bStatus);

		virtual bool	CheckPipeState() = 0;

	protected:

		TYPE			m_Type;
		char			m_szPipeName[64];
		HANDLE			m_hPipeEvent;
		HANDLE			m_PipeHandle;
		BOOL			DataRead;
		OVERLAPPED		m_ol;
		CircularBuffer	m_iBuffer;

		bool			m_Start;

		char			m_szPipeConStr[64];

		CRITICAL_SECTION	m_PipeCriSec; 

		
	private:

		
	};
						

	class CPipeServ : public CPipe
	{
	public:
	   CPipeServ(const char *Name);
	   ~CPipeServ();

	   bool  Init();
	   void  Release(){ delete this;}
	   void  Close();

	   bool  Accept();

	protected:
	private:
		virtual bool	CheckPipeState(){return true;}

	};

	class CPipeClient : public CPipe
	{
	public:
		CPipeClient(const char *Name,const char* pServerIp, const char* pUserName, const char* pUserPw);
		~CPipeClient();

		bool  Init();
		void  Release(){delete this;};
        bool  Trade( );
		void  Close();
		bool  CheckPipeState();
	private:

		static DWORD WINAPI ThreadRun(LPVOID lpParam);

	protected:
	private:
		char	m_szIP[32];
		char	m_szLoginName[16];
		char	m_szPassWord[16];

		char	m_szRemote[32];

		HANDLE	m_hThread;
	};

	//*****************************************
	//名 字：CPipeRouter
	//描 绘：管道中转		
	//*****************************************
	class PIP_CLASS CPipeRouter
	{
						/*--------------------------------------*/
						/*				成员方法区				*/
						/*--------------------------------------*/

		typedef vector<PIPE_INFO> PIPE_VEC;

	public:
		CPipeRouter();
		~CPipeRouter();
	
	public:
	
		bool	Init();

		void	Release();
		void	Close();

		bool	Accept( int nIndex );

		bool	Write( int nIndex, void* Buf, int nLen);
		int		Recv(char * RecvBuf );

		bool	GetStatus( int nNum ){ return m_bCliStart[nNum];}

		void	SetStatus( int nNum, bool bStatus, bool bIsClient );

		void	AddPipeInfo( char* pszPipeName, char* pszIp, char* pszLoginName, char* pszPwd );

		void	AddPipeInfo( PIPE_INFO& Info );
		bool	RemovePipeInfo( char* pszPipeName );

		bool	CheckPipeState( int nNum );

		bool	InitServer();
		bool	InitClient( int nIndex );
	protected:
	private:

		bool	InitData();

	
						/*--------------------------------------*/
						/*				成员变量区				*/
						/*--------------------------------------*/
	public:
	protected:
	private:
	
		HANDLE			m_hPipeEvent[NUM_PIPES];
		HANDLE			m_hSrvPipeHandle[NUM_PIPES];		//服务端句柄
		HANDLE			m_hCliPipeHandle[NUM_PIPES];		//客户端句柄
		char			m_szBuffer[NUM_PIPES][BUFFER_SIZE];	//服务端接收缓冲区
		OVERLAPPED		m_ol[NUM_PIPES];

		bool			m_bCliStart[NUM_PIPES];				//客户端启动
		bool			m_bSrvStart[NUM_PIPES];				//服务端启动

		char			m_szPipeConStr[NUM_PIPES][64];		//客户端管道连接字符串
		char			m_szRemote[NUM_PIPES][36];			

		PIPE_VEC		m_setPipe;							//路由配置信息

		int				m_nRouterCount;						//路由统计

		CRITICAL_SECTION	m_PipeCriSec; 
	
	}; // end class CPipeRouter
	

	PIPE_API  CPipe*  PipeServCreate( const char* );
	PIPE_API  CPipe*  PipeCliCreate(const char *Name,const char* pServerIp, const char* pUserName, const char* pUserPw);
	PIPE_API  CPipeRouter* PipeRouterCreate();
}