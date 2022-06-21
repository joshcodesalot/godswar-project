// **********************************************************************
//
// Copyright (c) 2004
// All Rights Reserved
//
// Author: zeng cs
// Created:	2004/11/03   0:27
//
// **********************************************************************

#pragma once

#include <windows.h>
#include <tchar.h>
#include "stdio.h"

namespace sbase
{

	//////////////////////////////////////////////////////////////////////
	class ILockObj
	{
	public:
		virtual void Lock	(void)	= 0;
		virtual void Unlock	(void)	= 0;
	};
	

	//////////////////////////////////////////////////////////////////////
	class CCriticalSection : public ILockObj
	{
	public:
		CCriticalSection( DWORD = 2000 )
		{
			//InitializeCriticalSectionAndSpinCount( &m_CritSec , spincount);
			InitializeCriticalSection( &m_CritSec );
		};
		
		virtual ~CCriticalSection()
		{
			DeleteCriticalSection( &m_CritSec );
		};
		
		void    Lock(void)
		{
			EnterCriticalSection( &m_CritSec );
		};
		
		void    Unlock(void)
		{
			LeaveCriticalSection( &m_CritSec );
		};
		
	private:
		CRITICAL_SECTION    m_CritSec;
	};
	
	
	//////////////////////////////////////////////////////////////////////
	class CMutexLock : public ILockObj
	{
	public:
		CMutexLock() : m_hMutex(NULL) {}
		~CMutexLock(void)	{ if (m_hMutex) ::CloseHandle(m_hMutex); }
		
		void Lock	(void)	{ if (m_hMutex) ::WaitForSingleObject(m_hMutex, INFINITE); }
		void Unlock	(void)	{ if (m_hMutex) ::ReleaseMutex(m_hMutex); }
		
	private:
		HANDLE m_hMutex;
		
		// static
	public:
		static CMutexLock* CreateNew(const char* pszName = NULL, bool bExistTest = false)
		{
			CMutexLock* pMutex = new CMutexLock;
			if (!pMutex)
				return NULL;
			
			pMutex->m_hMutex = ::CreateMutexA(NULL, false, pszName);
			if (!pMutex->m_hMutex)
			{
				delete pMutex;
				return NULL;
			}
			
			if (pszName && bExistTest)
			{
				if (ERROR_ALREADY_EXISTS == ::GetLastError())
				{
					delete pMutex;
					return NULL;
				}
			}
			
			return pMutex;
		}
	};
	

	//////////////////////////////////////////////////////////////////////
	class CSingleLock 
	{
	public:
		CSingleLock(ILockObj* pLock)
			: m_pLock(pLock)
		{ 		
			if (m_pLock) 
				m_pLock->Lock(); 
		}
		
		~CSingleLock	(void)	
		{
			
			if (m_pLock)
			{
				//printf("Unlock Start...\n");
				m_pLock->Unlock();
				//printf("Unlock Over...\n");
			
			}

		}
		
	private:
		ILockObj* m_pLock;
	};

} // end of namespace sbase

