//========================================================
//
//    Copyright (c) 2007,�������߹�����
//    All rights reserved.
//
//    �ļ����ƣ�CirBuffer.h
//    ժ  Ҫ��  CirBuffer��װ��
//    
//    ��ǰ�汾��1.01
//    ��   �ߣ� ����
//    ������ڣ�2007-12-15
//    ˵   ���� TCPճ������
//
//========================================================
#ifndef _FILE_IOCP_CLIENT_H_
#define _FILE_IOCP_CLIENT_H_

#include "PerIOData.h"

#define IP_MAXSIZE	32

namespace cnet
{
	class CSocket;

	class  CIOCP
	{
		friend class CSocket;
	public:
	public:
		CIOCP();
		~CIOCP();

		addrinfo *ResolveAddress(char *addr, char *port, int af, int type, int proto);

		CSocket *GetSocketObj(SOCKET s, int af);

		void FreeSocketObj(CSocket *obj);
		CSocket *RemoveSocketObj(CSocket **head, CSocket *buf);

		PerIOData *GetBufferObj(int buflen);
		void FreeBufferObj(PerIOData *obj);

		void InsertSocketObj(CSocket **head, CSocket *obj);
		int PostConnect(CSocket *sock, PerIOData *connobj);

		static DWORD WINAPI WorkerThread(LPVOID lpParam);

//		int PostSend(CSocket *sock, PerIOData *sendobj);
		void SetPort(int af, SOCKADDR *sa, USHORT port);

//		int PostRecv(CSocket *sock, PerIOData *recvobj);

		int HandleIo(CSocket *sock, PerIOData *buf, DWORD BytesTransfered, DWORD error);

		HANDLE CreateTempFile(char *filename, DWORD size);

		CSocket* GetClientSocket( void ){return m_ClientSck;}

		CSocket* GetSocketObj(SOCKET sck);

		bool Init(char* pszIP, int nPort, CSocket** ppSocket);
		bool InitSocket( void );
		bool ConnectServer( CSocket** ppSocket );

		LONG GetTotalRecv(){return gBytesRead;}
		LONG GetTotalSend(){return gBytesSent;}
		bool GetServerState(){return m_ServerIsOk;}

	protected:
	private:
		HANDLE           m_RecvThread;
		HANDLE           m_hCompletionPort;
		CSocket*		 gConnectionList;
		volatile LONG    gCurrentConnections;
		volatile LONG    gBytesRead;
		volatile LONG    gBytesSent;
		volatile LONG 	 gStartTime;
		volatile LONG 	 gBytesReadLast;
		volatile LONG 	 gBytesSentLast;
		volatile LONG 	 gStartTimeLast;
		volatile LONG 	 gTotalConnections;
		volatile LONG 	 gConnectionRefused;
		volatile LONG    m_OutstandingSends;

		SOCKET			 m_conSck;
		char			 m_szServerIP[IP_MAXSIZE];		// ������ip
		int				 m_nServerPort;					// ������port
		CSocket*	     m_ClientSck;					// �ͻ���Ψһsocket
		bool			 m_ServerIsOk;					// �������Ƿ�ر�
		PerIOData *		 m_pFreeIODataList;				// ��������
		CRITICAL_SECTION m_CriFreeIOList;

	};

}


#endif