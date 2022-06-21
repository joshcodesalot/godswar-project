//========================================================
//
//    Copyright (c) 2008,福州天盟工作室
//    All rights reserved.
//
//    文件名称 ： PipeClient.h 
//    摘    要 ： 管道客户端 
//    
//    当前版本 ： 1.0
//    作    者 ： 蔡智强
//    完成日期 ： 2008-7-1
//    说    明 ：
// 
//========================================================


#pragma  once

#include "PipeBase.h"

namespace ipc_pipe
{
	
	class PIPE_DLL CPipeClient: public CPipeBase
	{
	public:

		CPipeClient(TYPE ChannelType);
		~CPipeClient();
		
		bool	  InitClient(const char* pPipeName, const char* pServerIp, const char* pUserName, const char* pUserPwd);

		void*     Read(int&  iBytesRead);
		void*     Write(void *pSend, int size , int& iBytesSend);

	protected:

	private:

		CRITICAL_SECTION	m_ClientCriSec;

		


	};

}
