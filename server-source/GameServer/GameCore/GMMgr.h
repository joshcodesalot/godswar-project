#ifndef _FILE_GMMGR_H 
#define _FILE_GMMGR_H 

#include "../../Common/Include/Base/Inc/Thread.h"
#include "..\..\Common/Include/Base\Inc\IPCChannel.h"
#include "./GMCrl.h"
#include "..\..\Common/Include\Pipe\IPipe.h"

static char*  SQL_SELECT_PLACARD     =  "CALL SP_SELECT_PLACARD();";
class CWorld;
class CGMMgr : private sbase::IThreadEvent 
{
	typedef ipc_np::CPipe* (*PipeServerCreate)(const char* );
public:

public:
	static CGMMgr *Instance();
	bool Init( CWorld * pWorld );
	void Load_Placard();
	void Release(){ delete this;};
protected:
	virtual int		OnThreadCreate	(void){ return 0; }		
	virtual int		OnThreadDestroy	(void){ return 0; }	
	virtual int		OnThreadEvent	(void){ return 0; }          
	virtual int		OnThreadProcess	(void);	
private:
    CGMMgr();
	~CGMMgr();
	sbase::CTimer timerLogin;		                //GM¼àÌý¼ÆÊ±Æ÷
	static  CGMMgr *m_pGM;
	sbase::CThread*	 m_pThread;
	//ipc::CNamePipe   m_pPipe;
	ipc_np::CPipe*		m_pPipe;
	sbase::CCriticalSection	m_xLock;
	CWorld * m_pWorld;
	HMODULE m_PipeDll;
};


#endif