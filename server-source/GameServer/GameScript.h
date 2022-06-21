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
extern PyObject* AltarCreate(PyObject* self , PyObject* args);    //������̳
extern PyObject* AltarLevelup(PyObject* self , PyObject* args);  //������̳
extern PyObject* AltarDel(PyObject* self , PyObject* args);      //ɾ����̳
extern PyObject* OblationLevelup(PyObject* self , PyObject* args); //�����̳
extern PyObject* GetUnionMaxAltar(PyObject* self , PyObject* args);					//��ȡ��ҹ��������̳
extern PyObject* GetHavePlayerAltar(PyObject* self , PyObject* args);			//����������й���ļ�̳
extern PyObject* GetHaveAltars(PyObject* self , PyObject* args);
extern PyObject* GetPlayerAttr(PyObject* self , PyObject* args);
extern PyObject* AcceptGiftItem(PyObject* self , PyObject* args);
extern PyObject* AcceptGiftAttr(PyObject* self , PyObject* args);
extern PyObject* GuildQuestRelease(PyObject* self , PyObject* args);
extern PyObject* ChangeChatBubble(PyObject* self, PyObject* args);	//�ı�����������ʽ
extern PyObject* AddLevel(PyObject* self, PyObject* args);	//������ҵȼ�
extern PyObject* AddGlory(PyObject* self, PyObject* args);	//��������
extern PyObject* AddStone(PyObject* self, PyObject* args);	//���ӽ��
extern PyObject* AddConvertPoint(PyObject* , PyObject* args);

/*------------------------------------------------------------------------------
 *���ߣ�C.D
 *���ڣ�2008-06-16
 *�������̳ǵ������
 -----------------------------------------------------------------------------*/
extern PyObject* TimeDataChange(PyObject* self, PyObject* args);//���顢�ظ������ҩ��
extern PyObject* GetConsortiaData(PyObject* self, PyObject* args);//���������������ѫ��
extern PyObject* ChangeStorage(PyObject* self, PyObject* args);//���Ӳֿ�ռ�
extern PyObject* ChangeBag(PyObject* self, PyObject* args);//���Ӱ����ռ�
extern PyObject* ChangeHairstyle(PyObject* self, PyObject* args);//�ı�ͷ��
extern PyObject* ChangeHaircolor(PyObject* self, PyObject* args);//�ı�ͷ��ɫ
//------------------------------------------------------------------------------
/*------------------------------------------------------------------------------
*���ߣ�C.D
*���ڣ�2008-06-17
*�������̳ǵ������
-----------------------------------------------------------------------------*/
extern PyObject* ChangePK(PyObject* self, PyObject* args);//�Ƿ�����PK
extern PyObject* ChangeCamp(PyObject* self, PyObject* args);//�ı�������ɫ
extern PyObject* ChangeHideID(PyObject* self, PyObject* args);//�ı����������ʾ
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
