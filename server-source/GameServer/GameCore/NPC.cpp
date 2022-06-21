#include "stdafx.h"
#include "NPC.h"
#include "Region.h"
#include "Player.h"
#include "ItemManager.h"
#include "QuestManager.h"
#include "..\ServiceLoader.h"
#include "..\DataCore.h"
#include "..\ServiceLoader.h"

#include "..\..\Common\MsgDefine.h"
#include "..\..\Common\MsgTypes.h"

#include "../GameScript.h"


extern MSG_TALK_NPC msg_talk_npc;
extern MSG_NPC_DATA msg_npc_data;
extern MSG_NPC_ITEMDATA msg_npc_itemdata;
extern MSG_NPC_STORAGEDATA msg_npc_storagedata;
extern MSG_NPC_SELL msg_npc_sell;
extern MSG_NPC_QUESTS msg_npc_quests;
extern MSG_NPC_QUESTREWARD msg_npc_questreward;


CNPC::CNPC(void)
:	m_Module(NULL)
{
	m_eType = OBJECTTYPE_NPC;
	m_Flag = (NPCFlag)(NPC_FLAG_BANKER + NPC_FLAG_VENDOR);
	m_SysFlag = 0;
}

CNPC::~CNPC(void)
{}

bool CNPC::InitObjData( long   )
{

	return true;
}

void CNPC::ReloadSellingList()
{
	m_Module = ScriptSystem::Instance()->Reload(m_ScriptName.c_str());

	m_Items.clear();
	m_ItemBases.clear();
	m_Overlap.clear();
	m_BaseLevel.clear();
	m_AppendLevel.clear();
	m_Money.clear();

	GetType();
	LoadItemList();
}

void CNPC::ReloadQuests()
{
	m_Module = ScriptSystem::Instance()->Reload(m_ScriptName.c_str());

	for (int i = 0 ; i < QUEST_NPCTAKE_COUNT ; i++)
	{
		if (m_Quests[i].QuestID != -1)
			QuestManager::Instance()->EraseQuest(m_Quests[i].QuestID);

		m_Quests[i].Clear();

		if (m_QuestReward[i].QuestID != -1)
			QuestManager::Instance()->EraseQuestReward(m_QuestReward[i].QuestID);

		m_QuestReward[i].Clear();
	}

	GetType();
	LoadQuest();
	LoadQuestReward();
}

void CNPC::Clear()
{
	m_Items.clear();
	m_ItemBases.clear();
	m_Overlap.clear();
	m_BaseLevel.clear();
	m_AppendLevel.clear();
	m_Money.clear();

	for (int i = 0 ; i < QUEST_NPCTAKE_COUNT ; i++)
	{
		if (m_Quests[i].QuestID != -1)
			QuestManager::Instance()->EraseQuest(m_Quests[i].QuestID);

		m_Quests[i].Clear();

		if (m_QuestReward[i].QuestID != -1)
			QuestManager::Instance()->EraseQuest(m_QuestReward[i].QuestID);

		m_QuestReward[i].Clear();
	}

	m_Module = NULL;
}

void CNPC::GetData(CPlayer* player)
{
	if (HasFlag(NPC_FLAG_VENDOR))
		GetItemList(player);

	if (HasFlag(NPC_FLAG_BANKER))
		GetStorageData(player);
}

void CNPC::SetScriptName(const char* name)
{
	Clear();

	if (!name)
		return;

	if (PyType_Ready(&pyPlayer_type) < 0)
		return;

	m_ScriptName = name;

	if (m_ScriptName.empty())
		return;

	PyErr_Clear();

	m_Module = ScriptSystem::Instance()->ImportModule(name);

	if (!m_Module)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	Py_XINCREF(&pyPlayer_type);

	PyModule_AddObject(m_Module , "player", (PyObject*)&pyPlayer_type);

	CGameObject::s_World->g_ScriptNPC[name] = this->GetID();

	GetType();
	LoadItemList();
	LoadItemListType();
	LoadQuest();
	LoadQuestReward();
}

const char* CNPC::GetScriptName()
{
	return m_ScriptName.c_str();
}

void CNPC::GetType()
{
	if (!m_Module)
		return;

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;

	pFunc = PyObject_GetAttrString(m_Module , "GetType");

	if (!pFunc)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	pValue = PyObject_CallObject(pFunc, 0);

	if (!pValue)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	m_Flag = PyInt_AsLong(pValue);
	Py_XDECREF(pValue);

	if ( HasFlag( NPC_FLAG_SYS )  )
	{
		pFunc = PyObject_GetAttrString(m_Module , "GetSysType");

		if (!pFunc)
		{
			PyErr_Print();
			PyErr_Clear();
			return;
		}

		pValue = PyObject_CallObject(pFunc, 0);

		if (!pValue)
		{
			PyErr_Print();
			PyErr_Clear();

			return;
		}

		m_SysFlag = PyInt_AsLong(pValue);

		Py_XDECREF(pValue);
	}
}

void CNPC::LoadSysGift()
{
}

void CNPC::LoadItemList()
{
	if (!m_Module)
		return;

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;

	pFunc = PyObject_GetAttrString(m_Module , "GetItemList");

	if (!pFunc)
	{
		PyErr_Clear();

		return;
	}

	pValue = PyObject_CallObject(pFunc, 0);

	if (!pValue)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	int tuplesize = 0;

	if (PyTuple_Check(pValue))
		tuplesize = PyTuple_Size(pValue);
	else
		return;

	for (int i = 0 ; i < tuplesize ; i++)
	{
		vector<NPCItemData > datas;

		PyObject* list = PyTuple_GetItem(pValue , i);

		int size = PyList_Size(list);

		if (size == 0)
			continue;



		for (int j = 0 ; j < size ; j++)
		{
			NPCItemData data;

			PyObject* listitem = PyList_GetItem(list , j);

			PyClassObject* obj = (PyClassObject*)listitem;

			PyObject* Base = PyDict_GetItemString(obj->cl_dict,"Base");
			PyObject* BaseLV = PyDict_GetItemString(obj->cl_dict,"BaseLv");
			PyObject* Append = PyDict_GetItemString(obj->cl_dict,"Append");
			PyObject* AppendLV = PyDict_GetItemString(obj->cl_dict,"AppendLv");
			PyObject* Overlap = PyDict_GetItemString(obj->cl_dict,"Overlap");
			PyObject* Binding = PyDict_GetItemString(obj->cl_dict,"Binding");
			PyObject* Type = PyDict_GetItemString(obj->cl_dict,"Type");
			PyObject* Consume = PyDict_GetItemString(obj->cl_dict,"Consume");

			data.Base = PyInt_AsLong(Base);
			data.BaseLV = PyInt_AsLong(BaseLV);
			data.AppendLV = PyInt_AsLong(AppendLV);
			data.Overlap = PyInt_AsLong(Overlap);
			data.Binding = PyInt_AsLong(Binding);
			for (int z = 0 ; z < MAX_EQUIPAPPEND_COUNT ; z++)
			{
				PyObject* append = PyList_GetItem(Append , z);

				data.Append[z] = PyInt_AsLong(append);
			}

			int  Num = PyList_Size(Type);
			for ( int a = 0 ; a < Num ; a++ )
			{
				PyObject* append = PyList_GetItem(Type , a);
				data.Trade[NPCItemData::ITEM_SELL_TYPE(PyInt_AsLong(append))] = PyInt_AsLong(PyList_GetItem(Consume , a));
			}

			datas.push_back(data);
			m_ItemBases.push_back(data.Base);
			m_Overlap.push_back(data.Overlap);
			m_BaseLevel.push_back(data.BaseLV);
			m_AppendLevel.push_back(data.AppendLV);

			Item item;
			ItemManager::Instance()->CreateItem(&data , &item);

			if (item.IsClear())
			{
				printf("Item Error , ID=%d" , data.Base);
				ASSERT(0);
			}
		}

		m_Items.push_back(datas);

	}

	Py_XDECREF(pValue);

}

void CNPC::LoadQuest()
{
	if (!m_Module)
		return;

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;

	pFunc = PyObject_GetAttrString(m_Module , "GetQuest");

	if (!pFunc)
	{
		PyErr_Clear();

		return;
	}

	pValue = PyObject_CallObject(pFunc, 0);

	if (!pValue)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	int tuplesize = 1;

	if (PyTuple_Check(pValue) == 1)
		tuplesize = PyTuple_Size(pValue);

	for (int i = 0 ; i < tuplesize ; i++)
	{
		PyObject* Quest = NULL;

		if (PyTuple_Check(pValue) == 1)
			Quest = PyTuple_GetItem(pValue , i);
		else
			Quest = pValue;

		PyClassObject* obj = (PyClassObject*)Quest;

		PyObject* QuestID = PyDict_GetItemString(obj->cl_dict,"QuestID");

		PyObject* QuestSort = PyDict_GetItemString(obj->cl_dict,"QuestSort");

		PyObject* PrevQuestId = PyDict_GetItemString(obj->cl_dict,"PrevQuestId");
		PyObject* NextQuestId = PyDict_GetItemString(obj->cl_dict,"NextQuestId");
		PyObject* NextQuestsId = PyDict_GetItemString(obj->cl_dict,"NextQuestsId");

		PyObject* MinLevel = PyDict_GetItemString(obj->cl_dict,"MinLevel");
		PyObject* MaxLevel = PyDict_GetItemString(obj->cl_dict,"MaxLevel");

		PyObject* RequiredLevel = PyDict_GetItemString(obj->cl_dict,"RequiredLevel");
		PyObject* RequiredClass = PyDict_GetItemString(obj->cl_dict,"RequiredClass");
		PyObject* RequiredBelief = PyDict_GetItemString(obj->cl_dict,"RequiredBelief");
		PyObject* RequiredFaction = PyDict_GetItemString(obj->cl_dict,"RequiredFaction");

		PyObject* MinTime = PyDict_GetItemString(obj->cl_dict,"MinTime");
		PyObject* MaxTime = PyDict_GetItemString(obj->cl_dict,"MaxTime");

		PyObject* MaxNum = PyDict_GetItemString(obj->cl_dict,"MaxNum");
		PyObject* SpecialFlags = PyDict_GetItemString(obj->cl_dict,"SpecialFlags");
		PyObject* Money = PyDict_GetItemString(obj->cl_dict,"Money");
		PyObject* Bijou = PyDict_GetItemString(obj->cl_dict,"Bijou");

		PyObject* Item = PyDict_GetItemString(obj->cl_dict,"Item");
		PyObject* Creature = PyDict_GetItemString(obj->cl_dict,"Creature");
		PyObject* ItemCount = PyDict_GetItemString(obj->cl_dict,"ItemCount");
		PyObject* CreatureCount = PyDict_GetItemString(obj->cl_dict,"CreatureCount");
		PyObject* ItemProbability = PyDict_GetItemString(obj->cl_dict,"ItemProbability");

		PyObject* RewardMoney = PyDict_GetItemString(obj->cl_dict,"RewardMoney");
		PyObject* RewardBijou = PyDict_GetItemString(obj->cl_dict,"RewardBijou");
		PyObject* RewardItem = PyDict_GetItemString(obj->cl_dict,"RewardItem");

		PyObject* Repeat = PyDict_GetItemString(obj->cl_dict , "Repeat");

		PyObject* ExplorerMapID = PyDict_GetItemString(obj->cl_dict , "ExplorerMapID");
		PyObject* ExplorerMapPosition = PyDict_GetItemString(obj->cl_dict , "ExplorerMapPosition");

		PyObject* TeamLimit = PyDict_GetItemString( obj->cl_dict, "TeamLimit");

		PyObject* RewardUionBijou = PyDict_GetItemString( obj->cl_dict, "RewardUionBijou");
		PyObject* RewardUionMoney = PyDict_GetItemString( obj->cl_dict, "RewardUionMoney");
		PyObject* ReqConsortiaLV = PyDict_GetItemString( obj->cl_dict, "ReqConsortiaLV");


		m_Quests[i].NPCID = GetID();

		m_Quests[i].QuestID = PyInt_AsLong(QuestID);
		m_Quests[i].QuestSort = PyInt_AsLong(QuestSort);
		m_Quests[i].PrevQuestId = PyInt_AsLong(PrevQuestId);
		m_Quests[i].NextQuestId = PyInt_AsLong(NextQuestId);
		m_Quests[i].MinLevel = PyInt_AsLong(MinLevel);
		m_Quests[i].MaxLevel = PyInt_AsLong(MaxLevel);

		m_Quests[i].RequiredBelief = PyInt_AsLong(RequiredBelief);
		m_Quests[i].RequiredFaction = PyInt_AsLong(RequiredFaction);
		m_Quests[i].RequiredLevel = PyInt_AsLong(RequiredLevel);

		m_Quests[i].TeamLimit = PyInt_AsLong(TeamLimit);

		m_Quests[i].UionBijou = PyInt_AsLong(RewardUionBijou);
		m_Quests[i].UionMoney = PyInt_AsLong(RewardUionMoney);
		m_Quests[i].ReqConsortiaLV = PyInt_AsLong(ReqConsortiaLV);

		int ExplorerMapNum = 0;
		if (PyTuple_Check(ExplorerMapID) == 1)
		{
			ExplorerMapNum = PyTuple_Size(ExplorerMapID);
			for (int j = 0 ; j < ExplorerMapNum ; j++)
			{
				int id = PyInt_AsLong(PyTuple_GetItem(ExplorerMapID , j));
				int posx = PyInt_AsLong(PyTuple_GetItem(ExplorerMapPosition , j * 2));
				int posz = PyInt_AsLong(PyTuple_GetItem(ExplorerMapPosition , j * 2 + 1));

				if (id != -1){
					ExplorerInfo *info = new ExplorerInfo;
					info->MapID = id;
					info->PosX = posx;
					info->PosY = 0;
					info->PosZ = posz;
					m_Quests[i].ExplorerInfos.push_back(info);
				}
			}
		}
		else
		{
			int id = PyInt_AsLong(ExplorerMapID);
			int posx = PyInt_AsLong(PyTuple_GetItem(ExplorerMapPosition , 0));
			int posz = PyInt_AsLong(PyTuple_GetItem(ExplorerMapPosition , 1));

			if (id != -1){
				ExplorerInfo *info = new ExplorerInfo;
				info->MapID = id;
				info->PosX = posx;
				info->PosY = 0;
				info->PosZ = posz;
				m_Quests[i].ExplorerInfos.push_back(info);
			}
		}
		int NextQuestsNum = 0;

		if (PyTuple_Check(NextQuestsId) == 1)
		{	
			NextQuestsNum = PyTuple_Size(NextQuestsId);

			for (int j = 0 ; j < NextQuestsNum ; j++)
			{
				int n = PyInt_AsLong(PyTuple_GetItem(NextQuestsId , j));

				if (n != -1)
					m_Quests[i].NextQuestsID.push_back(n);
			}
		}
		else
		{
			int n = PyInt_AsLong(NextQuestsId);

			if (n != -1)
				m_Quests[i].NextQuestsID.push_back(n);
		}

		int RequiredClassNum = 0;

		if (PyTuple_Check(RequiredClass) == 1)
		{	
			RequiredClassNum = PyTuple_Size(RequiredClass);

			for (int j = 0 ; j < RequiredClassNum ; j++)
			{
				int n = PyInt_AsLong(PyTuple_GetItem(RequiredClass , j));

				if (n != -1)
					m_Quests[i].RequiredClass.push_back(n);
			}
		}
		else
		{
			int n = PyInt_AsLong(RequiredClass);

			if (n != -1)
				m_Quests[i].RequiredClass.push_back(n);
		}

		tm tmin;
		tm tmax;

		tmin.tm_hour = PyInt_AsLong(PyList_GetItem(MinTime , 0));
		tmin.tm_min = PyInt_AsLong(PyList_GetItem(MinTime , 1));
		tmin.tm_sec = PyInt_AsLong(PyList_GetItem(MinTime , 2));
		tmin.tm_wday = PyInt_AsLong(PyList_GetItem(MinTime , 3));

		tmax.tm_hour = PyInt_AsLong(PyList_GetItem(MaxTime , 0));
		tmax.tm_min = PyInt_AsLong(PyList_GetItem(MaxTime , 1));
		tmax.tm_sec = PyInt_AsLong(PyList_GetItem(MaxTime , 2));
		tmax.tm_wday = PyInt_AsLong(PyList_GetItem(MaxTime , 3));


		if (tmin.tm_wday != -1 && tmax.tm_wday != -1)
		{
			m_Quests[i].TimeStatus = QUEST_TIMESTATUS_WEEK;

			m_Quests[i].MinTime = tmin.tm_wday * 86400 + tmin.tm_hour * 3600 + tmin.tm_min * 60 + tmin.tm_sec;
			m_Quests[i].MaxTime = tmax.tm_wday * 86400 + tmax.tm_hour * 3600 + tmax.tm_min * 60 + tmax.tm_sec;
		}
		else
		{
			m_Quests[i].TimeStatus = QUEST_TIMESTATUS_DAY;

			m_Quests[i].MinTime = tmin.tm_hour * 3600 + tmin.tm_min * 60 + tmin.tm_sec;
			m_Quests[i].MaxTime = tmax.tm_hour * 3600 + tmax.tm_min * 60 + tmax.tm_sec;
		}

		m_Quests[i].MaxNum = PyInt_AsLong(MaxNum);
		m_Quests[i].SpecialFlags = PyInt_AsLong(SpecialFlags);
		m_Quests[i].Money = PyInt_AsLong(Money);
		m_Quests[i].Bijou = PyInt_AsLong(Bijou);
		m_Quests[i].RewardMoney = PyInt_AsLong(RewardMoney);
		m_Quests[i].RewardBijou = PyInt_AsLong(RewardBijou);

		m_Quests[i].Repeat = PyInt_AsLong(Repeat);

		for (int j = 0 ; j < QUEST_OBJECTIVES_COUNT ; j++)
		{
			m_Quests[i].Item[j] = PyInt_AsLong(PyList_GetItem(Item , j));
			m_Quests[i].Creature[j] = PyInt_AsLong(PyList_GetItem(Creature , j));
			m_Quests[i].ItemCount[j] = PyInt_AsLong(PyList_GetItem(ItemCount , j));
			m_Quests[i].CreatureCount[j] = PyInt_AsLong(PyList_GetItem(CreatureCount , j));
			m_Quests[i].ItemProbability[j] = PyInt_AsLong(PyList_GetItem(ItemProbability , j));
		}

		for (int j = 0 ; j < QUEST_REWARDS_COUNT ; j++)
		{
			PyObject* listitem = PyList_GetItem(RewardItem, j);
			PyClassObject* obj = (PyClassObject*)listitem;

			PyObject* Base = PyDict_GetItemString(obj->cl_dict,"Base");
			PyObject* BaseLV = PyDict_GetItemString(obj->cl_dict,"BaseLv");
			PyObject* Append = PyDict_GetItemString(obj->cl_dict,"Append");
			PyObject* AppLevel = PyDict_GetItemString(obj->cl_dict,"AppendLv");
			PyObject* Overlap = PyDict_GetItemString(obj->cl_dict,"Overlap");
			PyObject* Binding = PyDict_GetItemString(obj->cl_dict,"Binding");


			m_Quests[i].RewardItem[j].Base = PyInt_AsLong(Base);
			m_Quests[i].RewardItem[j].BaseLV = PyInt_AsLong(BaseLV);
			m_Quests[i].RewardItem[j].AppendLV = PyInt_AsLong(AppLevel);
			m_Quests[i].RewardItem[j].Overlap = PyInt_AsLong(Overlap);
			m_Quests[i].RewardItem[j].Binding = PyInt_AsLong(Binding);

			for (int z = 0 ; z < MAX_EQUIPAPPEND_COUNT ; z++)
			{
				PyObject* append = PyList_GetItem(Append , z);

				m_Quests[i].RewardItem[j].Append[z] = PyInt_AsLong(append);
			}
		}

		QuestManager::Instance()->InsertQuest(m_Quests[i].QuestID , &m_Quests[i]);

		PyErr_Print();
	}

	Py_XDECREF(pValue);

}

void CNPC::LoadQuestReward()
{
	if (!m_Module)
		return;

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;

	pFunc = PyObject_GetAttrString(m_Module , "GetQuestReward");

	if (!pFunc)
	{
		PyErr_Clear();

		return;
	}

	pValue = PyObject_CallObject(pFunc, 0);

	if (!pValue)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	int tuplesize = 1;

	if (PyTuple_Check(pValue) == 1)
		tuplesize = PyTuple_Size(pValue);

	for (int i = 0 ; i < tuplesize ; i++)
	{
		PyObject* Quest = NULL;

		if (PyTuple_Check(pValue) == 1)
			Quest = PyTuple_GetItem(pValue , i);
		else
			Quest = pValue;

		PyErr_Print();

		PyClassObject* obj = (PyClassObject*)Quest;

		PyObject* QuestID = PyDict_GetItemString(obj->cl_dict,"QuestID");

		PyObject* Credit = PyDict_GetItemString(obj->cl_dict,"Credit");
		PyObject* Contribute = PyDict_GetItemString(obj->cl_dict,"Contribute");
		PyObject* EXP = PyDict_GetItemString(obj->cl_dict,"EXP");
		PyObject* SkillPoint = PyDict_GetItemString(obj->cl_dict,"SkillPoint");

		PyObject* SpecialFlags = PyDict_GetItemString(obj->cl_dict,"SpecialFlags");
		PyObject* SpecialCount = PyDict_GetItemString(obj->cl_dict,"SpecialCount");

		PyObject* UionContribute = PyDict_GetItemString(obj->cl_dict,"UionContribute");

		m_QuestReward[i].NPCID = GetID();

		m_QuestReward[i].QuestID = PyInt_AsLong(QuestID);
		m_QuestReward[i].Credit = PyInt_AsLong(Credit);
		m_QuestReward[i].Contribute = PyInt_AsLong(Contribute);
		m_QuestReward[i].EXP = PyInt_AsLong(EXP);
		m_QuestReward[i].SkillPoint = PyInt_AsLong(SkillPoint);
		m_QuestReward[i].SpecialFlags = PyInt_AsLong(SpecialFlags);
		m_QuestReward[i].SpecialCount = PyInt_AsLong(SpecialCount);
		m_QuestReward[i].UionContribute = PyInt_AsLong(UionContribute);

		QuestManager::Instance()->InsertQuestReward(m_QuestReward[i].QuestID , &m_QuestReward[i]);

		PyErr_Print();
		PyErr_Clear();
	}

	Py_XDECREF(pValue);

}

void CNPC::GetSysGift(CPlayer* player)
{

}


void CNPC::GetItemList(CPlayer* player)
{

	ASSERT((int)m_NpcSell.size() == (int)m_Items.size());
	if ((int)m_NpcSell.size() <= 0 || (int)m_Items.size() <= 0)
	{
		return;
	}
	msg_npc_itemdata.NPCID = m_lID;

	for (unsigned int i=0; i< m_Items.size(); i++)
	{
		msg_npc_itemdata.Head.usSize = sizeof( msg_npc_itemdata.Head ) + 8;

		msg_npc_itemdata.TabIndex = i;
		msg_npc_itemdata.MaxTabIndex = (char)m_Items.size();
		msg_npc_itemdata.ItemCount = (char)m_Items[i].size();
		ASSERT(32 > msg_npc_itemdata.ItemCount);

		for (int j=0; j< m_Items[i].size() ; j++)
		{
			msg_npc_itemdata.Itemdata[j].Base = m_Items[i][j].Base;

			std::map<NPCItemData::ITEM_SELL_TYPE,int>::iterator itor = m_Items[i][j].Trade.begin();
			int iTypeNum = 0;
			for (;itor != m_Items[i][j].Trade.end(); itor++ ,iTypeNum++)
			{
				if (iTypeNum >= 3)
				{
					break;
				}
				msg_npc_itemdata.Itemdata[j].BuyType[iTypeNum] = itor->first;
				msg_npc_itemdata.Itemdata[j].BuyConsume[iTypeNum] = itor->second;
			}
			msg_npc_itemdata.Itemdata[j].BuyTypeNum = (char)m_Items[i][j].Trade.size();
			msg_npc_itemdata.Itemdata[j].AppendLV = m_Items[i][j].AppendLV;
			msg_npc_itemdata.Itemdata[j].BaseLV = m_Items[i][j].BaseLV;
			msg_npc_itemdata.Itemdata[j].Binding = m_Items[i][j].Binding;
			msg_npc_itemdata.Itemdata[j].Overlap = m_Items[i][j].Overlap;
			msg_npc_itemdata.Itemdata[j].Sell = m_NpcSell.at(i);

			msg_npc_itemdata.Head.usSize += sizeof(msg_npc_itemdata.Itemdata[j]);
		}

		s_World->SendMsgToClient(&msg_npc_itemdata , player->GetSocket());
	}
}


void CNPC::GrantItem(CPlayer* player)
{
	if (!m_Module)
		return;

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;

	pFunc = PyObject_GetAttrString(m_Module , "GetCreditItem");

	if (!pFunc)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	pValue = PyObject_CallObject(pFunc, 0);

	if (!pValue)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	int tuplesize = 0;

	if (PyTuple_Check(pValue))
		tuplesize = PyTuple_Size(pValue);
	else
		return;

	for (int i = 0 ; i < tuplesize ; i++)
	{

		PyObject* TupleItem = PyTuple_GetItem(pValue , i);
		pFunc = PyObject_GetAttrString(TupleItem, "Callback");
		if (!pFunc)
		{
			PyErr_Clear();
			return;
		}
		PyObject *pArg  = Py_BuildValue("(i)",player->GetcKnight());
		PyObject *AltarRequst =  PyEval_CallObject(pFunc, pArg);
		if ( PyBool_Check(AltarRequst)  )
		{
		}
		Py_XDECREF(AltarRequst);
	}

	Py_XDECREF(pValue);


}

void CNPC::GrantOfficial(CPlayer* player)
{
	if (!m_Module)
		return;

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;

	pFunc = PyObject_GetAttrString(m_Module , "GetOffPositon");

	if (!pFunc)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	pValue = PyObject_CallObject(pFunc, 0);

	if (!pValue)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	int tuplesize = 0;

	if (PyTuple_Check(pValue))
		tuplesize = PyTuple_Size(pValue);
	else
		return;

	for (int i = 0 ; i < tuplesize ; i++)
	{

		PyObject* TupleItem = PyTuple_GetItem(pValue , i);
		pFunc = PyObject_GetAttrString(TupleItem, "Callback");
		if (!pFunc)
		{
			PyErr_Clear();
			return;
		}
		PyObject *pArg  = Py_BuildValue("(i)",player->GetlPrestige());
		PyObject *AltarRequst =  PyEval_CallObject(pFunc, pArg);
		Py_XDECREF(pArg);
		if ( PyBool_Check(AltarRequst)  )
		{
		}	
		Py_XDECREF(AltarRequst);
	}
	Py_XDECREF(pValue);
}


void CNPC::GetQuestList(CPlayer* player)
{
	if (HasFlag(NPC_FLAG_QUESTGIVER))
	{
		msg_npc_quests.NPCID = GetID();
		msg_npc_quests.Count = 0;

		for (int i = 0 ; i < QUEST_NPCTAKE_COUNT ; i++)
		{	
			if (m_Quests[i].QuestID != -1 && m_Quests[i].SpecialFlags != QUEST_SPECIAL_FLAGS_NONE && m_Quests[i].QuestSort != QUEST_SORT_RANDOM)
			{
				msg_npc_quests.Quest[i].QuestID = m_Quests[i].QuestID;

				if (player->GetlFaction() == 1 && m_Quests[i].QuestID >= QUEST_COUNT)
				{
					msg_npc_quests.Quest[msg_npc_quests.Count].Available = player->GetQuest(m_Quests[i].QuestID)->CanAccept();
					msg_npc_quests.Count++;
				}
				else if (player->GetlFaction() == 0 && m_Quests[i].QuestID < QUEST_COUNT)
				{
					msg_npc_quests.Quest[msg_npc_quests.Count].Available = player->GetQuest(m_Quests[i].QuestID)->CanAccept();
					msg_npc_quests.Count++;
				}

			}
		}

		s_World->SendMsgToClient(&msg_npc_quests , player->GetSocket());
	}

	if (HasFlag(NPC_FLAG_QUESTRESPONDER))
	{
		msg_npc_questreward.NPCID = GetID();
		msg_npc_questreward.Count = 0;

		for (int i = 0 ; i < QUEST_NPCTAKE_COUNT ; i++)
		{
			if (m_QuestReward[i].QuestID != -1)
			{
				if (player->GetlFaction() == 1 && m_QuestReward[i].QuestID >= QUEST_COUNT)
				{
					msg_npc_questreward.QuestID[msg_npc_questreward.Count] = m_QuestReward[i].QuestID;
					msg_npc_questreward.Count++;
				}
				else if (player->GetlFaction() == 0 && m_QuestReward[i].QuestID < QUEST_COUNT)
				{
					msg_npc_questreward.QuestID[msg_npc_questreward.Count] = m_QuestReward[i].QuestID;
					msg_npc_questreward.Count++;
				}
			}
		}

		player->s_World->SendMsgToClient(&msg_npc_questreward , player->GetSocket());
	}

}

void CNPC::GetStorageData(CPlayer* player)
{
	player->SendStorageMsg();
}

eError CNPC::Talk(CPlayer* player)
{
	if (!m_Module)
		return NO_MSG_ERRO;

	PyObject *pFunc;
	PyObject  *pValue;

	PyErr_Clear();

	pFunc = PyObject_GetAttrString(m_Module , "Talk");

	if (!pFunc)
	{
		PyErr_Clear();

		return NO_MSG_ERRO;
	}

	if (!pFunc)
		return NO_MSG_ERRO;

 	PyObject *pArg  = Py_BuildValue("(i)",(int)player);
 	pValue = PyObject_CallObject(pFunc, pArg);


	if (!pValue)
	{
		PyErr_Print();
		PyErr_Clear();

		return NO_MSG_ERRO;
	}

	char* talk = PyString_AsString(pValue);

	PyErr_Print();

	msg_talk_npc.NPCID = m_lID;
	msg_talk_npc.NPCFlag = m_Flag;
	msg_talk_npc.SysFlag = m_SysFlag;

	memcpy(msg_talk_npc.Talk , talk , MAX_NAME);

	player->s_World->SendMsgToClient(&msg_talk_npc , player->GetSocket());

	Py_XDECREF(pValue);

	return NO_MSG_ERRO;
}

eError CNPC::Sell(CPlayer* player , int index , int num , int count , int base)
{
	if (!player->IsMsgOK(MSG_KITBAG_FLAG))
		return NO_MSG_ERRO;

	if (index < 0 || num < 0 || count <= 0 || base < 0)
	{
		return NO_MSG_ERRO;
	}

	if (index >= (int)m_Items.size())
	{
		GetItemList(player);

		return MSG_ERRO_0188;
	}

	if (num >= (int)m_Items[index].size())
	{
		GetItemList(player);

		return MSG_ERRO_0188;
	}

	if (player->GetTrade()->IsTrading())
	{
		return MSG_ERRO_0197;
	}

	Item item;

	ItemManager::Instance()->CreateItem(&m_Items[index][num] , &item);
	if (base != item.GetItemBaseAttribute()->ID)
	{
		GetItemList(player);
		return MSG_ERRO_0188;
	}
	std::map<NPCItemData::ITEM_SELL_TYPE,int>::iterator itor = m_Items[index][num].Trade.begin();
	int iTypeNum = 0;
	for (;itor != m_Items[index][num].Trade.end(); itor++ ,iTypeNum++)
	{
		if (iTypeNum >= 3)
		{
			break;
		}
		item.m_BuyType[iTypeNum] = itor->first;
		item.m_BuyConsume[iTypeNum] = itor->second;
	}
	item.m_BuyTypeNum = (char)m_Items[index][num].Trade.size();

	if ( !CheckNpcConsume(player,&item))
	{
		return MSG_ERRO_0198;
	}

	if (player->AddItem(&item))
	{
		UpdataNpcConsume(player,&item);
		msg_npc_sell.NPCID = m_lID;
		msg_npc_sell.Index = index;
		msg_npc_sell.Num = num;
		msg_npc_sell.Count = count;

		player->s_World->SendMsgToClient(&msg_npc_sell , player->GetSocket());
	}
	else
	{
	}
	return NO_MSG_ERRO;
}

UINT CNPC::CalculateAttack()
{
	return 0;
}


void CNPC::CalculatePassivePracticeDegree( int  , ATTACK_TYPE ,bool  )
{
	return ;
}

bool CNPC::CalculatePassivePracticeDegree(PhysiacalAttack , ATTACK_TYPE , bool  )
{
	return false;
}

bool CNPC::ChanegeDegree(int , int )
{
	return false;
}

bool CNPC::PassiveSkillUpgrade(UINT , int *)
{
	return true;
}

void  CNPC::AddPassiveSkill(int , int  , int  )
{

}

INT CNPC::CalculateAttackDamage(  CGameObject& , float   )
{
	return 0;
}

INT CNPC::CalculateAttackDamage(int )
{
	return 0;
}

ULONG CNPC::GetHit()
{
	return 0;  
}

ULONG CNPC::GetDodge()
{
	return 0;
}

void CNPC::AI(void)
{

}

bool CNPC::HasFlag(NPCFlag flag)
{
	return (m_Flag & flag) != 0;
}

void CNPC::AynObjToObj( CCell *, CCell * )
{

}

void CNPC::AynMeToOther( CCell *pOldCell, CCell *pNewCell )
{
	CGameObject::AynMeToOther( pOldCell, pNewCell  );
}

void CNPC::LoadItemListType()
{

	if (!m_Module)
		return;

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;

	pFunc = PyObject_GetAttrString(m_Module , "GetitemListType");

	if (!pFunc)
	{
		PyErr_Clear();
		return;
	}

	pValue = PyObject_CallObject(pFunc, 0);

	if (!pValue)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	int tuplesize = 0;

	if (PyTuple_Check(pValue))
		tuplesize = PyTuple_Size(pValue);
	else
		return;

	for (int i = 0 ; i < tuplesize ; i++)
	{

		PyObject* Type = PyTuple_GetItem(pValue , i);
		char Sell = (char)PyInt_AsLong(Type);
		m_NpcSell.push_back( Sell );

	}

	Py_XDECREF(pValue);
}

bool CNPC::CheckNpcConsume(CPlayer* player,Item* pItem)
{
	for (int i=0; i< pItem->m_BuyTypeNum; i++)
	{
		if (i >= 3 )
		{
			break;
		}

		switch(pItem->m_BuyType[i])
		{
		case ITEM_TRADE_PRESTIGE:
			{
				if (pItem->m_BuyConsume[i] > player->GetlPrestige())
				{
					return false;
				}
			}
			break;
		case ITEM_TRADE_CONSORTIA_CONTRIBUTE:
			{
			}
			break;
		case ITEM_TRADE_MONEY:
			{
				if ( player->GetlMoney() <  pItem->m_Overlap * pItem->m_BuyConsume[i] )
				{
					return false;
				}
			}
			break;
		case ITEM_TRADE_BIJOU:
			{
				if ( player->GetlStone() <  pItem->m_Overlap * pItem->m_BuyConsume[i] )
				{
					return false;
				}
			}
			break;
		default:
			{
				return false;
			}
			break;
		}
	}

	return true;
}
void CNPC::UpdataNpcConsume(CPlayer* player,Item* pItem)
{
	for (int i=0; i< pItem->m_BuyTypeNum; i++)
	{
		switch(pItem->m_BuyType[i])
		{
		case ITEM_TRADE_PRESTIGE:
			{
				player->SetlPrestige(player->GetlPrestige() - pItem->m_BuyConsume[i]); 
			}
			break;
		case ITEM_TRADE_CONSORTIA_CONTRIBUTE:
			{
			}
			break;
		case ITEM_TRADE_MONEY:
			{
				player->SetlMoney( player->GetlMoney() - pItem->m_Overlap * pItem->m_BuyConsume[i] );
			}
			break;
		case ITEM_TRADE_BIJOU:
			{
				player->SetlStone( player->GetlStone() - pItem->m_Overlap * pItem->m_BuyConsume[i] ); 
			}
			break;
		}
	}
}

void CNPC::SysList( CPlayer* player, int SysFlag, const int *Oper )
{
	MSG_SYS_FUN_USE  SysList;
	SysList.Head.usType = _MSG_SYS_FUN_USE;
	SysList.Head.usSize = sizeof(MSG_SYS_FUN_USE);
	SysList.NpcType = SysFlag;
	SysList.NPCID = m_lID;
	SysList.NextWin = false;

	if (!m_Module)
		return;

	PyObject  *pValue,*pValue1;
	PyErr_Clear();

	PyObject *pFunc = PyObject_GetAttrString(m_Module , "GetSysType");
	if (!pFunc)
	{
		PyErr_Print();
		PyErr_Clear();
		return;
	}

	if (!pFunc)
		return;

	pValue1 = PyObject_CallObject(pFunc, 0);

	int Sys = PyInt_AsLong(pValue1);

	if ( ( Sys & SysFlag ) ==  0 )
		return;
	Py_XDECREF(pValue1);
	PyObject* pFunc2 = PyObject_GetAttrString(m_Module , "GetContentList");
	if (!pFunc2)
	{
		PyErr_Print();
		PyErr_Clear();
		return;
	}

	if (!pFunc2)
		return;

	pValue = PyObject_CallObject(pFunc2, 0);

	if (!pValue)
	{
		PyErr_Print();
		PyErr_Clear();
		return;
	}

	if ( !PyDict_Check(pValue) )
		return ;

	PyObject*  Flag = PyInt_FromLong( SysFlag ) ;
	if ( !Flag )
		return;

	PyObject* DictItem = PyDict_GetItem( pValue, Flag ) ;

	if ( !PyTuple_Check(DictItem) )
		return ;

	PyObject* NewOper = PyTuple_New(8) ;
	int Index = 0;
	for ( int a = 0; a < 8; a++ )
	{
		if ( *Oper != -1 )
			Index = a;
		PyObject*  Temp = PyInt_FromLong(*Oper);
		PyTuple_SetItem( NewOper, a, Temp );
		Oper++;
	}


	PyObject *pArg  = PyTuple_New(2) ;
	PyTuple_SetItem( pArg, 0, NewOper );
	PyTuple_SetItem( pArg, 1, PyInt_FromLong((int)player) );


	PyObject* Item = PyTuple_GetItem(DictItem , Index);
	PyObject* pFunc1 = PyObject_GetAttrString(Item, "Callback");

	if (!pFunc1)
	{
		PyErr_Print();
		PyErr_Clear();
		return;
	}

	PyObject *DataRequst =  PyObject_CallObject(pFunc1, pArg);
	Py_XDECREF(NewOper);
	Py_XDECREF(pArg);

	if (!DataRequst)
	{
		PyErr_Print();
		PyErr_Clear();
		return;
	}

	int ResultSize = 0;
	if ( !PyTuple_Check( DataRequst ) )
		return ;

	ResultSize = PyTuple_Size(DataRequst);
	memset(SysList.Data,0L,20*sizeof(USHORT));
	for ( int a = 0; a<ResultSize;a++)
	{
		if( a == 0)
			SysList.NextWin =  (PyTuple_GetItem(DataRequst , a)== Py_True) ;
		else
			SysList.Data[a-1] = PyInt_AsLong( PyTuple_GetItem(DataRequst , a) );
	}

	if ( SysList.NextWin )
		player->s_World->SendMsgToClient(&SysList , player->GetSocket());


	Py_XDECREF(DataRequst);
	Py_XDECREF(pValue);
}