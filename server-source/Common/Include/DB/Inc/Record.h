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


#include <windows.h>
#include <vector>
#include "Recordset.h"
#include <mysql.h>
#include "IDB.h"
#include "..\..\Base\inc\Heap.h"

namespace dbtool
{
	class CField;
	class CRecord : public IRecord
	{
		friend class CRecordset;

		// Heap
	public:
		MYHEAP_DECLARATION(s_heap)

			// Constructor & Destructor
	protected:
		CRecord(CRecordset& res, UINT unAmountField);
		CRecord(CRecordset& res, MYSQL_ROW row, unsigned int& unNumFields);
		CRecord(CRecordset& res, MYSQL_FIELD* fields, unsigned int& unNumFields);
		virtual ~CRecord();
	private:
		CRecordset&		m_res;

		// Hide copy-constructor and assignment operator
		CRecord(const CRecord& rec);
		CRecord& operator=(const CRecord& rec);

		// Interface of IRecord
	public:
		virtual	unsigned long	Release	(void)	{ delete this; return 0; }
		virtual IData&		Field		(unsigned int unIndex);
		virtual IData&		Field		(unsigned int unIndex) const{ return this->Field(unIndex); }
		virtual IData&		Field		(const char* pszName);
		virtual IData&		Field		(const char* pszName) const { return this->Field(pszName); }
		virtual IData&		Key			(void);
		virtual bool		Update		(bool bSync = true)			{ return m_res.UpdateRecord(this, bSync); }
		virtual bool		Delete		(bool bArchive = false)		{ return m_res.DeleteRecord(this, bArchive); }
		virtual bool		Insert		(void)						{ return m_res.InsertRecord(this); }
		virtual void		ClsEditFlag	(void);
	private:
		std::vector<CField>		m_setFields;

		// Operation
	public:
		const char*	KeyName				(void);
		bool		BuildSQLOperation	(char* pszOperationSQL);

	private:
		void		BuildSQLCondition	(char* pszConditionSQL);
	};
}
