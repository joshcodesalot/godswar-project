// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料



// TODO: 在此处引用程序需要的其他头文件
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "../Base/Inc/SvrBase.h"
using namespace std;


#pragma comment(lib,"Ws2_32.lib")
namespace cnet
{

	#define DEFAULT_BUFFER_SIZE         1024   // default buffer size
	#define DEFAULT_OVERLAPPED_COUNT    1      // Number of overlapped recv per socket
	#define DEFAULT_CLIENT_CONNECTIONS  10     // Number of connections to initiate
	#define DEFAULT_FILE_SIZE           2000000// Default size of file for TransmitFile
	#define DEFAULT_SEND_COUNT          100    // How many send/TransmitFiles to perform

	#define OP_CONNECT      0                   // ConnectEx
	#define OP_READ         1                   // WSARecv/WSARecvFrom
	#define OP_WRITE        2                   // WSASend/WSASendTo
	#define OP_TRANSMIT     3                   // TransmitFile


	const int	RECV_BUFFER_SIZE	= 64*1024*2;
	const int	SEND_BUFFER_SIZE	= 64*1024*2;
}
