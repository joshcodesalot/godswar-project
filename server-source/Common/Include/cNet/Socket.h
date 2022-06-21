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
#ifndef _FILE_SOCKET_CLIENT_H
#define _FILE_SOCKET_CLIENT_H

#include "stdafx.h"
#include "CirBuffer.h"

namespace cnet
{
	class PerIOData;
	class CIOCP;
	class IEncryptor;

	class CSocket
	{
	public:
		CSocket( CIOCP* pIOCP );
		~CSocket();
		SOCKET               s;              
		int                  af;             
		BOOL                 bConnected;
		BOOL                 bClosing;      
		LPFN_CONNECTEX       lpfnConnectEx;
		LPFN_TRANSMITFILE    lpfnTransmitFile;
		volatile LONG        OutstandingRecv;
		volatile LONG        OutstandingSend;
		PerIOData *Repost;       
		CRITICAL_SECTION     SockCritSec;
		CSocket   *next,*prev;
	public:
		void OnRead( PerIOData* pPerIOData );
		void Release(){ delete this; }

		//�Ը�socketͶ�ݲ���
		int PostSend(PerIOData *Sendobj);
		int PostRecv(PerIOData *Recvobj);

		//������Ͷ�ݷ�������
		bool Write( void );
		//��Ϸ�߼��򻺳�����д����
		bool PackMsg( char *pMsg, size_t iLen );

		//�ӻ������ж����ѽ��ܵ�������
		bool Read( char **Paket );
		//��Ϸ�߼�������ϣ��ƶ�������ָ��
		void Remove( size_t nLen ){ m_iBuffer.Remove( nLen ); };

		bool IsValid(){ return s != INVALID_SOCKET && !m_BClosing; };
		void Refresh(); 
		void ZeroBufMem(){ s = INVALID_SOCKET;m_iBuffer.Clear();m_oBuffer.Clear();};


		void* operator new (size_t size);
		void  operator delete(void* p);

		CIOCP* GetIOCP(){return m_pIOCP;}

		const char*	GetPeerIP();

	protected:
	private:
		string			m_strIP;
		CIOCP*          m_pIOCP;

		CircularBuffer	m_iBuffer;
		CircularBuffer	m_oBuffer;
		IEncryptor      *m_Encryptor;

		int                m_Af;     
		volatile int       m_BClosing;        
		volatile LONG      PendingSend;

	};
}




#endif