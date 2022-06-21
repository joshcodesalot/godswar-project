//========================================================
//
//    Copyright (c) 2006,欢乐连线工作室
//    All rights reserved.
//
//    文件名称 ： GameServer.cpp
//    摘    要 ： 游戏服务器基础框架
//    
//    替换版本 ： 1.00
//    作    者 ： 林德辉
//    完成日期 ： 2007-01-08
//
//    当前版本 ： 2.00
//    作    者 ： Fenjune Li
//    完成日期 ： 2008-06-15
//
//========================================================

#include "stdafx.h"		
#include "World.h"
#include "GSMaster.h"
#include <MMSystem.h>
#pragma  comment(lib, "winmm.lib")

int _tmain(int , _TCHAR* )
{

	GSMaster GameServer;

	if( !GameServer.Init() )
	 	return -1;

	while( !CWorld::m_stopEvent )
	{	
		GameServer.Run();
	}

	// 释放资源
	GameServer.Release();

	return 0;
}
