//========================================================
//    Copyright (c) 2006,福州天盟工作室
//    All rights reserved.
//
//    文件名称 ： Database.h
//    摘    要 ： 数据库
//    
//    替代版本 ： 1.0
//    作    者 ： zeng cs
//    完成日期 ： 2005-07-25
//
//    当前版本 ： 1.01
//    修    正 ： 李锋军
//    完成日期 ： 2007-05-27 ——
//    说    明 ： 1、添加了内存池的管理机制
 
//========================================================

#pragma once

#pragma warning(disable:4786)
#include <winsock2.h>
#include <windows.h>
#include <list>
#include <string>
#include <vector>

#include <mysql.h>

#include "../../Base/Inc/Thread.h"
#include "../../Base/inc/Timer.h"
#include "../../Base/inc/Types.h"
#include "../../Base/Inc\mempool.h"

#include "IDB.h"




namespace dbtool
{ 

	// const
	const DWORD	_MAX_DBACCESSTIME	=10;
   
	/////////////////////////////////////////////////////////////////////////////
	class CDatabase : public IDatabase, private sbase::IThreadEvent
	{
		// Constructor & Destructor
		
	public:
		CDatabase();
		virtual ~CDatabase();

		// -------------- Interface of IDB ------------------
	public:
		virtual	unsigned long	Release			(void)		{ delete this; return 0; }
		virtual IRecordset*		CreateRecordset	(const char* pszSQL, MODE eMode);
		virtual bool            Execute( const char* szSQL, MODE eMode );
		virtual IRecord*		MakeDefaultRecord(const char* szTable, unsigned long id = 0);
		virtual void			GetStatInfo		(DB_INFO& infoDB)	{ this->UpdateStatInfo(infoDB); }
		virtual size_t          GetRequestSize(){ return AsyncSQL_RequestIn->size(); };
		virtual size_t          GetResultSize(){return AsyncSQL_Result.size(); }
		

		// Operation
	public:
		bool			IsOpen			(void)				{ return m_bOpen; }
		bool			Open			(const char* szDBServer = NULL, const char* szLoginName = NULL, const char* szPassword = NULL, const char* szDBName = NULL, bool bSQLChk = true);
		void			Close			(void);
		I64				GetLastInsertedID(void);
		MYSQL*          GetDBHandle      (void){ return m_hdbc; }
	public:
		MYSQL*			Connect			(const char* szHost, const char* szUser, const char* szPasswd, const char* szDB, 
			unsigned int uiPort = MYSQL_PORT, char* szSocket = NULL, unsigned int uiFlag = NULL );

		MYSQL_RES*		ExecuteSQL		(const char* szSQL);
		bool			ExecuteSyncSQL	(const char* pszSQL);
		bool			ExecuteAsyncSQL	(const char* pszSQL, void * pUser, SQL_RESULT_CALLBACK callback );
		bool			CheckSQL		(const char* szSQL);
		void            ChangeRequestVector();
	private:				
		MYSQL*		m_hdbc;
		HANDLE		m_hMutex;
		bool 		m_bOpen;
		char		m_szDBName[256];
		char        m_szHost[256];
		char        m_szPasswd[256];
		char        m_szUser[256];
	

		clock_t		m_tmAccess;

		// -------------- Interface of IThreadEvent ------------------
	protected:
		virtual int		OnThreadCreate	(void)			{ return 0; }		
		virtual int		OnThreadDestroy	(void)			{ return 0; }	
		virtual int		OnThreadEvent	(void);
		virtual int		OnThreadProcess	(void);		
	private:
		MYSQL*					m_hdbcAsync;
		sbase::CThread*			m_pThread;
		sbase::CCriticalSection	m_xLock;
		//DB Request List
		std::list<std::string>	m_setAsyncSQL; 
 		//DB Request List
 		std::vector< SQL_REQUEST > *AsyncSQL_RequestIn;
		//DB Request List
		std::vector< SQL_REQUEST > *AsyncSQL_RequestOut;

		//DB Request List1
		std::vector< SQL_REQUEST > AsyncSQL_Request1;
		//DB Request List2
		std::vector< SQL_REQUEST > AsyncSQL_Request2;

 		//DB Result List
 	    std::vector< PSQL_RESULT > AsyncSQL_Result; 
		
		// Implementation
	public:
		void			UpdateStatInfo	(DB_INFO& infoDB);
	protected:
		void			StatisticSQL	(const char* pszSQL, DWORD dwRun);
	private:
		DB_INFO			m_infoDB;
		sbase::CTimerMS	m_tm;

		int m_request_num;
		int m_deal_num;
		int m_result_num;

		DECLARE_FIXEDSIZE_ALLOCATOR(CDatabase);
	};

  

}
