#include "stdafx.h"

#include <winsock2.h>	  
#include <iostream>
#include <windows.h>
#include "GSMaster.h"
#include "ServiceLoader.h"
#include "DataCore.h"
#include "GameCore\GameObject.h"
#include "GameCore\Player.h"
#include "GameCore\NPC.h"
#include "GameCore\Mall.h"
#include "GameCore\Region.h"
#include "GameCore\KeyWords.h"
#include "..\Common\MsgDefine.h"
#include "..\Common\MsgTypes.h"
#include "..\Common/Include/DB\\Inc\Database.h"
#include "ChatCommand.h"
#include ".\GameCore\RoleRelation.h"
#include "..\Common\MsgDefine.h"
#include "GameCore\Player.h"
#include "Dial.h"
#include "GameCore\GameData.h"
#include "World.h"

#include "errormessage.h"

#define MAX_LOGIN_QUEUE 20

#pragma comment( lib, "user32.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "wsock32.lib") 

#pragma warning(disable:4390)

map< string, string > RegisterService::m_valid_map;
map< snet::CSocket*, string> RegisterService::m_Soket_map;
sbase::CCriticalSection RegisterService::m_xLock;

extern MSG_GOLD_BOX_RETURN msg_gold_box_return;

using namespace std;

extern void  GOutPut(int a);

static USHORT MsgRouter[2000];

DBService::DBService(CWorld* pWorld)
{
	m_pWorld = pWorld;
}

bool DBService::Init()
{
	char szDBServer[64] = "";
	char szLoginName[64] = "";
	char szPassword[64] = "";
	char szDBName[64] = "";

	sbase::CIni ini("config.ini", false);
	strcpy(szDBServer, ini.GetString("Database", "DBServer"));
	strcpy(szLoginName, ini.GetString("Database", "LoginName"));
	strcpy(szPassword, ini.GetString("Database", "Password"));
	strcpy(szDBName, ini.GetString("Database", "DBName"));
	m_pDB = dbtool::DatabaseCreate(szDBServer, szLoginName, szPassword, szDBName);

	m_Timer.Startup(DB_TIMER);
	Start();
	return true;
}

void DBService::Release()
{
	delete this;
}

void DBService::Run()
{
	if (!IsOpen())
		return;

	if (m_Timer.IsExpire())
	{
		m_Timer.Update();
	}
}

PipeService::PipeService(CWorld* pWorld)
{
	m_pWorld = pWorld;
}

bool PipeService::Init()
{
	printf(">>Initialize DB PipeClient...\n");

	m_hPipe = LoadLibrary("Pipe.dll");

	if (m_hPipe == NULL)
	{
		printf("LoadLibrary Failed!\n");

		FreeLibrary(m_hPipe);

		return false;
	}

	ReadIni();

	CreatePipeClient pipeclient = (CreatePipeClient)GetProcAddress(m_hPipe, "PipeCliCreate");

	m_pWorld->g_pBackUpMgr = pipeclient(PIPE_CLIENT_DB, m_szDBIp, m_szUserName, m_szUserPwd);

	if (m_pWorld->g_pBackUpMgr == NULL)
	{
		printf("Initialize DB PipeClient Failed!\n");

		FreeLibrary(m_hPipe);

		return false;
	}


	m_pThread = sbase::CThread::CreateNew(*this, sbase::CThread::RUN, 10);
	if (m_pThread == NULL)
	{
		printf("CThread::CreateNew failed!\n");
		return false;
	}
	m_pThread->SetThreadAMask(12);
	m_Timer.Startup(PIPE_TIMER);
	m_ConsortiaTimer.Startup(CONSOR_BACKUP_TIMEOUT);
	m_RegTimer.Startup(REGISTER_DB_TIMER);
	m_CheckTimer.Startup(CHECKT_PIPE_TIMER);

	if (!ConnectDBServer())
	{
		m_pWorld->g_pBackUpMgr->SetStatus(false);
		return false;

	}

	Start();

	return true;
}

void PipeService::ReadIni()
{
	memset(m_szDBIp, 0, sizeof(m_szDBIp));
	memset(m_szUserPwd, 0, sizeof(m_szUserPwd));
	memset(m_szUserName, 0, sizeof(m_szUserName));

	sbase::CIni ini("config.ini", false);

	strcpy(m_szDBIp, ini.GetString("PipeServer", "IP"));
	strcpy(m_szUserName, ini.GetString("PipeServer", "LoginName"));
	strcpy(m_szUserPwd, ini.GetString("PipeServer", "PassWord"));
}

void PipeService::Release()
{

	SAFE_DELETE(m_pThread);

	FreeLibrary(m_hPipe);

	delete this;
}

int PipeService::OnThreadProcess()
{

	if (!OnSend())
	{
		m_pWorld->g_pBackUpMgr->SetStatus(false);

	}

	return 1;

}

void PipeService::Run()
{
	if (!IsOpen() && !CWorld::m_startEvent)
		return;

	if (m_RegTimer.IsExpire())
	{
		m_RegTimer.Startup(REGISTER_DB_TIMER);

		if (!m_pWorld->g_pBackUpMgr->GetStatus())
		{
			if (!ConnectDBServer())
			{
				return;
			}
			else
			{

				Start();
				m_pWorld->g_pBackUpMgr->SetStatus(true);
				return;

			}
		}

	}

	if (m_Timer.IsExpire())
	{
		m_Timer.Startup(PIPE_TIMER);
		m_pWorld->BackUpPlayer();

	}

	if (m_ConsortiaTimer.IsExpire())
	{
		m_ConsortiaTimer.Startup(CONSOR_BACKUP_TIMEOUT);
		m_pWorld->BackUpConsortia();

	}
}

bool PipeService::ConnectDBServer()
{

	ReadIni();

	m_pWorld->g_pBackUpMgr->Init();

	return m_pWorld->g_pBackUpMgr->GetStatus();
}

bool PipeService::RegisterDBServer()
{

	return true;
}

bool PipeService::OnRead()
{
	return false;

}

bool PipeService::OnSend()
{
	BACKUP_MSG* pMsg = NULL;

	int nSendLen;

	while (m_pWorld->ReadBackBuff((char**)&pMsg))
	{
		if (!m_pWorld->g_pBackUpMgr->Write(pMsg, pMsg->Head.usSize, nSendLen))
		{
			return false;
		}

		m_pWorld->RemoveBackBuff(pMsg->Head.usSize);

		Sleep(1);
	}

	return true;
}

CommandService::CommandService(CWorld* pWorld)
{
	m_pWorld = pWorld;
}
bool CommandService::Init()
{
	m_Timer.Startup(COMMAND_TIMER);
	Start();
	return true;
}

void CommandService::Release()
{
	delete this;
}

void CommandService::Run()
{
	if (!IsOpen())
		return;

	if (m_Timer.IsExpire())
	{
		if (Command() == -1)
			exit(1);

		m_Timer.Update();
	}


}

KeyCommand* CommandService::GetConmandFunTable()
{
	return Commands;
}

void CommandService::Key_Help(char*, pFunPrintf pPrintf)
{
	printf("\n");
	for (int a = 0; a < AllCmds; a++)
	{
		printf("%-16s %-20s%-25s\n", Commands[a].cmd, "-", Commands[a].Note);
	}
	pPrintf("\nGodsWar> ");
}

void CommandService::Key_Exit(char*, pFunPrintf pPrintf)
{
	pPrintf("Exiting daemon...\n");
	exit(0);
}

void CommandService::Key_Quit(char*, pFunPrintf pPrintf)
{
	pPrintf("Exiting daemon...\n");
	exit(0);
}

void CommandService::Key_List(char*, pFunPrintf pPrintf)
{
	pPrintf("  ...[Execute]\n");
	for (int i = 0; i < MAX_REGION; i++)
	{
		printf("\nRegionID:%02d\tOnline: %d\t%s", i, m_pWorld->GetRegion(i)->GetOnline(), m_pWorld->GetRegion(i)->GetName().c_str());
	}
	pPrintf("\n\nAll Online: %d\n", m_pWorld->GetNowPlayer());
	pPrintf("\nGodsWar> ");
}

void CommandService::Key_Fps(char*, pFunPrintf pPrintf)
{
	pPrintf("  ...[Execute]\n");
	pPrintf("Fast FPS:%d\t\tAI FPS: %d\n", m_pWorld->nFPS, m_pWorld->nAIFPS);
	pPrintf("\nGodsWar> ");
}

void CommandService::Key_Resource(char*, pFunPrintf pPrintf)
{
	pPrintf("\nGodsWar> ");
}

void CommandService::Key_Reset(char*, pFunPrintf pPrintf)
{
	pPrintf("  ...[Execute]\n");
	pPrintf("\nReset Scene Data... Please waitting... \n");

	for (int i = 0, j = 1; i < m_pWorld->g_lMainCity; i++, j++)
	{
		m_pWorld->g_pRegion[i].ResetMonster();
	}
	for (int i = m_pWorld->g_lMainCity, j = 1; i < m_pWorld->g_lMainCity + m_pWorld->g_lCity; i++, j++)
	{
		m_pWorld->g_pRegion[i].ResetMonster();
	}
	for (int i = m_pWorld->g_lMainCity + m_pWorld->g_lCity, j = 1; i < m_pWorld->g_lMainCity + m_pWorld->g_lCity + m_pWorld->g_lField; i++, j++)
	{
		m_pWorld->g_pRegion[i].ResetMonster();
	}

	pPrintf("\nReset ready... \n");
	pPrintf("\nGodsWar> ");
}

void CommandService::Key_Role(char* command, pFunPrintf pPrintf)
{
	char* RoleName;
	int x = 0;
	while (command[x] == ' ')
		x++;
	RoleName = &command[x];
	if (!strlen(RoleName))
	{
		pPrintf("Error: Role name error!\x0d\x0a");
		return;
	}

	char buf[128];
	CPlayer* pPlayer = CPlayer::GetPlayerFromRoleName(RoleName);
	if (NULL == pPlayer)
	{
		pPrintf("Error: %s not found !\x0d\x0a", RoleName);
		return;
	}
	char pro[16], camp[16], GM[8];
	memset(pro, 0L, sizeof(pro));
	memset(camp, 0L, sizeof(camp));
	memset(GM, 0L, sizeof(GM));

	sprintf(pro, "%s", pPlayer->GetlClass());

	if (pPlayer->GetlFaction() == 0)
	{
		sprintf(camp, "斯巴达");
	}
	else
	{
		sprintf(camp, "雅典");
	}

	if (pPlayer->IsGM())
	{
		sprintf(GM, "是");
	}
	else
	{
		sprintf(GM, "否");
	}
	char MapName[64];
	strcpy(MapName, m_pWorld->GetRegionFromLogicID(pPlayer->GetlRegionID())->GetName().c_str());

	sprintf(buf, "%s|%s|%s|%d|%d|%d|%d|%d|%s|%f|%f|%d|%s\x0d\x0a", pPlayer->GetName(), pro, camp, pPlayer->GetcRank(), pPlayer->GetMaxHP(),
		pPlayer->GetMaxMP(), pPlayer->GetlHP(), pPlayer->GetlMP(), MapName, pPlayer->GetfX(), pPlayer->GetfZ(), pPlayer->GetMoney(), GM);
	pPrintf("\x0d\x0a==============================================================================\x0d\x0a");
	pPrintf("|Name|Pro|camp|F_LV|S_LV|MAX_HP|MAX_MP|Cur_HP|Cur_MP|MAP|POS_X|POS_Z|MONEY|GM| \x0d\x0a");
	pPrintf("------------------------------------------------------------------------------\x0d\x0a");
	pPrintf("%s", buf);
	pPrintf("==============================================================================\x0d\x0a");
	pPrintf("\nGodsWar> ");
}

void CommandService::Key_Accounts(char* command, pFunPrintf pPrintf)
{
	char* Accounts;
	int x = 0;
	while (command[x] == ' ')
		x++;
	Accounts = &command[x];
	if (!strlen(Accounts))
	{
		pPrintf("Error: Role name error!\x0d\x0a");
		return;
	}

	char buf[512];
	CPlayer* pPlayer = m_pWorld->GetPlayerFromAccounts(Accounts);
	if (NULL == pPlayer)
	{
		pPrintf("Error: %s not found !\x0d\x0a", Accounts);
		return;
	}

	map<string, UINT> Allrole = pPlayer->GetAllRole();
	map<string, UINT>::iterator itor = Allrole.begin();

	pPrintf("\x0d\x0a==============================================================================\x0d\x0a");
	pPrintf("|  Account  |    Character   | DBID    |\x0d\x0a");
	pPrintf("------------------------------------------------------------------------------\x0d\x0a");

	for (; itor != Allrole.end(); itor++)
	{
		sprintf(buf, "%-16s|%-24s| %d |\x0d\x0a", Accounts, itor->first.c_str(), itor->second);
		pPrintf("%s", buf);
	}
	pPrintf("==============================================================================\x0d\x0a");
}

void CommandService::Key_Queue(char*, pFunPrintf pPrintf)
{
}

void CommandService::Key_Reloadselling(char* command, pFunPrintf)
{
	sbase::ConsoleWriteStr(1, 2, "  ...[Execute]");

	string temp = command;
	temp.erase(temp.begin(), temp.begin() + 13);

	map<string, int>::iterator iter = m_pWorld->g_ScriptNPC.find(temp);
	if (iter == m_pWorld->g_ScriptNPC.end())
		return;
	CNPC* npc = m_pWorld->GetNPC(iter->second);
	if (!npc)
		return;

	npc->ReloadQuests();

	sbase::ConsoleWriteStr(1, 2, "Reload Quest Succeed");
	sbase::ConsoleWriteStr(1, 2, "GodsWar> ");
}
void CommandService::Key_Reloadquest(char* command, pFunPrintf)
{
	sbase::ConsoleWriteStr(1, 2, "  ...[Execute]");

	string temp = command;
	temp.erase(temp.begin(), temp.begin() + 13);

	map<string, int>::iterator iter = m_pWorld->g_ScriptNPC.find(temp);
	if (iter == m_pWorld->g_ScriptNPC.end())
		return;

	CNPC* npc = m_pWorld->GetNPC(iter->second);
	if (!npc)
		return;

	npc->ReloadQuests();

	sbase::ConsoleWriteStr(1, 2, "Reload Quest Succeed");
	sbase::ConsoleWriteStr(1, 2, "GodsWar> ");
}

void CommandService::Key_Broadcast(char*, pFunPrintf)
{
	sbase::CIni NoteIni("./config.ini", false);
	const char* Buffer;
	Buffer = NoteIni.GetString("Note", "World");
	sbase::ConsoleWriteStr(1, 2, Buffer);
	if (strlen(Buffer) > 0)
	{
		for (int i = 0, j = 1; i < m_pWorld->g_lMainCity; i++, j++)
		{
			m_pWorld->g_pRegion[i].SendNoteToClient(Buffer);
		}
		for (int i = m_pWorld->g_lMainCity, j = 1; i < m_pWorld->g_lMainCity + m_pWorld->g_lCity; i++, j++)
		{
			m_pWorld->g_pRegion[i].SendNoteToClient(Buffer);
		}
		for (int i = m_pWorld->g_lMainCity + m_pWorld->g_lCity, j = 1; i < m_pWorld->g_lMainCity + m_pWorld->g_lCity + m_pWorld->g_lField; i++, j++)
		{
			m_pWorld->g_pRegion[i].SendNoteToClient(Buffer);
		}
		sbase::ConsoleWriteColorText(FOREGROUND_GREEN, "Send note message succed!");
		sbase::ConsoleWriteColorText(FOREGROUND_GREEN, "GodsWar> ");
	}
}

void CommandService::Key_Send(char*, pFunPrintf pPrintf)
{
	sbase::CIni NoteIni("./config.ini", false);
	const char* Buffer;
	Buffer = NoteIni.GetString("Note", "Player");

	const char* RoleName;
	sbase::CIni NameIni("./config.ini", false);
	RoleName = NameIni.GetString("Note", "Name");

	CPlayer* pPlayer = CPlayer::GetPlayerFromRoleName(RoleName);
	if (NULL == pPlayer)
	{
		pPrintf("Error: Player is not exist!\x0d\x0a");
		return;
	}
	pPlayer->SendNoteToClient(Buffer);
	sbase::ConsoleWriteColorText(FOREGROUND_GREEN, "\nSend message to palyer successfully!");
	sbase::ConsoleWriteColorText(FOREGROUND_GREEN, "GodsWar> ");

}

void CommandService::Key_SaveAll(char*, pFunPrintf)
{
	for (int i = 0, j = 1; i < m_pWorld->g_lMainCity; i++, j++)
	{
		m_pWorld->g_pRegion[i].WriteDataToDB();
	}
	for (int i = m_pWorld->g_lMainCity, j = 1; i < m_pWorld->g_lMainCity + m_pWorld->g_lCity; i++, j++)
	{
		m_pWorld->g_pRegion[i].WriteDataToDB();
	}
	for (int i = m_pWorld->g_lMainCity + m_pWorld->g_lCity, j = 1; i < m_pWorld->g_lMainCity + m_pWorld->g_lCity + m_pWorld->g_lField; i++, j++)
	{
		m_pWorld->g_pRegion[i].WriteDataToDB();
	}

	sbase::ConsoleWriteColorText(FOREGROUND_GREEN, "\nupdate database succed!");
	sbase::ConsoleWriteColorText(FOREGROUND_GREEN, "GodsWar> ");
}

void CommandService::Key_Kick(char*, pFunPrintf pPrintf)
{
	const char* RoleName;
	sbase::CIni NoteIni("./config.ini", false);
	RoleName = NoteIni.GetString("Note", "Name");

	CPlayer* pPlayer = CPlayer::GetPlayerFromRoleName(RoleName);
	if (NULL == pPlayer)
	{
		pPrintf("Error: Player is not exist!\x0d\x0a");
		return;
	}
	m_pWorld->CloseSocket(pPlayer);
	sbase::ConsoleWriteColorText(FOREGROUND_GREEN, "\nKick palyer successfully!");
	sbase::ConsoleWriteColorText(FOREGROUND_GREEN, "GodsWar> ");
}

void  CommandService::Key_Move(char*, pFunPrintf pPrintf)
{
	const char* RoleName;
	sbase::CIni NoteIni("./config.ini", false);
	RoleName = NoteIni.GetString("Note", "Name");

	CPlayer* pPlayer = CPlayer::GetPlayerFromRoleName(RoleName);
	if (NULL == pPlayer)
	{
		pPrintf("Error: Player is not exist!\x0d\x0a");
		return;
	}
	pPlayer->SetlHP(0);
	pPlayer->Revive(eREVIVE_FREE);
	sbase::ConsoleWriteColorText(FOREGROUND_GREEN, "\nMove palyer successfully!");
	sbase::ConsoleWriteColorText(FOREGROUND_GREEN, "GodsWar> ");
}

void  CommandService::Key_Expand(char* command, pFunPrintf pPrintf)
{
	char* Num;
	int x = 0;
	while (command[x] == ' ')
		x++;
	Num = &command[x];
	if (!strlen(Num))
	{
		pPrintf("Error: Num error!\x0d\x0a");
		return;
	}

	if (strtoul(Num, (char**)NULL, 10) <= m_pWorld->GetAllowedOnline())
	{
		pPrintf("Error: Num error!\x0d\x0a");
		return;
	}

	m_pWorld->SetAllowedOnline(strtoul(Num, (char**)NULL, 10));
	sbase::ConsoleWriteColorText(FOREGROUND_GREEN, "\nExpand max player num successfully!");
	sbase::ConsoleWriteColorText(FOREGROUND_GREEN, "GodsWar> ");
}

long CommandService::ParseCommand(char* Argcommand)
{
	unsigned int x;
	size_t l = strlen(Argcommand);
	if (l < 2)
	{
		return 0;
	}

	for (x = 0; x < AllCmds; x++)
		if (!memcmp(Commands[x].cmd, Argcommand, Commands[x].cmdLen))
		{
			(this->*Commands[x].pfun)(&Argcommand[Commands[x].cmdLen], &::printf);
			break;
		}
	if (x == AllCmds)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "Unknown command!");
	}
	return 1;
}

long CommandService::Command(void)
{
	if (kbhit())
	{
		int key = getch();
		switch (key)
		{
		case 13:
		{
			if (lKeyCount == 0)
				break;

			ParseCommand(strKeyBuffer);
			lKeyCount = 0;
			memset(strKeyBuffer, 0, 256);
		}
		break;
		case 8:
		{
			if (lKeyCount > 0)
			{
				printf("\b \b");
				strKeyBuffer[lKeyCount] = 0;
				lKeyCount--;
			}
		}
		break;
		case 27:
		{
			lKeyCount = 0;
			memset(strKeyBuffer, 0, 256);
			printf("  ...[Cancel]\nCommand: ");
		}
		break;
		default:
		{
			if (lKeyCount >= 255)
			{
				break;
			}
			strKeyBuffer[lKeyCount] = (char)key;
			lKeyCount++;
			printf("%c", key);
		}
		break;
		}
	}

	return 1;
}


RegisterService::RegisterService(CWorld* pWorld, cnet::CIOCP* pNetClient) :m_pNetClient(pNetClient)
{
	m_pWorld = pWorld;

}
bool RegisterService::Init()
{
	m_pNetClient = new	cnet::CIOCP();
	m_Timer.Startup(REGISTER_TIMER);
	m_eState = OFFLINE;

	GetHostIP();

	return ConnectLoginServer();
}

void RegisterService::ReStart()
{

}

void RegisterService::Run()
{
	if (m_Timer.IsExpire())
	{
		if (!m_bIsConnect)
		{
			if (ConnectLoginServer())
			{
				RegisterToLoginServer();
			}

		}
		else
		{
			RegisterToLoginServer();
		}

		m_Timer.Update();
	}

	if (!m_pNetClient->GetServerState())
	{
		m_bIsConnect = false;
		return;
	}

	if (!OnRead())
	{
		m_bIsConnect = false;
		return;
	}

	if (!OnWrite())
	{
		m_bIsConnect = false;
		return;
	}

}

bool RegisterService::OnRead()
{
	if (m_pClientSck == NULL)
	{
		return false;
	}


	sbase::MsgHead* pszBuff = NULL;

	if (m_pClientSck->IsValid())
	{

		while (m_pClientSck->Read((char**)&pszBuff))
		{

			switch (pszBuff->usType)
			{

			case _MSG_LOGIN_RETURN_INFO:
			{
			}
			break;
			case _MSG_GAMESERVER_INFO:
			{
			}
			break;
			case _MSG_RESPONSE_GAMESERVER:
			{
			}
			break;
			case _MSG_VALIDATE_GAMESERVER:
			{
				MSG_VALIDATE_GAMESERVER* pGameValid = (MSG_VALIDATE_GAMESERVER*)pszBuff;
				CacheValidCode(pGameValid->Accounts, pGameValid->cCheckOutText);
				SendMsgToServer(pGameValid, pGameValid->Head.usSize);
			}
			break;
			default:
			{
				cout << "Unknown message type from LoginServer!" << endl;

			}
			break;
			}

			m_pClientSck->Remove(pszBuff->usSize);

		}
		return true;
	}
	else
	{
		cout << "服务端关闭!" << endl;
		return false;

	}


}

bool RegisterService::OnWrite()
{

	if (m_pClientSck == NULL || !m_bIsConnect)
	{
		return false;
	}

	if (m_pClientSck->IsValid())
	{
		return m_pClientSck->Write();
	}
	else
	{

	}
}



bool RegisterService::ConnectLoginServer()
{

	memset(m_strServerIP, 0L, sizeof(m_strServerIP));
	sbase::CIni ini("config.ini", false);

	m_ID = ini.GetData("System", "ID");
	m_Name = ini.GetString("System", "ServerName");
	m_nListenPort = ini.GetData("System", "ListenPort");
	m_nPlayerNum = ini.GetData("System", "MaxOnline");

	m_nServerPort = ini.GetData("LoginServer", "Port");
	strcpy(m_strServerIP, ini.GetString("LoginServer", "IP"));

	m_bIsConnect = false;

	if (m_pNetClient->Init(m_strServerIP, m_nServerPort, &m_pClientSck))
	{
		m_bIsConnect = RegisterToLoginServer();

	}
	else
	{
		m_pClientSck = NULL;
	}

	return m_bIsConnect;


}

const char* RegisterService::GetHostIP()
{

	if (m_strHostIp.empty())
	{
		char szHostIp[32] = { 0 };
		sbase::CIni ini("config.ini", false);

		strcpy(szHostIp, ini.GetString("System", "IP"));

		m_strHostIp = szHostIp;

	}

	return  m_strHostIp.c_str();
}


void RegisterService::Release()
{
	delete this;
	delete m_pNetClient;
}

bool RegisterService::RegisterToLoginServer(void)
{
	MSG_GAMESERVER_INFO MsgRegister;
	MsgRegister.Head.usSize = sizeof(MSG_GAMESERVER_INFO);
	MsgRegister.Head.usType = _MSG_GAMESERVER_INFO;
	MsgRegister.cID = m_ID;

	strcpy(MsgRegister.cIP, m_strHostIp.c_str());
	MsgRegister.uiPort = m_nListenPort;
	strcpy(MsgRegister.ServerName, m_Name.c_str());

	int nOnlineNum = m_pWorld->GetOnlinePlayer();


	if (nOnlineNum >= m_nPlayerNum * 0.8)
	{
		SetState(FULL);
		MsgRegister.cState = (BYTE)m_eState;
	}
	else if (nOnlineNum < m_nPlayerNum * 0.5)
	{
		SetState(NORMAL);
		MsgRegister.cState = (BYTE)m_eState;
	}
	else
	{
		SetState(BUSY);
		MsgRegister.cState = (BYTE)m_eState;

	}

	return SendMsgToServer(&MsgRegister, sizeof(MSG_GAMESERVER_INFO));
}

bool RegisterService::SendMsgToServer(void* pMsg, int ilen)
{
	sbase::CSingleLock xLock(&m_xLock);
	if (!m_pClientSck)
		return false;

	if (ilen != ((MsgHead*)pMsg)->usSize)
	{
		sbase::SysLogSave("RegisterService::SendMsgToServer Error, type=%d, size=%d", ((MsgHead*)pMsg)->usType, ((MsgHead*)pMsg)->usSize);
		return false;
	}

	return	m_pClientSck->PackMsg((char*)pMsg, ilen);
};

QueueService::QueueService(CWorld* pWorld)
{
	m_pWorld = pWorld;
}

QueueService::~QueueService()
{

}

bool QueueService::Init()
{
	m_Timer.Startup(10);
	Start();
	return true;
}

void QueueService::Release()
{
	delete this;
}

void QueueService::Run()
{
	if (!IsOpen())
		return;

	if (m_Timer.IsExpire())
	{
		m_pWorld->UpdateGameLoginQueue();
		m_Timer.Update();
	}

}


BackUpService::BackUpService(CWorld* pWorld, cnet::CIOCP* pNetClient) :m_pNetClient(pNetClient)
{
	m_pWorld = pWorld;
	m_bConnectDB = false;

}

BackUpService::~BackUpService()
{

}
#define REGION_BACKUP_TIMEOU	5000
#define CONSOR_BACKUP_TIMEOU	5000	
bool BackUpService::Init()
{
	m_pNetClient = new	cnet::CIOCP();

	m_pThread = sbase::CThread::CreateNew(*this, sbase::CThread::RUN, 10);

	if (m_pThread == NULL)
	{
		printf("CThread::CreateNew failed!\n");
		return false;
	}
	m_pThread->SetThreadAMask(12);
	m_Timer.Startup(BACKUP_TIMER);
	m_ConsortiaTimer.Startup(CONSOR_BACKUP_TIMEOU);

	m_PlayerTimer.Startup(REGION_BACKUP_TIMEOU);


	Start();
	return ConnectPipeServer();
}

void BackUpService::Release()
{
	delete this;
	delete m_pNetClient;
}

void BackUpService::Run()
{
	if (m_Timer.IsExpire())
	{
		if (!m_bIsConnect)
		{
			if (ConnectPipeServer())
			{
				RegisterToPipeServer();
			}

		}
		m_Timer.Update();
	}

	if (!m_pNetClient->GetServerState())
	{
		SetState(false);
		return;
	}

	if (!OnRead())
	{
		SetState(false);
		return;
	}

	if (m_PlayerTimer.IsExpire())
	{
		m_PlayerTimer.Startup(REGION_BACKUP_TIMEOUT);
		m_pWorld->BackUpPlayer();

	}

	if (m_ConsortiaTimer.IsExpire())
	{
		m_ConsortiaTimer.Startup(CONSOR_BACKUP_TIMEOUT);
		m_pWorld->BackUpConsortia();
	}
}
bool BackUpService::RegisterToPipeServer(void)
{
	MSG_PIPESERVER_INFO msg_pipeserver_info;
	msg_pipeserver_info.Head.usSize = sizeof(MSG_PIPESERVER_INFO);
	msg_pipeserver_info.Head.usType = _MSG_LOGIN;
	msg_pipeserver_info.ucInfo = 2;

	return SendMsgToServer(&msg_pipeserver_info, sizeof(MSG_PIPESERVER_INFO));

}
bool BackUpService::ConnectPipeServer()
{
	memset(m_strServerIP, 0L, sizeof(m_strServerIP));
	sbase::CIni ini("config.ini", false);

	m_nServerPort = ini.GetData("DBServer", "Port");
	strcpy(m_strServerIP, ini.GetString("DBServer", "IP"));

	SetState(false);
	if (m_pNetClient->Init(m_strServerIP, m_nServerPort, &m_pClientSocket))
	{
		bool bTemp = RegisterToPipeServer();
		SetState(bTemp);

	}
	else
	{
		m_pClientSocket = NULL;
	}

	return m_bIsConnect;
}
bool BackUpService::OnWrite()
{
	char temp[4096] = { 0 };
	string strRecv;
	if (m_pClientSocket == NULL || !m_bIsConnect)
	{
		return false;
	}


	BACKUP_MSG* pMsg = NULL;

	while (m_pWorld->ReadBackBuff((char**)&pMsg))
	{

		SendMsgToServer(pMsg, pMsg->Head.usSize);


		BACKUP_MSG* pszBuff = (BACKUP_MSG*)pMsg;

		m_pWorld->RemoveBackBuff(pMsg->Head.usSize);

	}

	if (m_pClientSocket->IsValid())
	{
		m_pClientSocket->Write();
	}
	else
	{
	}

	return true;
}
bool BackUpService::OnRead()
{
	if (m_pClientSocket == NULL)
	{
		return false;
	}

	sbase::MsgHead* pszBuff = NULL;

	if (m_pClientSocket->IsValid())
	{

		while (m_pClientSocket->Read((char**)&pszBuff))
		{
			switch (pszBuff->usType)
			{
			case _MSG_LOGIN_RETURN_INFO:
			{
				MSG_PIPESERVER_INFO* pPipeServer_Info = (MSG_PIPESERVER_INFO*)pszBuff;
				if (pPipeServer_Info->ucInfo == 1)
				{
					m_bConnectDB = true;
					cout << "Login DBServer Success!!!" << endl;
				}
				else
				{
					cout << "Login DBServer Failed!!!" << endl;
				}
			}
			break;
			default:
			{
				cout << "Unknown message type from LoginServer!" << endl;

			}
			break;
			}

			m_pClientSocket->Remove(pszBuff->usSize);

		}
		return true;
	}
	else
	{
		cout << "服务端关闭!" << endl;
		return false;

	}
}


int BackUpService::OnThreadProcess(void)
{
	if (!OnWrite())
	{
		SetState(false);
		return 0;
	}

	return 1;
}

void BackUpService::SetState(bool bState)
{
	sbase::CSingleLock lock(&m_xLock);
	m_bIsConnect = bState;
}

bool BackUpService::SendMsgToServer(void* pMsg, int ilen)
{
	return	m_pClientSocket->PackMsg((char*)pMsg, ilen);
}

ServerSocketEventHandle::ServerSocketEventHandle()
{
	USHORT Arrysize = 0;
	const SSockHandle* Talbe = GetSockHandleTab(Arrysize);

	memset(&MsgRouter, 0L, sizeof(MsgRouter));

	for (int a = 0; a < MSX_SERVER_MSG; a++)
	{
		if (a < Arrysize)
			MsgRouter[Talbe[a].MSG_ID] = a;
	}
}

ServerSocketEventHandle::~ServerSocketEventHandle()
{

}

void ServerSocketEventHandle::SendErrorInfo(int ErrID, CPlayer* pPlayer)
{
	if (ErrID == NO_MSG_ERRO)
		return;

	MSG_MANAGE_RETURN error;
	error.Head.usSize = sizeof(MSG_MANAGE_RETURN);
	error.Head.usType = _MSG_MANAGE_RETURN;
	error.ucDisc = ErrID;
	pPlayer->s_World->SendMsgToClient(&error, pPlayer->GetSocket());
}

void ServerSocketEventHandle::ResloveGamePacket(const void* pPacket, CPlayer* pPlayer)
{
	USHORT temp = 0;
	static const SSockHandle* NetEventHandle = GetSockHandleTab(temp);

	sbase::MsgHead* pHead = (sbase::MsgHead*)pPacket;

	sbase::ConsoleWriteColorText(FOREGROUND_INTENSITY, "Parket Type: %d", pHead->usType);

	eError Err = (this->*NetEventHandle[GetIndex(pHead->usType)].pfun)(pPacket, pPlayer);
	if (Err != NO_MSG_ERRO)
		SendErrorInfo(Err, pPlayer);
}

const SSockHandle* ServerSocketEventHandle::GetSockHandleTab(USHORT& Size) const
{
	static const SSockHandle MsgEventHandle[] =
	{
		{ _MSG_INVALID,             &ServerSocketEventHandle::Handle_UnknowMsg     },
		{ _MSG_ENTER_GAME,       		&ServerSocketEventHandle::Handle_EnterGame  		},
		{ _MSG_CLIENT_READY,      		&ServerSocketEventHandle::Handle_ClientReady 		},
		{ _MSG_LOGIN_GAMESERVER,    		&ServerSocketEventHandle::Handle_LoginGS   		},
		{ _MSG_CREATE_ROLE,       		&ServerSocketEventHandle::Handle_CreateRole  		},


#if GW2
		{ _MSG_INVITER_RETURN,		&ServerSocketEventHandle::Handle_CheckRecommendID },//cdx
		{ _MSG_INVITER_TIP,		&ServerSocketEventHandle::Handle_CheckRecommendID2 },//cdx
#endif

		{ _MSG_DELETE_ROLE,       		&ServerSocketEventHandle::Handle_DelRole   		},
		{ _MSG_WALK_BEGIN,       		&ServerSocketEventHandle::Handle_WalkBegin  		},
		{ _MSG_WALK,       	     	&ServerSocketEventHandle::Handle_Walk  		  },
		{ _MSG_WALK_END,        		&ServerSocketEventHandle::Handle_WaklEnd   		},
		{ _MSG_FIGHT,          		&ServerSocketEventHandle::Handle_Fight    		},
		{ _MSG_ATTACK,         		&ServerSocketEventHandle::Handle_Attack    		},
		{ _MSG_SKILL,          		&ServerSocketEventHandle::Handle_Skill    		},
		{ _MSG_LEARN,          		&ServerSocketEventHandle::Handle_Learn    		},
		{ _MSG_SKILLBACKUP,       		&ServerSocketEventHandle::Handle_SkillBackup 		},
		{ _MSG_SKILL_UPGRADE,      		&ServerSocketEventHandle::Handle_SkillUpgrade 		},
		{ _MSG_SKILL_INTERRUPT,      		&ServerSocketEventHandle::Handle_SkillInterrupt 	},
		{ _MSG_EFFECT,         		&ServerSocketEventHandle::Handle_Effect    		},
		{ _MSG_TALK,          		&ServerSocketEventHandle::Handle_Talk     		},
		{ _MSG_TALKCHANNEL,						&ServerSocketEventHandle::Handle_TalkChannel		},
		{ _MSG_PICKUPDROPS,       		&ServerSocketEventHandle::Handle_Drop     		},
		{ _MSG_USEOREQUIP,       		&ServerSocketEventHandle::Handle_Equip    		},
		{ _MSG_MOVEITEM,        		&ServerSocketEventHandle::Handle_MoveItem   		},
		{ _MSG_BREAK_ITEM,        		&ServerSocketEventHandle::Handle_BreakItem   		},
		{ _MSG_SELL,          		&ServerSocketEventHandle::Handle_Sell     		},
		{ _MSG_STORAGEITEM,       		&ServerSocketEventHandle::Handle_StorageItem 		},
		{ _MSG_STALL,          		&ServerSocketEventHandle::Handle_Stall    		},
		{ _MSG_STALLITEM,        		&ServerSocketEventHandle::Handle_StallItem  		},
		{ _MSG_STALLADDITEM,      		&ServerSocketEventHandle::Handle_StallAddItem 		},
		{ _MSG_STALLDELITEM,      		&ServerSocketEventHandle::Handle_StallDelItem 		},
		{ _MSG_STALLBUYITEM,      		&ServerSocketEventHandle::Handle_StallBuyItem 		},
		{ _MSG_TALKNPC,         		&ServerSocketEventHandle::Handle_TalkNPC   		},
		{ _MSG_NPCDATA,         		&ServerSocketEventHandle::Handle_NPCData   		},
		{ _MSG_SYS_NPC_DATA,          &ServerSocketEventHandle::Handle_SysFunNPC     },
		{ _MSG_NPCSELL,         		&ServerSocketEventHandle::Handle_NPCSell   		},
		{ _MSG_MALLSELL,         		&ServerSocketEventHandle::Handle_MallSell			},
		{ _MSG_ASSOCIATIONSELL,         &ServerSocketEventHandle::Handle_AssociationSell  },
		{ _MSG_MALLITEMDATA,					&ServerSocketEventHandle::Handle_MallItemData		},
		{ _MSG_ASSOCIATIONITEMDATA,				&ServerSocketEventHandle::Handle_AssociationItemData},
		{ _MSG_FINDQUEST,        		&ServerSocketEventHandle::Handle_FindQuest  		},
		{ _MSG_NPCACCEPTQUEST,     		&ServerSocketEventHandle::Handle_NPCAcceptQuest   },
		{ _MSG_NPCCANCELQUEST,     		&ServerSocketEventHandle::Handle_NPCCancelQuest   },
		{ _MSG_NPCCOMPLETEQUEST,    		&ServerSocketEventHandle::Handle_NPCCompleteQuest  },
		{ _MSG_NPCQUESTVIEW,      		&ServerSocketEventHandle::Handle_NPCQuestView    },
		{ _MSG_RELATION_REQUEST,    		&ServerSocketEventHandle::Handle_RelationRequest  },
		{ _MSG_RELATION_RESPONSE,    		&ServerSocketEventHandle::Handle_RelationResponse  },
		{ _MSG_RELATION_DELETE,     		&ServerSocketEventHandle::Handle_RelationDel    },
		{ _MSG_TRADE,          		&ServerSocketEventHandle::Handle_Trade       },
		{ _MSG_TRADE_MONEY,       		&ServerSocketEventHandle::Handle_TradeMoney     },
		{ _MSG_TRADE_ADDITEM,      		&ServerSocketEventHandle::Handle_TradeAddItem    },
		{ _MSG_TRADE_REMOVEITEM,    		&ServerSocketEventHandle::Handle_TradeRemoveItem  },
		{ _MSG_EQUIPFORGE,       		&ServerSocketEventHandle::Handle_EquipForge     },
		{ _MSG_EQUIPFORGE_EQUIP,    		&ServerSocketEventHandle::Handle_EquipForge_Equip  },
		{ _MSG_EQUIPFORGE_MATERIAL,   		&ServerSocketEventHandle::Handle_EquipMaterial   },
		{ _MSG_EQUIPFORGE_EQUIPCANCEL, 		&ServerSocketEventHandle::Handle_EquipCancel    },
		{ _MSG_EQUIPFORGE_MATERIALCANCEL,    &ServerSocketEventHandle::Handle_EquipMaterialCancel},
		{ _MSG_EQUIPFORGE_CANCEL,      	&ServerSocketEventHandle::Handle_EquipForgeCacel  },
		{ _MSG_TEAM_INVITE,         	&ServerSocketEventHandle::Handle_TeamInvite     },
		{ _MSG_TEAM_REJECT,         	&ServerSocketEventHandle::Handle_TeamReject     },
		{ _MSG_TEAM_INFO,						&ServerSocketEventHandle::Handle_TeamInfo			},
		{ _MSG_TEAM_ADD,          	&ServerSocketEventHandle::Handle_TeamAdd      },
		{ _MSG_TEAM_DELETE,         	&ServerSocketEventHandle::Handle_TeamDel      },
		{ _MSG_TEAM_REPLACELEADER,     	&ServerSocketEventHandle::Handle_TeamLeader     },
		{ _MSG_TEAM_DISSOLVE,        	&ServerSocketEventHandle::Handle_TeamDissolve    },
		{ _MSG_TEAM_LEAVE,         	&ServerSocketEventHandle::Handle_TeamLeave     },
		{ _MSG_BACKHOME,          	&ServerSocketEventHandle::Handle_BackHome      },
		{ _MSG_CONSORTIA_CREATE,      	&ServerSocketEventHandle::Handle_ConsortiaCreate  },
		{ _MSG_CONSORTIA_INVITE,      	&ServerSocketEventHandle::Handle_ConsortiaInvite  },
		{ _MSG_CONSORTIA_DISMISS,      	&ServerSocketEventHandle::Handle_ConsortiaDimiss  },
		{ _MSG_CONSORTIA_RESPONSE,     	&ServerSocketEventHandle::Handle_ConsortiaResponse },
		{ _MSG_CONSORTIA_EXIT,       	&ServerSocketEventHandle::Handle_ConsortiaExit   },
		{ _MSG_CONSORTIA_TEXT,       	&ServerSocketEventHandle::Handle_ConsortiaText   },
		{ _MSG_CONSORTIA_DUTY,       	&ServerSocketEventHandle::Handle_ConsortiaDuty   },
		{ _MSG_CONSORTIA_MEMBER_DEL,    	&ServerSocketEventHandle::Handle_ConsortiaMemDel  },
		{ _MSG_CONSORTIA_LVUP,					&ServerSocketEventHandle::Handle_ConsortiaLevelUp  },
		{ _MSG_ALTAR_CREATE,					&ServerSocketEventHandle::Handle_CreateAltar    },
		{ _MSG_ALTAR_LVUP,						&ServerSocketEventHandle::Handle_AltarLevelUp    },
		{ _MSG_ALTAR_OBLATION,					&ServerSocketEventHandle::Handle_AltarOblation   },
		{ _MSG_CRETIT_EXCHANGE,					&ServerSocketEventHandle::Handle_CretitExchange   },
		{ _MSG_GOLD_BOX,					  &ServerSocketEventHandle::Handle_OpenGoldBox    },
		{ _MSG_EXPLORER_QUEST,        	&ServerSocketEventHandle::Handle_Explorer  		},
		{ _MSG_TARGETINFO,		       	&ServerSocketEventHandle::Handle_GetTargetInfo	  },
		{ _MSG_DELAY_EXIT,           &ServerSocketEventHandle::Handle_DelayExit     },
	};

	Size = sizeof(MsgEventHandle) / sizeof(SSockHandle);
	return MsgEventHandle;
}

int ServerSocketEventHandle::GetIndex(int MsgID)
{
	if (MsgID < 0 || MsgID >= MSX_SERVER_MSG || MsgID < 10000 || MsgID >= 20000)
	{
		return MsgRouter[0];
	}

	return MsgRouter[MsgID];
}

eError ServerSocketEventHandle::Handle_SockSetup(const void* pBuf, CPlayer* pPlayer)
{
	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_SocketClose(const void* pBuf, CPlayer* pPlayer)
{
	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_EnterGame(const void* pBuf, CPlayer* pPlayer)
{
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_ENTER_GAME");
		return NO_MSG_ERRO;
	}
	if (pPlayer->FindRoleByName(pBuf))
	{
		return NO_MSG_ERRO;
	}

	return NO_MSG_ERRO;
}


eError ServerSocketEventHandle::Handle_ClientReady(const void* pBuf, CPlayer* pPlayer)
{
	MSG_CLIENT_READY* pReady = (MSG_CLIENT_READY*)pBuf;
#ifdef GODSWAR_TESTSWITCH_ON
	pPlayer = pPlayer->s_World->GetPlayerFromSocket(pPlayer->GetSocket());
#else
	pPlayer = pPlayer->s_World->GetPlayer(pReady->usUserID);
#endif
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_CLIENT_READY");
		return NO_MSG_ERRO;
	}
	pPlayer->SendPropertyMsg();
	pPlayer->SynGameData(true);
	pPlayer->s_World->GetRegionFromLogicID(pPlayer->GetlRegionID())->Join(pPlayer);
	pPlayer->OnProcess_PassiveSkill(pPlayer);
	pPlayer->OnProcess_ActiveSkill(pPlayer);
	pPlayer->SendRelationMsg();
	pPlayer->SendAltarMsg();
	pPlayer->SetWorldStatus(eGAMEWORLD);


	pPlayer->s_World->SetPlayer(pPlayer);

	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_LoginGS(const void* pBuf, CPlayer* pPlayer)
{
	MSG_LOGIN_GAMESERVER* pLogin = (MSG_LOGIN_GAMESERVER*)pBuf;
	snet::CSocket* pSocket = pPlayer->GetSocket();
	RegisterService::CacheSocket(pSocket, pLogin->Accounts);
	string validcode = RegisterService::GetValidCode(pLogin->Accounts);
	if (strcmp(pLogin->cCheckOutText, validcode.c_str()) != 0 || validcode.empty())
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_LOGIN_GAMESERVER");
		return NO_MSG_ERRO;
	}

	sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_LOGIN_GAMESERVER First");

	CPlayer* LoginedPlayer = CPlayer::GetPlayerFromAccounts(pLogin->Accounts);
	if (LoginedPlayer != NULL)
	{
		if (NULL != LoginedPlayer->GetSocket())
		{
			LoginedPlayer->GetSocket()->Refresh();
			pPlayer->s_World->GetIOCPServer()->PushNewClose(LoginedPlayer->GetSocket());
		}
		LoginedPlayer->DelayExit(0);
		Sleep(1);
		if (LoginedPlayer->IsMove())
			sbase::ConsoleWriteColorText(FOREGROUND_RED, "重复登陆,玩家强迫下线！");
	}

	{
		if (NULL != pPlayer)
		{
			REGISTER_MACRO(OnProcess_Attribute, &OnDealWithAttribute);
			REGISTER_MACRO(OnProcess_KitBag, &OnDealWithKitBag);
			REGISTER_MACRO(OnProcess_PassiveSkill, &OnDealWithPassiveSkill);
			REGISTER_MACRO(OnProcess_ActiveSkill, &OnDealWithActiveSkill);

			pPlayer->SetAccounts(pLogin->Accounts);
			pPlayer->SetValidCode(validcode);
			pPlayer->RequestBaseRoleInfoFromDB(pBuf);
			pPlayer->SetWorldStatus(eROLEINFOWAIT);
		}
		else
		{
			sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_LOGIN_GAMESERVER");
			return NO_MSG_ERRO;
		}
	}

	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_CreateRole(const void* pBuf, CPlayer* pPlayer)
{
	MSG_CREATE_ROLE* pCreateRole = (MSG_CREATE_ROLE*)pBuf;

	if (pCreateRole->cCamp > 1 || pCreateRole->cCamp < 0 ||
		pCreateRole->cGender > 1 || pCreateRole->cGender < 0 ||
		pCreateRole->ucProfession > 3 || pCreateRole->ucProfession < 0 ||
		pCreateRole->ucBelief > 1 || pCreateRole->ucBelief < 0 ||
		pCreateRole->ucHairStyle > 74 || pCreateRole->ucHairStyle < 0 ||
		pCreateRole->ucFaceShap > 4 || pCreateRole->ucFaceShap < 0)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_CREATE_ROLE]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}


	snet::CSocket* pSocket = pPlayer->GetSocket();
	if (pPlayer == NULL)
	{
		pPlayer->s_World->CloseSocket(pPlayer);
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_CREATE_ROLE");
		return NO_MSG_ERRO;
	}

	if (CKeyWords::Instance()->FindKeyWord(pCreateRole->Name))
	{
		MSG_MANAGE_RETURN error;
		error.Head.usSize = sizeof(MSG_MANAGE_RETURN);
		error.Head.usType = _MSG_MANAGE_RETURN;
		error.ucErrorType = _MSG_CREATE_ROLE;
		error.ucDisc = MSG_ERRO_0003;
		pPlayer->s_World->SendMsgToClient(&error, pPlayer->GetSocket());
		return NO_MSG_ERRO;
	}

	if (pPlayer->GetHaveRoleNum() >= 1)
	{
		MSG_MANAGE_RETURN error;
		error.Head.usSize = sizeof(MSG_MANAGE_RETURN);
		error.Head.usType = _MSG_MANAGE_RETURN;
		error.ucErrorType = _MSG_CREATE_ROLE;
		error.ucDisc = MSG_ERRO_0004;
		pPlayer->s_World->SendMsgToClient(&error, pPlayer->GetSocket());
		return NO_MSG_ERRO;
	}
	pPlayer->RequestAddRoleFromDB(pBuf);
	return NO_MSG_ERRO;
}


eError ServerSocketEventHandle::Handle_CheckRecommendID(const void* pBuf, CPlayer* pPlayer)
{
	
/*	MSG_CHECK_INVITER_ID error;
	error.Head.usSize = sizeof(MSG_CHECK_INVITER_ID);
	error.Head.usType = _MSG_CHECK_INVITER_ID;
	error.HttpID = 0;

	pPlayer->s_World->SendMsgToClient(&error, pPlayer->GetSocket());*/
#if GW2
	sbase::ConsoleWriteColorText(FOREGROUND_RED, " _MSG_CHECK_INVITER_ID");

	MSG_CHECK_INVITER_RETURN  msg_return;
	memset(&msg_return, 0, sizeof(MSG_CHECK_INVITER_RETURN));
	msg_return.Head.usSize = sizeof(MSG_CHECK_INVITER_RETURN);
	msg_return.Head.usType = _MSG_INVITER_RETURN;//caiduxiong//
	msg_return.count = 0;
	sbase::StrCopy(msg_return.cName1, "", 32);
	pPlayer->s_World->SendMsgToClient(&msg_return, pPlayer->GetSocket());

#endif
	return NO_MSG_ERRO;

}


struct	InviteeElm
{
	string	Name;
	int		InvitedPoint;
	int		Gold;
	int		LastInvitedPoint;
	int		LastGold;
	InviteeElm()
	{
		InvitedPoint = 0;
		Gold = 0;
		LastGold = 0;
		LastInvitedPoint = 0;
	}
};

enum Tip_Type
{
	eLastGold,
	eLastInvitedPoint
};

vector<InviteeElm>	m_InviteeVector;

eError ServerSocketEventHandle::Handle_CheckRecommendID2(const void* pBuf, CPlayer* pPlayer)
{
#if GW2
	MSG_INVITER_TIP		Inviter_tip;
	memset(&Inviter_tip, 0, sizeof(Inviter_tip));
	Inviter_tip.Head.usType = _MSG_INVITER_TIP;
	Inviter_tip.Head.usSize = sizeof(Inviter_tip);

	std::vector<InviteeElm>::iterator	iter;
	std::vector<InviteeElm>::iterator	iter_end = m_InviteeVector.end();

	for (iter = m_InviteeVector.begin(); iter != iter_end; iter++)
	{
		sbase::StrCopy(Inviter_tip.Name, (*iter).Name.c_str(), MAX_NAME);
		if ( (*iter).LastGold > 0 )
		{
			Inviter_tip.Type  = eLastGold;
			Inviter_tip.Value = (*iter).LastGold;
			pPlayer->s_World->SendMsgToClient( &Inviter_tip, pPlayer->GetSocket() );
		}

		if ( (*iter).LastInvitedPoint > 0 )
		{
			Inviter_tip.Type  = eLastInvitedPoint;
			Inviter_tip.Value = (*iter).LastInvitedPoint;
			pPlayer->s_World->SendMsgToClient( &Inviter_tip, pPlayer->GetSocket() );
		}
	}

pPlayer->SynGameData(true);
#endif
	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_DelRole(const void* pBuf, CPlayer* pPlayer)
{
	MSG_DELETE_ROLE* pDelRole = (MSG_DELETE_ROLE*)pBuf;

	if (strlen(pDelRole->Accounts) > 32 || strlen(pDelRole->Accounts) > 32)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_CREATE_ROLE]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	if (pPlayer != NULL)
		pPlayer->RequestDelRoleFromDB(pBuf);
	else
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_DELETE_ROLE");

	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_Walk(const void* pBuf, CPlayer* pPlayer)
{
	MSG_WALK* pWalk = (MSG_WALK*)pBuf;

	if (pPlayer)
	{
		pPlayer->GetRegion()->Walk(pPlayer, pWalk->x, pWalk->z, pWalk->fatan, pWalk->lState);
	}

	return NO_MSG_ERRO;
}
eError ServerSocketEventHandle::Handle_WalkBegin(const void* pBuf, CPlayer* pPlayer)
{
	MSG_WALK_BEGIN* pWalkBegin = (MSG_WALK_BEGIN*)pBuf;

	if (pWalkBegin->usMapID != pPlayer->GetRegion()->GetID())
	{
		return NO_MSG_ERRO;
	}

	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_WALK_BEGIN");
		return NO_MSG_ERRO;
	}

	if (pPlayer->GetWorldStatus() == eSCENECHANGE)
	{
		return NO_MSG_ERRO;
	}

	CRegion* pRegion = pPlayer->GetRegion();
	if (pRegion == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_WALK_BEGIN region flag");
		return NO_MSG_ERRO;
	}

	if (pPlayer->GetStall()->IsStart())
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_WALK_BEGIN GetStall ");
		return MSG_ERRO_0020;
	}

	if (pPlayer->GetSocket() == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_WALK_BEGIN socket != id ");
		return NO_MSG_ERRO;
	}

	pRegion->WalkBegin(pPlayer, pWalkBegin->X, pWalkBegin->Y, pWalkBegin->Z, pWalkBegin->OffsetX, pWalkBegin->OffsetY, pWalkBegin->OffsetZ, pWalkBegin->uiTick);
	pPlayer->SetLastMsg(_MSG_WALK_BEGIN);
	pPlayer->SetLastPos(pWalkBegin->X, pWalkBegin->Z);
	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_WaklEnd(const void* pBuf, CPlayer* pPlayer)
{
	MSG_WALK_END* pWalkEnd = (MSG_WALK_END*)pBuf;

	if (pWalkEnd->usMapID != pPlayer->GetRegion()->GetID())
	{
		return NO_MSG_ERRO;
	}

	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_WALK_END");
		return NO_MSG_ERRO;
	}

	if (pPlayer->GetWorldStatus() == eSCENECHANGE)
	{
		return NO_MSG_ERRO;
	}

	CRegion* pRegion = pPlayer->GetRegion();
	if (pRegion == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_WALK_END region flag ");
		return NO_MSG_ERRO;
	}

	if (pPlayer->GetSocket() == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_WALK_END socket != id ");
		return NO_MSG_ERRO;
	}

	pRegion->WalkEnd(pPlayer, pWalkEnd->X, pWalkEnd->Y, pWalkEnd->Z, pWalkEnd->fAtan2, pWalkEnd->uiTick, pWalkEnd->usSkillFlag);
	pPlayer->SetLastMsg(_MSG_WALK_END);
	pPlayer->SetLastPos(pWalkEnd->X, pWalkEnd->X);

	return NO_MSG_ERRO;
}


eError ServerSocketEventHandle::Handle_Fight(const void* pBuf, CPlayer* pPlayer)
{
	MSG_FIGHT* pFight = (MSG_FIGHT*)pBuf;
	CGameObject* pTarget = pPlayer->s_World->GetObject(pFight->uiTargetID);

	if (!pPlayer || !pTarget)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_FIGHT");
		return NO_MSG_ERRO;
	}

	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_Attack(const void* pBuf, CPlayer* pPlayer)
{
	MSG_ATTACK* pAttack = (MSG_ATTACK*)pBuf;

	if (pAttack->X > 256.0f || pAttack->X < -256.0f ||
		pAttack->Z > 256.0f || pAttack->Z < -256.0f)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_ATTACK]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	if (pPlayer->GetSocket() != NULL)
	{
		CRegion* pRegion = pPlayer->GetRegion();
		if (pRegion == NULL)
		{
			sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_ATTACK region");

			return NO_MSG_ERRO;
		}

		CGameObject* pTarget = NULL;
		if (pAttack->uiObjectID != -1)
			pTarget = pPlayer->s_World->GetObject(pAttack->uiObjectID);
		if (pTarget == NULL)
		{
			sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_ATTACK pTarget");
			return NO_MSG_ERRO;
		}

		if (pTarget->GetRegion() != pPlayer->GetRegion())
		{
			pPlayer->AddDanger();
			sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_ATTACK]", pPlayer->GetAccounts(), pPlayer->GetName());
			return NO_MSG_ERRO;
		}

		pRegion->Attack(pPlayer, pTarget, pAttack->X, pAttack->Y, pAttack->Z);

	}
	else
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_ATTACK");

		return NO_MSG_ERRO;
	}
	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_Skill(const void* pBuf, CPlayer* pPlayer)
{
	MSG_SKILL* pSkill = (MSG_SKILL*)pBuf;
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_SKILL");
		return NO_MSG_ERRO;
	}
	if (pPlayer->GetSocket() != NULL)
	{
		CRegion* pRegion = pPlayer->GetRegion();
		if (pRegion == NULL)
		{
			sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_SKILL pRegion");

			return NO_MSG_ERRO;
		}


		CGameObject* pTarget = NULL;
		if (pSkill->uiObjectID != -1)
			pTarget = pPlayer->s_World->GetObject(pSkill->uiObjectID);
		pPlayer->SetSkillMsg(pSkill);
		eError err = NO_MSG_ERRO;
		err = pRegion->Skill(pPlayer, pTarget, pSkill->x, 0.0f, pSkill->z, pSkill->cType, pSkill->x1, 0, pSkill->z1);
		pPlayer->SetLastMsg(_MSG_SKILL);
		return err;
	}
	else
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_SKILL socketid != id ");

		return NO_MSG_ERRO;
	}

	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_Learn(const void* pBuf, CPlayer* pPlayer)
{
	pBuf = NULL;
	pPlayer = NULL;
	return NO_MSG_ERRO;
}


eError ServerSocketEventHandle::Handle_SkillBackup(const void* pBuf, CPlayer* pPlayer)
{
	MSG_SKILL_BACKUP* pSkillBackup = (MSG_SKILL_BACKUP*)pBuf;
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_SKILLBACKUP");
		return NO_MSG_ERRO;
	}

	memcpy(pPlayer->m_SkillBackup.ItemID, pSkillBackup->Item, sizeof(int) * SKILLBACKUPCOUNT);
	memcpy(pPlayer->m_SkillBackup.SkillID, pSkillBackup->ID, sizeof(int) * SKILLBACKUPCOUNT);

	pPlayer->m_SkillBackup.RSkillID = pSkillBackup->RID;
	pPlayer->m_SkillBackup.RItemID = pSkillBackup->RItem;

	return NO_MSG_ERRO;
}


eError ServerSocketEventHandle::Handle_SkillUpgrade(const void* pBuf, CPlayer* pPlayer)
{
	MSG_SKILL_UPGRADE* pUpGradeSkill = (MSG_SKILL_UPGRADE*)pBuf;

	if (pUpGradeSkill->uiSkillID > 60000)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_SKILL_UPGRADE]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_SKILL_UPGRADE");
		return NO_MSG_ERRO;
	}
	if (!pPlayer->ChanegeDegree(pUpGradeSkill->uiSkillID, 1))
		return MSG_ERRO_0098;

	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_SkillInterrupt(const void* pBuf, CPlayer* pPlayer)
{
	MSG_SKILL_INTERRUPT* pSkillInterrupt = (MSG_SKILL_INTERRUPT*)pBuf;
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_SKILL_INTERRUPT");
		return NO_MSG_ERRO;
	}

	if (pPlayer->GetState() == OBJECT_CAST)
	{
		pPlayer->m_eState = OBJECT_IDLE;
		pPlayer->ChangeActiveSkillStatus(pPlayer->GetCurActiveSkillID(), TIME_STYLE_NONE);
		MSG_SKILL_INTERRUPT skillInterrupt;
		skillInterrupt.Head.usSize = sizeof(MSG_SKILL_INTERRUPT);
		skillInterrupt.Head.usType = _MSG_SKILL_INTERRUPT;
		skillInterrupt.uiID = pSkillInterrupt->uiID;
		pPlayer->s_World->SendMsgToClient(&skillInterrupt, pPlayer->GetSocket());
	}

	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_Effect(const void* pBuf, CPlayer* pPlayer)
{
	return NO_MSG_ERRO;
}


eError ServerSocketEventHandle::Handle_Talk(const void* pBuf, CPlayer* pPlayer)
{
	MSG_TALK* pTalk = (MSG_TALK*)pBuf;
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TALK");
		return NO_MSG_ERRO;
	}

	UINT deltaTime = timeGetTime() - pPlayer->unTalkTime;
	pPlayer->unTalkTime = timeGetTime();

	if (pTalk->uiUserID<0 || pTalk->uiUserID >= 5000 || pTalk->ucTextSize < 0 || pTalk->ucTextSize>255
		|| pTalk->ucResPlayerNameSize>MAX_NAME || pTalk->ucAimPlayerNameSize > MAX_NAME || deltaTime < 1000
		)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_TALK]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	if (pPlayer->HasTalkBan() || pPlayer->s_World->HasSpeakForbid(pPlayer->GetName()))
	{
		wstring unFound = L"你已被禁言！";
		MSG_TALK	msg_talk;
		msg_talk.ucAimPlayerNameSize = 0;
		msg_talk.ucResPlayerNameSize = 0;
		msg_talk.ucTextSize = (INT)((unFound.size() + 1) * 2);
		msg_talk.ucType = defSYSTEM;
		msg_talk.uiUserID = -1;
		wmemset(msg_talk.wcContent, 0, 256);
		wcscpy(msg_talk.wcContent, unFound.c_str());
		msg_talk.Head.usType = _MSG_TALK;
		msg_talk.Head.usSize = sizeof(msg_talk.Head) + 4 + 4 + 4;
		if ((msg_talk.ucTextSize % 4) != 0)
			msg_talk.Head.usSize += msg_talk.ucTextSize + 2;
		else
			msg_talk.Head.usSize += msg_talk.ucTextSize;

		pPlayer->s_World->SendMsgToClient(&msg_talk, pPlayer->GetSocket());
		return NO_MSG_ERRO;
	}

	if (pPlayer->GetSocket() != NULL)
	{
		CRegion* pRegion = pPlayer->GetRegion();
		if (pRegion == NULL)
		{
			sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TALK pRegion");

			return NO_MSG_ERRO;
		}

		CPlayer* pTarget = NULL;

		WCHAR	wcresPlayerName[MAX_NAME];
		char	resPlayerName[MAX_NAME * 2];
		wmemset(wcresPlayerName, 0, MAX_NAME);
		memset(resPlayerName, 0, MAX_NAME * 2);
		::lstrcpynW(wcresPlayerName, pTalk->wcContent + pTalk->ucAimPlayerNameSize, pTalk->ucResPlayerNameSize + 1);



		char Content[512];
		WideCharToMultiByte(CP_ACP, 0, pTalk->wcContent, -1, Content, 512, NULL, NULL);

		WideCharToMultiByte(CP_ACP, 0, wcresPlayerName, -1, resPlayerName, MAX_NAME * 2, NULL, NULL);
		if (strcmp(pPlayer->GetName(), resPlayerName))
		{
			pPlayer->AddDanger();
			sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_TALK_FAILNAME: ]", pPlayer->GetAccounts(), pPlayer->GetName());
			sbase::LogException(resPlayerName, "|", pPlayer->GetName());
			char	AimPNameSize[3];
			char	ResPNameSize[3];
			itoa(pTalk->ucAimPlayerNameSize, AimPNameSize, 10);
			itoa(pTalk->ucResPlayerNameSize, ResPNameSize, 10);
			string	strbufofsize1 = "| AimPlayerNameSize: ";
			strbufofsize1 += AimPNameSize;
			string	strbufofsize2 = "; ResPlayerNameSize: ";
			strbufofsize2 += ResPNameSize;
			sbase::LogException(Content, strbufofsize1, strbufofsize2);
			return NO_MSG_ERRO;
		}


		if (ChatHandler::Instance()->ParseCommands(pPlayer, Content))
		{

			return NO_MSG_ERRO;
		}

		if (defWHISPER == pTalk->ucType)
		{
			WCHAR namebuf[32];
			wmemset(namebuf, 0, 32);
			char name[64];
			memset(name, 0, 64);
			::lstrcpynW(namebuf, pTalk->wcContent, pTalk->ucAimPlayerNameSize + 1);
			WideCharToMultiByte(CP_ACP, 0, namebuf, -1, name, 64, NULL, NULL);

			char res = 0;
			pTarget = pPlayer->s_World->GetPlayerFromName(name, res);
			if (NULL != pTarget)
			{
				if (pTarget->ucChannel & defWHISPER)
				{
					pPlayer->s_World->SendMsgToClient(pTalk, pTarget->GetSocket());
					pTalk->ucType = defWHISPER_SELF;
					pPlayer->s_World->SendMsgToClient(pTalk, pPlayer->GetSocket());
				}
				else
				{
					wstring unFound = L"该玩家开启了私聊屏蔽将收不到您的消息";
					MSG_TALK	msg_talk;
					msg_talk.ucAimPlayerNameSize = 0;
					msg_talk.ucResPlayerNameSize = 0;
					msg_talk.ucTextSize = (INT)((unFound.size() + 1) * 2);
					msg_talk.ucType = defSYSTEM;
					msg_talk.uiUserID = -1;
					wmemset(msg_talk.wcContent, 0, 256);
					wcscpy(msg_talk.wcContent, unFound.c_str());
					msg_talk.Head.usType = _MSG_TALK;
					msg_talk.Head.usSize = sizeof(msg_talk.Head) + 4 + 4 + 4;
					if ((msg_talk.ucTextSize % 4) != 0)
						msg_talk.Head.usSize += msg_talk.ucTextSize + 2;
					else
						msg_talk.Head.usSize += msg_talk.ucTextSize;

					pPlayer->s_World->SendMsgToClient(&msg_talk, pPlayer->GetSocket());
				}

				return NO_MSG_ERRO;
			}
			else
			{
				wstring unFound = L"对不起，该玩家不在线或不存在";
				MSG_TALK	msg_talk;
				msg_talk.ucAimPlayerNameSize = 0;
				msg_talk.ucResPlayerNameSize = 0;
				msg_talk.ucTextSize = (INT)((unFound.size() + 1) * 2);
				msg_talk.ucType = defSYSTEM;
				msg_talk.uiUserID = -1;
				wmemset(msg_talk.wcContent, 0, 256);
				wcscpy(msg_talk.wcContent, unFound.c_str());
				msg_talk.Head.usType = _MSG_TALK;
				msg_talk.Head.usSize = sizeof(msg_talk.Head) + 4 + 4 + 4;
				if ((msg_talk.ucTextSize % 4) != 0)
					msg_talk.Head.usSize += msg_talk.ucTextSize + 2;
				else
					msg_talk.Head.usSize += msg_talk.ucTextSize;

				pPlayer->s_World->SendMsgToClient(&msg_talk, pPlayer->GetSocket());
			}
		}
		else
		{
			switch (pTalk->ucType)
			{
			case defWORLD:
			{
				if (pPlayer->IsGM())
				{
					pRegion->Talk_GM(pPlayer, pTalk);
				}
				else
				{
					break;
					pRegion->Talk_World(pPlayer, pTalk);
				}

			}
			break;
			case defLOCAL:
			{
				pTalk->ucChatBubbleSytle = pPlayer->GetChatBubbleType();
				pRegion->Talk_Local(pPlayer, pTalk);
			}
			break;
			case defGUILD:
			{
				pRegion->Talk_Union(pPlayer, pTalk);
			}
			break;
			case defCAMP:
			{
				pRegion->Talk_Faction(pPlayer, pTalk);
			}
			break;
			case defTEAM:
			{
				pRegion->Talk_Team(pPlayer, pTalk);
			}
			break;
			default:
				break;
			}

		}


	}
	else
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TALK socketid != id ");

		return NO_MSG_ERRO;
	}

	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_TalkChannel(const void* pBuf, CPlayer* pPlayer)
{
	MSG_TALKCHANNEL* pMsg = (MSG_TALKCHANNEL*)pBuf;
	pPlayer->ucChannel = pMsg->ucChannel;

	return	NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_Drop(const void* pBuf, CPlayer* pPlayer)
{
	MSG_PICKUP_DROPS* pPickup = (MSG_PICKUP_DROPS*)pBuf;

	if (pPickup->uiID < 10000 ||
		pPickup->uiID > 30000 ||
		pPickup->PickupIndex >= 8 ||
		pPickup->PickupIndex < 0
		)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_PICKUP_DROPS]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	CMonster* pMonster = pPlayer->s_World->GetMonster(pPickup->uiID);

	if (!pMonster || !pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_PICKUPDROPS");
		return NO_MSG_ERRO;
	}

	CRegion* pRegion = pPlayer->GetRegion();
	if (!pRegion)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_PICKUPDROPS pRegion");

		return NO_MSG_ERRO;
	}

	pRegion->PickupDrops(pPlayer, pMonster, pPickup->PickupIndex);

	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_Equip(const void* pBuf, CPlayer* pPlayer)
{
	MSG_USEOREQUIP_ITEM* pMsg = (MSG_USEOREQUIP_ITEM*)pBuf;


	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_USEOREQUIP");
		return NO_MSG_ERRO;
	}
	CRegion* pRegion = pPlayer->GetRegion();

	if (!pRegion)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_USEOREQUIP pRegion");

		return NO_MSG_ERRO;
	}

	pRegion->UseOrEquip(pPlayer, pMsg->Index, pMsg->Num);

	return NO_MSG_ERRO;

}

eError ServerSocketEventHandle::Handle_MoveItem(const void* pBuf, CPlayer* pPlayer)
{
	int ErrorId = -1;

	MSG_MOVE_ITEM* pMsg = (MSG_MOVE_ITEM*)pBuf;

	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_MOVEITEM pPlayer=null");
		return NO_MSG_ERRO;
	}

	ErrorId = pPlayer->MoveItem(pMsg->Index, pMsg->Num, pMsg->IndexAim, pMsg->NumAim);


	return NO_MSG_ERRO;
}


eError ServerSocketEventHandle::Handle_BreakItem(const void* pBuf, CPlayer* pPlayer)
{
	int ErrorId = -1;

	MSG_BREAK_ITEM* pMsg = (MSG_BREAK_ITEM*)pBuf;

	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_BREAK_ITEM pPlayer=null");
		return NO_MSG_ERRO;
	}

	Item* item = pPlayer->GetItem(pMsg->Index, pMsg->Num);

	if (!item)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_BREAK_ITEM orgitem=null");

		return NO_MSG_ERRO;
	}

	if (pMsg->Flags == 0)
	{
		if (pMsg->Index >= MAX_BAGS || pMsg->Index < 0
			|| pMsg->Num >= MAX_BAG_GRID || pMsg->Num < 0)
		{
			pPlayer->AddDanger();
			sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_BREAK_ITEM]", pPlayer->GetAccounts(), pPlayer->GetName());
			return NO_MSG_ERRO;
		}

		if (!item->m_Lock)
		{
			item->m_Lock = true;
		}

	}

	else if (pMsg->Flags == 1)
	{
		if (pMsg->Index >= MAX_BAGS || pMsg->Index < 0
			|| pMsg->Num >= MAX_BAG_GRID || pMsg->Num < 0)
		{
			pPlayer->AddDanger();
			sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_BREAK_ITEM]", pPlayer->GetAccounts(), pPlayer->GetName());
			return NO_MSG_ERRO;
		}

		if (item->m_Lock)
		{
			item->m_Lock = false;
		}

	}

	else if (pMsg->Flags == 2)
	{
		if (pMsg->Index >= MAX_BAGS || pMsg->Index < 0
			|| pMsg->IndexAim >= MAX_BAGS || pMsg->IndexAim < 0
			|| pMsg->Num >= MAX_BAG_GRID || pMsg->Num < 0
			|| pMsg->NumAim >= MAX_BAG_GRID || pMsg->NumAim < 0
			|| pMsg->Overlap < 1 || pMsg->Overlap >= item->GetItemBaseAttribute()->Overlap)
		{
			pPlayer->AddDanger();
			sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_BREAK_ITEM]", pPlayer->GetAccounts(), pPlayer->GetName());
			return NO_MSG_ERRO;
		}

		ErrorId = pPlayer->BreakItem(pMsg->Index, pMsg->Num, pMsg->IndexAim, pMsg->NumAim, pMsg->Overlap);

	}

	return NO_MSG_ERRO;
}


eError  ServerSocketEventHandle::Handle_Sell(const void* pBuf, CPlayer* pPlayer)
{
	int ErrorId = -1;

	MSG_SELL* pMsg = (MSG_SELL*)pBuf;

	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_SELL");
		return NO_MSG_ERRO;
	}

	ErrorId = pPlayer->SellItem(pMsg->Index, pMsg->Num);

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_StorageItem(const void* pBuf, CPlayer* pPlayer)
{
	MSG_STORAGE_ITEM* pMsg = (MSG_STORAGE_ITEM*)pBuf;
	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_STORAGEITEM");
		return NO_MSG_ERRO;
	}

	if (pMsg->Storage)
	{
		if (pMsg->Money != 0)
		{
			if (pMsg->Money < 0)
			{

				return NO_MSG_ERRO;
			}

			if (pPlayer->GetMoney() < pMsg->Money)
			{
				return NO_MSG_ERRO;
			}

			pPlayer->GetStorage()->AddMoney(pMsg->Money);
			pPlayer->AddMoney(-pMsg->Money);
			pPlayer->s_World->SendMsgToClient(pMsg, pPlayer->GetSocket());
		}
		else
		{
			bool succeed = false;

			if (pMsg->Index == -1)
			{
				succeed = pPlayer->BagToStoragePly(pMsg->IndexAim, pMsg->NumAim, pPlayer->GetuiStoreNum());
			}
			else
			{
				succeed = pPlayer->BagToStorage(pMsg->IndexAim, pMsg->NumAim, pMsg->Index);
			}

			if (succeed)
			{
				pPlayer->s_World->SendMsgToClient(pMsg, pPlayer->GetSocket());
			}
		}
	}
	else
	{
		if (pMsg->Money != 0)
		{
			if (pMsg->Money < 0)
			{

				return NO_MSG_ERRO;
			}

			if (pPlayer->GetStorage()->GetMoney() < pMsg->Money)
			{

				return NO_MSG_ERRO;
			}

			pPlayer->GetStorage()->AddMoney(-pMsg->Money);
			pPlayer->AddMoney(pMsg->Money);
			pPlayer->s_World->SendMsgToClient(pMsg, pPlayer->GetSocket());
		}
		else
		{
			bool succeed = false;

			if (pMsg->IndexAim == -1 && pMsg->NumAim == -1)
				succeed = pPlayer->StorageToBag(pMsg->Index);
			else if (pMsg->NumAim == -1)
				succeed = pPlayer->GetStorage()->Switch(pMsg->Index, pMsg->IndexAim);
			else
				succeed = pPlayer->StorageToBag(pMsg->Index, pMsg->IndexAim, pMsg->NumAim);

			if (succeed)
				pPlayer->s_World->SendMsgToClient(pMsg, pPlayer->GetSocket());
		}
	}

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_Stall(const void* pBuf, CPlayer* pPlayer)
{
	MSG_STALL* pMsg = (MSG_STALL*)pBuf;

	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_STALL");
		return NO_MSG_ERRO;
	}

	pPlayer->GetStall()->SetText(pMsg->Text);
	pPlayer->GetStall()->SetState(pMsg->State);

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_StallItem(const void* pBuf, CPlayer* pPlayer)
{
	MSG_STALLITEM* pMsg = (MSG_STALLITEM*)pBuf;

	if (pMsg->uiID > 5000 || strlen(pMsg->SellerName) > (MAX_NAME - 1))
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_STALLITEM]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	CPlayer* pSell = pPlayer->s_World->GetPlayer(pMsg->uiID);

	if (!pPlayer || !pSell || pSell == pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_STALLITEM");
		return NO_MSG_ERRO;
	}

	if (strcmp(pSell->GetName(), pMsg->SellerName))
		return	NO_MSG_ERRO;

	if (!pSell->GetStall()->IsStart())
		return	NO_MSG_ERRO;

	pSell->GetStall()->GetGoods(pPlayer);

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_StallAddItem(const void* pBuf, CPlayer* pPlayer)
{
	int ErrorId = -1;

	MSG_STALLADDITEM* pMsg = (MSG_STALLADDITEM*)pBuf;

	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_STALLADDITEM");
		return NO_MSG_ERRO;
	}

	if (pMsg->Index < 0 || pMsg->Index >= MAX_GOODS || pMsg->Pocket<0 || pMsg->Pocket >= MAX_POCKETS
		|| pMsg->BagIndex > pPlayer->GetuiBagNum() || pMsg->BagIndex >= MAX_BAGS || pMsg->BagIndex < 0 || pMsg->BagNum < 0 || pMsg->BagNum >= MAX_BAG_GRID
		|| pMsg->Money < 0 || pMsg->Bijou < 0)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_STALLADDITEM]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	ErrorId = pPlayer->GetStall()->SetGoods(pMsg->Index, pMsg->Pocket, pMsg->BagIndex, pMsg->BagNum, pMsg->Money, pMsg->Bijou);
	if (0 == ErrorId)
	{
		pMsg->StallID = pPlayer->GetStall()->GetGoods(pMsg->Index, pMsg->Pocket)->StallID;
		pPlayer->s_World->SendMsgToClient(pMsg, pPlayer->GetSocket());
	}
	else
	{
		if (MSG_ERRO_015F == ErrorId)
		{
			pPlayer->AddDanger();
			sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[SETGOODS_LOCKED]", pPlayer->GetAccounts(), pPlayer->GetName());
			return NO_MSG_ERRO;
		}
	}

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_StallDelItem(const void* pBuf, CPlayer* pPlayer)
{
	int ErrorId = -1;

	MSG_STALLDELITEM* pMsg = (MSG_STALLDELITEM*)pBuf;

	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_STALLDELITEM");
		return NO_MSG_ERRO;
	}

	if (pMsg->Index < 0 || pMsg->Index >= MAX_GOODS || pMsg->Pocket<0 || pMsg->Pocket>MAX_POCKETS)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_STALLDELITEM]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	ErrorId = pPlayer->GetStall()->DelGoods(pMsg->Index, pMsg->Pocket);

	if (MSG_ERRO_015F == ErrorId)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_DELGOODS_UNLOCK]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}
	if (MSG_ERRO_015A == ErrorId)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_DELGOODS_ITEMNULL]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}
	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_StallBuyItem(const void* pBuf, CPlayer* pPlayer)
{
	if (!pPlayer)
		return NO_MSG_ERRO;

	MSG_STALLBUYITEM* pMsg = (MSG_STALLBUYITEM*)pBuf;
	CPlayer* pSell = pPlayer->s_World->GetPlayer(pMsg->uiSellID);
	CPlayer* pBuy = pPlayer->s_World->GetPlayer(pMsg->uiBuyID);

	if (!pSell || !pBuy || pSell == pBuy)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_STALLBUYITEM");
		return NO_MSG_ERRO;
	}

	if (strcmp(pSell->GetName(), pMsg->SellerName))
		return	NO_MSG_ERRO;
	if (strcmp(pBuy->GetName(), pMsg->CustomName))
		return	NO_MSG_ERRO;
	if (!pSell->GetStall()->IsStart())
		return	NO_MSG_ERRO;

	if (pMsg->Index < 0 || pMsg->Index >= MAX_GOODS || pMsg->Pocket<0 || pMsg->Pocket>MAX_POCKETS)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_STALLBUYITEM]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	eError ErrorId = pSell->GetStall()->BuyGoods(pBuy, pMsg->StallID, pMsg->Index, pMsg->Pocket);
	if (ErrorId == MSG_ERRO_015B)
	{
		pPlayer->s_World->SendMsgToClient(pMsg, pSell->GetSocket());
		pPlayer->s_World->SendMsgToClient(pMsg, pBuy->GetSocket());
	}
	return ErrorId;
}

eError	ServerSocketEventHandle::Handle_TalkNPC(const void* pBuf, CPlayer* pPlayer)
{
	MSG_TALK_NPC* pMsg = (MSG_TALK_NPC*)pBuf;
	CNPC* pNpc = pPlayer->s_World->GetNPC(pMsg->NPCID);

	if (!pPlayer || !pNpc)
	{
		if (pPlayer)
		{
			pPlayer->AddDanger();
			sbase::LogException("Message type Exception, Accounts : %s, Role : %s, [_MSG_SKILL]", pPlayer->GetAccounts(), pPlayer->GetName());
		}
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TALKNPC");
		return NO_MSG_ERRO;
	}

	if (pPlayer->GetlFaction() != pNpc->GetlFaction() && pNpc->GetlFaction() != 2)
	{

		return NO_MSG_ERRO;
	}

	pNpc->Talk(pPlayer);

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_NPCData(const void* pBuf, CPlayer* pPlayer)
{
	MSG_NPC_DATA* pMsg = (MSG_NPC_DATA*)pBuf;
	CNPC* pNpc = pPlayer->s_World->GetNPC(pMsg->NPCID);

	if (!pPlayer || !pNpc)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_NPCDATA");
		return NO_MSG_ERRO;
	}

	pNpc->GetData(pPlayer);

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_NPCSell(const void* pBuf, CPlayer* pPlayer)
{
	eError ErrorId = NO_MSG_ERRO;
	MSG_NPC_SELL* pMsg = (MSG_NPC_SELL*)pBuf;
	CNPC* pNpc = pPlayer->s_World->GetNPC(pMsg->NPCID);

	if (!pPlayer || !pNpc)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_NPCSELL");
		return NO_MSG_ERRO;
	}

	ErrorId = pNpc->Sell(pPlayer, pMsg->Index, pMsg->Num, pMsg->Count, pMsg->Base);

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_FindQuest(const void* pBuf, CPlayer* pPlayer)
{
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_FINDQUEST");
		return NO_MSG_ERRO;
	}

	pPlayer->FindQuest();

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_Explorer(const void* pBuf, CPlayer* pPlayer)
{
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_FINDQUEST");
		return NO_MSG_ERRO;
	}

	pPlayer->Explorer();

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_NPCAcceptQuest(const void* pBuf, CPlayer* pPlayer)
{
	eError ErrorId = NO_MSG_ERRO;

	MSG_NPC_ACCEPTQUEST* pMsg = (MSG_NPC_ACCEPTQUEST*)pBuf;
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_NPCACCEPTQUEST");
		return NO_MSG_ERRO;
	}

	ErrorId = pPlayer->AcceptQuest(pMsg->QuestID);

	return ErrorId;
}

eError	ServerSocketEventHandle::Handle_NPCCancelQuest(const void* pBuf, CPlayer* pPlayer)
{
	int ErrorId = -1;

	MSG_NPC_CANCELQUEST* pMsg = (MSG_NPC_CANCELQUEST*)pBuf;
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_NPCCANCELQUEST");
		return NO_MSG_ERRO;
	}

	ErrorId = pPlayer->CancelQuest(pMsg->QuestID);

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_NPCCompleteQuest(const void* pBuf, CPlayer* pPlayer)
{
	eError ErrorId = NO_MSG_ERRO;

	MSG_NPC_COMPLETEQUEST* pMsg = (MSG_NPC_COMPLETEQUEST*)pBuf;
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_NPCCOMPLETEQUEST");
		return NO_MSG_ERRO;
	}

	ErrorId = pPlayer->CompleteQuest(pMsg->QuestID, pMsg->Choice);

	return ErrorId;
}

eError	ServerSocketEventHandle::Handle_NPCQuestView(const void* pBuf, CPlayer* pPlayer)
{
	MSG_NPC_QUESTVIEW* pMsg = (MSG_NPC_QUESTVIEW*)pBuf;
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_NPCQUESTVIEW");
		return NO_MSG_ERRO;
	}

	pPlayer->ViewQuest(pMsg->QuestID);

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_RelationRequest(const void* pBuf, CPlayer* pPlayer)
{
	MSG_RELATION_REQUEST* pMsg = (MSG_RELATION_REQUEST*)pBuf;

	if (pMsg->cType > 1)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_RELATION_REQUEST]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_RELATION_REQUEST");
		return NO_MSG_ERRO;
	}
	RELATION tmpRelation;
	CRegion* pRegion;
	IF_OK(pPlayer)
	{
		pRegion = pPlayer->GetRegion();
		if (pRegion == NULL)
		{
			sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_RELATION_REQUEST pRegion");

			return NO_MSG_ERRO;
		}
	}
	else
	{
		return NO_MSG_ERRO;
	}

	char Result;
	CPlayer* pRelationPlayer = pPlayer->s_World->GetPlayerFromName(pMsg->cName_Response, Result);
	if (pRelationPlayer == NULL)
	{
		if (Result == -1)
		{
			return MSG_ERRO_021E;
		}
	}

	tmpRelation.City = pPlayer->s_World->GetRegionFromLogicID(pRelationPlayer->GetlRegionID())->GetID();
	tmpRelation.Camp = pRelationPlayer->GetlFaction();
	tmpRelation.strName = pMsg->cName_Response;
	tmpRelation.Type = (RELATION_TYPE)pMsg->cType;
	tmpRelation.Business = pRelationPlayer->GetlClass();
	tmpRelation.LV = pRelationPlayer->GetRank();

	eError error = pPlayer->UpdateRelation(tmpRelation, Result);
	if (!error)
	{
		if (!pMsg->cType)
		{
			if (pPlayer->GetlFaction() == pRelationPlayer->GetlFaction())
			{
				MSG_RELATION RelationInfo;
				RelationInfo.Head.usSize = sizeof(MSG_RELATION);
				RelationInfo.Head.usType = _MSG_RELATION;
				RelationInfo.bUnion = pRelationPlayer->GetlFaction();
				RelationInfo.LV = pRelationPlayer->GetcRank();
				RelationInfo.ucCity = pPlayer->s_World->GetRegionFromLogicID(pRelationPlayer->GetlRegionID())->GetID();
				memcpy(RelationInfo.cName, pRelationPlayer->GetName(), sizeof(RelationInfo.cName));
				RelationInfo.Business = pRelationPlayer->GetlClass();
				RelationInfo.IsOnline = true;
				RelationInfo.Style = TYPE_FRIEND;
				pPlayer->s_World->SendMsgToClient(&RelationInfo, pPlayer->GetSocket());
			}
			else
				return MSG_ERRO_021F;

		}
		else
		{
			MSG_RELATION RelationInfo;
			RelationInfo.Head.usSize = sizeof(MSG_RELATION);
			memcpy(RelationInfo.cName, pMsg->cName_Response, sizeof(RelationInfo.cName));
			RelationInfo.Head.usType = _MSG_RELATION;
			RelationInfo.Style = TYPE_BLACKLIST;
			pPlayer->s_World->SendMsgToClient(&RelationInfo, pPlayer->GetSocket());
		}
	}
	else
	{
		return error;
	}

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_RelationResponse(const void* pBuf, CPlayer* pPlayer)
{
	MSG_RELATION_RESPONSE* pMsg = (MSG_RELATION_RESPONSE*)pBuf;

	char Result;
	CPlayer* pOherPlayer = pPlayer->s_World->GetPlayerFromName(pMsg->cName_Request, Result);
	CPlayer* pMySelf = pPlayer;
	if (pOherPlayer == NULL || pMySelf == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_RELATION_RESPONSE");
		return NO_MSG_ERRO;
	}
	if (pMsg->IsAgree)
	{
		if (pPlayer != NULL)
		{
			MSG_RELATION RelationInfo;
			RelationInfo.Head.usSize = sizeof(MSG_RELATION);
			RelationInfo.Head.usType = _MSG_RELATION;
			RelationInfo.bUnion = pOherPlayer->GetlFaction();
			RelationInfo.LV = pOherPlayer->GetcRank();
			RelationInfo.ucCity = pPlayer->s_World->GetRegionFromLogicID(pOherPlayer->GetlRegionID())->GetID();
			RelationInfo.Business = pOherPlayer->GetlClass();
			memcpy(RelationInfo.cName, pMySelf->GetName(), sizeof(RelationInfo.cName));
			RelationInfo.IsOnline = true;
			RelationInfo.Style = 0;
			pPlayer->s_World->SendMsgToClient(&RelationInfo, pPlayer->GetSocket());
		}
	}
	else
	{
		if (pOherPlayer != NULL)
		{
			if (pOherPlayer == pMySelf)
			{
				pOherPlayer->DeleteRelation(pMsg->cName_Request);
				return NO_MSG_ERRO;
			}
			MSG_RELATION_RESPONSE Response;
			Response.Head.usSize = sizeof(MSG_RELATION_RESPONSE);
			Response.Head.usType = _MSG_RELATION_RESPONSE;
			Response.IsAgree = false;
			memcpy(Response.cName_Request, pMySelf->GetName(), sizeof(Response.cName_Request));
			pPlayer->s_World->SendMsgToClient(&Response, pPlayer->GetSocket());
		}

	}
	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_RelationDel(const void* pBuf, CPlayer* pPlayer)
{
	MSG_RELATION_DELETE* pMsg = (MSG_RELATION_DELETE*)pBuf;

	if (pMsg->Style > 1)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_RELATION_DELETE]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_RELATION_DELETE");
		return NO_MSG_ERRO;
	}

	pPlayer->DeleteRelation(pMsg->cName_Request);
	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_Trade(const void* pBuf, CPlayer* pPlayer)
{
	eError ErrorId = NO_MSG_ERRO;

	MSG_TRADE* msg = (MSG_TRADE*)pBuf;

	CPlayer* p = pPlayer->s_World->GetPlayer(msg->uiID);
	CPlayer* ptrader = pPlayer->s_World->GetPlayer(msg->uiObjectID);

	if (!p || !ptrader)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TRADE");
		return NO_MSG_ERRO;
	}
	float fPX = p->GetPosX() - ptrader->GetPosX();
	float fPZ = p->GetPosZ() - ptrader->GetPosZ();
	float fDist = fPX * fPX + fPZ * fPZ;

	if (p->GetRegion() != ptrader->GetRegion() || fDist < 0 || fDist > 225)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_TRADE]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;

	}
	if (strcmp(msg->chName, p->GetName()) || strcmp(msg->chObjectName, ptrader->GetName()))
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_TRADE]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	switch (msg->State)
	{
	case TradeStart:
	{
		ErrorId = p->GetTrade()->StartTrade(ptrader);
	}
	break;
	case TradeLock:
	{
		ErrorId = p->GetTrade()->LockTrade(pPlayer);
	}
	break;
	case TradeCancel:
		if (p->GetTrade()->GetTradeState() != TradeIdle)
		{
			p->GetTrade()->GetTrader()->GetTrade()->EndTrade();
			p->GetTrade()->EndTrade();

			p->GetTrade()->GetTrader()->GetTrade()->ClearTrade();
			p->GetTrade()->ClearTrade();
			SendErrorInfo(MSG_ERRO_024F, ptrader);
			SendErrorInfo(MSG_ERRO_024F, p);
			return NO_MSG_ERRO;
		}
		break;
	default:
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TRADE");
	}
	break;
	}

	return ErrorId;
}

eError	ServerSocketEventHandle::Handle_TradeMoney(const void* pBuf, CPlayer* pPlayer)
{
	eError ErrorId = NO_MSG_ERRO;

	MSG_TRADE_MONEY* msg = (MSG_TRADE_MONEY*)pBuf;

	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TRADE_MONEY");
		return NO_MSG_ERRO;
	}
	if (strcmp(msg->chName, pPlayer->GetName()))
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_TRADE_MONEY]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	ErrorId = pPlayer->GetTrade()->SetMoney(msg->Money, msg->Bijou, pPlayer);

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_TradeAddItem(const void* pBuf, CPlayer* pPlayer)
{
	eError ErrorId = NO_MSG_ERRO;

	MSG_TRADE_ADDITEM* msg = (MSG_TRADE_ADDITEM*)pBuf;

	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TRADE_ADDITEM");
		return NO_MSG_ERRO;
	}
	if (strcmp(msg->chName, pPlayer->GetName()))
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_TRADE_ADDITEM]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	Item* item = pPlayer->GetItem(msg->Bag, msg->Grid);

	if (!item)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TRADE_ADDITEM");

		return NO_MSG_ERRO;
	}

	ErrorId = pPlayer->GetTrade()->SetItem(msg->Index, item, pPlayer);

	return ErrorId;
}

eError	ServerSocketEventHandle::Handle_TradeRemoveItem(const void* pBuf, CPlayer* pPlayer)
{
	eError ErrorId = NO_MSG_ERRO;

	MSG_TRADE_REMOVEITEM* msg = (MSG_TRADE_REMOVEITEM*)pBuf;
	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TRADE_REMOVEITEM");
		return NO_MSG_ERRO;
	}

	if (strcmp(msg->chName, pPlayer->GetName()))
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_TRADE_REMOVEITEM]", pPlayer->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	ErrorId = pPlayer->GetTrade()->RemoveItem(msg->Index, pPlayer);

	return ErrorId;
}

eError	ServerSocketEventHandle::Handle_EquipForge(const void* pBuf, CPlayer* pPlayer)
{

	eError ErrorId = NO_MSG_ERRO;

	MSG_EUQIPFORGE* msg = (MSG_EUQIPFORGE*)pBuf;
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_EQUIPFORGE");
		return NO_MSG_ERRO;
	}

	sbase::ConsoleWriteColorText(FOREGROUND_RED, "recv _MSG_EQUIPFORGE HEAD[%d,%d] Type: %d Success: %d", msg->Head.usType, msg->Head.usSize, msg->Type, msg->Success);

	ErrorId = pPlayer->GetEquipForge()->Forge(msg->Type);

	return ErrorId;
}

eError	ServerSocketEventHandle::Handle_EquipForge_Equip(const void* pBuf, CPlayer* pPlayer)
{
	eError ErrorId = NO_MSG_ERRO;

	MSG_EQUIPFORGE_EQUIP* msg = (MSG_EQUIPFORGE_EQUIP*)pBuf;
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_EQUIPFORGE_EQUIP");
		return NO_MSG_ERRO;
	}
	char Ifbuf[128];
	_stprintf(Ifbuf, "Handle_EquipForge_Equip:%d", msg->Type);
	sbase::ConsoleWriteColorText(FOREGROUND_RED, Ifbuf);

	ErrorId = pPlayer->GetEquipForge()->SetEquip(pPlayer->GetItem(msg->Index, msg->Num), msg->Index, msg->Num, msg->Type);

	return ErrorId;
}

eError	ServerSocketEventHandle::Handle_EquipMaterial(const void* pBuf, CPlayer* pPlayer)
{

	eError ErrorId = NO_MSG_ERRO;

	MSG_EQUIPFORGE_MATERIAL* msg = (MSG_EQUIPFORGE_MATERIAL*)pBuf;
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_EQUIPFORGE_MATERIAL");
		return NO_MSG_ERRO;
	}

	char Ifbuf[128];
	_stprintf(Ifbuf, "宝石:%d", msg->Type);
	sbase::ConsoleWriteColorText(FOREGROUND_RED, Ifbuf);

	ErrorId = pPlayer->GetEquipForge()->SetMaterial(pPlayer->GetItem(msg->Index, msg->Num), msg->Index, msg->Num, msg->Type);

	return ErrorId;
}

eError	ServerSocketEventHandle::Handle_EquipCancel(const void* pBuf, CPlayer* pPlayer)
{

	eError ErrorId = NO_MSG_ERRO;
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_EQUIPFORGE_EQUIPCANCEL");
		return NO_MSG_ERRO;
	}

	ErrorId = pPlayer->GetEquipForge()->ClearEquip();

	return ErrorId;
}

eError	ServerSocketEventHandle::Handle_EquipMaterialCancel(const void* pBuf, CPlayer* pPlayer)
{

	eError ErrorId = NO_MSG_ERRO;
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_EQUIPFORGE_MATERIALCANCEL");
		return NO_MSG_ERRO;
	}

	ErrorId = pPlayer->GetEquipForge()->ClearMaterial();

	return ErrorId;
}

eError	ServerSocketEventHandle::Handle_EquipForgeCacel(const void* pBuf, CPlayer* pPlayer)
{
	eError ErrorId = NO_MSG_ERRO;
	if (pPlayer == NULL)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_EQUIPFORGE_CANCEL");
		return NO_MSG_ERRO;
	}

	ErrorId = pPlayer->GetEquipForge()->ClearForge();

	return ErrorId;
}

eError	ServerSocketEventHandle::Handle_TeamInvite(const void* pBuf, CPlayer* pPlayer)
{
	MSG_TEAM* msg = (MSG_TEAM*)pBuf;
	char	res = 0;

	CPlayer* pResPlayer = pPlayer->s_World->GetPlayerFromName(msg->chResPlayer, res);
	CPlayer* pAimPlayer = pPlayer->s_World->GetPlayerFromName(msg->chAimPlayer, res);

	if (!pResPlayer)
		return	NO_MSG_ERRO;

	if (pResPlayer == pAimPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TEAM_INVITE");

		return NO_MSG_ERRO;
	}
	if (!pAimPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TEAM_INVITE");
		MSG_TIP msg_tip;
		msg_tip.Head.usSize = sizeof(MSG_TIP);
		msg_tip.Head.usType = _MSG_TEAM_TIP;
		wmemset(msg_tip.chName, 0, 64);
		wmemset(msg_tip.tip, 0, 128);
		std::wstring	buf = L"\0";
		std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
		buf = L"邀请失败，对方可能不在线";
		std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
		pPlayer->s_World->SendMsgToClient(&msg_tip, pResPlayer->GetSocket());
		return NO_MSG_ERRO;
	}

	if (pResPlayer->GetlFaction() != pAimPlayer->GetlFaction())
	{
		MSG_TIP msg_tip;
		msg_tip.Head.usSize = sizeof(MSG_TIP);
		msg_tip.Head.usType = _MSG_TEAM_TIP;
		wmemset(msg_tip.chName, 0, 64);
		wmemset(msg_tip.tip, 0, 128);
		std::wstring	buf = L"\0";
		std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
		buf = L"对方和你不是相同的阵营";
		std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
		pPlayer->s_World->SendMsgToClient(&msg_tip, pResPlayer->GetSocket());
		return NO_MSG_ERRO;
	}


	if (NULL == pAimPlayer->m_pTeamLeader)
	{
		if ((NULL == pResPlayer->m_pTeamLeader))
		{
			pPlayer->s_World->SendMsgToClient(msg, pAimPlayer->GetSocket());
			MSG_TIP msg_tip;
			msg_tip.Head.usSize = sizeof(MSG_TIP);
			msg_tip.Head.usType = _MSG_TEAM_TIP;
			wmemset(msg_tip.chName, 0, 64);
			wmemset(msg_tip.tip, 0, 128);
			std::wstring	buf = L"\0";
			std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
			buf = L"你向对方发出了组队邀请";
			std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
			pPlayer->s_World->SendMsgToClient(&msg_tip, pResPlayer->GetSocket());

			return NO_MSG_ERRO;
		}

		if ((NULL != pResPlayer->m_pTeamLeader) &&
			(!pResPlayer->IsLeader()))
		{
			return NO_MSG_ERRO;
		}

		if ((NULL != pResPlayer->m_pTeamLeader) &&
			(pResPlayer->IsLeader()))
		{
			if (pResPlayer->m_pTeamLeader->m_Teammates.size() < Max_TEAMMATES_NUM)
			{
				pPlayer->s_World->SendMsgToClient(msg, pAimPlayer->GetSocket());
				MSG_TIP msg_tip;
				msg_tip.Head.usSize = sizeof(MSG_TIP);
				msg_tip.Head.usType = _MSG_TEAM_TIP;
				wmemset(msg_tip.chName, 0, 64);
				wmemset(msg_tip.tip, 0, 128);
				std::wstring	buf = L"\0";
				std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
				buf = L"你向对方发出了组队邀请";
				std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
				pPlayer->s_World->SendMsgToClient(&msg_tip, pResPlayer->GetSocket());

			}
			else
			{
				MSG_TIP msg_tip;
				msg_tip.Head.usSize = sizeof(MSG_TIP);
				msg_tip.Head.usType = _MSG_TEAM_TIP;
				wmemset(msg_tip.chName, 0, 64);
				wmemset(msg_tip.tip, 0, 128);
				std::wstring	buf = L"\0";
				std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
				buf = L"队伍已满";
				std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
				pPlayer->s_World->SendMsgToClient(&msg_tip, pResPlayer->GetSocket());
			}

			return NO_MSG_ERRO;
		}

	}
	else
	{
		if (pResPlayer->m_pTeamLeader)
		{
			MSG_TIP msg_tip;
			msg_tip.Head.usSize = sizeof(MSG_TIP);
			msg_tip.Head.usType = _MSG_TEAM_TIP;
			wmemset(msg_tip.chName, 0, 64);
			wmemset(msg_tip.tip, 0, 128);
			std::wstring	buf = L"\0";
			std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
			buf = L"你所邀请的玩家已经有自己的队伍";
			std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
			pPlayer->s_World->SendMsgToClient(&msg_tip, pResPlayer->GetSocket());
			return NO_MSG_ERRO;
		}
		else
		{
			msg->Head.usType = _MSG_TEAM_REQUEST;
			pPlayer->s_World->SendMsgToClient(msg, pAimPlayer->m_pTeamLeader->GetSocket());
			MSG_TIP msg_tip;
			msg_tip.Head.usSize = sizeof(MSG_TIP);
			msg_tip.Head.usType = _MSG_TEAM_TIP;
			wmemset(msg_tip.chName, 0, 64);
			wmemset(msg_tip.tip, 0, 128);
			std::wstring	buf = L"\0";
			std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
			buf = L"你向对方发出了组队申请";
			std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
			pPlayer->s_World->SendMsgToClient(&msg_tip, pResPlayer->GetSocket());
		}

	}

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_TeamReject(const void* pBuf, CPlayer* pPlayer)
{
	MSG_TEAM* msg = (MSG_TEAM*)pBuf;
	char	res = 0;

	CPlayer* pResPlayer = pPlayer->s_World->GetPlayerFromName(msg->chResPlayer, res);
	CPlayer* pAimPlayer = pPlayer->s_World->GetPlayerFromName(msg->chAimPlayer, res);

	if (!pResPlayer || !pAimPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TEAM_REJECT");
		return NO_MSG_ERRO;
	}
	MSG_TIP msg_tip;
	msg_tip.Head.usSize = sizeof(MSG_TIP);
	msg_tip.Head.usType = _MSG_TEAM_TIP;
	wmemset(msg_tip.chName, 0, 64);
	wmemset(msg_tip.tip, 0, 128);
	WCHAR	wcBuf[64];
	wmemset(wcBuf, 0, 64);
	MultiByteToWideChar(CP_ACP, 0, msg->chAimPlayer, -1, wcBuf, 64);
	std::wstring buf = wcBuf;
	std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
	buf += L"拒绝了你的请求";
	std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
	pPlayer->s_World->SendMsgToClient(&msg_tip, pResPlayer->GetSocket());

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_TeamInfo(const void* pBuf, CPlayer* pPlayer)
{
	MSG_TEAMINFO* msg = (MSG_TEAMINFO*)pBuf;
	char	res = 0;

	CPlayer* pMySelf = pPlayer;
	CPlayer* pAimPlayer = pPlayer->s_World->GetPlayerFromName(msg->chAimName, res);

	if (!pMySelf || !pAimPlayer)
		return NO_MSG_ERRO;

	if (pAimPlayer->m_pTeamLeader)
		msg->bIsAimInTeam = true;
	else
		msg->bIsAimInTeam = false;

	if (pMySelf->m_pTeamLeader)
		msg->bIsSelfInTeam = true;
	else
		msg->bIsSelfInTeam = false;

	pPlayer->s_World->SendMsgToClient(msg, pPlayer->GetSocket());

	return	NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_TeamAdd(const void* pBuf, CPlayer* pPlayer)
{
	int ErrorId = -1;

	MSG_TEAM* msg = (MSG_TEAM*)pBuf;
	char	res = 0;

	CPlayer* pResPlayer = pPlayer->s_World->GetPlayerFromName(msg->chResPlayer, res);
	CPlayer* pAimPlayer = pPlayer->s_World->GetPlayerFromName(msg->chAimPlayer, res);

	if (!pResPlayer || !pAimPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TEAM_ADD");
		return NO_MSG_ERRO;
	}

	if (pResPlayer->GetlFaction() != pAimPlayer->GetlFaction())
	{

		return NO_MSG_ERRO;
	}

	if (pResPlayer)
	{
		ErrorId = pResPlayer->AddTeammate_New(pAimPlayer);
	}
	else
	{

	}

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_TeamDel(const void* pBuf, CPlayer* pPlayer)
{
	MSG_TEAM* msg = (MSG_TEAM*)pBuf;

	char	res = 0;
	CPlayer* pResPlayer = pPlayer->s_World->GetPlayerFromName(msg->chResPlayer, res);
	CPlayer* pAimPlayer = pPlayer->s_World->GetPlayerFromName(msg->chAimPlayer, res);

	if (!pResPlayer || !pAimPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TEAM_DELETE");
		return NO_MSG_ERRO;
	}

	if (pResPlayer->IsLeader())
	{
		pResPlayer->RemoveTeammate_New(pAimPlayer);
	}
	else
	{

	}

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_TeamLeader(const void* pBuf, CPlayer* pPlayer)
{
	int ErrorId = -1;

	MSG_TEAM* msg = (MSG_TEAM*)pBuf;
	char	res = 0;

	CPlayer* pResPlayer = pPlayer->s_World->GetPlayerFromName(msg->chResPlayer, res);
	CPlayer* pAimPlayer = pPlayer->s_World->GetPlayerFromName(msg->chAimPlayer, res);

	if (!pResPlayer || !pAimPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TEAM_DELETE");
		return NO_MSG_ERRO;
	}

	if (pResPlayer->IsLeader())
	{
		ErrorId = pResPlayer->ChangeLeader_New(pAimPlayer);
	}
	else
	{

	}

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_TeamDissolve(const void* pBuf, CPlayer* pPlayer)
{
	MSG_TEAM* msg = (MSG_TEAM*)pBuf;
	char	res = 0;
	CPlayer* pResPlayer = pPlayer->s_World->GetPlayerFromName(msg->chResPlayer, res);

	if (!pResPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TEAM_DELETE");
		return NO_MSG_ERRO;
	}

	if (pResPlayer->IsLeader())
	{
		pResPlayer->ClearTeam_New();
	}
	else
	{

	}

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_TeamLeave(const void* pBuf, CPlayer* pPlayer)
{
	MSG_TEAM* msg = (MSG_TEAM*)pBuf;
	char	res = 0;
	CPlayer* pResPlayer = pPlayer->s_World->GetPlayerFromName(msg->chResPlayer, res);

	if (!pResPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_TEAM_DELETE");
		return NO_MSG_ERRO;
	}

	if (pResPlayer->m_pTeamLeader)
	{
		pResPlayer->LeaveTeam_New();
	}
	else
	{

	}

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_BackHome(const void* pBuf, CPlayer* pPlayer)
{
	int ErrorId = -1;

	MSG_BACKHOME* msg = (MSG_BACKHOME*)pBuf;
	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_BACKHOME");
		return NO_MSG_ERRO;
	}

	ErrorId = pPlayer->Revive(msg->ReviveType);

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_ConsortiaCreate(const void* pBuf, CPlayer* pPlayer)
{
	MSG_CONSORTIA_CREATE* msg = (MSG_CONSORTIA_CREATE*)pBuf;
	if (strlen(msg->ci.acName) == 0)
	{
		return MSG_ERRO_035D;
	}

	if (pPlayer)
	{
		int Bag;
		int Num;

		MSG_CONSORTIA_CREATE_RESPONSE MsgCreateResponse;
		MsgCreateResponse.Head.usSize = sizeof(MsgCreateResponse);
		MsgCreateResponse.Head.usType = _MSG_CONSORTIA_CREATE_RESPONSE;
		MsgCreateResponse.uiPlayerID = msg->uiPlayerID;
		MsgCreateResponse.ci = msg->ci;
		MsgCreateResponse.ci.iLevel = 1;
		MsgCreateResponse.ci.lCredit = 0;
		MsgCreateResponse.ci.lfound = 0;
		MsgCreateResponse.ci.lbijou = 0;
		MsgCreateResponse.cm = msg->cm;
		MsgCreateResponse.cm.acDuty = CONSORTIA_CHAIRMAN;
		MsgCreateResponse.cm.ucBusiness = pPlayer->GetlClass();
		MsgCreateResponse.cm.uiContribute = 0;
		if (pPlayer->HaveConsortia())
		{
			MsgCreateResponse.Style = 3;
			return MSG_ERRO_0359;
		}
		else if (pPlayer->GetMoney() - CREATE_CONSORTIA_NEED_MONEY < 0)
		{
			MsgCreateResponse.Style = 2;
			return MSG_ERRO_035A;
		}
		else if (pPlayer->GetRank() < CREATE_CONSORTIA_NEED_LEVEL)
		{
			MsgCreateResponse.Style = 1;
			return MSG_ERRO_035B;
		}
		else if (pPlayer->s_World->g_pConsortiaManager->GetConsortiaInfo(string(msg->ci.acName)) != NULL)
		{
			MsgCreateResponse.Style = 4;
			return MSG_ERRO_035C;
		}
		else if (!pPlayer->FindItem(CREATE_CONSORTIA_NEED_ITEM, &Bag, &Num))
		{
			return MSG_ERRO_035E;
		}
		else
		{
			MsgCreateResponse.Style = 0;
			ConsortiaLevelUp* Basefound = pPlayer->s_World->g_pConsortiaManager->GetConsortiaLevelUp(1);
			ConsortiaElem NewConsortia;
			strcpy(NewConsortia.acName, msg->ci.acName);
			strcpy(NewConsortia.acTextInfo, msg->ci.acTextInfo);
			NewConsortia.uiID = -1;
			NewConsortia.lfound = Basefound->lfound;
			NewConsortia.lbijou = Basefound->lbijou;
			NewConsortia.iLevel = 1;
			NewConsortia.lCredit = 0;
			NewConsortia.m_pAltar.clear();
			pPlayer->SetConsortiaInfo(pPlayer->s_World->g_pConsortiaManager->AddConsortia(NewConsortia));
			pPlayer->RequestAddConsortiaFromDB(pBuf);

			if (pPlayer->DelItem(Bag, Num))
			{
				MSG_SYS_DEL_ITEM del_item;
				del_item.Head.usSize = sizeof(MSG_SYS_DEL_ITEM);
				del_item.Head.usType = _MSG_SYS_DEL_ITEM;

				del_item.index = Bag;
				del_item.num = Num;
				pPlayer->s_World->SendMsgToClient(&del_item, pPlayer->GetSocket());

			}


			return NO_MSG_ERRO;
		}
	}
	else
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_CONSORTIA_CREATE");
		return NO_MSG_ERRO;
	}

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_ConsortiaInvite(const void* pBuf, CPlayer* pPlayer)
{
	MSG_CONSORTIA_INVITE* msg = (MSG_CONSORTIA_INVITE*)pBuf;
	CPlayer* pInvitePlayer = pPlayer;
	CPlayer* pInvitedPlayer = CPlayer::GetPlayerFromRoleName(msg->ciInvitedName);
	if (NULL != pInvitePlayer && NULL != pInvitedPlayer)
	{
		if (pInvitedPlayer->HaveConsortia())
		{
			return MSG_ERRO_0368;
		}
		else if (pInvitedPlayer->GetlFaction() != pInvitePlayer->GetlFaction())
		{
			return MSG_ERRO_0369;
		}
		else if (pInvitePlayer->GetcUnionBusiness() < CONSORTIA_DIRECTOR)
		{
			return MSG_ERRO_036A;
		}
		else if (pInvitePlayer->IsMemeberFull())
		{
			return MSG_ERRO_036B;
		}
		else if (pInvitePlayer->s_World->g_pConsortiaManager->HaveMember(pInvitePlayer->GetConsortia(), pInvitedPlayer->GetName()))
		{
			return MSG_ERRO_036C;
		}
		else
		{
			strcpy(msg->ciInvitedName, pInvitePlayer->GetName());
			strcpy(msg->ciConsortiaName, pInvitePlayer->GetConsortia()->acName);
			pPlayer->s_World->SendMsgToClient(msg, pInvitedPlayer->GetSocket());
		}
	}
	else
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_CONSORTIA_INVITE");
		return NO_MSG_ERRO;
	}

	return NO_MSG_ERRO;

}

eError	ServerSocketEventHandle::Handle_ConsortiaDimiss(const void* pBuf, CPlayer* pPlayer)
{
	MSG_CONSORTIA_DISMISS* msg = (MSG_CONSORTIA_DISMISS*)pBuf;
	if (pPlayer)
	{
		return pPlayer->DismissConsortia();
	}
	else
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_CONSORTIA_DISMISS");
		return NO_MSG_ERRO;
	}

	return NO_MSG_ERRO;

}

eError	ServerSocketEventHandle::Handle_ConsortiaResponse(const void* pBuf, CPlayer* pPlayer)
{
	MSG_CONSORTIA_RESPONSE* msg = (MSG_CONSORTIA_RESPONSE*)pBuf;
	CPlayer* pReceivePlayer = CPlayer::GetPlayerFromRoleName(msg->ciReceiveName);
	CPlayer* pSendPlayer = pPlayer->s_World->GetPlayer(msg->uiSendPlayerID);
	if (pReceivePlayer && pSendPlayer && !pSendPlayer->HaveConsortia())
	{
		if (0 == msg->Style && pReceivePlayer->GetConsortia())
		{
			if (pReceivePlayer->s_World->g_pConsortiaManager->HaveMember(pReceivePlayer->GetConsortia(), pSendPlayer->GetName()))
				return NO_MSG_ERRO;
			if (pReceivePlayer->s_World->g_pConsortiaManager->IsMemberFull(pReceivePlayer->GetConsortia()))
				return MSG_ERRO_036B;

			static ConsortiaRelationElem Elem;
			Elem.strName = pSendPlayer->GetName();
			Elem.lMapID = pSendPlayer->GetRegion()->GetID();
			Elem.LV = pSendPlayer->GetcRank();
			Elem.Job = CONSORTIA_MEMBER;
			Elem.ucBusiness = pSendPlayer->GetlClass();
			Elem.uiContribute = pSendPlayer->GetuiUionContribute();
			pReceivePlayer->AddConsortiaMember(Elem);

			pSendPlayer->SetConsortiaInfo(const_cast<ConsortiaElem*>(pReceivePlayer->GetConsortia()));
			pSendPlayer->SetcUnionBusiness(CONSORTIA_MEMBER);
			pSendPlayer->SendConsortiaMsg();
		}
	}
	else
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_CONSORTIA_RESPONSE");
		return NO_MSG_ERRO;
	}

	return NO_MSG_ERRO;

}

eError	ServerSocketEventHandle::Handle_ConsortiaExit(const void* pBuf, CPlayer* pPlayer)
{
	MSG_CONSORTIA_EXIT* pExitMsg = (MSG_CONSORTIA_EXIT*)pBuf;
	if (NULL != pPlayer)
		return pPlayer->DelMeFromConsortia();
	else
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_CONSORTIA_EXIT");
	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_ConsortiaText(const void* pBuf, CPlayer* pPlayer)
{
	MSG_CONSORTIA_TEXT* pTEXTMsg = (MSG_CONSORTIA_TEXT*)pBuf;
	if (NULL != pPlayer)
		return pPlayer->ChangeConsortiaPlacard(pTEXTMsg->NewPlacard);
	else
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_CONSORTIA_TEXT");

	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_ConsortiaDuty(const void* pBuf, CPlayer* pPlayer)
{
	MSG_CONSORTIA_DUTY* pDutyMsg = (MSG_CONSORTIA_DUTY*)pBuf;
	CPlayer* Player = CPlayer::GetPlayerFromRoleName(pDutyMsg->OldName);
	if (NULL != Player)
		Player->AppointDuty(pDutyMsg->NewName, (CONSORTIA_JOB_TYPE)pDutyMsg->NewDuty);
	else
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_CONSORTIA_DUTY");
	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_ConsortiaMemDel(const void* pBuf, CPlayer* pPlayer)
{
	MSG_CONSORTIA_MEMBER_DEL* pMemberDelMsg = (MSG_CONSORTIA_MEMBER_DEL*)pBuf;
	if (NULL != pPlayer)
		return pPlayer->DelMemeber(pMemberDelMsg->strName);
	else
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_CONSORTIA_MEMBER_DEL");
	return NO_MSG_ERRO;
}
eError	ServerSocketEventHandle::Handle_UnknowMsg(const void* pBuf, CPlayer* pPlayer)
{
	sbase::MsgHead* pHead = (sbase::MsgHead*)pBuf;
	pPlayer->AddDanger();
	sbase::LogException("Message type Exception ,Accounts : %s, Role: %s, [%d,%d]", pPlayer->GetAccounts(), pPlayer->GetName(), pHead->usType, pHead->usSize);
	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_ConsortiaLevelUp(const void* pBuf, CPlayer* pPlayer)
{
	MSG_CONSORTIA_LVUP* pConsortiaMsg = (MSG_CONSORTIA_LVUP*)pBuf;
	if (NULL != pPlayer)
	{
		return pPlayer->ConsortiaLevelup();
	}

	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_CreateAltar(const void* pBuf, CPlayer* pPlayer)
{
	MSG_ALTAR_CREATE* pAltarMsg = (MSG_ALTAR_CREATE*)pBuf;
	if (NULL != pPlayer)
	{
		pPlayer->CreateAltar(pAltarMsg->iAltarID);
	}

	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_AltarLevelUp(const void* pBuf, CPlayer* pPlayer)
{
	MSG_ALTAR_LVUP* AltarMsg = (MSG_ALTAR_LVUP*)pBuf;
	if (NULL != pPlayer)
	{
		if (AltarMsg->iAltarID < 1 || AltarMsg->iAltarID > 12)
		{
			return NO_MSG_ERRO;
		}
		pPlayer->AltarLevelUp(AltarMsg->iAltarID);

	}

	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_AltarOblation(const void* pBuf, CPlayer* pPlayer)
{
	MSG_ALTAR_OBLATION* pOblationMsg = (MSG_ALTAR_OBLATION*)pBuf;
	if (NULL != pPlayer)
	{
		if (pOblationMsg->iAltarID < 1 || pOblationMsg->iAltarID > 12)
		{
			return NO_MSG_ERRO;
		}
		pPlayer->OblationLevelUp(pOblationMsg->iAltarID, eALTAR_LEVEL_UP);

	}

	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_CretitExchange(const void* pBuf, CPlayer* pPlayer)
{
	MSG_CRETIT_EXCHANGE* pExchangeMsg = (MSG_CRETIT_EXCHANGE*)pBuf;
	if (NULL != pPlayer)
	{
		pPlayer->SetlPrestige(pPlayer->GetlPrestige() - 10);
		pPlayer->SynGameData(true);

	}

	return NO_MSG_ERRO;
}

eError ServerSocketEventHandle::Handle_MallSell(const void* pBuf, CPlayer* pPlayer)
{
	eError ErrorId = NO_MSG_ERRO;

	MSG_MALL_SELL* pMsg = (MSG_MALL_SELL*)pBuf;

	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_MALLSELL");
		return NO_MSG_ERRO;
	}
	if (pPlayer->GetlFaction() == 1)
	{
		ErrorId = CMall::Instance()->AthensMallSell(pPlayer, pMsg->Index, pMsg->Num, pMsg->Count, pMsg->Base);
	}
	else
	{
		ErrorId = CMall::Instance()->SpartaMallSell(pPlayer, pMsg->Index, pMsg->Num, pMsg->Count, pMsg->Base);
	}


	return ErrorId;
}
eError ServerSocketEventHandle::Handle_AssociationSell(const void* pBuf, CPlayer* pPlayer)
{
	eError ErrorId = NO_MSG_ERRO;

	MSG_MALL_SELL* pMsg = (MSG_MALL_SELL*)pBuf;

	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_MALLSELL");
		return NO_MSG_ERRO;
	}

	if (pPlayer->GetlFaction() == 1)
	{
		ErrorId = CMall::Instance()->AthensAssociatonSell(pPlayer, pMsg->Index, pMsg->Num, pMsg->Count, pMsg->Base);
	}
	else
	{
		ErrorId = CMall::Instance()->SpartaAssociatonSell(pPlayer, pMsg->Index, pMsg->Num, pMsg->Count, pMsg->Base);
	}


	return ErrorId;
}

eError ServerSocketEventHandle::Handle_MallItemData(const void* pBuf, CPlayer* pPlayer)
{
	eError ErrorId = NO_MSG_ERRO;

	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_MALLITEMDATA");
		return NO_MSG_ERRO;
	}
	if (pPlayer->GetlFaction() == 1)
	{
		ErrorId = CMall::Instance()->GetAthensMallData(pPlayer);
	}
	else
	{
		ErrorId = CMall::Instance()->GetSpartaMallData(pPlayer);
	}


	return ErrorId;
}

eError ServerSocketEventHandle::Handle_AssociationItemData(const void* pBuf, CPlayer* pPlayer)
{
	eError ErrorId = NO_MSG_ERRO;

	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_ASSOCIATIONITEMDATA");
		return NO_MSG_ERRO;
	}

	if (pPlayer->GetlFaction() == 1)
	{
		ErrorId = CMall::Instance()->GetAthensAssociationData(pPlayer);
	}
	else
	{
		ErrorId = CMall::Instance()->GetSpartaAssociationData(pPlayer);
	}

	return ErrorId;
}
eError ServerSocketEventHandle::Handle_OpenGoldBox(const void* pBuf, CPlayer* pPlayer)
{
	int ErrorId = -1;

	MSG_GOLD_BOX* pOpenGoldBox = (MSG_GOLD_BOX*)pBuf;


	if (!pPlayer)
	{
		sbase::ConsoleWriteColorText(FOREGROUND_RED, "_MSG_GOLD_BOX");
		return NO_MSG_ERRO;
	}

	CDial::Instance()->SetPlayer(pPlayer);

	if (CDial::Instance()->isMsgWrong(pOpenGoldBox))
	{
		return NO_MSG_ERRO;
	}

	ErrorId = CDial::Instance()->isGoldBoxIDExist(pOpenGoldBox->Iter, pOpenGoldBox->GoldBoxId);

	if (!ErrorId)
	{
		switch (pOpenGoldBox->Type)
		{
		case 8:
		{
			if (pPlayer->GetGoldBoxId())
			{
				return MSG_ERRO_041C;
			}
			UINT key_id = CDial::Instance()->m_DefaultItemDataMap[pOpenGoldBox->GoldBoxId].DialID;

			int cur_key = pPlayer->CanOpenbox(pOpenGoldBox->Iter, pOpenGoldBox->GoldBoxId, key_id);
			if (cur_key == 255)
			{
				return MSG_ERRO_041D;
			}

			CDial::Instance()->RandomItemInDial(pOpenGoldBox->GoldBoxId);

			int ItemNum = CDial::Instance()->RandomItem(pOpenGoldBox->GoldBoxId);
			if (ItemNum != -1)
			{
				if (pPlayer->ExpendGoldBoxIter(pOpenGoldBox->Iter, cur_key, pOpenGoldBox->GoldBoxId, key_id))
				{
					msg_gold_box_return.Head.usSize = sizeof(msg_gold_box_return);
					msg_gold_box_return.Head.usType = _MSG_GOLD_BOX_RETURN;
					msg_gold_box_return.ucInfo = 8;
					msg_gold_box_return.DialItem[0] = ItemNum;
					pPlayer->s_World->SendMsgToClient(&msg_gold_box_return, pPlayer->GetSocket());
				}
			}
		}
		break;
		case 0:
		{
			break;
			if (pPlayer->GetGoldBoxId())
			{
				return MSG_ERRO_041C;
			}

			UINT key_id = CDial::Instance()->m_DefaultItemDataMap[pOpenGoldBox->GoldBoxId].DialID;

			int cur_key = pPlayer->CanOpenbox(pOpenGoldBox->Iter, pOpenGoldBox->GoldBoxId, key_id);
			if (cur_key == -1)
			{
				return MSG_ERRO_041D;
			}
			if (!(pPlayer->ExpendGoldBoxKey(cur_key)))
			{
				return MSG_ERRO_041D;
			}


			pPlayer->SetCurrentIter(pOpenGoldBox->Iter);
			pPlayer->SetGoldBoxId(pOpenGoldBox->GoldBoxId);

			msg_gold_box_return.Head.usSize = sizeof(msg_gold_box_return);
			msg_gold_box_return.Head.usType = _MSG_GOLD_BOX_RETURN;
			msg_gold_box_return.ucInfo = 1;


			CDial::Instance()->RandomItemInDial(pOpenGoldBox->GoldBoxId);

			msg_gold_box_return.DialItem[DIALITEMCOUNT] = CDial::Instance()->GetDefaultItemData(pOpenGoldBox->GoldBoxId).ItemID;
			memcpy(msg_gold_box_return.DialItem, CDial::Instance()->GetInDialItemID(), sizeof(UINT) * DIALITEMCOUNT);

			pPlayer->s_World->SendMsgToClient(&msg_gold_box_return, pPlayer->GetSocket());
		}
		break;
		case 1:
		{
			break;
			if (pPlayer->GetGoldBoxId() != pOpenGoldBox->GoldBoxId)
			{
				return MSG_ERRO_041F;
				break;
			}

			UINT key_id = CDial::Instance()->m_DefaultItemDataMap[pOpenGoldBox->GoldBoxId].DialID;

			int ItemNum = CDial::Instance()->RandomItem(pOpenGoldBox->GoldBoxId);
			if (ItemNum != -1)
			{
				msg_gold_box_return.Head.usSize = sizeof(msg_gold_box_return);
				msg_gold_box_return.Head.usType = _MSG_GOLD_BOX_RETURN;
				msg_gold_box_return.ucInfo = 3;
				msg_gold_box_return.DialItem[0] = ItemNum;
				pPlayer->s_World->SendMsgToClient(&msg_gold_box_return, pPlayer->GetSocket());

				if (pPlayer->IsDoubleItem())
				{
					msg_gold_box_return.Head.usSize = sizeof(msg_gold_box_return);
					msg_gold_box_return.Head.usType = _MSG_GOLD_BOX_RETURN;
					msg_gold_box_return.ucInfo = 4;
					msg_gold_box_return.DialItem[0] = CDial::Instance()->SecondItem(pOpenGoldBox->GoldBoxId);
					pPlayer->s_World->SendMsgToClient(&msg_gold_box_return, pPlayer->GetSocket());
				}
			}

		}
		break;
		case 2:
		{
			break;
			if (!pPlayer->GetGoldBoxId())
			{
				return MSG_ERRO_0420;
				break;
			}

			UINT key_id = CDial::Instance()->m_DefaultItemDataMap[pOpenGoldBox->GoldBoxId].DialID;

			int ItemNum = CDial::Instance()->DefaultItem(pOpenGoldBox->GoldBoxId);
			if (ItemNum)
			{
				pPlayer->SetGoldBoxId(0);
				msg_gold_box_return.Head.usSize = sizeof(msg_gold_box_return);
				msg_gold_box_return.Head.usType = _MSG_GOLD_BOX_RETURN;
				msg_gold_box_return.ucInfo = 5;
				msg_gold_box_return.DialItem[0] = ItemNum;
				pPlayer->s_World->SendMsgToClient(&msg_gold_box_return, pPlayer->GetSocket());

				if (pPlayer->IsDoubleItem())
				{
					msg_gold_box_return.Head.usSize = sizeof(msg_gold_box_return);
					msg_gold_box_return.Head.usType = _MSG_GOLD_BOX_RETURN;
					msg_gold_box_return.ucInfo = 4;
					msg_gold_box_return.DialItem[0] = CDial::Instance()->SecondItem(pOpenGoldBox->GoldBoxId);
					pPlayer->s_World->SendMsgToClient(&msg_gold_box_return, pPlayer->GetSocket());
				}
			}
		}
		break;
		default:
			break;
		}
	}
	return NO_MSG_ERRO;
}

eError  ServerSocketEventHandle::Handle_SysFunNPC(const void* pBuf, CPlayer* pPlayer)
{
	MSG_SYS_NPC_DATA* NPCList = (MSG_SYS_NPC_DATA*)pBuf;

	CNPC* pNpc = pPlayer->s_World->GetNPC(NPCList->NPCID);

	pNpc->SysList(pPlayer, NPCList->NpcType, NPCList->State);
	return NO_MSG_ERRO;
}

eError	ServerSocketEventHandle::Handle_GetTargetInfo(const void* pBuf, CPlayer* pPlayer)
{
	MSG_TARGETINFO* pMsg = (MSG_TARGETINFO*)pBuf;
	char a = 0;
	CPlayer* pAimPlayer = pPlayer->s_World->GetPlayerFromName(pMsg->AimName, a);
	CPlayer* pResPlayer = pPlayer;

	if (!pAimPlayer || (pResPlayer->GetID() != pMsg->resPlayerID))
	{
		return	MSG_ERRO_005C;
	}
	pAimPlayer->RequestGameData(pResPlayer);

	return	NO_MSG_ERRO;
}
eError ServerSocketEventHandle::Handle_DelayExit(const void* pPortMsg, CPlayer* pPlayer)
{
	MSG_DELAY_EXIT* pMsg = (MSG_DELAY_EXIT*)pPortMsg;
	if (pPlayer != NULL)
	{
		if (pMsg->type == 0)
		{
			pPlayer->ExitGame_TimeStart();
		}
		else
		{
			pPlayer->ExitGame_TimeEnd();
		}
	}
	return NO_MSG_ERRO;
}

ClientSocketEventHandle::ClientSocketEventHandle(GSMaster* pMaster) :m_pMaster(pMaster)
{
	USHORT Arrysize = 0;
	const CSockHandle* Talbe = GetSockHandleTab(Arrysize);

	for (int a = 0; a < MAX_CLIENT_MSG; a++)
	{
		if (a < Arrysize)
			MsgRouter[Talbe[a].MSG_ID] = a;
	}
}

ClientSocketEventHandle::~ClientSocketEventHandle()
{

}

void ClientSocketEventHandle::ResloveGamePacket(const void* pPacket)
{
	USHORT temp = 0;
	static const CSockHandle* NetEventHandle = GetSockHandleTab(temp);

	sbase::MsgHead* pHead = (sbase::MsgHead*)pPacket;

	(this->*NetEventHandle[GetIndex(pHead->usType)].pfun)(pPacket, m_pMaster);

}


const CSockHandle* ClientSocketEventHandle::GetSockHandleTab(USHORT& Size) const
{
	static const CSockHandle MsgEventHandle[] =
	{
		{ _MSG_INVALID,       &ClientSocketEventHandle::Handle_UnknowMsg      },
		{ _MSG_VALIDATE_GAMESERVER, &ClientSocketEventHandle::Handle_UnknowMsg      },
	};

	Size = sizeof(MsgEventHandle) / sizeof(CSockHandle);
	return MsgEventHandle;
}

int ClientSocketEventHandle::GetIndex(int MsgID)
{
	if (MsgID < 0 || MsgID >= MAX_CLIENT_MSG || MsgID < 300 || MsgID > 600)
	{
		return MsgRouter[0];
	}

	return MsgRouter[MsgID];
}

eError ClientSocketEventHandle::Handle_UnknowMsg(const void* pPortMsg, GSMaster* pMaster)
{
	sbase::ConsoleWriteColorText(FOREGROUND_RED, "UnknowMsg");
	pPortMsg = NULL;
	pMaster = NULL;
	return NO_MSG_ERRO;
}

eError ClientSocketEventHandle::Handle_CheckWords(const void* pPortMsg, GSMaster* pMaster)
{
	return NO_MSG_ERRO;
}

