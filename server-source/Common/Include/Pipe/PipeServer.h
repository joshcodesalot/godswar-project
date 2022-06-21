//========================================================
//
//    Copyright (c) 2008,福州天盟工作室
//    All rights reserved.
//
//    文件名称 ： PipeServer.h 
//    摘    要 ： 管道服务端
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
	class PIPE_DLL CPipeServer : public CPipeBase
	{
	public:

		CPipeServer(TYPE ChannelType);
		~CPipeServer();
		bool      InitServer(const char * cName);
		bool      Accept();
		void*     Read(int&  iBytesRead);
		void*     Write(int& iBytesSend);

	protected:

	private:

		HANDLE	  m_hPipeEvent;

		OVERLAPPED m_ol;

	};

}
