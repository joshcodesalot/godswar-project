//========================================================
//
//    Copyright (c) 2006,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� Command.cpp
//    ժ    Ҫ �� ��Ϸ�����������н���ģ��
//    
//    ��ǰ�汾 �� 1.00
//    ��    �� �� �ֵ»�
//    ������� �� 2007-01-08
//
//========================================================

#include "stdafx.h"
#include "GSMaster.h"
#include "DataCore.h"
#include "GameCore\Region.h"
#include "GameCore\RepRegion.h"
#include "GameCore\GameObject.h"
#include "GameCore\Player.h"
#include "GameCore\NPC.h"
#include "GameCore\Monster.h"
#include "World.h"


#include "..\Common\MsgDefine.h"
#include "..\Common\MsgTypes.h"
#include "ServiceLoader.h"

extern MSG_SCENE_CHANGE msg_scene_change;

// extern void ReloadScript();

char strKeyBuffer[256];
long lKeyCount = 0;
