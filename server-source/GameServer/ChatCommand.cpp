#include "stdafx.h"
#include "ChatCommand.h"
#include "GameCore/Player.h"
#include "ServiceLoader.h"
#include "DataCore.h" 
#include <direct.h>
#include "World.h"

ChatHandler* ChatHandler::m_This = NULL;


ChatHandler::ChatHandler()
{
}

ChatHandler::~ChatHandler()
{

}

ChatHandler* ChatHandler::Instance()
{
	if (!m_This)
		m_This = new ChatHandler();

	return m_This;
}

void ChatHandler::Release()
{
	delete this;

	m_This = NULL;
}

void ChatHandler::Init()
{
	ChatCommand* pcc = GetCommandTable();

	while (pcc)
	{
		if (pcc->Name)
			m_CommandsMap[pcc->Name] = pcc;
		else
			return;

		pcc++;
	}
}

ChatCommand* ChatHandler::GetCommandTable()
{
	static ChatCommand commandTable[] =
	{
		{"addmoney" ,		OBJECTTYPE_PLAYER ,	&ChatHandler::AddMoney ,	NULL , NULL},
		{"move" ,			OBJECTTYPE_PLAYER ,	&ChatHandler::Move ,		NULL , NULL},
		{ "fly" ,			OBJECTTYPE_PLAYER ,	&ChatHandler::Fly ,			NULL , NULL},
		{ "levelup" ,		OBJECTTYPE_PLAYER ,	&ChatHandler::LevelUp ,		NULL , NULL},
		{ "kick" ,			OBJECTTYPE_PLAYER ,	&ChatHandler::Kick ,		NULL , NULL},
 		{ "ban",            OBJECTTYPE_PLAYER,  &ChatHandler::Ban ,	     	NULL , NULL},
 		{ "ping",           OBJECTTYPE_PLAYER,  &ChatHandler::Ping ,	    NULL , NULL},
 		{ "placard",        OBJECTTYPE_PLAYER,  &ChatHandler::Placard,    	NULL , NULL},
 		{ "online",         OBJECTTYPE_PLAYER,  &ChatHandler::Online ,    	NULL , NULL},
		{ NULL ,			OBJECTTYPE_PLAYER ,	NULL ,						NULL , NULL}
	};

	return commandTable;
}

bool ChatHandler::ParseCommands(CPlayer* player , const char* args)
{
	while (*args == ' ') args++;

	while (*args)
	{
		if (*args == '/')
		{
			args++;

			ExecuteCommandInTable(player , (char*)args);

			return true;
		}
		else
			args++;
	}

	return false;
}


bool ChatHandler::ExecuteCommandInTable(CPlayer* player , char* text)
{
	while (*text == ' ') text++;

	int pos = 0;

	while (text[pos] != ' ' && text[pos] != 0) pos++;

	if(pos == 0)
		return false;

	char text_bak = text[pos];
	text[pos] = 0;

	if (m_CommandsMap.find(text) != m_CommandsMap.end() )
	{
		if (player->GetType() < m_CommandsMap[text]->Type)
			return false;

		string command = text;
		text[pos] = text_bak;

		if ( m_CommandsMap[command]->Handler && player->IsGM() ) 
		{
			(this->*(m_CommandsMap[command]->Handler))(player , text + pos);
		}
	}

	return false;
}

bool ChatHandler::AddMoney(CPlayer* player , const char* args)
{
	player->AddMoney(atoi(args));

	return true;
}

bool ChatHandler::Move(CPlayer* player , const char* args)
{
	string s = args;

	int n = (int)s.find(",");

	if (n == -1)
		return false;

	s[n] = '\0';

	int x = atoi(s.c_str());
	s.erase(s.begin() , s.begin() + n + 1);
	int z = atoi(s.c_str());

	//player->SetPos((float)x , 0 , (float)z );
	player->Fly(player->GetRegion()->GetID(),(float)x , 0 , (float)z);

	return true;
}

bool ChatHandler::Fly(CPlayer* player , const char* args)
{
	int id = atoi(args);
    float x =0.0f,z=0.0f;

	//临时演示用
    if ( id == 8 )
    {
         x = 17.0f;
		 z = 130.0f;
    }
	else if ( id == 17 )
	{
		x = 77.0f;
		z = -3.0f;
	}
	else if ( id == 21 )
	{
		x = 76.0f;
		z = -3.0f;
	}

	player->Fly(id,x,0.0f,z);

	return true;
}

bool ChatHandler::LevelUp(CPlayer* player , const char* args)
{
	int l = atoi(args);
	
	if ( l <= 0 )
       return false;

	if( ( player->GetcRank() + l ) > PLAYER_MAXLV )
		return false;

	player->SetcRank( (BYTE)(player->GetcRank() + l) );
	

	player->PlayerUpGrade(true);

	return true;
}

bool ChatHandler::Kick(CPlayer* player , const char* args)
{
	string s = args;

	int n = (int)s.find(' ');
	if (n != -1)
		s.erase(s.begin() + n);

	char c;
	CPlayer* bad = player->s_World->GetPlayerFromName(s , c);

	if (!bad)
		return false;

	if ( bad->GetlFaction() == 1 )  //athen
	{
		bad->SetlRegionID(  bad->GetRegion()->GetAthensRevival()->MapID );
		bad->SetfX( bad->GetRegion()->GetAthensRevival()->X );
		bad->SetfZ( bad->GetRegion()->GetAthensRevival()->Z );
	}
	else if ( bad->GetlFaction() == 0 ) //sparta
	{
		bad->SetlRegionID( bad->GetRegion()->GetSpartaRevival()->MapID );
		bad->SetfX( bad->GetRegion()->GetSpartaRevival()->X );
		bad->SetfZ( bad->GetRegion()->GetSpartaRevival()->Z );
	}


	player->s_World->KickPlayer(bad);

	return true;
}

bool ChatHandler::Ban(CPlayer* player , const char* args)
{
// 	string s = args;
// 
// 	int n = (int)s.find(' ');
// 	if (n != -1)
// 		s.erase(s.begin() + n);
// 
// 	char c;
// 	CPlayer* BanPlayer = GetPlayerFromName(s , c);
// 	if (!BanPlayer)
// 		return false;
// 
//    MSG_PLAYER_BAN  msg_ban;
//    msg_ban.Head.usSize  = sizeof( MSG_PLAYER_BAN );
//    msg_ban.Head.usType  = _MSG_BAN_PLAYER;
//    if ( 1)
//    {
// 	    msg_ban.Style = 0; //IP
// 	    memcpy( msg_ban.Str, BanPlayer->GetIP(), 32 );
//    }
//    else 
//    {
// 	    msg_ban.Style = 1; //accounts
//         memcpy( msg_ban.Str, BanPlayer->GetAccounts(), 32 );
//    }
//    memcpy( msg_ban.GM, player->GetName(), 32 );
//    memcpy( msg_ban.Reason, "");
// 
//    g_pLoader->SendMsgToServer( &msg_ban, sizeof(MSG_PLAYER_BAN) );
   return true;
}

bool ChatHandler::Ping(CPlayer* player , const char* args)
{
	const char *RoleName;
	int x=0;
	while(args[x]==' ')
		x++;
	RoleName = &args[x];
	if(!strlen(RoleName))
	{
		return false;
	}

	char buf[128];
	CPlayer *pPlayer = CPlayer::GetPlayerFromRoleName( RoleName );
	if ( NULL== pPlayer )
		return false;

	char pro[16],camp[16],GM[8];
	memset( pro, 0L, sizeof(pro));
	memset( camp, 0L, sizeof(camp));
	memset( GM, 0L, sizeof(GM));

	sprintf( pro, "%d",pPlayer->GetlClass());//@@@ 职业名字

	if ( pPlayer->GetlFaction( ) == 0 )
	{
		sprintf( camp, "斯巴达");
	}
	else
	{
		sprintf( camp, "雅典");
	}

	if ( pPlayer->IsGM() )
	{
		sprintf( GM, "是");
	}
	else
	{
		sprintf( GM, "否");
	}
	char MapName[64];
	WCHAR	wcBuf[256];
	MSG_TALK	msg_talk;
	wstring _talk ;

	strcpy( MapName, pPlayer->s_World->GetRegionFromLogicID( pPlayer->GetlRegionID() )->GetName().c_str() );
	sprintf( buf,"账号:%s|IP:%s|角色:%s|地图:%s|GM:%s|X:%3.0f|Z:%3.0f|在线时长:%d",pPlayer->GetAccounts(),pPlayer->GetIP(), pPlayer->GetName(),
		      MapName, GM,pPlayer->GetfX(), pPlayer->GetfZ(), pPlayer->GetOnlineTime() );

	wmemset( wcBuf, 0, 256 );
	MultiByteToWideChar(CP_ACP, 0, buf, -1, wcBuf, 256 );
	_talk.clear();
	_talk = wcBuf;
	msg_talk.ucAimPlayerNameSize = 0;
	msg_talk.ucResPlayerNameSize = 0;
	msg_talk.ucTextSize = (INT)((_talk.size() + 1 )*2);
	msg_talk.ucType = defSYSTEM;
	msg_talk.uiUserID = -1;
	wmemset(msg_talk.wcContent, 0, 256);
	wcscpy(msg_talk.wcContent, _talk.c_str());

	msg_talk.Head.usType = _MSG_TALK;
	/*msg_talk.Head.usSize = (INT)(sizeof(MSG_TALK) - 512 + msg_talk.ucTextSize);*/
	msg_talk.Head.usSize = sizeof(msg_talk.Head) + 4 + 4 + 4;//Head + INT + INT + BYTE*4
	if ( (msg_talk.ucTextSize%4) != 0 )//不能被四整除
		msg_talk.Head.usSize += msg_talk.ucTextSize + 2;
	else
		msg_talk.Head.usSize += msg_talk.ucTextSize;

	player->s_World->SendMsgToClient( &msg_talk, player->GetSocket() ); 

	///
	char buf2[128];
	memset(buf2, 0, 128);
	sprintf( buf2,"职业:%s|阵营:%s|等级:%d|金钱:%d|注册时间:%s",pro, camp, pPlayer->GetcRank(), 
		pPlayer->GetMoney(), pPlayer->GetRegisterTime() );

	wmemset( wcBuf, 0, 256 );
	MultiByteToWideChar(CP_ACP, 0, buf2, -1, wcBuf, 256 );
	_talk.clear();
	_talk = wcBuf;
	msg_talk.ucAimPlayerNameSize = 0;
	msg_talk.ucResPlayerNameSize = 0;
	msg_talk.ucTextSize = (INT)((_talk.size() + 1 )*2);
	msg_talk.ucType = defSYSTEM;
	msg_talk.uiUserID = -1;
	wmemset(msg_talk.wcContent, 0, 256);
	wcscpy(msg_talk.wcContent, _talk.c_str());

	msg_talk.Head.usType = _MSG_TALK;
	/*msg_talk.Head.usSize = (INT)(sizeof(MSG_TALK) - 512 + msg_talk.ucTextSize);*/
	msg_talk.Head.usSize = sizeof(msg_talk.Head) + 4 + 4 + 4;//Head + INT + INT + BYTE*4
	if ( (msg_talk.ucTextSize%4) != 0 )//不能被四整除
		msg_talk.Head.usSize += msg_talk.ucTextSize + 2;
	else
		msg_talk.Head.usSize += msg_talk.ucTextSize;

	player->s_World->SendMsgToClient( &msg_talk, player->GetSocket() ); 

    return true;
}

bool ChatHandler::Placard(CPlayer* player , const char* args)
{
   return true;
}

bool ChatHandler::Online(CPlayer* player , const char* args)
{
	player->s_World->GetNowPlayer();
   char Online[64];
   sprintf( Online,"目前在线人数:%d!", player->s_World->GetNowPlayer() );
   WCHAR	wcBuf[64];
   wmemset( wcBuf, 0, 64 );
   MultiByteToWideChar(CP_ACP, 0, Online, -1, wcBuf, 64 );

   MSG_TALK	msg_talk;
   wstring _talk = wcBuf;
   msg_talk.ucAimPlayerNameSize = 0;
   msg_talk.ucResPlayerNameSize = 0;
   msg_talk.ucTextSize = (INT)((_talk.size() + 1 )*2);
   msg_talk.ucType = defSYSTEM;
   msg_talk.uiUserID = -1;
   wmemset(msg_talk.wcContent, 0, 256);
   wcscpy(msg_talk.wcContent, _talk.c_str());

   msg_talk.Head.usType = _MSG_TALK;
   /*msg_talk.Head.usSize = (INT)(sizeof(MSG_TALK) - 512 + msg_talk.ucTextSize);*/
   msg_talk.Head.usSize = sizeof(msg_talk.Head) + 4 + 4 + 4;//Head + INT + INT + BYTE*4
   if ( (msg_talk.ucTextSize%4) != 0 )//不能被四整除
	   msg_talk.Head.usSize += msg_talk.ucTextSize + 2;
   else
	   msg_talk.Head.usSize += msg_talk.ucTextSize;

   player->s_World->SendMsgToClient( &msg_talk, player->GetSocket() ); 

   return true;
}

