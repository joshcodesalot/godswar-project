// **********************************************************************
//
// Copyright (c) 2004
// All Rights Reserved
//
// Author: zeng cs
// Created:	2006/11/01
//
// **********************************************************************
#pragma once

#include "SvrBase.h"

namespace sbase
{
	//////////////////////////////////////////////////////////////////////////
	class IModule
	{
	public:
		enum STATUS {	STATUS_NONE, STATUS_PRECREATE, STATUS_CREATE, STATUS_RUN, STATUS_POSTDESTROY, STATUS_DESTROY, STATUS_EXIT, STATUS_ERR_PRECREATE, STATUS_ERR_CREATE, STATUS_ERR_RUN, };
		virtual STATUS		QueryStatus(void)	const	= 0;	
		virtual ULONG		Release(void)				= 0;
		virtual const char* GetName(void)		const	= 0;
		virtual bool		PreCreate(void)				= 0;
		virtual bool		Create(void)				= 0;
		virtual bool		Run(void)					= 0;
		virtual void		PostDestroy(void)			= 0;
	};
}