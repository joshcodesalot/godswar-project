#ifndef _GAMESCRIPT_H_
#define _GAMESCRIPT_H_

#include "GameCore/Player.h"
#include "GameCore/NPC.h"

#include "GameCore/QuestDef.h"

 struct pyScriptObject
 {
 	PyObject_HEAD
 	ScriptObject obj;
 };


extern void AddHP(PyObject* self , PyObject* args);
extern void AddMP(PyObject* self , PyObject* args);
extern void PlayerAttack(PyObject* self , PyObject* args);
extern void ObjMagicAttack(PyObject* self , PyObject* args);
extern void AddHPByMagic(PyObject* self , PyObject* args);
extern void AddEXP(PyObject* self , PyObject* args);
extern void AddSkillEXP(PyObject* self , PyObject* args);
extern void AddMoney(PyObject* self , PyObject* args);
extern void AddCredit(PyObject* self , PyObject* args);
extern void AddContribute(PyObject* self , PyObject* args);
extern PyObject* Fly(PyObject* self , PyObject* args);
extern void ChangePlayeBuniness(PyObject* self , PyObject* args);
extern void AddStatus(PyObject* self , PyObject* args);
extern void Learn(PyObject*self , PyObject* args);
extern void AddMultiHP(PyObject* self , PyObject* args);
extern PyObject* AltarCreate(PyObject* self , PyObject* args);    //创建祭坛
extern PyObject* AltarLevelup(PyObject* self , PyObject* args);  //升级祭坛
extern PyObject* AltarDel(PyObject* self , PyObject* args);      //删除祭坛
extern PyObject* OblationLevelup(PyObject* self , PyObject* args); //供奉祭坛
extern PyObject* GetUnionMaxAltar(PyObject* self , PyObject* args);					//获取玩家公会的最大祭坛
extern PyObject* GetHavePlayerAltar(PyObject* self , PyObject* args);			//返回玩家现有供奉的祭坛
extern PyObject* GetHaveAltars(PyObject* self , PyObject* args);
extern PyObject* GetPlayerAttr(PyObject* self , PyObject* args);
extern PyObject* AcceptGiftItem(PyObject* self , PyObject* args);
extern PyObject* AcceptGiftAttr(PyObject* self , PyObject* args);
extern PyObject* GuildQuestRelease(PyObject* self , PyObject* args);
extern PyObject* ChangeChatBubble(PyObject* self, PyObject* args);	//改变聊天气泡样式
extern PyObject* AddLevel(PyObject* self, PyObject* args);	//增加玩家等级
extern PyObject* AddGlory(PyObject* self, PyObject* args);	//增加声望
extern PyObject* AddStone(PyObject* self, PyObject* args);	//增加金币
extern PyObject* AddConvertPoint(PyObject* , PyObject* args);

/*------------------------------------------------------------------------------
 *作者：C.D
 *日期：2008-06-16
 *描述：商城道具添加
 -----------------------------------------------------------------------------*/
extern PyObject* TimeDataChange(PyObject* self, PyObject* args);//经验、回复、神恩药剂
extern PyObject* GetConsortiaData(PyObject* self, PyObject* args);//公会银符、金符、勋章
extern PyObject* ChangeStorage(PyObject* self, PyObject* args);//增加仓库空间
extern PyObject* ChangeBag(PyObject* self, PyObject* args);//增加包裹空间
extern PyObject* ChangeHairstyle(PyObject* self, PyObject* args);//改变头发
extern PyObject* ChangeHaircolor(PyObject* self, PyObject* args);//改变头发色
//------------------------------------------------------------------------------
/*------------------------------------------------------------------------------
*作者：C.D
*日期：2008-06-17
*描述：商城道具添加
-----------------------------------------------------------------------------*/
extern PyObject* ChangePK(PyObject* self, PyObject* args);//是否允许PK
extern PyObject* ChangeCamp(PyObject* self, PyObject* args);//改变名字颜色
extern PyObject* ChangeHideID(PyObject* self, PyObject* args);//改变玩家姓名显示
//------------------------------------------------------------------------------

static PyMemberDef pyPlayer_members[] = {
	{"ID" , T_INT , offsetof(pyScriptObject , obj.ID) , RESTRICTED , ""},
	{"Rank" , T_INT , offsetof(pyScriptObject , obj.Rank) , RESTRICTED , ""},

	{NULL}
};

static PyMethodDef pyPlayer_methods[] = {
	{NULL}  /* Sentinel */
};

static PyTypeObject pyPlayer_type = {
	PyObject_HEAD_INIT( NULL )
	0,		/* ob_size */
	"player",		/* tp_name */
	sizeof(pyScriptObject),	/* tp_basicsize */
	0,			/* tp_itemsize */
	/* methods */
	0,	/* tp_dealloc */
	0,			/* tp_print */
	0,	/* tp_getattr */
	0,	/* tp_setattr */
	0,			/* tp_compare */
	0,	/* tp_repr */
	0,			/* tp_as_number */
	0,			/* tp_as_sequence */
	0,			/* tp_as_mapping */
	0,			/* tp_as_hash */
	0, 0, 0, 0, 0, Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
	0,			/* tp_doc */
	0, 0, 0, 0, 0, 0,
	0,	/* tp_methods */
	pyPlayer_members,		/* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	0,      /* tp_init */
	0,                         /* tp_alloc */
	0,                 /* tp_new */
};


#endif
