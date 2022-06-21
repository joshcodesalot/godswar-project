#ifndef _FILE_BACK_H 
#define _FILE_BACK_H 

#include "../../Common/Include/Base/Inc/Thread.h"


class CBackup : private sbase::IThreadEvent 
{
public:

public:
	static CBackup *Instance();
	bool Init();
	void Release(){ delete this;};
protected:
	virtual int		OnThreadCreate	(void){ return 0; }		
	virtual int		OnThreadDestroy	(void){ return 0; }	
	virtual int		OnThreadEvent	(void){ return 0; }          
	virtual int		OnThreadProcess	(void);	
private:
	CBackup();
	~CBackup();
	static  CBackup *m_pBackup;
	sbase::CThread*	 m_pThread;
	sbase::CCriticalSection	m_xLock;
};


#endif