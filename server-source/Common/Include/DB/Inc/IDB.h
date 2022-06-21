// ********************************************************************
//
// Copyright (c) 2004
// All Rights Reserved
//
// Author: zeng cs
// Created: 2005/07/25
//
// Mender: FenJune Li
// Changed:2007-04-10
// ********************************************************************

#pragma once
//#define DB_API  

#ifdef DB_EXPORTS
	#define DB_API extern "C" __declspec(dllexport)
#else
	#define DB_API extern "C" __declspec(dllimport)
#endif

#include "../../Base/inc/Types.h"
#include "../../Base/inc/IMessage.h"




namespace dbtool
{


	struct DB_INFO
	{
		DWORD dwTotalSQL;
		DWORD dwSQLPer5Min;
		DWORD dwSQLRunPer5Min;
		DWORD dwSQLAvgPer5Min;
		DWORD dwSQLMaxPer5Min;

		DWORD dwSelectPer5Min;
		DWORD dwSelectRunPer5Min;
		DWORD dwSelectAvgPer5Min;
		DWORD dwSelectMaxPer5Min;

		DWORD dwUpdatePer5Min;
		DWORD dwUpdateRunPer5Min;
		DWORD dwUpdateAvgPer5Min;
		DWORD dwUpdateMaxPer5Min;

		DWORD dwInsertPer5Min;
		DWORD dwInsertRunPer5Min;
		DWORD dwInsertAvgPer5Min;
		DWORD dwInsertMaxPer5Min;

		DWORD dwDeletePer5Min;
		DWORD dwDeleteRunPer5Min;
		DWORD dwDeleteAvgPer5Min;
		DWORD dwDeleteMaxPer5Min;
	};

	const int	_MAX_SQL = 4000;

    
	//////////////////////////////////////////////////////////////////////
	class IData
	{
	public:
		virtual	unsigned long	Release(void) = 0;

		// conversion function
		virtual					operator bool() const				= 0;

		virtual					operator char() const				= 0;
		virtual					operator unsigned char() const		= 0;

		virtual					operator short() const				= 0;
		virtual					operator unsigned short() const		= 0;

		virtual					operator long() const				= 0;
		virtual					operator unsigned long() const		= 0;

		virtual					operator int() const				= 0;
		virtual					operator unsigned int() const		= 0;

		virtual					operator __int64() const			= 0;
		virtual					operator unsigned __int64() const	= 0;

		virtual					operator float() const				= 0;
		virtual					operator double() const				= 0;

		virtual					operator char* () const				= 0;
		virtual					operator const char* () const		= 0;

		// overload assignment operator
		virtual IData&			operator = (bool bOp)				= 0;

		virtual IData&			operator = (char chOp)				= 0;
		virtual IData&			operator = (unsigned char chOp)		= 0;

		virtual IData&			operator = (short nOp)				= 0;
		virtual IData&			operator = (unsigned short unOp)	= 0;

		virtual IData&			operator = (long lOp)				= 0;
		virtual IData&			operator = (unsigned long lOp )		= 0;

		virtual IData&			operator = (int iOp)				= 0;
		virtual IData&			operator = (unsigned int uOp)		= 0;

		virtual IData&			operator = (__int64 uOp)			= 0;
		virtual IData&			operator = (unsigned __int64 uOp)	= 0;

		virtual IData&			operator = (float fltOp)			= 0;
		virtual IData&			operator = (double dblOp)			= 0;

		virtual IData&			operator = (const char* szVal)		= 0;
	};

	//////////////////////////////////////////////////////////////////////
	class IRecord
	{
	public:
		virtual	unsigned long	Release		(void)					= 0;

		// return field data by field index
		virtual IData&			Field		(unsigned int idx)		= 0;
		virtual IData&			Field		(unsigned int idx) const= 0;

		// return field data by field name
		virtual IData&			Field		(const char* pszName)	= 0;
		virtual IData&			Field		(const char* pszName) const= 0;

		// return the key data of this record
		virtual IData&			Key			(void)					= 0;

		// 通过 UPDATE 语句来更新 修改过 的数据到数据库，
		// 缺省使用同步方式，调用者要等待数据库更新完成后返回。
		// 也可指定异步方式，调用者不需要等待数据库更新完成，直接返回true
		virtual bool			Update		(bool bSync = true)		= 0;
		virtual bool			Delete		(bool bArchive = false)	= 0;
		virtual bool			Insert		(void)					= 0;

		// clear the edit flag of all field in this record, so update operation will ignored.
		virtual void			ClsEditFlag	(void)					= 0;
	};

	//////////////////////////////////////////////////////////////////////
	class IRecordset
	{
	public:
		virtual	unsigned long	Release		(void)					= 0;

		// get record from record set 
		virtual IRecord*		GetRecord	(void) const			= 0;

		// count record in set.	 return -1: error
		virtual unsigned long	CountRecord	(void) const			= 0;

		// move to specified record
		virtual void			Move		(unsigned int index)	= 0;

		// update this record by "update, delete or insert" sql
		// bFlag = Sync or Archive
		virtual bool			Update		(bool bFlag = true)		= 0;

		// clear the edit flag of all field in this record, so update operation will ignored.
		virtual void			ClsEditFlag	(void)					= 0;

		// make a new default record, it doesn`t exist in dbtool
		virtual IRecord*		MakeDefRecord(void)					= 0;

		virtual int					FieldsCount	(void) const        = 0;
	};
	

	//Result
	typedef struct sql_result
	{

		UINT   			Account_ID;
		UINT   			UserID;
		void   			(*callback)( sql_result*);
		void            *pPlayer;
		bool   			Falg;
		IRecordset      *pResult;  
		void            Release(){ SAFE_RELEASE( pResult);pResult = NULL;delete this;} 
		sql_result& operator = (sql_result* Result)
		{
			if ( this == Result )
			{
				return *this;
			}

			this->Account_ID =  Result->Account_ID;
			this->UserID     =  Result->UserID;
			this->pResult    =  Result->pResult;
			this->callback   =  Result->callback;
			this->Falg       =  Result->Falg;
			this->pResult    =  Result->pResult;

			return *this;
		}
	}*PSQL_RESULT, SQL_RESULT;


	typedef void (*SQL_RESULT_CALLBACK)( PSQL_RESULT );

	//////////////////////////////////////////////////////////////////////
	enum MODE { MODE_NONE = 0,	MODE_READ, 	MODE_EDIT };
	class IDatabase
	{
	public:
		virtual	unsigned long	Release			(void)				= 0;

		// create a record set by specified "select" sql
		virtual IRecordset*		CreateRecordset	(const char* szSQL, MODE eMode)	= 0;

		//Execute SP without return value
		virtual bool            Execute( const char* szSQL, MODE eMode )=0;

		// make a new default record by "insert" sql.
		virtual IRecord*		MakeDefaultRecord(const char* szTable, unsigned long id = 0)= 0;

		// get statistic info of dbtool running 
		virtual void			GetStatInfo		(DB_INFO& infoDB)	= 0;

		//get status of dbtool 
		virtual size_t          GetRequestSize() = 0;

		virtual size_t          GetResultSize() = 0;

		virtual bool            ExecuteAsyncSQL(const char* pszSQL,void *pUser, SQL_RESULT_CALLBACK callback ) =0 ;

	};

	//Request
	typedef struct sql_request
	{
		char           Desrible[4000];
	    void           *pPalyer;
		void          (*callback)( sql_result* );
	}*PSQL_REQUEST, SQL_REQUEST;



	


	//////////////////////////////////////////////////////////////////////
	// create IDatabase interface
	//////////////////////////////////////////////////////////////////////
	DB_API IDatabase*	DatabaseCreate	(const char* szDBServer, const char* szLoginName, const char* szPassword, const char* szDBName, bool bEnableSQLChk = true); 
	DB_API IDatabase&	InstanceGet		(void);
	DB_API void			Destroy			(void);
}

