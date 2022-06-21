// ********************************************************************
//
// Copyright (c) 2004
// All Rights Reserved
//
// Author: zeng cs
// Created: 2005/07/25
//
// ********************************************************************

#pragma once

#pragma warning(disable:4996)

#include <vector>
#include "Database.h"
#include "Field.h"
#include <mysql.h>
#include "IDB.h"
#include "..\..\Base\inc\Heap.h"

namespace dbtool
{

	class CRecord;
	class CRecordset : public IRecordset
	{
		friend class CDatabase;
		friend class CRecord;

		// Heap
	public:	
		MYHEAP_DECLARATION(s_heap)

			// Constructor & Destructor
	protected:
		CRecordset(CDatabase& dbtool, MODE eMode);
		virtual ~CRecordset();
	private:
		CDatabase&		m_db;
		MODE			m_eMode;

		// Hide copy-constructor and assignment operator
		CRecordset(const CRecordset& res);
		CRecordset& operator=(const CRecordset& res);


		// Interface of IRecordset
	public:
		virtual unsigned long	Release		(void)			{ if (m_setRecord.empty()) delete this; return 0; }
		virtual IRecord*		GetRecord	(void) const;
		virtual unsigned long	CountRecord	(void) const	{ return m_setRecord.size(); }
		virtual void			Move		(unsigned int nIndex);
		virtual bool			Update		(bool bSync = true);
		virtual void			ClsEditFlag	(void);
		virtual IRecord*		MakeDefRecord(void);
	private:
		int 			m_nCursor;

		// Operation
	protected:
		bool				Create		(const char* pszSQL);
		bool                Create      ( MYSQL_RES* pRES );
		void				Remove		(CRecord* pRec);
		CRecord*			MakeDefaultRecord(const char* pszTable);

		int					GetTableName(char* szTableName)	{ ::strcpy(szTableName, m_szTableName);	}
		int					FieldsCount	(void) const		{ return m_setFieldInfo.size(); }
		const FieldInfo&	GetFieldInfo(unsigned int nIndex) const	{ return m_setFieldInfo[nIndex]; }

		void				BuildSQLCondition(char* pszConditionSQL);
		void				BuildSQLOperation(char* pszOperationSQL);

		bool				UpdateRecord(CRecord* pRec, bool bSync = true);
		bool				DeleteRecord(CRecord* pRec, bool bArchive = false);
		bool				InsertRecord(CRecord* pRec);
	private:
		char			m_szSQL[_MAX_SQL];
		char 			m_szTableName[32];
		unsigned int	m_unKeyIndex;
		unsigned int	m_unAutoIncIndex;


		std::vector<FieldInfo>	m_setFieldInfo;
		std::vector<CRecord*>	m_setRecord;
	};

}

