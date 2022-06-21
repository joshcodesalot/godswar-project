// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料

// TODO: 在此处引用程序需要的其他头文件
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h > 
#include <mswsock.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <iostream>
#include "../Base/Inc/SvrBase.h"
using namespace std;

#pragma comment(lib,"Ws2_32.lib")
namespace snet
{
	#define MAX_COMPLETION_THREAD_COUNT 32 
	#define DEFAULT_OVERLAPPED_COUNT	8
	#define DEFAULT_BUFFER_SIZE			1024*2
	#define	BURST_ACCEPT_COUNT			50
	#define MAX_OVERLAPPED_SENDS		200
	#define SINGLE_BUFF_SIZE			1024
	#define OP_ACCEPT					0                  
	#define OP_READ						1                  
	#define OP_WRITE					2 

	const int	RECV_BUFFER_SIZE	= 32*1024;
	const int	SEND_BUFFER_SIZE	= 64*1024*2;
}


