//========================================================
//
//    Copyright (c) 2008,欢乐连线工作室
//    All rights reserved.
//
//    文件名称 ： PerIOData.h
//    摘    要 ： 单句柄数据声明头文件
//    
//    当前版本 ： 1.00
//    作    者 ： 李锋军
//    完成日期 ： 2008-05-01
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
