#include "../Inc/stdafx.h"
#include "../Inc/Command.h"

#include "../Inc/LoginLogic.h"


#define  AllCmds2 sizeof(Commands)/sizeof(KeyCommand)

namespace loginserver
{

	char strKeyBuffer[256];
	long lKeyCount = 0;

	void  CommandService::Key_Help(char*, pFunPrintf pPrintf)
	{
		printf("\n");
		for (int a = 0; a < AllCmds; a++)
		{
			printf("%-16s  %-20s%-25s\n", Commands[a].cmd, "-", Commands[a].Note);
		}
		pPrintf("\nLoginServer> ");
	}

	void  CommandService::Key_Exit(char*, pFunPrintf pPrintf)
	{
		pPrintf("Exiting daemon...\n");
		exit(0);
	}

	void  CommandService::Key_Quit(char*, pFunPrintf pPrintf)
	{
		pPrintf("Exiting daemon...\n");
		exit(0);
	}

	void  CommandService::Key_List(char*, pFunPrintf pPrintf)
	{
		m_pLoginSrv->ShowServerList();
		pPrintf("\nLoginServer> ");
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
				printf("   ...[Cancel]\nCommand: ");
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

	long CommandService::ParseCommand(char* Argcommand)
	{
		unsigned int x;
		size_t l = strlen(Argcommand);
		if (l < 2)
		{
			return 0;
		}

		for (x = 0; x < AllCmds; x++)
		{
			if (!memcmp(Commands[x].cmd, Argcommand, Commands[x].cmdLen))
			{
				(this->*Commands[x].pfun)(&Argcommand[Commands[x].cmdLen], &::printf);
				break;
			}
		}

		if (x == AllCmds)
		{
			sbase::ConsoleWriteColorText(FOREGROUND_RED, "		...Unknown command!\n ");
		}

		return 1;
	}

	bool CommandService::Run()
	{
		if (!IsOpen())
			return false;

		if (m_Timer.IsExpire())
		{
			if (Command() == -1)
				return false;

			m_Timer.Update();
		}
		return true;

	}

	CommandService::CommandService(login::CLogin* pLoginSrv)
	{
		m_pLoginSrv = pLoginSrv;
	}

	bool CommandService::Init()
	{
		m_Timer.Startup(COMMAND_TIMER);
		Start();

		printf("LoginServer> ");
		return true;
	}

	KeyCommand* CommandService::GetConmandFunTable()
	{
		return Commands;
	}

}