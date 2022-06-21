#ifndef _FILE_SOCKET_H_
#define _FILE_SOCKET_H_

#include "stdafx.h"
#include "CirBuffer.h"

namespace snet
{
	class PerIOData;
	class CIOCP;
	class IEncryptor;

	class CSocket
	{
	public:
		CSocket( CIOCP* pIOCP );
		~CSocket();
         
		void OnRead( PerIOData* pPerIOData );
		void OnWrite();

		int PostSend(PerIOData *Sendobj);
		int PostRecv(PerIOData *Recvobj);

		bool Write( void );
		bool PackMsg( char *pMsg, size_t iLen );

		bool Read( char **Paket );
		void Remove( size_t nLen ){ m_iBuffer.Remove( nLen ); };

		bool IsValid(){ return m_Sock != INVALID_SOCKET && !m_BClosing; };
		void Refresh(); 
		void Initnalize( SOCKET Sock,int Af );
		void Finalize();
		void Release(){ delete this;};

		void*  operator new	(size_t size);
		void   operator delete(void* p);

		size_t GetL(){ return m_oBuffer.GetLength();}

		snet::CIOCP* GetIOCP(){return m_pIOCP;}

		void SetPeerInfo(SOCKADDR *sa, int salen);
		const char*	GetPeerIP();

		size_t GetiSpace(){ return m_iBuffer.Space();} 
	public:
		CIOCP*             m_pIOCP;
		SOCKET             m_Sock;              
		int                m_Af;     
		volatile int       m_BClosing;        
		volatile LONG      OutstandingRecv;
		volatile LONG      OutstandingSend;
		volatile LONG      PendingSend;
		CRITICAL_SECTION   SockCritSec; 
		SOCKADDR_IN		   m_addrRemote;
		CSocket			   *next;
	private:
		IEncryptor      *m_Encryptor;
		CircularBuffer   m_iBuffer;
		CircularBuffer   m_oBuffer;

		char			m_IP[NI_MAXHOST];
		char			m_PORT[NI_MAXSERV];
	};
}

#endif