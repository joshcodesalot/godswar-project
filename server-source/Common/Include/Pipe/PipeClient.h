//========================================================
//
//    Copyright (c) 2008,�������˹�����
//    All rights reserved.
//
//    �ļ����� �� PipeClient.h 
//    ժ    Ҫ �� �ܵ��ͻ��� 
//    
//    ��ǰ�汾 �� 1.0
//    ��    �� �� ����ǿ
//    ������� �� 2008-7-1
//    ˵    �� ��
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
