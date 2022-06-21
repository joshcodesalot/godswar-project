// **********************************************************************
//
// Copyright (c) 2004
// All Rights Reserved
//
// Author: zeng cs
// Created:	2004/11/03   0:25
//
// **********************************************************************

#pragma once


#include <windows.h>
#include <string>

namespace sbase
{
	class CHeap
	{
	public:
		CHeap(const char* pszClassName = NULL);
		~CHeap();
		
	public:
		void*		Alloc(size_t size);
		void		Free(void* ptr);
		bool		IsValidPt(void* p);
	protected:
		HANDLE		m_hHeap;
		LONG		m_lNumAlloc;
		LONG		m_lMaxAlloc;
		std::string	m_strName;
	};

}	// end of namespace sbase


	
// Marco define	
#define	MYHEAP_DECLARATION(VAR)											\
		void*  operator new	(size_t size)	{ return VAR.Alloc(size); }	\
		void   operator delete	(void* p)	{ VAR.Free(p); }			\
		void*  operator new[] (size_t size)	{ return VAR.Alloc(size); }	\
		void   operator delete[] (void* p)	{ VAR.Free(p); }			\
	public:																\
		static bool	IsValidPt(void* p)		{ return VAR.IsValidPt(p); }\
	protected:															\
		static sbase::CHeap	VAR;
	
	
#define	MYHEAP_IMPLEMENTATION(T,VAR)									\
	sbase::CHeap	T::VAR(#T);								
	
