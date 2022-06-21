//========================================================
//    Copyright (c) 2006,�������˹�����
//    All rights reserved.
//
//    �ļ����� �� IPCChannel.h
//    ժ    Ҫ �� �ŵ�����
//    
//    ��ǰ�汾 �� 1.0
//    ��    �� �� ����
//    ������� �� 2007-01-16
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
		//�ŵ�����
        TYPE     m_Type;
		//�����ܵ�ʵ�����
		HANDLE   m_PipeHandle;
		DWORD    m_size;
		//�����ֽ���
		DWORD    m_BytesRead;
		//�����ֽ���
		DWORD    m_ByresWritten;
		//���ͻ�����
		char     m_SendBuffer[BUF_SIZE];
		//���ܻ�����
		char     m_ReadBuffer[BUF_SIZE];

		bool      m_Start;

	};

}
