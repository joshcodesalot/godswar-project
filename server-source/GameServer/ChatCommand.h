#ifndef _CHATCOMMAND_H_
#define _CHATCOMMAND_H_

#include "GameCore/GameData.h"

class ChatHandler;
class CPlayer;


class ChatCommand
{
public:
	const char*		Name;	//命令
	eObjectType		Type;	//权限
	bool (ChatHandler::*Handler)(CPlayer* player , const char* args);	//回调
	const char*		Help;
	ChatCommand*	ChildCommands;
};

class ChatHandler
{	
public:

	void Init();

	static ChatHandler* Instance();
	
	void Release();

	bool ParseCommands(CPlayer* player , const char* args);

private:

	ChatHandler();
	~ChatHandler();

	static ChatHandler* m_This;

	bool AddMoney(CPlayer* player , const char* args);
	bool Move(CPlayer* player , const char* args);
	bool Fly(CPlayer* player , const char* args);
	bool LevelUp(CPlayer* player , const char* args);
	bool Kick(CPlayer* player , const char* args);
	bool Ban(CPlayer* player , const char* args);    
	bool Ping(CPlayer* player , const char* args);
	bool Placard(CPlayer* player , const char* args);
	bool Online(CPlayer* player , const char* args);

	ChatCommand* GetCommandTable();

	bool ExecuteCommandInTable(CPlayer* player , char* text);
	
	std::map<string , ChatCommand*> m_CommandsMap;
};

#endif



