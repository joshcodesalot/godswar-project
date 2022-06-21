//========================================================
//
//    Copyright (c) 2008,福州天盟工作室
//    All rights reserved.
//
//    文件名称 ： PipeBase.h 
//    摘    要 ： 管道基类
//    
//    当前版本 ： 1.0
//    作    者 ： 蔡智强
//    完成日期 ： 2008-7-1
//    说    明 ：
// 
//========================================================

#ifndef _PIPE_BASE_H_
#define _PIPE_BASE_H_

#include "Pre.h"

#include <windows.h>
#include <string>
using namespace std;


#define  PIPE_CLIENT_LOGIN  "IPC_Login"
#define  PIPE_CLIENT_GM		"IPC_GAME" 
#define  PIPE_CLIENT_DB		"IPC_BACKUP"

#define  PIPE_SERVER_LOGIN  "\\\\.\\Pipe\\IPC_Login"
#define  PIPE_SERVER_GM		"\\\\.\\Pipe\\IPC_GAME" 
#define  PIPE_SERVER_DB		"\\\\.\\Pipe\\IPC_BACKUP"

#define  BUF_SIZE			10240
#define  CIRCUL_BUF_SIZE	1024*64

#include "CirBuffer.h"

//*****************************************
//名 字：CPipeBase
//描 绘：管道基类		
//*****************************************
namespace ipc_pipe
{
	enum TYPE{ NAMEPIPE_SERVER,NAMEPIPE_CLIENT};
	class PIPE_DLL CPipeBase
	{
						/*--------------------------------------*/
						/*				成员方法区				*/
						/*--------------------------------------*/
	public:

		CPipeBase();
		virtual~CPipeBase();

		void      SetSendBuf( void *pSend, int size );
		char*     GetRecvBuf( );
		bool      Finalize(void);
		bool      Refresh();
		bool      GetStatus(){ return m_Start;}

		void	  SetStatus(bool bStatus){ m_Start = bStatus;}	

		void	OnRead( void *pRecv, int size );

		//服务器投递发送请求
		bool WriteBuf( void );
		//游戏逻辑向缓冲区中写数据
		bool PackMsg( char *pMsg, size_t iLen );

		//从缓冲区中读出已接受到的数据
		bool ReadBuf( char **Paket );
		//游戏逻辑处理完毕，移动缓冲区指针
		void Remove( size_t nLen ){ m_iBuffer.Remove( nLen ); };

	protected:
	private:

						/*--------------------------------------*/
						/*				成员变量区				*/
						/*--------------------------------------*/
	public:
	protected:

		//信道类型
		TYPE     m_Type;
		//命名管道实例句柄
		HANDLE   m_PipeHandle;
		DWORD    m_size;
		//发送字节数
		DWORD    m_BytesRead;
		//接受字节数
		DWORD    m_ByresWritten;
		//发送缓冲区
		char     m_SendBuffer[BUF_SIZE];
		//接受缓冲区
		char     m_ReadBuffer[BUF_SIZE];

		snet::CircularBuffer	m_iBuffer;
		snet::CircularBuffer	m_oBuffer;

		bool     m_Start;
	private:

		CRITICAL_SECTION		m_PipeCriSec; 


	}; // end class CPipeBase
}



#endif // _PIPE_BASE_H_
