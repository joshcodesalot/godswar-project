#include "StdAfx.h"
#include "Region.h"
#include "GameObject.h"
#include "Player.h"
#include "NPC.h"
#include "Monster.h"


#include "..\ServiceLoader.h"


#include "..\..\Common\MsgDefine.h"
#include "..\..\Common\MsgTypes.h"

extern MSG_OBJECTINFO msg, msg2other;
extern MSG_LEAVE msg_leave;
extern MSG_WALK_BEGIN msg_walk_begin;
extern MSG_WALK_END msg_walk_end;


long CRegion::JoinRegionSizeOperate( int gx, int gy, CPlayer* pPlayer )
{
	if( gx < 0 || gx >= REGION_SIZE )
		return -1;
	if( gy < 0 || gy >= REGION_SIZE )
		return -1;

	return 0;
}

long CRegion::WalkInRegionSizeOperate( int gx, int gy, CPlayer* pPlayer )
{
	if( gx < 0 || gx >= REGION_SIZE )
		return -1;
	if( gy < 0 || gy >= REGION_SIZE )
		return -1;

	return 0;
}


long CRegion::WalkOutRegionSizeOperate( int gx, int gy, CPlayer* pPlayer )
{
	if( gx < 0 || gx >= REGION_SIZE )
		return -1;
	if( gy < 0 || gy >= REGION_SIZE )
		return -1;


	return 0;
}

long CRegion::WalkMsgRegionSizeOperate( int gx, int gy, CPlayer* )
{
	if( gx < 0 || gx >= REGION_SIZE )
		return -1;
	if( gy < 0 || gy >= REGION_SIZE )
		return -1;

	return 0;
}

long CRegion::WalkBeginMsgRegionSizeOperate( int gx, int gy, CPlayer*)
{
	if( gx < 0 || gx >= REGION_SIZE )
		return -1;
	if( gy < 0 || gy >= REGION_SIZE )
		return -1;

	return 0;
}

long CRegion::WalkEndMsgRegionSizeOperate( int gx, int gy, CPlayer* )
{
	if( gx < 0 || gx >= REGION_SIZE )
		return -1;
	if( gy < 0 || gy >= REGION_SIZE )
		return -1;

	return 0;
}
