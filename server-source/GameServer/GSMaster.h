//========================================================
//
//    Copyright (c) 2006,欢乐连线工作室
//    All rights reserved.
//
//    文件名称 ： GameServer.h
//    摘    要 ： 游戏服务器基础框架
//    
//    原始版本 ： 1.00
//    作    者 ： 林德辉
//    完成日期 ： 2007-01-08
//
//    当前版本 ： 1.01
//    修    改 ： 李锋军
//    完成日期 ： 2007-05-11
//    备    注 ： 1、增加DB数据的回调函数
//
//========================================================

#pragma once

#include "../Common/Include/Base/Inc/Thread.h"
#include "./ServiceLoader.h"

class CPlayer;
class CWorld;

typedef int(* pFunPrintf)(const char*,...);
#define  REGISTER_MACRO(x,y)  pPlayer->x = y;
#define MAX_COMMAND_TOTAL		13


void	 __cdecl 	OnDealWithAttribute( CPlayer *pPlayer,void *Loader );
void	 __cdecl 	OnDealWithPassiveSkill( CPlayer *pPlayer );
void	 __cdecl 	OnDealWithActiveSkill( CPlayer *pPlayer );
void	 __cdecl 	OnDealWithKitBag( CPlayer *pPlayer );

extern char			strKeyBuffer[256];
extern long			lKeyCount;


class  GSMaster : private sbase::IThreadEvent 
{
	enum{ 
		   SERVICE_DB,
		   //SERVICE_PIPE,
		   SRVICE_COMMAND,
		   SERVICE_REGISTER,
		   SERVICE_QUEUE,
		   SERVICE_BACKUP,
		   SERVICE_COUNT 
	    };
public:
	GSMaster();
	~GSMaster();
	bool Init();
	void Run();
	void Release();
	IService* GetDBService(){return m_Service[SERVICE_DB];}

	dbtool::IDatabase* GetDatabase(){return (dbtool::IDatabase*)(m_Service[SERVICE_DB]->GetValue());}
protected:
	virtual int		OnThreadCreate	(void){ return 0; }		
	virtual int		OnThreadDestroy	(void){ return 0; }	
	virtual int		OnThreadEvent	(void){ return 0; }          
	virtual int		OnThreadProcess	(void);
private:

	static DWORD WINAPI BackUpThreadRun(LPVOID lpParam); 

public:
	void			GOutPut();
	void			GOutPut( int a );

	BYTE			GetTotalPlayerNum();
	void			CommandRelease( void );

private:
	sbase::CThread*		m_pThread;
	CWorld*				m_pWorld;
//	ClientSocketEventHandle*	m_pSocketHandle;
	ServerSocketEventHandle*	m_pServerSocketEventHandle;
	IService*			m_Service[SERVICE_COUNT];
	cnet::CIOCP*		m_pNetClient;

};





