//*********************************************************************
//
// Copyright (c) 2004
// All Rights Reserved
//
// Author: zeng cs
// Created: 2005/11/18
//
//*********************************************************************

#pragma once


#include <windows.h>
#include <set>
#include <map>

// type...
typedef unsigned long		TIME;
typedef	unsigned long		U32;
typedef	long				I32;

#ifdef WIN32
	typedef __int64			I64;
#else
	typedef long long		I64;
#endif

#ifndef SOCKET
	typedef unsigned int	SOCKET;
#endif
	

// macro defines...
#undef	NULL
#define NULL    0

#undef	IN
#define IN

#undef	OUT
#define OUT

#define	SYSTEM_NAME 		"SYSTEM"			// 广播消息时代表“系统”的名字串
#define	ALLUSERS_NAME		"ALLUSERS"			// TALK中代表“大家”的名字串
	
	
// max size define
const int	_MAX_MSGSIZE		= 1024;


// OBJID space define 
typedef unsigned __int64		LLOBJID;
typedef unsigned long			LOBJID;
typedef LOBJID					OBJID;

#define hash_set set
#define hash_map map
#define hash_multimap multimap
// set
#include <vector>
typedef std::vector<OBJID>		ID_VEC;

#include <deque>
typedef std::deque<OBJID>		ID_DEQ;

#include <hash_set>
typedef std::hash_set<OBJID>	ID_SET;

//#include <set>
//typedef std::set<OBJID>		ID_SET;


// enum
enum { ID_NONE = 0, ID_SYS = 1, ID_MONEY, ID_INVALID = 0xffffffff };
enum GENDER		{ GENDER_MALE, GENDER_FEMALE};


// struct
struct OBJPOS 
{
	float x, y, z;
	OBJPOS(float nPosX = 0, float nPosY = 0, float nPosZ = 0) 
		: x(nPosX), y(nPosY), z(nPosZ) {}
};

struct OBJSIZE {
	int w, h, l;
	OBJSIZE(int nWidth = 0, int nHeight = 0, int nLength = 0) 
		: w(nWidth), h(nHeight), l(nLength) {}
};

__forceinline ULONGLONG MakeULL (ULONG ulHigh, ULONG ulLow)
{
	return ( ((ULONGLONG)ulHigh) << 32 | (ULONGLONG)ulLow );
}

enum RET_TYPE		{ RET_NONE, RET_LOGIN_ACC, RET_LOGIN_ROLE, RET_OK, RET_CONTINUE, RET_FAIL, RET_DESTROY,};

namespace sbase
{
	class CMsg;
}

enum RANGE
{
	RANGE_ME,
	RANGE_9BLOCK,
	RANGE_MAP,
	RANGE_SERVER,
	RANGE_ALLSERVER,
};


enum SQL_TYPE
{
	SELECT_ATTARIBUTE            ,
	SELECT_PASSIVE_SKILL         ,
	SELECT_ACTIVE_SKILL          ,
	SELECT_BAG_EQUIP             ,
	SELECT_RELATION              ,
	UPDATE_ATTARIBUTE            ,
	UPDATE_PASSIVE_SKILL         ,
	UPDATE_ACTIVE_SKILL          ,
	UPDATE_BAG_EQUIP             ,
	UPDATE_RELATION              ,
};



