// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows XP ����߰汾�Ĺ��ܡ�
#define _WIN32_WINNT 0x0501	// ����ֵ����Ϊ��Ӧ��ֵ���������� Windows �������汾��
#endif						

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <memory.h>
#include <conio.h>
#include <iostream>
#include <list>
#include <fstream>
#include <vector>
#include <time.h>
#include <vector>
#include <map>
#include <winsock2.h>
#include "../Common/Include/Base/inc/SvrBase.h"

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
