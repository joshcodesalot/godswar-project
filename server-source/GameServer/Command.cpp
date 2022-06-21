//========================================================
//
//    Copyright (c) 2006,欢乐连线工作室
//    All rights reserved.
//
//    文件名称 ： Command.cpp
//    摘    要 ： 游戏服务器命令行解释模块
//    
//    当前版本 ： 1.00
//    作    者 ： 林德辉
//    完成日期 ： 2007-01-08
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
