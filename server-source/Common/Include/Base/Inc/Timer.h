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
#include <time.h>
#include <stdlib.h>


namespace sbase
{
	//////////////////////////////////////////////////////////////////////
	// ��ʱ�࣬�ԡ��롱Ϊ��λ
	//////////////////////////////////////////////////////////////////////
	class CTimer  
	{
	public:
		CTimer(int nIntervalSecs = 0) { m_nInterval = nIntervalSecs; m_tUpdateTime = 0; }		// =0 : ���������ʼ��
		// virtual ~CTimer() {}
		CTimer& operator = (const CTimer& rhs)
		{
			m_nInterval		= rhs.m_nInterval;
			m_tUpdateTime	= rhs.m_tUpdateTime;
			return *this;
		}
		
	public:
		// ����ʱ���ǩ
		void	Update		(void)					{ m_tUpdateTime = time(NULL); }
		
		// ���ڶ�ʱ����������ʱ�ӡ�ÿ�δ���ʱΪtrue��
		bool	ToNextTime	(void)					{ if(IsExpire()) return Update(),true; else return false; }
		
		// ��ʼ��ʱ��Ƭ��(��������
		void	SetInterval	(int nSecs)				{ m_nInterval = nSecs; }
		
		// ��ʼ����ʱ��(ͬSetInterval)
		void	Startup		(int nSecs)				{ m_nInterval = nSecs; Update(); }
		
		// ʱ���Ƿ񴥷�(ֻ����һ��)����Startup���ʹ�ã���ʵ��ֻ����һ�ε�ʱ�ӡ�
		bool	TimeOver	(void)					;//{ if(IsActive() && IsExpire()) return Clear(),true; return false; }
		void	Clear		(void)					{ m_tUpdateTime = m_nInterval = 0; }
		bool	IsActive	(void)					{ return m_tUpdateTime != 0; }
		void	IncInterval	(int nSecs, int nLimit)	{ m_nInterval = __min(m_nInterval+nSecs, nLimit); }
		void	DecInterval	(int nSecs)				{ m_nInterval = __max(m_nInterval-nSecs, 0); }
		
		// ����Ƿ�ʱ
		bool	IsExpire	(void)	;			

		// ����Ƿ�ʱ��ָ��ʱ��Ƭ������ͬһʱ���ǩ�������ͬ��ʱ�¼���(��ʵ��һ��������ƶ��ʱ��)
		bool	IsExpire	(int nSecs)				{ return time(NULL) >= m_tUpdateTime+nSecs; }
		//��鵱ǰʱ���Ƿ񳬹�����ָ��ʱ�俪ʼ��ָ��ʱ������
		bool    IsExpire    ( time_t Start, int Interval){ return time(NULL) >= Start + Interval; }
		
		// ���ڱ䳤�ĳ�ʱ�¼���
		bool	ToNextTime	(int nSecs)				{ if(IsExpire(nSecs)) return Update(),true; else return false; }
		
		// ʱ�ӻ���ۣ�������ֵ���������ֵ������ͬToNextTime(...)
		bool	ToNextTick	(int nSecs);
		//	void	AppendInterval	(int nSecs)			{ if(ToNextTime()) m_nInterval=nSecs; else m_nInterval+=nSecs; }	// ToNextTime(): ��֤������nSecs��
		
	public: // get
		time_t		GetRemain()							{ return m_tUpdateTime ? __min(__max(m_nInterval - (time(NULL)-(int)m_tUpdateTime), 0), m_nInterval) : 0; }
		
	protected:
		int		m_nInterval;
		time_t	m_tUpdateTime;
	};
	
	
	//////////////////////////////////////////////////////////////////////
	// ��ʱ�࣬�ԡ����롱Ϊ��λ
	//////////////////////////////////////////////////////////////////////
	class CTimerMS
	{
	public:
		CTimerMS(int nIntervalSecs = 0) { m_nInterval = nIntervalSecs; m_tUpdateTime = 0; }		// =0 : ���������ʼ��
		// virtual ~CTimerMS() {}
		CTimerMS& operator = (const CTimerMS& rhs)
		{
			m_nInterval		= rhs.m_nInterval;
			m_tUpdateTime	= rhs.m_tUpdateTime;
			return *this;
		}
		
	public:
		void	Update		(void)					{ m_tUpdateTime = clock(); }
		bool	IsExpire	(void)					{ return clock() >= m_tUpdateTime+m_nInterval; }
		bool	ToNextTime	(void)					{ if(IsExpire()) return Update(),true; else return false; }
		void	SetInterval	(int nMilliSecs)		{ m_nInterval = nMilliSecs; }
		
		void	Startup		(int nMilliSecs)		{ m_nInterval = nMilliSecs; Update(); }
		bool	TimeOver	(void)					{ if(IsActive() && IsExpire()) return Clear(),true; return false; }
		void	Clear		(void)					{ m_nInterval = m_tUpdateTime = 0; }
		bool	IsActive	(void)					{ return m_tUpdateTime != 0; }
		void	IncInterval	(int nMilliSecs, int nLimit)		{ m_nInterval = __min(m_nInterval+nMilliSecs, nLimit); }
		void	DecInterval	(int nMilliSecs)		{ m_nInterval = __max(m_nInterval-nMilliSecs, 0); }
		
		bool	IsExpire	(int nMilliSecs)		{ return clock() >= m_tUpdateTime+nMilliSecs; }
		bool	ToNextTime	(int nMilliSecs)		{ if(IsExpire(nMilliSecs)) return Update(),true; else return false; }
		bool	ToNextTick	(int nMilliSecs);
		//	void	AppendInterval	(int nMilliSecs)	{ if(ToNextTime()) m_nInterval=nMilliSecs; else m_nInterval+=nMilliSecs; }	// ToNextTime(): ��֤������nSecs��
		
	public: // get
		int		GetInterval	(void)					{ return m_nInterval; }
		DWORD	GetUpdateTime(void)					{ return m_tUpdateTime; }
		
	protected:
		int		m_nInterval;
		clock_t	m_tUpdateTime;
	};
}
