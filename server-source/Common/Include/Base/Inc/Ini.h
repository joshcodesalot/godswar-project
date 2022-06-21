// **********************************************************************
//
// Copyright (c) 2004
// All Rights Reserved
//
// Author: zeng cs
// Created:	2006/01/18
//
// **********************************************************************

#pragma once

#pragma warning(disable:4786)
#include <map>

namespace sbase
{
	
	class sstring;
	class CIni  
	{
	public:
		CIni(bool bCritical);
		CIni(const char* pszFile, bool bCritical);
		virtual ~CIni();
		
		bool	SearchSection	(const char* pszSection) const;
		
		int			GetData		(const char* pszSection, const char* pszIndex) const;
		float       GetFloatData     (const char* pszSection, const char* pszIndex)const ;
	 
		const char* GetString	(const char* pszSection, const char* pszIndex) const;
		bool	Open		(const char* pszIniFile);
		
	private:
		bool	ParseSection(char* pszLine, sbase::sstring& str) const;
		bool	ParseContent(char* pszLine, sbase::sstring& strIndex, sbase::sstring& strContent) const;
		bool	IsValidLine	(const char* pszLine) const;
		
		struct SECTION
		{
			std::map<sbase::sstring, sbase::sstring> setInfo;
		};
		
		std::map<sbase::sstring, SECTION> m_setSection;
		bool	m_bCritical;
		
		std::string m_strFileName;
		
		public:
			static CIni*	CreateNew	(const char* pszIniFile, bool bCritical = false);
	};
	
}
