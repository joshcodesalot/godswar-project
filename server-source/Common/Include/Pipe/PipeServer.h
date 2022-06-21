//========================================================
//
//    Copyright (c) 2008,�������˹�����
//    All rights reserved.
//
//    �ļ����� �� PipeServer.h 
//    ժ    Ҫ �� �ܵ������
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
