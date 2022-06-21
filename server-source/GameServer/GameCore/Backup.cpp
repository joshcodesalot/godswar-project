#include "stdafx.h"
#include ".\BackUP.h"
#include "../ServiceLoader.h"
#include "../GSMaster.h"

#pragma warning(disable:4702)

CBackup *CBackup::m_pBackup = NULL;

CBackup::CBackup():m_pThread(NULL)
{

}

CBackup::~CBackup()
{
	SAFE_DELETE( m_pThread );
}

bool CBackup::Init()
{
	m_pThread = sbase::CThread::CreateNew(*this, sbase::CThread::RUN, 11 );
	if (!m_pThread)
	{
		::MessageBox(NULL, "Create NetWorker thread fail.", "Error", MB_OK|MB_ICONERROR);
	}
	m_pThread->SetThreadAMask( 12 );
	return true;
}

CBackup *CBackup::Instance()
{
	if (   NULL == m_pBackup )
	{
		m_pBackup = new CBackup;
	}
	return m_pBackup;
}

int	CBackup::OnThreadProcess(void)
{
	for ( ; ; )
	{
       // BackUp();
		Sleep(10);
	}
	return 1;
}

