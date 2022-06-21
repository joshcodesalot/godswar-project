//========================================================
//
//    Copyright (c) 2006,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� GameServer.cpp
//    ժ    Ҫ �� ��Ϸ�������������
//    
//    �滻�汾 �� 1.00
//    ��    �� �� �ֵ»�
//    ������� �� 2007-01-08
//
//    ��ǰ�汾 �� 2.00
//    ��    �� �� Fenjune Li
//    ������� �� 2008-06-15
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

	// �ͷ���Դ
	GameServer.Release();

	return 0;
}
