// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>
#include <string.h>
#include <memory.h>
#include <conio.h>
#include <iostream>
#include <list>
#include <fstream>
#include <vector>
#include <time.h>
#include <vector>
#include <map>
#include "../../Common/Include/Base/inc/SvrBase.h"
// TODO: reference additional headers your program requires here
 
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

#pragma comment( lib, "user32.lib" )
#pragma comment( lib, "winmm.lib" )
