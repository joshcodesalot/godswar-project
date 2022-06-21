#include "stdafx.h"
#include "Error.h"
#include "Player.h"
#include "..\ServiceLoader.h"
#include "..\..\Common\MsgDefine.h"
#include "..\..\Common\MsgTypes.h"

extern MSG_ERROR msg_error;


void SendServerError(ServerErrorID ID , CPlayer* player)
{
	ASSERT(ID < ErrorCount);

	if (ID > ErrorCount)
		ID = ErrorCount;

	memcpy(msg_error.Error , ServerErrorStr[ID].c_str() , MAX_NAME);

	player->s_World->SendMsgToClient(&msg_error , player->GetSocket());
};



