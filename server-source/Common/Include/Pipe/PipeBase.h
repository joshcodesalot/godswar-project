//========================================================
//
//    Copyright (c) 2008,�������˹�����
//    All rights reserved.
//
//    �ļ����� �� PipeBase.h 
//    ժ    Ҫ �� �ܵ�����
//    
//    ��ǰ�汾 �� 1.0
//    ��    �� �� ����ǿ
//    ������� �� 2008-7-1
//    ˵    �� ��
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
//�� �֣�CPipeBase
//�� �棺�ܵ�����		
//*****************************************
namespace ipc_pipe
{
	enum TYPE{ NAMEPIPE_SERVER,NAMEPIPE_CLIENT};
	class PIPE_DLL CPipeBase
	{
						/*--------------------------------------*/
						/*				��Ա������				*/
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

		//������Ͷ�ݷ�������
		bool WriteBuf( void );
		//��Ϸ�߼��򻺳�����д����
		bool PackMsg( char *pMsg, size_t iLen );

		//�ӻ������ж����ѽ��ܵ�������
		bool ReadBuf( char **Paket );
		//��Ϸ�߼�������ϣ��ƶ�������ָ��
		void Remove( size_t nLen ){ m_iBuffer.Remove( nLen ); };

	protected:
	private:

						/*--------------------------------------*/
						/*				��Ա������				*/
						/*--------------------------------------*/
	public:
	protected:

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

		snet::CircularBuffer	m_iBuffer;
		snet::CircularBuffer	m_oBuffer;

		bool     m_Start;
	private:

		CRITICAL_SECTION		m_PipeCriSec; 


	}; // end class CPipeBase
}



#endif // _PIPE_BASE_H_
