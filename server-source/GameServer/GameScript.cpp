#include "stdafx.h"
#include "GameScript.h"
#include "./gamecore/SkillManager.h"
#include "DataCore.h"
#include "./gamecore/monster.h"
#include "./GameCore/Player.h"
#include "./GameCore/ItemManager.h"
#include <vector>
#include "World.h"
 
extern MSG_MAGIC_DAMAGE msg_magic;
 
static PyMethodDef GameAPI_methods[] =
{
	{"AddHP" , (PyCFunction)AddHP , METH_VARARGS , ""},
	{"AddMP" , (PyCFunction)AddMP , METH_VARARGS , ""},
	{"PlayerAttack", (PyCFunction)PlayerAttack, METH_VARARGS, ""},
	{"MagicAttack",(PyCFunction)ObjMagicAttack, METH_VARARGS, ""},
	{"AddHPByMagic",(PyCFunction)AddHPByMagic, METH_VARARGS, ""},
	{"AddEXP",(PyCFunction)AddEXP, METH_VARARGS, ""},
	{"AddSkillEXP",(PyCFunction)AddSkillEXP, METH_VARARGS, ""},
	{"AddMoney",(PyCFunction)AddMoney, METH_VARARGS, ""},
	{"AddCredit",(PyCFunction)AddCredit, METH_VARARGS, ""},
	{"AddContribute",(PyCFunction)AddContribute, METH_VARARGS, ""},
	{"Fly",(PyCFunction)Fly, METH_VARARGS, ""},
	{"ChangePlayeBuniness",(PyCFunction)ChangePlayeBuniness, METH_VARARGS, ""},
	{"AddStatus",(PyCFunction)AddStatus, METH_VARARGS, ""},
	{"Learn",(PyCFunction)Learn, METH_VARARGS, ""},
	{"AddMultiHP" , (PyCFunction)AddMultiHP , METH_VARARGS , ""},
	{"AltarCreate" ,(PyCFunction)AltarCreate, METH_VARARGS, ""},
	{"AltarLevelup" , (PyCFunction)AltarLevelup, METH_VARARGS, ""},
	{"AltarDel" ,(PyCFunction)AltarDel, METH_VARARGS, ""},
	{"OblationLevelup" ,(PyCFunction)OblationLevelup, METH_VARARGS, ""},
	{"GetHaveAltars" , (PyCFunction)GetHaveAltars , METH_VARARGS , ""},
	{"AcceptGiftItem" , (PyCFunction)AcceptGiftItem , METH_VARARGS , ""},
	{"GetPlayerAttr" , (PyCFunction)GetPlayerAttr , METH_VARARGS , ""},
	{"AcceptGiftAttr" , (PyCFunction)AcceptGiftAttr , METH_VARARGS , ""},
	{"GuildQuestRelease" , (PyCFunction)GuildQuestRelease , METH_VARARGS , ""},
	{"GetUnionMaxAltar" , (PyCFunction)GetUnionMaxAltar , METH_VARARGS , ""},
	{"GetHavePlayerAltar" , (PyCFunction)GetHavePlayerAltar , METH_VARARGS , ""},
	{"ChangeChatBubble" , (PyCFunction)ChangeChatBubble , METH_VARARGS , ""},
	{"AddLevel" , (PyCFunction)AddLevel , METH_VARARGS , ""},
	{"AddGlory" , (PyCFunction)AddGlory , METH_VARARGS , ""},
	{"AddStone" , (PyCFunction)AddStone , METH_VARARGS , ""},
	{"AddConvertPoint" , (PyCFunction)AddConvertPoint , METH_VARARGS , ""},
	/*------------------------------------------------------------------------------
	*作者：C.D
	*日期：2008-06-16
	*描述：商城道具添加
	-----------------------------------------------------------------------------*/
	{"TimeDataChange" , (PyCFunction)TimeDataChange , METH_VARARGS , ""},
	{"GetConsortiaData" , (PyCFunction)GetConsortiaData , METH_VARARGS , ""},
	{"ChangeStorage" , (PyCFunction)ChangeStorage , METH_VARARGS , ""},
	{"ChangeBag" , (PyCFunction)ChangeBag , METH_VARARGS , ""},
	{"ChangeHairstyle" , (PyCFunction)ChangeHairstyle , METH_VARARGS , ""},
	{"ChangeHaircolor" , (PyCFunction)ChangeHaircolor , METH_VARARGS , ""},
	//------------------------------------------------------------------------------
	/*------------------------------------------------------------------------------
	*作者：C.D
	*日期：2008-06-17
	*描述：商城道具添加
	-----------------------------------------------------------------------------*/
	{"ChangePK" , (PyCFunction)ChangePK , METH_VARARGS , ""},
	{"ChangeCamp" , (PyCFunction)ChangeCamp , METH_VARARGS , ""},
	{"ChangeHideID" , (PyCFunction)ChangeHideID , METH_VARARGS , ""},
	//------------------------------------------------------------------------------
	{NULL , NULL , NULL , NULL}
};

void ExcScript( CGameObject* p1 , CGameObject* p2 , int MagicID )
{
	const MagicData* data = p1->s_World->g_pSkillManager->GetMagic(MagicID);
	PyObject* module = ScriptSystem::Instance()->GetModule("Script.Skill");
	if (!module)
		return;

	if (p1)
		if (p1->IsDead() && data->iEffectTurn < 2)
			return;

	if (p2)
		if (p2->IsDead())
			return;

	//const MagicData* data = p1->s_World->g_pSkillManager->GetMagic(MagicID);

	pyScriptObject* pyo1 = (pyScriptObject*)PyObject_NEW(pyScriptObject , &pyPlayer_type);
	pyScriptObject* pyo2 = (pyScriptObject*)PyObject_NEW(pyScriptObject , &pyPlayer_type);

	p1->GetScriptObject(&pyo1->obj);
	p2->GetScriptObject(&pyo2->obj);

	PyObject *pFunc;
	PyObject *pArgs, *pValue;

	pFunc = PyObject_GetAttrString(module , data->ScriptFun);

	if (!pFunc)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	pArgs = PyTuple_New(3);
	PyTuple_SetItem(pArgs , 0 , (PyObject*)pyo1);
	PyTuple_SetItem(pArgs , 1 , (PyObject*)pyo2);
	PyTuple_SetItem(pArgs , 2 , PyInt_FromLong(MagicID));
	pValue = PyObject_CallObject(pFunc, pArgs);
	Py_XDECREF(pArgs);

	if (!pValue)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

 	Py_XDECREF(pValue);

	PyErr_Print();
	PyErr_Clear();
}

bool InitGameScript()
{
	if (PyType_Ready(&pyPlayer_type) < 0)
		return false;

	PyObject* submodule = Py_InitModule3( "GameAPI" , GameAPI_methods, "");
	Py_XINCREF(&pyPlayer_type);
	PyModule_AddObject(submodule , "player", (PyObject*)&pyPlayer_type);

	PyObject* skill = ScriptSystem::Instance()->ImportModule("Script.Skill");
	Py_XINCREF(&pyPlayer_type);
	PyModule_AddObject(skill , "player", (PyObject*)&pyPlayer_type);

	PyObject* Altar = ScriptSystem::Instance()->ImportModule("Script.Altar");
	Py_XINCREF(&pyPlayer_type);
	PyModule_AddObject(Altar , "player", (PyObject*)&pyPlayer_type);

 
	PyObject* SysFun = ScriptSystem::Instance()->ImportModule("Script.SysFun");
	Py_XINCREF(&pyPlayer_type);
	PyModule_AddObject(SysFun , "player", (PyObject*)&pyPlayer_type);

	return true;
}

// void ReloadScript()
// {
// 	ScriptSystem::Instance()->Reload();
// 
// 	for (int i = ACADEMIC_PLAYER_MEM_ID ; i < ACADEMIC_PLAYER_MEM_ID + ACADEMIC_NPC_MEM_ID ; i++)
// 	{
// 		CNPC* npc = GetNPC(i);
// 
// 		if (npc)
// 			npc->SetScriptName(npc->GetScriptName());
// 	}
// }

void Learn(PyObject* , PyObject* args)
{
 	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
 	//PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
 	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);
 
 	int id		= PyInt_AsLong(arg0);
 	//int id2		= PyInt_AsLong(arg1);
 	int magic	= PyInt_AsLong(arg2);
 
 	CGameObject*		obj1		= (CGameObject*)(id);

 
 
 	// 发送执行消息
 	MSG_MAGIC_PERFORM mgicPerform;
 
 	mgicPerform.Head.usSize = sizeof( MSG_MAGIC_PERFORM );
 	mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
 	mgicPerform.uiID		= obj1->GetID();
 	mgicPerform.x			= static_cast<CPlayer*>(obj1)->m_SkillMsg.x1;
 	mgicPerform.z			= static_cast<CPlayer*>(obj1)->m_SkillMsg.z1;
 	mgicPerform.uiObjectID	= static_cast<CPlayer*>(obj1)->GetID();
 	mgicPerform.ucMagicID	= magic;
 
 	//发送魔法Perform消息
 	obj1->GetRegion()->SendAreaMsgOneToOther( obj1->GetCurrentCell(), &mgicPerform );

	//必须调用下这个,莫名其妙
	PyErr_Print();
	PyErr_Clear();
}

void ObjMagicAttack(PyObject* , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);

	int id		= PyInt_AsLong(arg0);
	int id2		= PyInt_AsLong(arg1);
	int magic	= PyInt_AsLong(arg2);

	CGameObject*		obj1		= (CGameObject*)(id);
	CGameObject*		obj2		= (CGameObject*)(id2);

	if (!obj1 || !obj2)
		return;

	const	MagicData*	mgcData = obj1->s_World->g_pSkillManager->GetMagic( magic );

	

	//判断命中
	ATTACK_TYPE  attack_type = obj1->JudgeAttackStyle( obj2  );
	//计算伤害
	int  damage = obj1->CalculateAttackDamage(  magic , *obj2 );

	//怪物已经脱离战斗
	if ( obj2->GetType() == OBJECTTYPE_MONSTER && ((CMonster*)obj2)->IsLeaveFight() )
		attack_type = ATTACK_MISS;
	
	if(   attack_type  ==  ATTACK_FORCE   ) 
	{
		//暴击额外增加状态
/*		obj1->AddStatus( g_pStatusManager->GetStatus( 10 ) );*/
		damage *= 1.5;

	    obj2->AddHP(-damage);

		//***************状态处理************************
		//中断吟唱
		if ( OBJECT_CAST == obj2->GetState() && obj2->GetHaltIntonate() > 0.0f )
		{
			obj2->ResetCastState();
			//obj2->SetState( OBJECT_IDLE );
		}
		 
        //返还HP
		int HPResume = int( damage * obj2->GetReboundDamage() +  (int)damage * obj2->GetReturnDamage() );
		obj1->AddHP(HPResume);

		obj2->SetHaltIntonate( 0.00f );
		obj2->SetReboundDamage( 0.00f );
		//************************************************
        

		if (obj2->GetType() == OBJECTTYPE_MONSTER) 
		{
			obj1->GetRegion()->StartFight(obj1 , obj2);
			obj2->GetRegion()->StartFight(obj2 , obj1);

			obj2->AddEnmity( obj1, mgcData->iEnmity + damage );
			obj1->AddEnmity(obj2 , 0);
		}
		else if (obj2->GetType() == OBJECTTYPE_PLAYER)
		{
// 			if (obj2->GetFight())
// 			{
// 				//同步仇恨
// 				if (damage > 0)
// 					obj1->CoagentEnmityList(obj2 , damage);
// 				else
// 					obj1->CoagentEnmityList(obj2 , -damage);
// 			}
		}

		//计算经验和等级
		obj1->CalculateExpAndLevel( *obj2 );
		//obj1->CalculateExp( obj2->GetlHP() <= 0 );

     
		DAMAGE_INFO dmg =  obj2->GetDamageInfo();

		dmg.Type = ATTACK_FORCE;

		obj2->SetDamageInfo( dmg );
	}
	else if ( attack_type   ==  ATTACK_HIT )
	{

		obj2->AddHP(-damage);

		if ( obj2->GetType() == OBJECTTYPE_MONSTER ) 
		{
			obj1->GetRegion()->StartFight(obj1 , obj2);
			obj2->GetRegion()->StartFight(obj2 , obj1);

			obj2->AddEnmity( obj1, damage +  mgcData->iEnmity );
			obj1->AddEnmity(obj2 , 0);
		}
		else if (obj2->GetType() == OBJECTTYPE_PLAYER)
		{
// 			if (obj2->GetFight())
// 			{
// 				//同步仇恨
// 				if (damage > 0)
// 					obj1->CoagentEnmityList(obj2 , damage);
// 				else
// 					obj1->CoagentEnmityList(obj2 , -damage);
// 			}
		}

		//计算攻击方各个被动技能经验()
// 		if( obj2->GetlHP() <= 0 )
// 		   obj1->CalculatePassivePracticeDegree( magic, attack_type, true );
// 		else
// 		   obj1->CalculatePassivePracticeDegree( magic, attack_type, false );
		obj1->CalculateExpAndLevel( *obj2 );
		//obj1->CalculateExp( obj2->GetlHP() <= 0 );

		DAMAGE_INFO dmg = obj2->GetDamageInfo();

		dmg.Type = ATTACK_HIT;

		obj2->SetDamageInfo( dmg );
	}
	else if ( attack_type   ==  ATTACK_MISS )
	{
		if ( obj2->GetType() == OBJECTTYPE_MONSTER ) 
		{
			obj1->GetRegion()->StartFight(obj1 , obj2);
			obj2->GetRegion()->StartFight(obj2 , obj1);

			obj2->AddEnmity( obj1, 1 );
			obj1->AddEnmity(obj2 , 0);
		}

		DAMAGE_INFO dmg = obj2->GetDamageInfo();

		dmg.Type = ATTACK_MISS;

		obj2->SetDamageInfo( dmg );
	}






	//必须调用下这个,莫名其妙
	PyErr_Print();
	PyErr_Clear();
}

void AddHP(PyObject* , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);
	PyObject* arg3 = PyTuple_GET_ITEM(args, 3);

	int id		 = PyInt_AsLong(arg0);
	int targetid = PyInt_AsLong(arg1);
	int hp		 = PyInt_AsLong(arg2);
    int magic	 = PyInt_AsLong(arg3);

	CGameObject* obj = (CGameObject*)id;
	CGameObject* targetobj = (CGameObject*)(targetid);

	if (!targetobj)
		return;

	const	MagicData*	mgcData = targetobj->s_World->g_pSkillManager->GetMagic( magic );



	//加血算法
	int AppendHP = hp * ( 1  + obj->GetAllfCure() + targetobj->GetAllfBeCure()  );
	obj->CoagentEnmityList( obj, AppendHP );

	targetobj->AddHP(AppendHP);

 	// 发送执行消息
 	MSG_MAGIC_PERFORM mgicPerform;
 
 	mgicPerform.Head.usSize = sizeof( MSG_MAGIC_PERFORM );
 	mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
 	mgicPerform.uiID		= obj->GetID();
 	mgicPerform.x			= static_cast<CPlayer*>(obj)->m_SkillMsg.x1;
 	mgicPerform.z			= static_cast<CPlayer*>(obj)->m_SkillMsg.z1;
 	mgicPerform.uiObjectID	= targetobj->GetID();//targetid/*static_cast<CPlayer*>(obj)->GetID()*/;
 	mgicPerform.ucMagicID	= magic;
 
	msg_magic.uiID        = obj->GetID();
	if(obj->GetTarget())
	{
		msg_magic.uiObjectID  = obj->GetTarget()->GetID();
	}
	else
	{
		msg_magic.uiObjectID  = targetobj->GetID();
		//msg_magic.uiObjectID  = obj->GetID();
	}
 	msg_magic.ucMagicID   = magic;
 	msg_magic.Attack_Type = ATTACK_HIT;
 	msg_magic.Status_Type = ATTACK_HIT;
 	msg_magic.iPower = -AppendHP;
 	msg_magic.x1 = static_cast<CPlayer*>(obj)->m_SkillMsg.x1;
 	msg_magic.z1 = static_cast<CPlayer*>(obj)->m_SkillMsg.z1;
	msg_magic.bType=0;
	//msg_magic.uiObjectID = static_cast<CPlayer*>(obj)->GetID();

 	//发送魔法Perform消息
 	obj->GetRegion()->SendAreaMsgOneToOther( obj->GetCurrentCell(), &msg_magic );
 	obj->GetRegion()->SendAreaMsgOneToOther( obj->GetCurrentCell(), &mgicPerform );

	//必须调用下这个,莫名其妙
	PyErr_Print();
	PyErr_Clear();
}

void AddMultiHP(PyObject* , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);
	PyObject* arg3 = PyTuple_GET_ITEM(args, 3);

	int id		 = PyInt_AsLong(arg0);
	int targetid = PyInt_AsLong(arg1);
	int hp		 = PyInt_AsLong(arg2);
	int magic	 = PyInt_AsLong(arg3);

	CGameObject* obj = (CGameObject*)id;
	CGameObject* targetobj = (CGameObject*)targetid;

	if (!targetobj)
		return;

	const	MagicData*	mgcData = targetobj->s_World->g_pSkillManager->GetMagic( magic );

	

	//加血算法
	int AppendHP =  hp * ( 1 +  obj->GetAllfCure() + targetobj->GetAllfBeCure()  );
	obj->CoagentEnmityList( obj, AppendHP );

	targetobj->AddHP(AppendHP);

	// 发送执行消息
	MSG_MAGIC_PERFORM mgicPerform;

	mgicPerform.Head.usSize = sizeof( MSG_MAGIC_PERFORM );
	mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
	mgicPerform.uiID		= obj->GetID();
	mgicPerform.x			= static_cast<CPlayer*>(obj)->m_SkillMsg.x1;
	mgicPerform.z			= static_cast<CPlayer*>(obj)->m_SkillMsg.z1;
	mgicPerform.uiObjectID	= static_cast<CPlayer*>(obj)->GetID();
	mgicPerform.ucMagicID	= magic;

	DAMAGE_INFO dmg = targetobj->GetDamageInfo();
	dmg.Type = ATTACK_HIT;
	dmg.HP = -AppendHP;
	targetobj->SetDamageInfo( dmg );

	//msg_magic.uiID        = obj->GetID();
	//if(obj->GetTarget())
	//	msg_magic.uiObjectID  = obj->GetTarget()->GetID();
	//else
	//	msg_magic.uiObjectID  = obj->GetID();
	//msg_magic.ucMagicID   = magic;
	//msg_magic.Attack_Type = ATTACK_HIT;
	//msg_magic.Status_Type = ATTACK_HIT;
	//msg_magic.iPower = -AppendHP;
	//msg_magic.x1 = static_cast<CPlayer*>(obj)->m_SkillMsg.x1;
	//msg_magic.z1 = static_cast<CPlayer*>(obj)->m_SkillMsg.z1;

	//发送魔法Perform消息
	//obj->GetRegion()->SendAreaMsgOneToOther( obj->GetCurrentCell(), &msg_magic );
	//obj->GetRegion()->SendAreaMsgOneToOther( obj->GetCurrentCell(), &mgicPerform );

	//必须调用下这个,莫名其妙
	PyErr_Print();
	PyErr_Clear();
}

void AddHPByMagic(PyObject* , PyObject* args)
{
//  暂无用到
// 	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
// 	//PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
// 	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);
// 
// 	int id		= PyInt_AsLong(arg0);
// 	//int id2		= PyInt_AsLong(arg1);
// 	int magic	= PyInt_AsLong(arg2);
// 
// 	CGameObject*		obj		= GetObject(id);
// 	const	MagicData*	mgcData = ::g_pSkillManager->GetMagic( magic );
// 
// 	int hp = (int)mgcData->fPower;
// 
// 	if (!obj)
// 		return;
// 
// 	//加血算法
//      int AppendHP = mgcData->fPower * ( 1 + obj->GetAllfCure() + obj->GetAllfBeCure()  ) + hp;
// 	obj->CoagentEnmityList( obj, AppendHP );
// 
// 	obj->AddHP(AppendHP);
// 
//  	// 发送执行消息
//  	MSG_MAGIC_PERFORM mgicPerform;
//  
//  	mgicPerform.Head.usSize = sizeof( MSG_MAGIC_PERFORM );
//  	mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
//  	mgicPerform.uiID		= obj->GetID();
//  	mgicPerform.x			= static_cast<CPlayer*>(obj)->m_SkillMsg.x1;
//  	mgicPerform.z			= static_cast<CPlayer*>(obj)->m_SkillMsg.z1;
//  	mgicPerform.uiObjectID	= static_cast<CPlayer*>(obj)->GetID();
//  	mgicPerform.ucMagicID	= magic;
//  
//  	msg_magic.ucMagicID   = magic;
//  	msg_magic.Attack_Type = ATTACK_HIT;
//  	msg_magic.Status_Type = ATTACK_HIT;
//  	msg_magic.iPower = -AppendHP;
//  	msg_magic.x1 = static_cast<CPlayer*>(obj)->m_SkillMsg.x1;
//  	msg_magic.z1 = static_cast<CPlayer*>(obj)->m_SkillMsg.z1;
//  
//  	//发送魔法Perform消息
//  	obj->GetRegion()->SendAreaMsgOneToOther( obj->GetCurrentCell(), &msg_magic );
//  	obj->GetRegion()->SendAreaMsgOneToOther( obj->GetCurrentCell(), &mgicPerform );
// 
// 	//必须调用下这个,莫名其妙
// 	PyErr_Print();
// 	PyErr_Clear();
}


void AddMP(PyObject* , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);
	PyObject* arg3 = PyTuple_GET_ITEM(args, 3);

	int id1 = PyInt_AsLong(arg0);
	int id2 = PyInt_AsLong(arg1);
	int mp = PyInt_AsLong(arg2);
	int magic = PyInt_AsLong(arg3);

	CGameObject* obj = (CGameObject*)(id1);
	CGameObject* Targetobj = (CGameObject*)(id2);
	int AppendHP = mp * ( 1  + obj->GetAllfCure() + Targetobj->GetAllfBeCure()  );

	if (!obj)
		return;

	obj->AddMP(mp);

	// 发送执行消息
	MSG_MAGIC_PERFORM mgicPerform;

	mgicPerform.Head.usSize = sizeof( MSG_MAGIC_PERFORM );
	mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
	mgicPerform.uiID		= obj->GetID();
	mgicPerform.x			= static_cast<CPlayer*>(obj)->m_SkillMsg.x1;
	mgicPerform.z			= static_cast<CPlayer*>(obj)->m_SkillMsg.z1;
	mgicPerform.uiObjectID	= static_cast<CPlayer*>(obj)->GetID();
	mgicPerform.ucMagicID	= magic;

	msg_magic.ucMagicID   = magic;
	msg_magic.Attack_Type = ATTACK_HIT;
	msg_magic.Status_Type = ATTACK_HIT;
	msg_magic.iPower = -AppendHP;
	msg_magic.x1 = static_cast<CPlayer*>(obj)->m_SkillMsg.x1;
	msg_magic.z1 = static_cast<CPlayer*>(obj)->m_SkillMsg.z1;
	msg_magic.bType=1;
	msg_magic.uiObjectID = static_cast<CPlayer*>(obj)->GetID();


	//发送魔法Perform消息
	obj->GetRegion()->SendAreaMsgOneToOther( obj->GetCurrentCell(), &msg_magic );
	obj->GetRegion()->SendAreaMsgOneToOther( obj->GetCurrentCell(), &mgicPerform );

	//必须调用下这个,莫名其妙
	PyErr_Print();
	PyErr_Clear();
}

void PlayerAttack(PyObject* , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);

	int id		= PyInt_AsLong(arg0);
	int id2		= PyInt_AsLong(arg1);
	int magic	= PyInt_AsLong(arg2);

	CGameObject* obj1 = (CGameObject*)(id);
	CGameObject* obj2 = (CGameObject*)(id2);

	if (!obj1 || !obj2)
		return;

	const MagicData* mgcData = obj1->s_World->g_pSkillManager->GetMagic( magic );

 	// 等级保护
// 	if( obj1->GetType() == OBJECTTYPE_PLAYER && obj1->GetRank() <= 15 )
// 		return;
// 	if( obj2->GetType() == OBJECTTYPE_PLAYER && obj2->GetRank() <= 15 )
// 		return;

	//判断命中
	ATTACK_TYPE  attack_type = obj1->JudgeAttackStyle( obj2 );

	//怪物已经脱离战斗
	if ( obj2->GetType() == OBJECTTYPE_MONSTER && ((CMonster*)obj2)->IsLeaveFight() )
		attack_type = ATTACK_MISS;

	if(   attack_type  ==  ATTACK_FORCE   ) 
	{
		//计算伤害													
		int  damage = obj1->CalculateAttackDamage(  magic , *obj2 ) * 1.5 ;
		obj2->AddHP(-damage);

		if ( obj2->GetType() == OBJECTTYPE_MONSTER ) 
		{
			obj1->GetRegion()->StartFight(obj1 , obj2);
			obj2->GetRegion()->StartFight(obj2 , obj1);

			obj2->AddEnmity( obj1, damage );
			obj1->AddEnmity(obj2 , 0);
		}
		else if (obj2->GetType() == OBJECTTYPE_PLAYER)
		{
// 			if (obj2->GetFight())
// 			{
// 				//同步仇恨
// 				if (damage > 0)
// 					obj1->CoagentEnmityList(obj2 , damage);
// 				else
// 					obj1->CoagentEnmityList(obj2 , -damage);
// 			}
		}

		//计算经验和等级
		obj1->CalculateExpAndLevel( *obj2 );

		//计算攻击方各个被动技能经验
// 		if( obj2->GetlHP() <= 0 )
// 			obj1->CalculatePassivePracticeDegree( magic, attack_type, true );
// 		else
// 			obj1->CalculatePassivePracticeDegree( magic, attack_type, false );
		//obj1->CalculateExp( obj2->GetlHP() <= 0 );
	
		DAMAGE_INFO dmg = obj2->GetDamageInfo();

		dmg.Type = ATTACK_FORCE;

		obj2->SetDamageInfo( dmg );
	}
	else if ( attack_type   ==  ATTACK_HIT )
	{
		//计算伤害
		int  damage = obj1->CalculateAttackDamage(  magic , *obj2 );
		obj2->AddHP(-damage);

		if ( obj2->GetType() == OBJECTTYPE_MONSTER ) 
		{
			obj1->GetRegion()->StartFight(obj1 , obj2);
			obj2->GetRegion()->StartFight(obj2 , obj1);

			obj2->AddEnmity( obj1, damage );
			obj1->AddEnmity(obj2 , 0);
		}
		else if (obj2->GetType() == OBJECTTYPE_PLAYER)
		{
// 			if (obj2->GetFight())
// 			{
// 				//同步仇恨
// 				if (damage > 0)
// 					obj1->CoagentEnmityList(obj2 , damage);
// 				else
// 					obj1->CoagentEnmityList(obj2 , -damage);
// 			}
		}

		//计算经验和等级
		obj1->CalculateExpAndLevel( *obj2 );

		DAMAGE_INFO dmg = obj2->GetDamageInfo();

		dmg.Type = ATTACK_HIT;

		obj2->SetDamageInfo( dmg );
	}
	else if ( attack_type   ==  ATTACK_MISS )
	{

		if ( obj2->GetType() == OBJECTTYPE_MONSTER ) 
		{
			obj1->GetRegion()->StartFight(obj1 , obj2);
			obj2->GetRegion()->StartFight(obj2 , obj1);

			obj2->AddEnmity( obj1, 1 );
			obj1->AddEnmity(obj2 , 0);
		}

		DAMAGE_INFO dmg = obj2->GetDamageInfo();

		dmg.Type = ATTACK_MISS;

		obj2->SetDamageInfo( dmg );
	}

	//必须调用下这个,莫名其妙
	PyErr_Print();
	PyErr_Clear();
}

void AddEXP(PyObject* , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int id = PyInt_AsLong(arg0);
	int exp = PyInt_AsLong(arg1);

	CPlayer* player = (CPlayer*)(id);

	if (!player)
		return;

	player->AddExp(exp);

	player->SynGameData(true);


	PyErr_Print();
	PyErr_Clear();
}

void AddSkillEXP(PyObject*  , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int id = PyInt_AsLong(arg0);
	int exp = PyInt_AsLong(arg1);

	CPlayer* player = ( CPlayer* )(id);

	if (!player)
		return;

	player->CalculateExp(true , exp);


	PyErr_Print();
	PyErr_Clear();
}

void AddMoney(PyObject* , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int id = PyInt_AsLong(arg0);
	int money = PyInt_AsLong(arg1);

	CPlayer* player = (CPlayer*)(id);

	if (!player)
		return;

	player->AddMoney(money);

	player->SynGameData(true);



	//更新客端数据显示
	PyErr_Print();
	PyErr_Clear();
}

PyObject* AddStone(PyObject* , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int id = PyInt_AsLong(arg0);
	int Bijou = PyInt_AsLong(arg1);

	CPlayer* player = (CPlayer*)(id);

	if (!player)
	{
		Py_XINCREF(Py_False);
		return Py_False;
	}

	player->AddBijou(Bijou);
	player->SynGameData(true);

	//更新客端数据显示
	PyErr_Print();
	PyErr_Clear();

	Py_XINCREF(Py_True);
	return	Py_True;
}

PyObject* AddGlory(PyObject* , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int id = PyInt_AsLong(arg0);
	int money = PyInt_AsLong(arg1);

	CPlayer* player = (CPlayer*)(id);
	
	if (!player)
	{
		Py_XINCREF(Py_False);
		return Py_False;
	}

	player->AddPrestige(money);
	player->SynGameData(true);

	//更新客端数据显示
	PyErr_Print();
	PyErr_Clear();

	Py_XINCREF(Py_True);
	return	Py_True;
}

PyObject* AddConvertPoint(PyObject* , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int id = PyInt_AsLong(arg0);
	int money = PyInt_AsLong(arg1);

	CPlayer* player = (CPlayer*)(id);

	if (!player)
	{
		Py_XINCREF(Py_False);
		return Py_False;
	}

	player->AddPoint(money);
	player->SendUseOrEquip(1);

	//更新客端数据显示
	PyErr_Print();
	PyErr_Clear();

	Py_XINCREF(Py_True);
	return	Py_True;
}

void AddCredit(PyObject* , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	//PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int id = PyInt_AsLong(arg0);
	//int credit = PyInt_AsLong(arg1);

	CPlayer* player = ( CPlayer* )(id);

	if (!player)
		return;

	PyErr_Print();
	PyErr_Clear();
}

void AddContribute(PyObject* , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	//PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int id = PyInt_AsLong(arg0);
	//int contribute = PyInt_AsLong(arg1);

	CPlayer* player = (CPlayer*)(id);

	if (!player)
		return;

	PyErr_Print();
	PyErr_Clear();
}

PyObject* Fly(PyObject* , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);
	PyObject* arg3 = PyTuple_GET_ITEM(args, 3);
	PyObject* arg4 = PyTuple_GET_ITEM(args, 4);

	long id  = PyInt_AsLong(arg0);
	
	CPlayer *pPlayer = (CPlayer*)id;

	//CPlayer* pPlayer = GetPlayer(id);

	int fly = PyInt_AsLong(arg1);
	float x = (double)PyFloat_AsDouble(arg2);
	float y = (double)PyFloat_AsDouble(arg3);
	float z = (double)PyFloat_AsDouble(arg4);

	if(!pPlayer->IsDead())
	{ 
		// 发送执行消息
		MSG_MAGIC_PERFORM mgicPerform;
		mgicPerform.Head.usSize = sizeof( MSG_MAGIC_PERFORM );
		mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
		mgicPerform.uiID = pPlayer->GetID();
		mgicPerform.x = pPlayer->GetPosX();
		mgicPerform.z = pPlayer->GetPosZ();
		mgicPerform.uiObjectID = pPlayer->GetID();
		mgicPerform.ucMagicID = pPlayer->GetCurActiveSkillID();
		if( pPlayer->s_World->g_pSkillManager->GetMagic(mgicPerform.ucMagicID) != NULL )
			pPlayer->GetRegion()->SendAreaMsgOneToOther( pPlayer->GetCurrentCell(), &mgicPerform );

		pPlayer->Fly(fly , x , y , z); 
	}

	PyErr_Print();
	PyErr_Clear();

    return PyInt_FromLong(1);
}

void ChangePlayeBuniness(PyObject* , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int id = PyInt_AsLong(arg0);
	int nclass = PyInt_AsLong(arg1);

	CPlayer* player = (CPlayer*)(id);

	if (!player)
		return;

	player->ChangePlayeBuniness(nclass);

	PyErr_Print();
	PyErr_Clear();
}

void AddStatus(PyObject* , PyObject* args)
{
 	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
 	//PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
 	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);
 
 	int id		= PyInt_AsLong(arg0);
 	//int id2		= PyInt_AsLong(arg1);
 	int magic	= PyInt_AsLong(arg2);
 
   	CGameObject*		obj1		= (CGameObject*)(id);
	
	if (obj1 == NULL)
	{
		return;
	}
   	const MagicData *pMagic = obj1->s_World->g_pSkillManager->GetMagic( magic );
   	//状态
   	for (size_t i = 0 ; i < pMagic->m_Status.size() ; i++) 
   	{
   		if ( (pMagic->m_Status[i]) < 0  ) //去状态
   		{
   			obj1->DelStatus( obj1->s_World->g_pStatusManager->GetStatus( (-1)*(pMagic->m_Status[i]) ));
   		}
   		else
   		{
   			obj1->AddStatus( obj1->s_World->g_pStatusManager->GetStatus(pMagic->m_Status[i]) );
   		}
   
   	}
  
  	// 发送执行消息
  	MSG_MAGIC_PERFORM mgicPerform;
  
  	mgicPerform.Head.usSize = sizeof( MSG_MAGIC_PERFORM );
  	mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
  	mgicPerform.uiID		= obj1->GetID();
  	mgicPerform.x			= static_cast<CPlayer*>(obj1)->m_SkillMsg.x1;
  	mgicPerform.z			= static_cast<CPlayer*>(obj1)->m_SkillMsg.z1;
  	mgicPerform.uiObjectID	= static_cast<CPlayer*>(obj1)->GetID();
  	mgicPerform.ucMagicID	= magic;
  
  	obj1->GetRegion()->SendAreaMsgOneToOther( obj1->GetCurrentCell(), &mgicPerform );

	PyErr_Print();
	PyErr_Clear();
}

PyObject* AltarCreate(PyObject* self , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int PlayerAddr = PyInt_AsLong(arg0);
	int ID = PyInt_AsLong(arg1);
	CPlayer *pPlayer = (CPlayer*)PlayerAddr;
	if (  !pPlayer->CreateAltar( ID ) )
	{
		Py_XINCREF(Py_False);
		return Py_False;
	}

	Py_XINCREF(Py_True);
	return Py_True;
}

PyObject*  AltarLevelup(PyObject* , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int PlayerAddr = PyInt_AsLong(arg0);
	int ID = PyInt_AsLong(arg1);

	//祭坛升级
	CPlayer *pPlayer = (CPlayer*)PlayerAddr;
	if (  !pPlayer->AltarLevelUp( ID ) )
	{
		Py_XINCREF(Py_False);
		return Py_False;
	}
		
	Py_XINCREF(Py_True);
	return Py_True;

}

PyObject*  AltarDel(PyObject* self , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int PlayerAddr = PyInt_AsLong(arg0);
	int ID = PyInt_AsLong(arg1);

	//祭坛升级
	CPlayer *pPlayer = (CPlayer*)PlayerAddr;
	if ( !pPlayer->DelAltar( ID ) )
	{
		Py_XINCREF(Py_False);
		return Py_False;
	}

	Py_XINCREF(Py_True);
	return Py_True;
}

PyObject* GetHaveAltars(PyObject* self , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	int id  = PyInt_AsLong(arg0);
	CPlayer *pPlayer = (CPlayer*)id;
	vector<int>& Has = pPlayer->GetHaveAltars();
	vector<int>::iterator itor = Has.begin();
	PyObject *AltarsTuple  = PyList_New(Has.size()) ;

	int Num = 0;
	for ( ; itor!= Has.end(); itor++ )
		PyList_SetItem( AltarsTuple, Num++, PyInt_FromLong(*itor) );

	return AltarsTuple;
}


PyObject* OblationLevelup(PyObject* self , PyObject* args)
{

	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);

	int PlayerAddr = PyInt_AsLong(arg0);
	int ID = PyInt_AsLong(arg1);
	int type = PyInt_AsLong(arg2);

	//祭坛升级
	CPlayer *pPlayer = (CPlayer*)PlayerAddr;
	if (  !pPlayer->OblationLevelUp(ID,(eAltarDealType)type) )
	{
		Py_XINCREF(Py_False);
		return Py_False;
	}

	Py_XINCREF(Py_True);
	return Py_True;
}

PyObject* GetPlayerAttr(PyObject* self , PyObject* args)
{
	enum {
		  FIELD_NAME, 
		  FIELD_GENDER,
		  FIELD_REGIONID,
		  FIELD_X,
		  FIELD_Y,
		  FIELD_Z,
		  FIELD_SPEED,
		  FIELD_STYLE,
		  FIELD_FACTION,					
		  FIELD_PRESTIGE,
		  FIELD_KNIGHT,
		  FIELD_UNION,
		  FIELD_UNIONBUSINESS,
		  FIELD_UIUIONCONTRIBUTE,
		  FIELD_CLASS,
		  FIELD_EXP,
		  FIELD_RANK,
		  FIELD_HP,
		  FIELD_MP,
		  FIELD_ATTACKDISTANCE,
		  FIELD_ATTACKSPEED,
		  FIELD_HAIRSTYLE,
		  FIELD_PUPILCOLOR,
		  FIELD_BELIEF,
		  FIELD_MONEY,
		  FIELD_STONE,
		  FIELD_SKILL_POINT,
		  FIELD_SKILL_EXP,
          //Altar
		  FIELD_MAXHP,        
		  FIELD_MAXMP,            
		  FIELD_RESUMEHP,         
		  FIELD_RESUMEMP,         
		  FIELD_ATTACK,			
		  FIELD_DEFEND,			
		  FIELD_MAGICATTACK,     
		  FIELD_MAGICDEFEND,      
		  FIELD_HIT,				
		  FIELD_DODGE,				
		  FIELD_CRITAPPEND,       
		  FIELD_CRITDEFEND,      
		  FIELD_PHYDAMAGEAPPEND, 
		  FIELD_MAGICDAMAGEAPPEND,
		  FIELD_DAMAGESORB,       
		  FIELD_BECURE,           
		  FIELD_CURE,             
		  FIELD_STATUSHIT,       
		  FIELD_STATUDDODGE
	};        

	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int id  = PyInt_AsLong(arg0);
	CPlayer *pPlayer = (CPlayer*)id;
	long Type  = PyInt_AsLong(arg1);
	int Value = 0;

	switch(  Type )
	{
	case FIELD_NAME:
		break;
	case FIELD_GENDER:
		Value =  pPlayer->GetGender();
		break;
	case FIELD_REGIONID:
		Value = pPlayer->GetlRegionID();
		break;
	case FIELD_X:
		Value = pPlayer->GetfX();
		break;
	case FIELD_Y:
		Value = pPlayer->GetfX();
		break;
	case FIELD_Z:
		Value = pPlayer->GetfZ();
		break;
	case FIELD_SPEED:
		Value = pPlayer->GetfSpeed();
		break;
	case FIELD_STYLE:
		Value =  pPlayer->GetlStyle();
		break;
	case FIELD_FACTION:
		Value = pPlayer->GetlFaction();
		break;
	case FIELD_PRESTIGE:
		Value = pPlayer->GetlPrestige();
		break;
	case FIELD_KNIGHT:
		Value = pPlayer->GetcKnight();
		break;
	case FIELD_UNION:
		Value = pPlayer->GetcUnion();
		break;
	case FIELD_UNIONBUSINESS:
		Value = pPlayer->GetcUnionBusiness();
		break;
	case FIELD_UIUIONCONTRIBUTE:
		Value = pPlayer->GetuiUionContribute();
		break;
	case FIELD_CLASS:
		Value = pPlayer->GetlClass();
		break;
	case FIELD_EXP:
		Value = pPlayer->GetcExp();
		break;
	case FIELD_RANK:
		Value = pPlayer->GetcRank();
		break;
	case FIELD_HP:
		Value = pPlayer->GetlHP();
		break;
	case FIELD_MP:
		Value = pPlayer->GetlMP();
		break;
	case FIELD_ATTACKDISTANCE:
		Value = pPlayer->GetusAttackDistance();
		break;
	case FIELD_ATTACKSPEED:
		Value = pPlayer->GetusAttackSpeed();
		break;
	case FIELD_HAIRSTYLE:
		Value = pPlayer->GetcHairStyle();
		break;
	case FIELD_PUPILCOLOR:
		Value = pPlayer->GetcPupilColor();
		break;
	case FIELD_BELIEF:
		Value = pPlayer->GetusBelief();
		break;
	case FIELD_MONEY:
		Value = pPlayer->GetlMoney();
		break;
	case FIELD_STONE :
		Value = pPlayer->GetlStone();
		break;
	case FIELD_SKILL_POINT:
		Value = pPlayer->GetSkillPoint();
		break;
	case FIELD_SKILL_EXP :
		Value = pPlayer->GetSkillExp();
		break;
	case FIELD_MAXHP:
		Value = pPlayer->GetAltarlMaxHP();
		break;
	case FIELD_MAXMP:
		Value = pPlayer->GetAltarlMaxMP();
		 break;
	case  FIELD_RESUMEHP :
         Value = pPlayer->GetAltarlResumeHP();
		break;
	case  FIELD_RESUMEMP:
	    Value = pPlayer->GetAltarlResumeMP();
		break;
	case  FIELD_ATTACK:
		Value = pPlayer->GetAltarlAttack();
		break;
	case FIELD_DEFEND:
		Value = pPlayer->GetAltarlDefend();
		break;
	case FIELD_MAGICATTACK:
		 Value = pPlayer->GetAltarlMagicAttack();
		break;
	case FIELD_MAGICDEFEND:
		 Value = pPlayer->GetAltarlMagicDefend();
		break;
	case  FIELD_HIT:
		  Value = pPlayer->GetAltarlHit();
		break;
	case FIELD_DODGE :
		   Value = pPlayer->GetAltarlDodge();
		break;
	case FIELD_CRITAPPEND:
		   Value = pPlayer->GetAltarlCritAppend();
		break;
	case  FIELD_CRITDEFEND:
		   Value = pPlayer->GetAltarlCritDefend();
		break;
	case  FIELD_PHYDAMAGEAPPEND:
		Value = pPlayer->GetAltarfPhyDamageAppend();
		break;
	case FIELD_MAGICDAMAGEAPPEND:
		Value = pPlayer->GetAltarfMagicDamageAppend();
		break;
	case FIELD_DAMAGESORB:
	     Value = pPlayer->GetAltarlDamageSorb();
		break;
	case FIELD_BECURE:
		 Value = pPlayer->GetAltarfBeCure();
		break;          
	case FIELD_CURE:
		Value = pPlayer->GetAltarfCure();
		break;
	case FIELD_STATUSHIT:
		Value = pPlayer->GetAltarlStatusHit();
		break;
	case FIELD_STATUDDODGE:
		Value = pPlayer->GetAltarlStatudDodge();
		break;
	default:
		break;
	}
	return PyInt_FromLong(Value);
}

PyObject* AcceptGiftItem(PyObject* self , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int id  = PyInt_AsLong(arg0);
	CPlayer *pPlayer = (CPlayer*)id;
	PyClassObject* obj = (PyClassObject*)arg1;

	if ( pPlayer == NULL )
	{
		sbase::ConsoleWriteColorText( FOREGROUND_RED,"AcceptGiftItem  pPlayer is NULL" );
	}

	if ( obj == NULL )
	{
		sbase::ConsoleWriteColorText( FOREGROUND_RED,"AcceptGiftItem  obj is NULL" );
	}

	PyObject* Base = PyDict_GetItemString(obj->cl_dict,"Base");
	PyObject* BaseLV = PyDict_GetItemString(obj->cl_dict,"BaseLv");
	PyObject* Append = PyDict_GetItemString(obj->cl_dict,"Append");
	PyObject* AppLevel = PyDict_GetItemString(obj->cl_dict,"AppendLv");
	PyObject* Overlap = PyDict_GetItemString(obj->cl_dict,"Overlap");
	PyObject* Binding = PyDict_GetItemString(obj->cl_dict,"Binding");
	PyObject* Type = PyDict_GetItemString(obj->cl_dict,"Type");
	PyObject* Consume = PyDict_GetItemString(obj->cl_dict,"Consume");

	NPCItemData data;
	data.Base = PyInt_AsLong(Base);
	data.BaseLV = PyInt_AsLong(BaseLV);
	data.AppendLV = PyInt_AsLong(AppLevel);
	data.Overlap = PyInt_AsLong(Overlap);
	data.Binding = PyInt_AsLong(Binding);

	for (int z = 0 ; z < MAX_EQUIPAPPEND_COUNT ; z++)
	{
		PyObject* append = PyList_GetItem(Append , z);
		data.Append[z] = PyInt_AsLong(append);
	}

	Item item;
	ItemManager::Instance()->CreateItem(&data , &item);

	if ( pPlayer->AddItem(&item) )
	{
		//添加道具成功
		pPlayer->SendAddItem(data,item);

		Py_XINCREF(Py_True);
		return Py_True;
	}
	Py_XINCREF(Py_False);
	return Py_False;
}


PyObject* AcceptGiftAttr(PyObject* self , PyObject* args)
{
	enum {
		FIELD_NAME, 
		FIELD_GENDER,
		FIELD_REGIONID,
		FIELD_X,
		FIELD_Y,
		FIELD_Z,
		FIELD_SPEED,
		FIELD_STYLE,
		FIELD_FACTION,					
		FIELD_PRESTIGE,
		FIELD_KNIGHT,
		FIELD_UNION,
		FIELD_UNIONBUSINESS,
		FIELD_UIUIONCONTRIBUTE,
		FIELD_CLASS,
		FIELD_EXP,
		FIELD_RANK,
		FIELD_HP,
		FIELD_MP,
		FIELD_ATTACKDISTANCE,
		FIELD_ATTACKSPEED,
		FIELD_HAIRSTYLE,
		FIELD_PUPILCOLOR,
		FIELD_BELIEF,
		FIELD_MONEY,
		FIELD_STONE,
		FIELD_SKILL_POINT,
		FIELD_SKILL_EXP,
		//Altar
		FIELD_MAXHP,        
		FIELD_MAXMP,            
		FIELD_RESUMEHP,         
		FIELD_RESUMEMP,         
		FIELD_ATTACK,			
		FIELD_DEFEND,			
		FIELD_MAGICATTACK,     
		FIELD_MAGICDEFEND,      
		FIELD_HIT,				
		FIELD_DODGE,				
		FIELD_CRITAPPEND,       
		FIELD_CRITDEFEND,      
		FIELD_PHYDAMAGEAPPEND, 
		FIELD_MAGICDAMAGEAPPEND,
		FIELD_DAMAGESORB,       
		FIELD_BECURE,           
		FIELD_CURE,             
		FIELD_STATUSHIT,       
		FIELD_STATUDDODGE
	};        

	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	long id  = PyInt_AsLong(arg0);
	CPlayer *pPlayer = (CPlayer*)id;

	long iType   =  PyInt_AsLong(PyObject_GetAttrString( arg1, "Type"));
	long iValue  =  PyInt_AsLong(PyObject_GetAttrString( arg1, "Value"));

	if ( pPlayer == NULL )
	{
		sbase::ConsoleWriteColorText( FOREGROUND_RED,"AcceptGiftAttr  pPlayer is NULL, Type=%d  Value=%d",iType,iValue );
	}


	switch(  iType )
	{
	case FIELD_NAME:
		break;
	case FIELD_GENDER:
		pPlayer->SetGender(iValue);
		break;
	case FIELD_REGIONID:
		pPlayer->SetlRegionID(iValue);
		break;
	case FIELD_X:
		pPlayer->SetfX(iValue);
		break;
	case FIELD_Y:
		pPlayer->SetfX(0);
		break;
	case FIELD_Z:
		pPlayer->SetfZ(iValue);
		break;
	case FIELD_SPEED:
		pPlayer->SetfSpeed(iValue);
		break;
	case FIELD_STYLE:
		pPlayer->SetlStyle(iValue);
		break;
	case FIELD_FACTION:
		pPlayer->SetlFaction(iValue);
		break;
	case FIELD_PRESTIGE:
		if ( iValue < 0 &&  pPlayer->GetlPrestige() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
            return Py_False;
		}
		pPlayer->SetlPrestige( pPlayer->GetlPrestige() + iValue );
		break;
	case FIELD_KNIGHT:
		pPlayer->SetcKnight(iValue);
		break;
	case FIELD_UNION:
		pPlayer->SetcUnion(iValue);
		break;
	case FIELD_UNIONBUSINESS:
		pPlayer->SetcUnionBusiness(iValue);
		break;
	case FIELD_UIUIONCONTRIBUTE:
		if ( iValue < 0 &&  pPlayer->GetuiUionContribute() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		pPlayer->SetuiUionContribute( pPlayer->GetuiUionContribute() + iValue );
		break;
	case FIELD_CLASS:
		pPlayer->SetlClass(iValue);
		break;
	case FIELD_EXP:
		if ( iValue < 0 &&  pPlayer->GetcExp() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}

		pPlayer->SetcExp( pPlayer->GetcExp() + iValue );
		pPlayer->CalculatePrestigeExpLevel();
		break;
	case FIELD_RANK:
		if ( iValue < 0 &&  pPlayer->GetcRank() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}

		pPlayer->SetcRank( pPlayer->GetcRank() + iValue);
		break;
	case FIELD_HP:
		if ( iValue < 0 &&  pPlayer->GetlHP() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}

		pPlayer->SetlHP( pPlayer->GetlHP() + iValue );
		break;
	case FIELD_MP:
		if ( iValue < 0 &&  pPlayer->GetlMP() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}

		pPlayer->SetlMP( pPlayer->GetlMP() + iValue);
		break;
	case FIELD_ATTACKDISTANCE:
		if ( iValue < 0 &&  pPlayer->GetusAttackDistance() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}

		pPlayer->SetusAttackDistance( pPlayer->GetusAttackDistance() + iValue);
		break;
	case FIELD_ATTACKSPEED:
		if ( iValue < 0 &&  pPlayer->GetusAttackSpeed() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		pPlayer->SetusAttackSpeed( pPlayer->GetusAttackSpeed() + iValue);
		break;
	case FIELD_HAIRSTYLE:

		pPlayer->SetcHairStyle(iValue);
		break;
	case FIELD_PUPILCOLOR:
		pPlayer->SetcPupilColor(iValue);
		break;
	case FIELD_BELIEF:
		pPlayer->SetusBelief(iValue);
		break;
	case FIELD_MONEY:
		if ( iValue < 0 &&  pPlayer->GetlMoney() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		pPlayer->SetlMoney( pPlayer->GetlMoney() + iValue );
		break;
	case FIELD_STONE :
		pPlayer->SetlStone( pPlayer->GetlStone() + iValue );
		break;
	case FIELD_SKILL_POINT:
		if ( iValue < 0 &&  pPlayer->GetSkillPoint() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		pPlayer->SetSkillPoint( pPlayer->GetSkillPoint() + iValue );
		break;
	case FIELD_SKILL_EXP :
		if ( iValue < 0 &&  pPlayer->GetSkillExp() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		pPlayer->SetSkillExp( pPlayer->GetSkillExp() + iValue );
		break;
	case FIELD_MAXHP:
		if ( iValue < 0 &&  pPlayer->GetAltarlMaxHP() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		 pPlayer->SetAltarlMaxHP( pPlayer->GetAltarlMaxHP() + iValue );
		break;
	case FIELD_MAXMP:
		if ( iValue < 0 &&  pPlayer->GetAltarlMaxMP() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}

		 pPlayer->SetAltarlMaxMP(  pPlayer->GetAltarlMaxMP() + iValue ) ;
		break;
	case  FIELD_RESUMEHP :

		if ( iValue < 0 &&  pPlayer->GetAltarlResumeHP() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}

		pPlayer->SetAltarlResumeHP( pPlayer->GetAltarlResumeHP() + iValue );
		break;
	case  FIELD_RESUMEMP:

		if ( iValue < 0 &&  pPlayer->GetAltarlResumeMP() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}

		pPlayer->SetAltarlResumeMP( pPlayer->GetAltarlResumeMP() + iValue );
		break;
	case  FIELD_ATTACK:

		if ( iValue < 0 &&  pPlayer->GetAltarlAttack() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}

		pPlayer->SetAltarlAttack( pPlayer->GetAltarlAttack() + iValue );
		break;
	case FIELD_DEFEND:
		if ( iValue < 0 &&  pPlayer->GetAltarlDefend() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}

		pPlayer->SetAltarlDefend( pPlayer->GetAltarlDefend() + iValue );
		break;
	case FIELD_MAGICATTACK:
		if ( iValue < 0 &&  pPlayer->GetAltarlMagicAttack() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		pPlayer->SetAltarlMagicAttack(  pPlayer->GetAltarlMagicAttack() + iValue );
		break;
	case FIELD_MAGICDEFEND:
		if ( iValue < 0 &&  pPlayer->GetAltarlMagicDefend() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}

		pPlayer->SetAltarlMagicDefend( pPlayer->GetAltarlMagicDefend() + iValue );
		break;
	case  FIELD_HIT:
		if ( iValue < 0 &&  pPlayer->GetAltarlHit() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}

		pPlayer->SetAltarlHit( pPlayer->GetAltarlHit() + iValue );
		break;
	case FIELD_DODGE :
		if ( iValue < 0 &&  pPlayer->GetAltarlDodge() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		pPlayer->SetAltarlDodge( pPlayer->GetAltarlDodge() + iValue );
		break;
	case FIELD_CRITAPPEND:
		if ( iValue < 0 &&  pPlayer->GetAltarlCritAppend() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		pPlayer->SetAltarlCritAppend(  pPlayer->GetAltarlCritAppend() + iValue );
		break;
	case  FIELD_CRITDEFEND:
		if ( iValue < 0 &&  pPlayer->GetAltarlCritDefend() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		pPlayer->SetAltarlCritDefend( pPlayer->GetAltarlCritDefend() + iValue );
		break;
	case  FIELD_PHYDAMAGEAPPEND:
		if ( iValue < 0 &&  pPlayer->GetAltarfPhyDamageAppend() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		pPlayer->SetAltarfPhyDamageAppend( pPlayer->GetAltarfPhyDamageAppend() + iValue );
		break;
	case FIELD_MAGICDAMAGEAPPEND:
		if ( iValue < 0 &&  pPlayer->GetAltarfMagicDamageAppend() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		pPlayer->SetAltarfMagicDamageAppend(  pPlayer->GetAltarfMagicDamageAppend() + iValue );
		break;
	case FIELD_DAMAGESORB:
		if ( iValue < 0 &&  pPlayer->GetAltarlDamageSorb() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		 pPlayer->SetAltarlDamageSorb( pPlayer->GetAltarlDamageSorb() + iValue );
		break;
	case FIELD_BECURE:
		if ( iValue < 0 &&  pPlayer->GetAltarfBeCure() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		pPlayer->SetAltarfBeCure( pPlayer->GetAltarfBeCure() + iValue );
		break;          
	case FIELD_CURE:
		if ( iValue < 0 &&  pPlayer->GetAltarfCure() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		pPlayer->SetAltarfCure( pPlayer->GetAltarfCure() + iValue );
		break;
	case FIELD_STATUSHIT:
		if ( iValue < 0 &&  pPlayer->GetAltarlStatusHit() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		pPlayer->SetAltarlStatusHit( pPlayer->GetAltarlStatusHit() + iValue );
		break;
	case FIELD_STATUDDODGE:
		if ( iValue < 0 &&  pPlayer->GetAltarlStatudDodge() < abs( iValue ) )
		{
			Py_XINCREF(Py_False);
			return Py_False;
		}
		pPlayer->SetAltarlStatudDodge( pPlayer->GetAltarlStatudDodge() + iValue );
		break;
	default:
		break;
	}
	PyErr_Print();
	PyErr_Clear();
	pPlayer->SynGameData(true);

	Py_XINCREF(Py_True);
	return Py_True;
}
PyObject* GuildQuestRelease(PyObject* self , PyObject* args)
{

	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int PlayerAddr = PyInt_AsLong(arg0);
	int type = PyInt_AsLong(arg1);

	//处理公会任务
	CPlayer *pPlayer = (CPlayer*)PlayerAddr;
	pPlayer->H_ConsortiaQuest( type );

	Py_XINCREF(Py_True);
	return Py_True;
}

//获取玩家公会的最大祭坛
PyObject* GetUnionMaxAltar(PyObject* self , PyObject* args)		
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);

	int PlayerAddr = PyInt_AsLong(arg0);
	int Value = 0;

	//祭坛升级
	CPlayer *pPlayer = (CPlayer*)PlayerAddr;
	if (  Value = pPlayer->GetConsortiaMaxAltar() )
		return PyInt_FromLong(Value);

	return PyInt_FromLong(0);
}
//返回玩家现有供奉的祭坛
PyObject* GetHavePlayerAltar(PyObject* self , PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	int id  = PyInt_AsLong(arg0);
	CPlayer *pPlayer = (CPlayer*)id;
	vector<int>& Has = pPlayer->GetHavePlayerAltars();
	vector<int>::iterator itor = Has.begin();
	PyObject *AltarsTuple  = PyList_New(Has.size()) ;

	int Num = 0;
	for ( ; itor!= Has.end(); itor++ )
		PyList_SetItem( AltarsTuple, Num++, PyInt_FromLong(*itor) );

	return AltarsTuple;
}

PyObject*	ChangeChatBubble(PyObject* self, PyObject* args)
{

	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);
	PyObject* arg3 = PyTuple_GET_ITEM(args, 3);

	int id		 = PyInt_AsLong(arg0);	
	int style	 = PyInt_AsLong(arg2);	
	
	CPlayer* pPlayer = (CPlayer *)( id );

	if (!pPlayer)
	{
		Py_XINCREF(Py_False);
		return	Py_False;
	}
	
	pPlayer->SetChatBubbleType((BYTE)style);	

	// 发送执行消息用于测试		
	
	wstring unFound = L"您已经更改了聊天气泡样式";
	MSG_TALK	msg_talk;
	msg_talk.ucAimPlayerNameSize = 0;
	msg_talk.ucResPlayerNameSize = 0;
	msg_talk.ucTextSize = (INT)((unFound.size() + 1 )*2);
	msg_talk.ucType = defSYSTEM;
	msg_talk.uiUserID = -1;
	wmemset(msg_talk.wcContent, 0, 256);
	wcscpy(msg_talk.wcContent, unFound.c_str());
	msg_talk.Head.usType = _MSG_TALK;
	/*msg_talk.Head.usSize = (INT)(sizeof(MSG_TALK) - 512 + msg_talk.ucTextSize);*/
	msg_talk.Head.usSize = sizeof(msg_talk.Head) + 4 + 4 + 4;//Head + INT + INT + BYTE*4
	if ( (msg_talk.ucTextSize%4) != 0 )//不能被四整除
		msg_talk.Head.usSize += msg_talk.ucTextSize + 2;
	else
		msg_talk.Head.usSize += msg_talk.ucTextSize;

	pPlayer->s_World->SendMsgToClient(&msg_talk, pPlayer->GetSocket());

	//必须调用下这个，为什么呢？
	PyErr_Print();
	PyErr_Clear();

	Py_XINCREF(Py_True);
	return	Py_True;
}

PyObject*	AddLevel(PyObject* self, PyObject* args)
{

	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);

	int id		 = PyInt_AsLong(arg0);	
	int deltaLevel	 = PyInt_AsLong(arg1);	

	CPlayer* pPlayer = (CPlayer*)( id );

	if (!pPlayer)
	{
		Py_XINCREF(Py_False);
		return	Py_False;
	}
	if (deltaLevel<=0)
	{
		ASSERT(0);//Skill.py配置错误
		Py_XINCREF(Py_False);
		return	Py_False;
	}

	int Level = pPlayer->GetcRank() + deltaLevel;
	if (Level > PLAYER_MAXLV)
		Level = PLAYER_MAXLV; //增加的等级已经超过最大限制
	pPlayer->SetcRank( (BYTE)Level );
	pPlayer->PlayerUpGrade(true);	

	// 发送执行消息用于测试		

	wstring unFound = L"您已经提升了自身等级";
	MSG_TALK	msg_talk;
	msg_talk.ucAimPlayerNameSize = 0;
	msg_talk.ucResPlayerNameSize = 0;
	msg_talk.ucTextSize = (INT)((unFound.size() + 1 )*2);
	msg_talk.ucType = defSYSTEM;
	msg_talk.uiUserID = -1;
	wmemset(msg_talk.wcContent, 0, 256);
	wcscpy(msg_talk.wcContent, unFound.c_str());
	msg_talk.Head.usType = _MSG_TALK;
	/*msg_talk.Head.usSize = (INT)(sizeof(MSG_TALK) - 512 + msg_talk.ucTextSize);*/
	msg_talk.Head.usSize = sizeof(msg_talk.Head) + 4 + 4 + 4;//Head + INT + INT + BYTE*4
	if ( (msg_talk.ucTextSize%4) != 0 )//不能被四整除
		msg_talk.Head.usSize += msg_talk.ucTextSize + 2;
	else
		msg_talk.Head.usSize += msg_talk.ucTextSize;

	pPlayer->s_World->SendMsgToClient(&msg_talk, pPlayer->GetSocket());

	//必须调用下这个，为什么呢？
	PyErr_Print();
	PyErr_Clear();

	Py_XINCREF(Py_True);
	return	Py_True;
}

/*------------------------------------------------------------------------------
*作者：C.D
*日期：2008-06-16
*描述：商城道具添加
-----------------------------------------------------------------------------*/
PyObject*	TimeDataChange(PyObject* self, PyObject* args)
{

	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);


	int iArg0	 = PyInt_AsLong(arg0);	
	int iArg1	 = PyInt_AsLong(arg2);
	int iArg2	 = PyInt_AsLong(arg2);

	//经验、回复、神恩药剂
	
	PyErr_Print();
	PyErr_Clear();

	Py_XINCREF(Py_True);
	return	Py_True;//or return	Py_False;	
}

/*------------------------------------------------------------------------------
*作者：lt
*日期：2008-06-17
*描述：商城道具添加
-----------------------------------------------------------------------------*/
PyObject*	GetConsortiaData(PyObject* self, PyObject* args)
{

	//公会银符4660 
	//公会金符4670 
	//公会勋章4680 
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);

	int iArg0	 = PyInt_AsLong(arg0);	
	int iArg1	 = PyInt_AsLong(arg1);
	int iArg2	 = PyInt_AsLong(arg2);
	CPlayer* pPlayer = (CPlayer*)( iArg0 );

	//公会银符、金符、勋章
	if(!iArg1)
	{ pPlayer->SetConsortia_lfound(iArg2); }
	else if( iArg1 == 1) { pPlayer->SetConsortia_lbijou(iArg2); }
	else if( iArg1 == 2) 
	{
		int Uion=pPlayer->GetuiUionContribute();
		pPlayer->SetuiUionContribute(Uion+iArg2);
	}
	pPlayer->BroadcastConsortiaBaseInfo();

	pPlayer->SubOverlap();

	// 发送动作执行消息
	int id		= PyInt_AsLong(arg0);
	CGameObject*		obj		= (CGameObject*)(id);
	int magic	= PyInt_AsLong(arg2);
	MSG_MAGIC_PERFORM mgicPerform;

	mgicPerform.Head.usSize = sizeof( MSG_MAGIC_PERFORM );
	mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
	mgicPerform.uiID		= obj->GetID();
	mgicPerform.x			= static_cast<CPlayer*>(obj)->m_SkillMsg.x1;
	mgicPerform.z			= static_cast<CPlayer*>(obj)->m_SkillMsg.z1;
	mgicPerform.uiObjectID	= static_cast<CPlayer*>(obj)->GetID();
	mgicPerform.ucMagicID	= magic;
	//发送魔法Perform消息
	obj->GetRegion()->SendAreaMsgOneToOther( obj->GetCurrentCell(), &mgicPerform );

	PyErr_Print();
	PyErr_Clear();

	Py_XINCREF(Py_True);
	return	Py_True;//or return	Py_False;	
}

/*------------------------------------------------------------------------------
*作者：lt
*日期：2008-06-17
*描述：商城道具添加
-----------------------------------------------------------------------------*/
PyObject*	ChangeStorage(PyObject* self, PyObject* args)
{

	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);

	int iArg0	 = PyInt_AsLong(arg0);	
	int iArg1	 = PyInt_AsLong(arg1);
	int iArg2	 = PyInt_AsLong(arg2);
	CPlayer* pPlayer = (CPlayer*)( iArg0 );

	//增加仓库空间 4690
	int Num=(pPlayer->GetuiStoreNum()+1);
	if(Num>DEFAULT_STORAGE_COUNT){Num=DEFAULT_STORAGE_COUNT;}
	pPlayer->SetuiStoreNum(Num);
	pPlayer->SendStorageMsg();

	pPlayer->SubOverlap();

	// 发送动作执行消息
	int id		= PyInt_AsLong(arg0);
	CGameObject*		obj		= (CGameObject*)(id);
	int magic	= PyInt_AsLong(arg2);
	MSG_MAGIC_PERFORM mgicPerform;

	mgicPerform.Head.usSize = sizeof( MSG_MAGIC_PERFORM );
	mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
	mgicPerform.uiID		= obj->GetID();
	mgicPerform.x			= static_cast<CPlayer*>(obj)->m_SkillMsg.x1;
	mgicPerform.z			= static_cast<CPlayer*>(obj)->m_SkillMsg.z1;
	mgicPerform.uiObjectID	= static_cast<CPlayer*>(obj)->GetID();
	mgicPerform.ucMagicID	= magic;
	//发送魔法Perform消息
	obj->GetRegion()->SendAreaMsgOneToOther( obj->GetCurrentCell(), &mgicPerform );

	PyErr_Print();
	PyErr_Clear();

	Py_XINCREF(Py_True);
	return	Py_True;//or return	Py_False;	
}

/*------------------------------------------------------------------------------
*作者：lt
*日期：2008-06-17
*描述：商城道具添加
-----------------------------------------------------------------------------*/
PyObject*	ChangeBag(PyObject* self, PyObject* args)
{

	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);


	int iArg0	 = PyInt_AsLong(arg0);	
	int iArg1	 = PyInt_AsLong(arg1);
	int iArg2	 = PyInt_AsLong(arg2);
	CPlayer* pPlayer = (CPlayer*)( iArg0 );
	
	//增加包裹空间4695
	int Num=(pPlayer->GetuiBagNum()+1);
	if(Num>4)
	{
		Num=4;
		Py_XINCREF(Py_False);
		return Py_False;
	}

	pPlayer->SetuiBagNum(Num);
	pPlayer->SendKitBagMsg( 1 );

	pPlayer->SubOverlap();

	// 发送动作执行消息
	int id		= PyInt_AsLong(arg0);
	CGameObject*		obj		= (CGameObject*)(id);
	int magic	= PyInt_AsLong(arg2);
	MSG_MAGIC_PERFORM mgicPerform;

	mgicPerform.Head.usSize = sizeof( MSG_MAGIC_PERFORM );
	mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
	mgicPerform.uiID		= obj->GetID();
	mgicPerform.x			= static_cast<CPlayer*>(obj)->m_SkillMsg.x1;
	mgicPerform.z			= static_cast<CPlayer*>(obj)->m_SkillMsg.z1;
	mgicPerform.uiObjectID	= static_cast<CPlayer*>(obj)->GetID();
	mgicPerform.ucMagicID	= magic;
	//发送魔法Perform消息
	obj->GetRegion()->SendAreaMsgOneToOther( obj->GetCurrentCell(), &mgicPerform );

	PyErr_Print();
	PyErr_Clear();

	Py_XINCREF(Py_True);
	return	Py_True;//or return	Py_False;	
}

/*------------------------------------------------------------------------------
*作者：lt
*日期：2008-06-17
*描述：商城道具添加
-----------------------------------------------------------------------------*/
PyObject*	ChangeHairstyle(PyObject* self, PyObject* args)
{

	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);


	int iArg0	 = PyInt_AsLong(arg0);	
	int iArg1	 = PyInt_AsLong(arg1);
	int iArg2	 = PyInt_AsLong(arg2);
	CPlayer* pPlayer = (CPlayer*)( iArg0 );

	char Ifbuf[128];
	_stprintf(Ifbuf,"改变头发s:%d",pPlayer->GetcHairStyle());
	//sbase::ConsoleWriteColorText( FOREGROUND_RED,Ifbuf );

	pPlayer->SetcHairStyle(10*iArg1);//10,20,30是头发类型 10+1,+2,+3为变颜色

	//改变头发 4700

	_stprintf(Ifbuf,"改变头发d:%d",pPlayer->GetcHairStyle());
	//sbase::ConsoleWriteColorText( FOREGROUND_RED,Ifbuf );

	pPlayer->SubOverlap();

	pPlayer->SendUseOrEquip(0);

	// 发送动作执行消息
	int id		= PyInt_AsLong(arg0);
	CGameObject*		obj		= (CGameObject*)(id);
	int magic	= PyInt_AsLong(arg2);
	MSG_MAGIC_PERFORM mgicPerform;

	mgicPerform.Head.usSize = sizeof( MSG_MAGIC_PERFORM );
	mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
	mgicPerform.uiID		= obj->GetID();
	mgicPerform.x			= static_cast<CPlayer*>(obj)->m_SkillMsg.x1;
	mgicPerform.z			= static_cast<CPlayer*>(obj)->m_SkillMsg.z1;
	mgicPerform.uiObjectID	= static_cast<CPlayer*>(obj)->GetID();
	mgicPerform.ucMagicID	= magic;
	//发送魔法Perform消息
	obj->GetRegion()->SendAreaMsgOneToOther( obj->GetCurrentCell(), &mgicPerform );

	PyErr_Print();
	PyErr_Clear();

	Py_XINCREF(Py_True);
	return	Py_True;//or return	Py_False;	
}

/*------------------------------------------------------------------------------
*作者：lt
*日期：2008-06-17
*描述：商城道具添加
-----------------------------------------------------------------------------*/
PyObject*	ChangeHaircolor(PyObject* self, PyObject* args)
{

	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);
	PyObject* arg1 = PyTuple_GET_ITEM(args, 1);
	PyObject* arg2 = PyTuple_GET_ITEM(args, 2);


	int iArg0	 = PyInt_AsLong(arg0);	
	int iArg1	 = PyInt_AsLong(arg1);
	int iArg2	 = PyInt_AsLong(arg2);
	CPlayer* pPlayer = (CPlayer*)( iArg0 );

	char Ifbuf[128];
	_stprintf(Ifbuf,"改变头发s:%d",pPlayer->GetcHairStyle());
	//sbase::ConsoleWriteColorText( FOREGROUND_RED,Ifbuf );

	int CurHair=pPlayer->GetcHairStyle();
	pPlayer->SetcHairStyle(CurHair+iArg1);//10,20,30是头发类型 10+1,+2,+3为变颜色

	//改变头发色4710
	_stprintf(Ifbuf,"改变头发d:%d",pPlayer->GetcHairStyle());
	//sbase::ConsoleWriteColorText( FOREGROUND_RED,Ifbuf );

	pPlayer->SubOverlap();

	pPlayer->SendUseOrEquip(0);


	// 发送动作执行消息
	int id		= PyInt_AsLong(arg0);
	CGameObject*		obj		= (CGameObject*)(id);
	int magic	= PyInt_AsLong(arg2);
	MSG_MAGIC_PERFORM mgicPerform;

	mgicPerform.Head.usSize = sizeof( MSG_MAGIC_PERFORM );
	mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
	mgicPerform.uiID		= obj->GetID();
	mgicPerform.x			= static_cast<CPlayer*>(obj)->m_SkillMsg.x1;
	mgicPerform.z			= static_cast<CPlayer*>(obj)->m_SkillMsg.z1;
	mgicPerform.uiObjectID	= static_cast<CPlayer*>(obj)->GetID();
	mgicPerform.ucMagicID	= magic;
	//发送魔法Perform消息
	obj->GetRegion()->SendAreaMsgOneToOther( obj->GetCurrentCell(), &mgicPerform );

	PyErr_Print();
	PyErr_Clear();

	Py_XINCREF(Py_True);
	return	Py_True;//or return	Py_False;	
}


//------------------------------------------------------------------------------
/*------------------------------------------------------------------------------
*作者：C.D
*日期：2008-06-17
*描述：商城道具添加
-----------------------------------------------------------------------------*/
PyObject*	ChangePK(PyObject* self, PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);

	PyObject* arg1 = PyTuple_GET_ITEM(args, 2);


	int id		= PyInt_AsLong(arg0);	
	int Type	= PyInt_AsLong(arg1);

	CPlayer* pPlayer = (CPlayer*)( id );

	if(0 == Type)
		pPlayer->SetPKProtected(false);
	else
		pPlayer->SetPKProtected(true);

	PyErr_Print();
	PyErr_Clear();

	Py_XINCREF(Py_True);
	return	Py_True;//or return	Py_False;	
}

PyObject*	ChangeCamp(PyObject* self, PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);

	PyObject* arg1 = PyTuple_GET_ITEM(args, 2);


	int id		= PyInt_AsLong(arg0);	
	int Type	= PyInt_AsLong(arg1);

	CPlayer* pPlayer = (CPlayer*)( id );

	if(0 == Type)
		pPlayer->SetGreenName(false);
	else
		pPlayer->SetGreenName(true);

	PyErr_Print();
	PyErr_Clear();

	Py_XINCREF(Py_True);
	return	Py_True;//or return	Py_False;
}

PyObject*	ChangeHideID(PyObject* self, PyObject* args)
{
	PyObject* arg0 = PyTuple_GET_ITEM(args, 0);

	PyObject* arg1 = PyTuple_GET_ITEM(args, 2);


	int id		= PyInt_AsLong(arg0);	
	int Type	= PyInt_AsLong(arg1);

	CPlayer* pPlayer = (CPlayer*)( id );

	if(0 == Type)
		pPlayer->SetFalseName(false);
	else
		pPlayer->SetFalseName(true);

	PyErr_Print();
	PyErr_Clear();

	Py_XINCREF(Py_True);
	return	Py_True;//or return	Py_False;
}

//------------------------------------------------------------------------------