// ********************************************************************
//
// Copyright (c) 2004
// All Rights Reserved
//
// Author: zeng cs
// Created: 2005/12/1
//
// ********************************************************************

#pragma once


#include "Types.h"
#include "SvrBase.h"

namespace game
{
	class IGame;
	class IConnect;
	class IGameModule;
}
namespace sbase
{
	// msg head
	struct  MsgHead
	{
		USHORT	usSize;
		USHORT	usType;
	};

	enum MSG_CHANNEL
	{
		MSG_USER	= 0,
		MSG_VIEW	= 11,
		MSG_MAP		= 12,
		MSG_SERVER	= 13,
		MSG_FRIND	= 21,
		MSG_FAMILY	= 31,
		MSG_TEAM	= 32,
		MSG_GUILD	= 33,
		MSG_STATE	= 34,
	};
	
	class CMsg
	{
	public:
		CMsg()	{ this->Init(); }
		virtual ~CMsg()	{}
		
		const char*			GetBuf		(void) const	{ return m_bufMsg; }
		USHORT				GetSize		(void) const	{ return m_head.usSize; }
		USHORT				GetType		(void) const	{ return m_head.usType; }
		USHORT				GetHeadSize	(void) const	{ return sizeof(MsgHead); }
		bool				IsValid		(void) const;
		ULONG				Release		(void)			{ delete this; return 0; }

	protected:
		union 
		{
			char		m_bufMsg[_MAX_MSGSIZE];
			MsgHead		m_head;
		};

		void				Init		(void);
		
	public:	
		virtual bool		Create		(const void* pszBuf, USHORT usBufLen);
	public:
		// process 
// 		virtual RET_TYPE	Process		(void *pInfo, game::IConnect& rConnect, game::IGame& rRoot)		{ ASSERT(!"Msg Precess By Default!"); return RET_NONE; }
// 		virtual RET_TYPE	Process		(game::IConnect& rConnect, game::IGame& rRoot)					{ ASSERT(!"Msg Precess By Default!"); return RET_NONE; }
// 		virtual RET_TYPE	Process		(OBJID idActor, game::IGame& rRoot)								{ ASSERT(!"Msg Precess By Default!"); return RET_NONE; }
	};
}
