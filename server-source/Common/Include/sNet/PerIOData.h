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

#ifndef _FILE_PERIODATA_H_
#define _FILE_PERIODATA_H_

#include "Socket.h"

namespace snet
{
	class  PerIOData
	{
	public:

		PerIOData(int BufLen);
		~PerIOData();

		WSAOVERLAPPED    m_OL;
		SOCKET           m_Client;
		HANDLE           m_HPostAccept;
		char*            m_Buf;
		int              m_BufLen;
		int              m_iOperation; 
		CSocket*         m_Socket;
		PerIOData*       m_Next;

	public:

   		void* operator new (size_t size);
   		void  operator delete(void* p);

		void Release(){ delete this; }

	protected:
	private:
	};
}

#endif
