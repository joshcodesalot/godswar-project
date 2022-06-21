//========================================================
//
//    Copyright (c) 2008,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� IOCP.h
//    ժ    Ҫ �� ��ɶ˿�ʵ��ͷ�ļ�
//    
//    ��ǰ�汾 �� 1.00
//    ��    �� �� ����
//    ������� �� 2008-05-01
//
//========================================================

#ifndef _FILE_IOCP_SERVER_H_
#define _FILE_IOCP_SERVER_H_

#include "stdafx.h"
#include "PerIOData.h"
#include "ListenSocket.h"
#include <deque>

namespace snet
{
	class CIOCP
	{

	public:
		CIOCP();
		~CIOCP();
		bool Init( char *Addr, char *Port, int AF, int Type, int Proto, long MaxCon );

		addrinfo *ResolveAddress( char *Addr, char *Port, int AF, int Type, int Proto );
		int PrintAddress(SOCKADDR *Sa, int Salen);

		PerIOData* GetPerIOData( int BufLen );
		void FreePerIOData(PerIOData *PerIODat);

		static DWORD WINAPI CompletionThread(LPVOID lpParam);
		void HandleIo(ULONG_PTR Key, PerIOData *Buf, HANDLE CompPort, DWORD BytesTransfered, DWORD Error);

        void EnqueuePendingOperation(PerIOData **Head, PerIOData **End, PerIOData *Obj, int Op );
		PerIOData *DequeuePendingOperation(PerIOData  **Head, PerIOData **End, int Op);
		void ProcessPendingOperations();

		void FreeSocketObj(CSocket *Sockobj);
        CSocket* GetSocketObj( SOCKET Sock, int AF );
		static DWORD WINAPI MonitorThread(LPVOID lpParam);

        CSocket* PopNewConnect();
		void PushNewConnect( CSocket* newSocket );

		CSocket* PopCloseConnect();
		void PushNewClose( CSocket* newSocket );

	public:

		HANDLE				m_CompletionPort;
		CLisetenSocket*		m_pLiSetenSock;
		addrinfo*			m_Addrptr;
		SYSTEM_INFO			SysInfo; 
		int					m_WaitCount;
		int					m_BufferSize;
		unsigned int		HaveSend;

		HANDLE				m_WaitEvents[MAX_COMPLETION_THREAD_COUNT];
		CRITICAL_SECTION	m_CriSection;
		CRITICAL_SECTION	m_BufferListCs;
		CRITICAL_SECTION	m_BSocketListCs;
		CRITICAL_SECTION	m_PendingCritSec;
		CRITICAL_SECTION	m_ConnectCritSec;
		CRITICAL_SECTION	m_CloseCritSec;

	    volatile LONG		m_Connections;
		LONG                m_MaxConnections;
		volatile LONG		m_OutstandingSends;
		PerIOData*			m_FreeBufferList;
		CSocket*			m_FreeSocketList;

		PerIOData*			m_PendingSendList;
		PerIOData*			m_PendingSendListEnd;

		std::deque<CSocket*> m_NewConnetct;
		std::deque<CSocket*> m_NewClose;


	};

}
#endif
