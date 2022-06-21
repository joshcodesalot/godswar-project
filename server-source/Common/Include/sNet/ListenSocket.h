//========================================================
//
//    Copyright (c) 2008,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� ListenSocket.h
//    ժ    Ҫ �� ����Socket��ͷ�ļ�
//    
//    ��ǰ�汾 �� 1.00
//    ��    �� �� ����
//    ������� �� 2008-05-01
//
//========================================================

#ifndef _FILE_CLIENTSOCKET_H_
#define _FILE_CLIENTSOCKET_H_

#include "PerIOData.h"

namespace snet
{
	class  CLisetenSocket
	{
		friend class CIOCP;
	public:
		CIOCP           *m_pIOCP;
		SOCKET          m_Socket;
		int             m_AddressFamily;
		PerIOData       *m_PendingAccepts; 
		volatile long   m_PendingAcceptCount;
		int             m_iHiWaterMark;
		int          	m_iLoWaterMark;
		HANDLE          m_HAcceptEvent;
		HANDLE          m_HRepostAccept;
		volatile long   m_RepostCount;
		LPFN_ACCEPTEX             m_lpfnAcceptEx;
		LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptExSockaddrs;
		CRITICAL_SECTION ListenCritSec;
		CLisetenSocket *Next;
		bool Init( addrinfo *Ptr, CIOCP *pIOCP );
		int PostAccept( PerIOData *acceptobj );
		void InsertPendingAccept( PerIOData *BufObj );
		void RemovePendingAccept( PerIOData *BufObj );

	};
}
#endif
