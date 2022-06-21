// **********************************************************************
//
// Copyright (c) 2004
// All Rights Reserved
//
// Author: zeng cs
// Created:	2004/11/03   0:28
//
// **********************************************************************

#pragma once


#include <windows.h>
#include "SyncObjs.h"


namespace sbase
{
	CONST ULONG TIME_WAITINGCLOSE	= 3000;		// 3000ms
	
	class IThreadEvent  
	{
	public:
		virtual int		OnThreadCreate	(void)	= 0;		
		virtual int		OnThreadDestroy	(void)	= 0;
		
		virtual int		OnThreadEvent  	(void)	= 0;
		virtual int		OnThreadProcess	(void)	= 0;
	};
	

	class CThread  
	{
	public:
		CThread(IThreadEvent& event);
		virtual ~CThread();
		CThread & operator=( const CThread & ) {}
	public:
		enum { SUSPEND = true, RUN = false };
		bool	Init		(bool bSuspend = SUSPEND, DWORD dwWorkInterval = INFINITE);
		bool	Close		(void);
		void	Resume		(void);
		void	Suspend		(void);
		HANDLE  GetHandle   (){ return m_hThread; };
		
		void	SetEvent	(void)					{ ::SetEvent(m_hWorkEvent); }
		
		enum { STATUS_INIT = 0, STATUS_SUSPEND, STATUS_RUNNING, STATUS_CLOSING, STATUS_CLOSED };
		int		GetStatus	(void) const			{ return m_nStatus; }
		void    SetThreadAMask( int curAff ){ SetThreadAffinityMask( m_hThread, curAff ); };
	protected:
		DWORD				m_dwWorkInterval;
		IThreadEvent&		m_event;
		
	private:
		HANDLE				m_hThread;
		HANDLE				m_hExitEvent;
		HANDLE				m_hWorkEvent;
		
		DWORD				m_id;
		volatile int		m_nStatus;
		
		static DWORD WINAPI RunThread(LPVOID pThreadParameter);
		
	public:
		static CThread*	CreateNew(IThreadEvent& refEvent, bool bSuspend = SUSPEND, DWORD dwWorkInterval = INFINITE);
		
	};
}

