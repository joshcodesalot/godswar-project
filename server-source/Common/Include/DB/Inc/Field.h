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
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <mysql.h>
#include "IDB.h"

namespace dbtool
{
	struct FieldInfo
	{
		std::string		strName;
		unsigned int	unType;
		unsigned int	unAttr;
		unsigned int	unLen;
	};

	class CField : public IData
	{
		friend class CRecord;

		// Constructor & Destructor
	public:
		CField(const FieldInfo& info);
		CField(const CField& field);
		virtual ~CField();

		CField& operator=(const CField& field);

		// Interface
	public:
		virtual	unsigned long Release(void)		{ delete this; return 0; }

		virtual			operator bool() const;

		virtual			operator char() const;
		virtual			operator unsigned char() const;

		virtual			operator short() const;
		virtual			operator unsigned short() const;

		virtual			operator long() const;
		virtual			operator unsigned long() const;

		virtual			operator int() const;
		virtual			operator unsigned int() const;

		virtual			operator __int64() const;
		virtual			operator unsigned __int64() const;

		virtual			operator float() const;
		virtual			operator double() const;

		virtual			operator char* () const;
		virtual			operator const char* () const;

		virtual IData&	operator = (bool bOp);

		virtual IData&	operator = (char chOp);
		virtual IData&	operator = (unsigned char chOp);

		virtual IData&	operator = (short sOp);
		virtual IData&	operator = (unsigned short usOp);

		virtual IData&	operator = (long lOp);
		virtual IData&	operator = (unsigned long ulOp );

		virtual IData&	operator = (int iOp);
		virtual IData&	operator = (unsigned int uiOp);

		virtual IData&	operator = (__int64 iOp64);
		virtual IData&	operator = (unsigned __int64 ui64Op);

		virtual IData&	operator = (float fltOp);
		virtual IData&	operator = (double dblOp);

		virtual IData&	operator = (const char* szVal);

		// Attributes
	public:
		const char*		GetName		(void) const		{ return m_info.strName.c_str(); }
		unsigned int	GetType		(void) const		{ return m_info.unType; }
		unsigned int	GetAttr		(void) const		{ return m_info.unAttr; }
		unsigned int	GetLen		(void) const		{ return m_info.unLen; }

		bool			SetValue	(const char* pszValue);
		bool			IsChanged	(void) const		{ return m_bChanged; }
		void			TagChanged	(bool bChange)		{ m_bChanged = bChange; }

	private:
		const FieldInfo&	m_info;

		union
		{
			__int64		m_i64Val;
			double		m_dVal;
		};
		std::string		m_strVal;

		bool			m_bChanged;
	};
}


