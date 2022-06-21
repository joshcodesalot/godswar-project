
//========================================================
//
//    Copyright (c) 2006,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� Player.h
//    ժ    Ҫ �� ��̬������ģ��
//    
//    ��ǰ�汾 �� 1.01
//    ��    �� �� ����
//    ������� �� 2007-01-17
//
//    ��    �� �� zeng cs
//    ������� �� 2004/11/03   
//
//    ��ע:  1)������Release�汾��Log���
//========================================================

#pragma once

#pragma warning(disable:4996)
#include <windows.h>
#include <stdio.h>
#include <assert.h>

#include "Types.h"
#include "Heap.h"
#include "String.h"

//-------------------Memory leak--------------------------------
#define _CRTDBG_MAP_ALLOC
#include<stdlib.h>
#include<crtdbg.h>
//--------------------------------------------------------------


// macro...
#undef	SAFE_DELETE
#define SAFE_DELETE(ptr)		{ if(ptr){ try{ delete ptr; }catch(...){ /*sbase::SysLogSave("CATCH: *** SAFE_DELETE() crash! *** at %s, %d", __FILE__, __LINE__);*/ } ptr = 0; } }

#undef	SAFE_RELEASE
#define SAFE_RELEASE(ptr)		{ if(ptr){ try{ ptr->Release(); }catch(...){ sbase::SysLogSave("CATCH: *** SAFE_RELEASE() crash! *** at %s, %d", __FILE__, __LINE__); } ptr = 0; } }


#undef	ASSERT
// #undef	CHECK
#undef	IF_OK
#undef	IF_NOT
#undef	PURE_VIRTUAL

#ifdef	_DEBUG
	#define		ASSERT			assert

	#define		IF_OK(x)		if( ((x)) ? 1 : ( assert(!("IF_OK: " #x)),0 ) )
	#define		IF_NOT(x)		if( (!(x)) ? ( assert(!("IF_NOT: " #x)),1 ) : 0 )

	#define		PURE_VIRTUAL	= 0;
#else
	#define		ASSERT(x)		 if(!(x)) { sbase::SysLogSave("��ASSERT(%s)�� in %s, %d", #x, __FILE__, __LINE__);  }

	#define		IF_OK(x)		if( ((x)) ? 1 : ( sbase::SysLogSave("��IF_OK(%s)�� in %s, %d", #x, __FILE__, __LINE__),0 ) )
	#define		IF_NOT(x)		if( (!(x)) ? ( sbase::SysLogSave("��IF_NOT(%s)�� in %s, %d", #x, __FILE__, __LINE__),1 ) : 0 )

	#define		PURE_VIRTUAL	{ ASSERT(!"PURE_VIRTUAL_FUNCTION"); }
#endif


// functions...
namespace sbase
{
	void	LogSave			(const char* pszName, const char* fmt, ...);
	void	SysLogSave		(const char* fmt, ...);
	void	GmLogSave		(const char* fmt, ...);
	void	ErrorMsg		(const char* fmt, ...);
	void	DebugMsg		(const char* fmt, ...);
	void	DebugLogMsg		(const char* fmt, ...);
	void    LogException    (const char* psbuf, ...);

	
	enum TIME_TYPE{ TIME_MILLISECOND=0, TIME_SECOND, TIME_MINUTE, TIME_DAY, TIME_DAYTIME, TIME_STAMP };
	time_t	TimeGet			(TIME_TYPE type=TIME_MILLISECOND);
	
	int		RandGet			(INT nMax, bool bRest=false);
	double	RandomRateGet	(double dRange);
	
	bool	StrCopy			(char* pszTarget, const char* pszSource, UINT unBufSize);
	LPCTSTR	DumpBuffer		(const char* buf, UINT nSize);
	bool	StrCheck		(const char* pszString);
	bool	LegalStrCheck	(const char* pszString);
	
	void	ConsoleWriteStr (DWORD nCoordX, DWORD nCoordY, const char* fmt, ... );
	void	ConsoleSetup	(DWORD dwWindowSizeX = 80, DWORD dwWindowSizeY = 80 );
	void    ConsoleWriteColorText( DWORD nColor,const char* fmt,... );
	void    ConsoleWriteText( const char* fmt,... );
	void    SetConsoleFontColor( DWORD Color=FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED );
	void    SetConsoleTitle( const char* fmt,... );

	bool	CnStrInit		(void);
	void	CnStrDestroy	(void);
	char*	CnStrGet		(char* pszStrIndex);

	ULONG	MaxAxisOffsetGet(OBJPOS posThis, OBJPOS posThat);
	bool	IsValidDistance	(OBJPOS lhs, OBJPOS rhs, UINT unDistance);
}

#define LOG		sbase::LogSave
#define LOGSYS	sbase::SysLogSave
#define LOGERR	sbase::ErrorMsg
#define LOGDBG	sbase::DebugMsg
#define LOGGM	sbase::GmLogSave

