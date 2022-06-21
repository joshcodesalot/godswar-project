//========================================================
//    Copyright (c) 2006,福州天盟工作室
//    All rights reserved.
//
//    文件名称 ： IPCChannel.h
//    摘    要 ： 信道服务
//    
//    当前版本 ： 1.0
//    作    者 ： 李锋军
//    完成日期 ： 2007-01-16
//========================================================
#pragma  once


#define  PIPE_LOGIN  "\\\\.\\Pipe\\IPC_Login"
#define  PIPE_GM		"\\\\.\\Pipe\\IPC_GAME" 
#define  PIPE_DB		"\\\\.\\Pipe\\IPC_BACKUP"

#define  BUF_SIZE  4096
#include <windows.h>



namespace ipc
{
	enum TYPE{ NAMEPIPE_SERVER,NAMEPIPE_CLIENT,};
	class CNamePipe
	{
	public:
      CNamePipe(TYPE ChannelType);
      ~CNamePipe();
      bool      Init(const char * cName);
	  bool      Accept();
	  void*     Read(int&  iBytesRead);
	  void*     Write(int& iBytesSend);
	  void      SetSendBuf( void *pSend, int size );
	  char*     GetRecvBuf( );
	  bool      Finalize(void);
	  bool      Refresh();
	  bool      GetStatus(){ return m_Start;}
	protected:

	private:
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

		bool      m_Start;

	};

}
