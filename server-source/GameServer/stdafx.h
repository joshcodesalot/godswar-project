// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#include <stdio.h>
#include <tchar.h>



// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <algorithm>
//#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <list>
#include <tchar.h>
#include <fstream>
//#include <hash_map>
#include <vector>
//#include <map>
#include <time.h>
#include <math.h>
#include <winsock2.h>

#include "..\Common/Include/Base\Inc\Timer.h"
#include "..\Common/Include/Base\Inc\Types.h"
#include "..\Common/Include/Base\inc\IniFile.h"
#include "..\Common/Include/Base\Inc\SvrBase.h"
#include "..\Common/Include/Base\inc\Ini.h"
#include "..\Common/Include/Base\inc\Msg.h"
#include "..\Common/Include/DB\Inc\IDB.h"
#include "..\Common/Include/Base\Inc\IPCChannel.h"

#include "..\Common/Include/sNet\Socket.h"
#include "..\Common/Include/sNet\IOCP.h"

#include "..\Common/Include/cNet\Socket.h"
#include "..\Common/Include/cNet\IOCP.h"


#ifdef _DEBUG
    #pragma comment( lib, "..\\Common\\Lib\\Debug\\DBD.lib" )
    #pragma comment( lib, "..\\Common\\Lib\\Debug\\PipeD.lib" )
	#pragma comment( lib, "..\\Common\\Lib\\Debug\\BaseD.lib" )
	#pragma comment( lib, "..\\Common\\Lib\\Debug\\SNetD.lib" )
	#pragma comment( lib, "..\\Common\\Lib\\Debug\\CNetD.lib" )
    #pragma comment( lib, "..\\Common\\Lib\\Debug\\PyScriptD.lib" )
#else
    #pragma comment( lib, "..\\Common\\Lib\\Release\\DB.lib" )
    #pragma comment( lib, "..\\Common\\Lib\\Release\\Pipe.lib" )
	#pragma comment( lib, "..\\Common\\Lib\\Release\\Base.lib" )
	#pragma comment( lib, "..\\Common\\Lib\\Release\\SNet.lib" )
	#pragma comment( lib, "..\\Common\\Lib\\Release\\CNet.lib" )
    #pragma comment( lib, "..\\Common\\Lib\\Release\\PyScript.lib" )
#endif


using namespace std;


#if !defined( SAFE_DELETE )
#define SAFE_DELETE( p )			{ if(( p ) != NULL) { delete ( p ); (p)=NULL; } }
#endif

#if !defined( SAFE_DELETE_ARRAY )
#define SAFE_DELETE_ARRAY( p )		{ if(( p ) != NULL) { delete[] ( p ); (p)=NULL; } }
#endif

#if !defined( SAFE_RELEASE )
#define SAFE_RELEASE( p )			{ if(( p ) != NULL) { (p)->Release(); (p)=NULL; } }
#endif

#if !defined( SAFE_FREE )
#define SAFE_FREE( p )				{ if(( p ) != NULL) { free(p); (p)=NULL; } }
#endif

// #ifdef _DEBUG
// void *  __cdecl memcpy(__out_bcount_full_opt(_MaxCount) void * _Dst, __in_bcount_opt(_MaxCount) const void * _Src, __in size_t _MaxCount);
// #endif
