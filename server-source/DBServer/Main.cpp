// BackUp.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include "./Master.h"

using namespace std;

 int _tmain(int , _TCHAR* )
 {   
	 CMaster BackUpMgr;
	 if ( !BackUpMgr.Init() )
	 {
		 return 0;
	 }

	 while ( !CMaster::m_stopEvent )
	 {
		 BackUpMgr.Run();
		 Sleep(10);
	 }

	 BackUpMgr.Release();
}
