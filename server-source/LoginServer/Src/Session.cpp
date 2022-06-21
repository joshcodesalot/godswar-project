#include "..\Inc\Session.h"
#include "..\inc\LoginLogic.h"

DEFINE_FIXEDSIZE_ALLOCATOR(CSession, 2000, CMemoryPool::GROW_FAST);

CSession::CSession(const char* Name, const char* PassWord, void* Socket, login::CLogin& Logion) :
	m_Logion(Logion), m_bGS(false), m_bDelAccout(false)
{
	strcpy(m_Name, Name);
	strcpy(m_PassWord, PassWord);
	m_Socket = Socket;
}

CSession::~CSession()
{

}

void   CSession::OnDBRequest() const
{
	char szSQL[128];
	snet::CSocket* pSocket = static_cast<snet::CSocket*>(m_Socket);
	sprintf(szSQL, SQL_SELECT_ACCOUNT_INFO, m_Name, m_PassWord, pSocket->GetPeerIP());

	m_Logion.m_pDB->ExecuteAsyncSQL(szSQL, (void*)this, &CSession::OnDBResponse);
}

void CSession::OnDBResponse(dbtool::PSQL_RESULT result)
{
	IF_NOT(result)
	{
		printf("One Error...");
		return;
	}
	CSession* pSession = static_cast<CSession*>(result->pPlayer);
	if (pSession == NULL)
	{
		printf("Two Error...");
		return;
	}

	dbtool::IRecordset* pRes = result->pResult;
	if (!pRes)
	{
		printf("Three Error...");
		SAFE_DELETE(pSession);
		return;
	}

	int RecordCount = pRes->CountRecord();
	if (0 == RecordCount)
	{
		printf("Four Error...");
		SAFE_DELETE(pSession);
		return;
	}

	pRes->Move(0);
	dbtool::IRecord* Record = pRes->GetRecord();
	IF_NOT(Record)
	{
		printf("Five Error...");
		SAFE_DELETE(pSession);
		return;
	}

	enum { AC_EXIT, AC_BAN, IP_BAN };
	char Result = Record->Field(UINT(AC_EXIT));
	char ban_account = Record->Field(UINT(AC_BAN));
	char ban_ip = Record->Field(UINT(IP_BAN));


	MSG_LOGIN_RETURN_INFO  Login_info;
	Login_info.Head.usSize = sizeof(MSG_LOGIN_RETURN_INFO);
	Login_info.Head.usType = _MSG_LOGIN_RETURN_INFO;
	Login_info.ucInfo = Result;

	if (Result == 1)
	{
		if (ban_account == 1)
			Login_info.ucInfo++;

		if (ban_ip == 1)
			Login_info.ucInfo += 2;
	}


	pSession->m_Logion.SendMsg(&Login_info, pSession->m_Socket, Login_info.Head.usSize);

	pSession->m_Logion.CacheAccounts(pSession->GetAccount(), pSession);

	if (Result == 1)
	{
		login::CLogin::GAMESERVER_ID::iterator itor;
		for (itor = pSession->m_Logion.m_GameServerMap.begin(); itor != pSession->m_Logion.m_GameServerMap.end(); itor++)
		{
			MSG_GAMESERVER_INFO  GameSer = *(*itor).second;
			memset(GameSer.cIP, 0L, sizeof(GameSer.cIP));
			GameSer.uiPort = 0;
#if GW2
			GameSer.cRecommend = 1;
			GameSer.cFlag = 1;
#endif
			pSession->m_Logion.SendMsg(&GameSer, pSession->m_Socket, GameSer.Head.usSize);
		}

	}

}

