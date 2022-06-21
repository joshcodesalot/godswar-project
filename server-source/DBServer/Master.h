#include "stdafx.h"
#include "Service.h"

class  CMaster
{
	enum eServieType
	{
		SERVICE_DB,
		//SERVICE_PIPE,
		SERVICE_BACKUP,
		SERVICE_COMAND,
		SERVICE_COUNT,
	};
	friend class CommandService;
public:
	static volatile bool m_stopEvent;
	CMaster();
	~CMaster();

	bool Init();
	void Run();
	void Release();
private:
	void UpdateTime();
	void ShutDownServ( time_t time );
private:
	sbase::CTimer m_ShutdownTimer;
	IService* m_Service[SERVICE_COUNT];
};

