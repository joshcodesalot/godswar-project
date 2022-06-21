//========================================================
//
//    Copyright (c) 2008,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� PerIOData.h
//    ժ    Ҫ �� �������������ͷ�ļ�
//    
//    ��ǰ�汾 �� 1.00
//    ��    �� �� ����
//    ������� �� 2008-05-01
//
//========================================================

#ifndef _PERIODATA_CLIENT_H_
#define _PERIODATA_CLIENT_H_

#include "Socket.h"

namespace cnet
{
	class  PerIOData
	{
	public:

		PerIOData( int Len );
		~PerIOData();

		WSAOVERLAPPED        ol;
		HANDLE               hFile;         // Open file handle for TransmitFile
		char                *buf;           // Buffer for recv/send/AcceptEx
		int                  buflen;        // Length of the buffer
		int                  operation;     // Type of operation issued

		SOCKADDR_STORAGE     addr;
		int                  addrlen;

		CSocket*			pSocket;

		PerIOData *next;

	public:

		void* operator new (size_t size);
		void  operator delete(void* p);

		void Release(){ delete this; }

	protected:
	private:
	};
}

#endif // _PERIODATA_CLIENT_H_

