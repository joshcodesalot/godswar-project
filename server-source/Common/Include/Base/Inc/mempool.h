//========================================================
//
//    Copyright (c) 2006,欢乐连线工作室
//    All rights reserved.
//
//    文件名称 ： mempool.h
//    摘    要 ： 内存池
//    
//    当前版本 ： 1.01
//    作    者 ： 李锋军
//    完成日期 ： 2007-05-30
//
//========================================================

#ifndef MEMPOOL_H
#define MEMPOOL_H

#ifdef _WIN32
#pragma once
#endif

#include "./SvrBase.h"
#include "SyncObjs.h"


typedef void (*MemoryPoolReportFunc_t)( char const* pMsg, ... );

class CMemoryPool
{
public:
	enum
	{
		GROW_NONE=0,	
		GROW_FAST=1,
		GROW_SLOW=2	
	};

	CMemoryPool(int blockSize, int numElements, int growMode = GROW_FAST);
	~CMemoryPool();

	void*		Alloc();
	void*		Alloc( unsigned int amount );
	void		Free(void *pMem);

	void		Clear();

	static void SetErrorReportFunc( MemoryPoolReportFunc_t func );


private:
	class CBlob
	{
	public:
		CBlob	*m_pPrev, *m_pNext;
		int		m_NumBytes;		
		char	m_Data[1];
	};


	void		Init();
	void		AddNewBlob();
	void		ReportLeaks();


private:
    sbase::CCriticalSection		m_cs;
	int			m_BlockSize;
	int			m_BlocksPerBlob;

	int			m_GrowMode;

	CBlob			m_BlobHead;
	void			*m_pHeadOfFreeList;
	int				m_BlocksAllocated;
	unsigned short	m_NumBlobs;

	static MemoryPoolReportFunc_t g_ReportFunc;
};

#define DECLARE_FIXEDSIZE_ALLOCATOR( _class )									\
   public:																		\
   inline void* operator new( size_t size ) { return s_Allocator.Alloc((unsigned int)size); }   \
   inline void* operator new( size_t size, int, const char *, int ) { return s_Allocator.Alloc((unsigned int)size); }   \
   inline void  operator delete( void* p ) { s_Allocator.Free(p); }		\
   inline void  operator delete( void* p, int , const char *, int  ) { s_Allocator.Free(p); }   \
   void*  operator new[] (size_t size)	{ return s_Allocator.Alloc((unsigned int)size); }	\
   void   operator delete[] (void* p)	{ s_Allocator.Free(p); }			\
  private:																		\
  static   CMemoryPool   s_Allocator

#define DEFINE_FIXEDSIZE_ALLOCATOR( _class, _initsize, _grow )					\
	CMemoryPool   _class::s_Allocator(sizeof(_class), _initsize, _grow)



#define DECLARE_FIXEDSIZE_ALLOCATOR_EXTERNAL( _class, _allocator )				\
   public:																		\
   inline void* operator new( size_t size )  { return s_pAllocator->Alloc(size); }   \
   inline void* operator new( size_t size, int nBlockUse, const char *pFileName, int nLine )  { return s_pAllocator->Alloc(size); }   \
   inline void  operator delete( void* p )   { s_pAllocator->Free(p); }		\
   private:																		\
   static   CMemoryPool*   s_pAllocator

#define DEFINE_FIXEDSIZE_ALLOCATOR_EXTERNAL( _class, _allocator )				\
	CMemoryPool*   _class::s_pAllocator = _allocator

#endif 
