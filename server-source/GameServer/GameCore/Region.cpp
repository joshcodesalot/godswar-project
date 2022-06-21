#include "StdAfx.h"
#include <mmsystem.h>
#include "Region.h"
#include "GameObject.h"
#include "Player.h"
#include "NPC.h"
#include "Monster.h"
#include "..\DataCore.h"
#include "Map.h"
#include "../World.h"
#include "..\ServiceLoader.h"
#include ".\SkillManager.h"
#include "ItemManager.h"

extern void ExcScript(CGameObject* p1, CGameObject* p2, int MagicID);

#include "..\..\Common\MsgDefine.h"
#include "..\..\Common\MsgTypes.h"


MSG_GOLD_BOX_RETURN msg_gold_box_return;
MSG_OBJECTINFO msg, msg2other;
MSG_OBJECTINFO msg_monster, msg_npc;
MSG_LEAVE msg_leave;
MSG_WALK_BEGIN msg_walk_begin;
MSG_WALK_END msg_walk_end;
MSG_WALK msg_walk;
MSG_TALK msg_talk;
MSG_FIGHT msg_fight;
MSG_ATTACK msg_attack;
MSG_SKILL  msg_skill;
MSG_EFFECT msg_effect;
MSG_LEARN  msg_skill_learn;
MSG_ACTIVESKILL_INFO  msg_activeskill_info;
MSG_PASSIVESKILL_INFO msg_passiveskill_info;
MSG_DEAD msg_dead;
MSG_DROPS msg_drops;
MSG_MAGIC_DAMAGE msg_magic;
MSG_SCENE_CHANGE msg_scene_change;
MSG_PICKUP_DROPS msg_pickup_drops;
MSG_MOVE_ITEM msg_move_item;
MSG_BREAK_ITEM msg_break_item;
MSG_SELL msg_sell;
MSG_STALL msg_stall;
MSG_STALLADDITEM msg_stalladditem;
MSG_STALLITEM msg_stallitem;
MSG_STALLBUYITEM msg_stallbuyitem;
MSG_TALK_NPC msg_talk_npc;
MSG_NPC_DATA msg_npc_data;
MSG_NPC_ITEMDATA msg_npc_itemdata;
MSG_MALL_ITEMDATA msg_gamemall_itemdate;
MSG_MALL_ITEMDATA msg_associationmall_itemdate;
MSG_ASSOCIATIONDISCOUNT msg_associationdiscount;
MSG_NPC_STORAGEDATA msg_npc_storagedata;
MSG_NPC_SELL msg_npc_sell;
MSG_MALL_SELL msg_mall_sell;
MSG_MALL_SELL msg_association_sell;
MSG_NPC_QUEST msg_npc_quest;
MSG_NPC_NEXTQUEST msg_npc_nextquest;
MSG_NPC_QUESTS msg_npc_quests;
MSG_NPC_QUESTSAVAILABLE msg_npc_questsavailable;
MSG_NPC_QUESTSUNAVAILABLE msg_npc_questsunavailable;
MSG_NPC_QUESTREWARD msg_npc_questreward;
MSG_NPC_ACCEPTQUEST msg_npc_acceptquest;
MSG_NPC_CANCELQUEST msg_npc_cancelquest;
MSG_NPC_COMPLETEQUEST msg_npc_completequest;
MSG_NPC_QUESTFAILD msg_npc_questfaild;
MSG_NPC_QUESTVIEW msg_npc_questview;
MSG_NPC_REWARDQUEST msg_npc_rewardquest;
MSG_NPC_QUESTKILLORCAST msg_npc_questkillorcast;
MSG_PLAYER_ACCEPTQUESTS msg_player_acceptquests;
MSG_ERROR msg_error;

MSG_NPC_QUESTEXPLORERRESULT msg_npc_questexplorerresult;

MSG_FINDQUESTRESULT msg_findquestresult;

MSG_EQUIPFORGE_EQUIP msg_equipforge_equip;
MSG_EQUIPFORGE_MATERIAL msg_equipforge_material;
MSG_EUQIPFORGE msg_equipforge;

MSG_RELATION  msg_relation;
MSG_RELATION_RESPONSE msg_responses;
MSG_TRADE msg_trade;
MSG_TRADE_MONEY msg_trade_money;
MSG_TRADE_ADDITEM msg_trade_additem;
MSG_TRADE_ITEM msg_trade_item;
MSG_TRADE_REMOVEITEM msg_trade_removeitem;

MSG_STATUS  msg_status_disappear;

MSG_USEOREQUIP_ITEM msg_useorequip_item;

MSG_SYN_GAMEDATA  msg_syn_gamedata;


enum {
	FACTION_SAME,
	FACTION_DIFFERENT,
	FACTION_ALL
};

CRegion::CRegion()
{
	msg.Head.usType = _MSG_OBJECTINFO;
	msg.Head.usSize = sizeof(MSG_OBJECTINFO);

	msg2other.Head.usType = _MSG_OBJECTINFO;
	msg2other.Head.usSize = sizeof(_MSG_OBJECTINFO);

	msg_monster.Head.usSize = sizeof(MSG_OBJECTINFO);
	msg_monster.Head.usType = _MSG_OBJECTINFO;

	msg_npc.Head.usSize = sizeof(MSG_OBJECTINFO);
	msg_npc.Head.usType = _MSG_OBJECTINFO;

	msg_leave.Head.usType = _MSG_LEAVE;
	msg_leave.Head.usSize = sizeof(MSG_LEAVE);

	msg_walk_begin.Head.usType = _MSG_WALK_BEGIN;
	msg_walk_begin.Head.usSize = sizeof(MSG_WALK_BEGIN);

	msg_walk_end.Head.usType = _MSG_WALK_END;
	msg_walk_end.Head.usSize = sizeof(MSG_WALK_END);

	msg_walk.Head.usType = _MSG_WALK;
	msg_walk.Head.usSize = sizeof(MSG_WALK);

	msg_fight.Head.usType = _MSG_FIGHT;
	msg_fight.Head.usSize = sizeof(MSG_FIGHT);

	msg_attack.Head.usType = _MSG_ATTACK;
	msg_attack.Head.usSize = sizeof(MSG_ATTACK);

	msg_dead.Head.usType = _MSG_DEAD;
	msg_dead.Head.usSize = sizeof(MSG_DEAD);

	msg_drops.Head.usType = _MSG_DROPS;
	msg_drops.Head.usSize = sizeof(MSG_DROPS);

	msg_talk.Head.usType = _MSG_TALK;
	msg_talk.Head.usSize = sizeof(MsgHead) + 16;

	msg_skill.Head.usType = _MSG_SKILL;
	msg_skill.Head.usSize = sizeof(MSG_SKILL);
	msg_skill.ucMP = 10;

	msg_effect.Head.usType = _MSG_EFFECT;
	msg_effect.Head.usSize = sizeof(MSG_EFFECT);

	msg_skill_learn.Head.usType = _MSG_LEARN;
	msg_skill_learn.Head.usSize = sizeof(MSG_LEARN);

	msg_magic.Head.usType = _MSG_MAGIC_DAMAGE;
	msg_magic.Head.usSize = sizeof(MSG_MAGIC_DAMAGE);
	msg_scene_change.Head.usType = _MSG_SCENE_CHANGE;
	msg_scene_change.Head.usSize = sizeof(MSG_SCENE_CHANGE);

	msg_pickup_drops.Head.usType = _MSG_PICKUPDROPS;
	msg_pickup_drops.Head.usSize = sizeof(MSG_PICKUP_DROPS);

	msg_useorequip_item.Head.usType = _MSG_USEOREQUIP;
	msg_useorequip_item.Head.usSize = sizeof(MSG_USEOREQUIP_ITEM);

	msg_activeskill_info.Head.usType = _MSG_ACTIVESKILL_INFO;
	msg_activeskill_info.Head.usSize = sizeof(MSG_ACTIVESKILL_INFO);

	msg_passiveskill_info.Head.usType = _MSG_PASSIVESKILL_INFO;
	msg_passiveskill_info.Head.usSize = sizeof(MSG_PASSIVESKILL_INFO);

	msg_move_item.Head.usType = _MSG_MOVEITEM;
	msg_move_item.Head.usSize = sizeof(MSG_MOVE_ITEM);

	msg_break_item.Head.usType = _MSG_BREAK_ITEM;
	msg_break_item.Head.usSize = sizeof(MSG_BREAK_ITEM);

	msg_sell.Head.usType = _MSG_SELL;
	msg_sell.Head.usSize = sizeof(MSG_SELL);

	msg_stall.Head.usType = _MSG_STALL;
	msg_stall.Head.usSize = sizeof(MSG_STALL);

	msg_stalladditem.Head.usType = _MSG_STALLADDITEM;
	msg_stalladditem.Head.usSize = sizeof(MSG_STALLADDITEM);

	msg_stallitem.Head.usType = _MSG_STALLITEM;
	msg_stallitem.Head.usSize = sizeof(MSG_STALLITEM);

	msg_stallbuyitem.Head.usType = _MSG_STALLBUYITEM;
	msg_stallbuyitem.Head.usSize = sizeof(MSG_STALLBUYITEM);

	msg_talk_npc.Head.usType = _MSG_TALKNPC;
	msg_talk_npc.Head.usSize = sizeof(MSG_TALK_NPC);

	msg_npc_data.Head.usType = _MSG_NPCDATA;
	msg_npc_data.Head.usSize = sizeof(MSG_NPC_DATA);

	msg_error.Head.usType = _MSG_ERROR;
	msg_error.Head.usSize = sizeof(MSG_ERROR);

	msg_npc_itemdata.Head.usType = _MSG_NPCITEMDATA;
	msg_npc_itemdata.Head.usSize = sizeof(MSG_NPC_ITEMDATA);

	msg_gamemall_itemdate.Head.usType = _MSG_MALLITEMDATA;
	msg_gamemall_itemdate.Head.usSize = sizeof(MSG_MALL_ITEMDATA);

	msg_associationmall_itemdate.Head.usType = _MSG_ASSOCIATIONITEMDATA;
	msg_associationmall_itemdate.Head.usSize = sizeof(MSG_MALL_ITEMDATA);

	msg_mall_sell.Head.usType = _MSG_MALLSELL;
	msg_mall_sell.Head.usSize = sizeof(MSG_MALL_SELL);

	msg_association_sell.Head.usType = _MSG_ASSOCIATIONSELL;
	msg_association_sell.Head.usSize = sizeof(MSG_MALL_SELL);


	msg_associationdiscount.Head.usType = _MSG_ASSOCIATIONDISCOUNT;
	msg_associationdiscount.Head.usSize = sizeof(MSG_ASSOCIATIONDISCOUNT);

	msg_npc_sell.Head.usType = _MSG_NPCSELL;
	msg_npc_sell.Head.usSize = sizeof(MSG_NPC_SELL);

	msg_npc_quest.Head.usType = _MSG_NPCQUEST;
	msg_npc_quest.Head.usSize = sizeof(MSG_NPC_QUEST);

	msg_npc_nextquest.Head.usType = _MSG_NPCNEXTQUEST;
	msg_npc_nextquest.Head.usSize = sizeof(MSG_NPC_NEXTQUEST);

	msg_npc_quests.Head.usType = _MSG_NPCQUESTS;
	msg_npc_quests.Head.usSize = sizeof(MSG_NPC_QUESTS);

	msg_npc_questsavailable.Head.usType = _MSG_NPCQUESTSAVAILABLE;
	msg_npc_questsavailable.Head.usSize = sizeof(MSG_NPC_QUESTSAVAILABLE);

	msg_npc_questsunavailable.Head.usType = _MSG_NPCQUESTSUNAVAILABLE;
	msg_npc_questsunavailable.Head.usSize = sizeof(MSG_NPC_QUESTSUNAVAILABLE);

	msg_npc_questreward.Head.usType = _MSG_NPCQUESTREWARD;
	msg_npc_questreward.Head.usSize = sizeof(MSG_NPC_QUESTREWARD);

	msg_npc_acceptquest.Head.usType = _MSG_NPCACCEPTQUEST;
	msg_npc_acceptquest.Head.usSize = sizeof(MSG_NPC_ACCEPTQUEST);

	msg_npc_cancelquest.Head.usType = _MSG_NPCCANCELQUEST;
	msg_npc_cancelquest.Head.usSize = sizeof(MSG_NPC_CANCELQUEST);

	msg_npc_completequest.Head.usType = _MSG_NPCCOMPLETEQUEST;
	msg_npc_completequest.Head.usSize = sizeof(MSG_NPC_COMPLETEQUEST);

	msg_npc_questfaild.Head.usType = _MSG_NPCQUESTFAILD;
	msg_npc_questfaild.Head.usSize = sizeof(MSG_NPC_QUESTFAILD);

	msg_npc_questview.Head.usType = _MSG_NPCQUESTVIEW;
	msg_npc_questview.Head.usSize = sizeof(MSG_NPC_QUESTVIEW);

	msg_npc_rewardquest.Head.usType = _MSG_NPCREWARDQUEST;
	msg_npc_rewardquest.Head.usSize = sizeof(MSG_NPC_REWARDQUEST);

	msg_npc_questkillorcast.Head.usType = _MSG_NPCQUESTKILLORCAST;
	msg_npc_questkillorcast.Head.usSize = sizeof(MSG_NPC_QUESTKILLORCAST);

	msg_npc_questexplorerresult.Head.usType = _MSG_QUESTEXPLORERRESULT;
	msg_npc_questexplorerresult.Head.usSize = sizeof(MSG_NPC_QUESTEXPLORERRESULT);

	msg_player_acceptquests.Head.usType = _MSG_PLAYER_ACCEPTQUESTS;
	msg_player_acceptquests.Head.usSize = sizeof(MSG_PLAYER_ACCEPTQUESTS);

	msg_findquestresult.Head.usType = _MSG_FINDQUESTRESULT;
	msg_findquestresult.Head.usSize = sizeof(MSG_FINDQUESTRESULT);

	msg_equipforge_equip.Head.usType = _MSG_EQUIPFORGE_EQUIP;
	msg_equipforge_equip.Head.usSize = sizeof(MSG_EQUIPFORGE_EQUIP);

	msg_equipforge_material.Head.usType = _MSG_EQUIPFORGE_MATERIAL;
	msg_equipforge_material.Head.usSize = sizeof(MSG_EQUIPFORGE_MATERIAL);

	msg_equipforge.Head.usType = _MSG_EQUIPFORGE;
	msg_equipforge.Head.usSize = sizeof(MSG_EUQIPFORGE);

	msg_relation.Head.usType = _MSG_RELATION;
	msg_relation.Head.usSize = sizeof(MSG_RELATION);

	msg_responses.Head.usType = _MSG_RELATION_RESPONSE;
	msg_responses.Head.usSize = sizeof(MSG_RELATION_RESPONSE);

	msg_trade.Head.usType = _MSG_TRADE;
	msg_trade.Head.usSize = sizeof(MSG_TRADE);

	msg_trade_money.Head.usType = _MSG_TRADE_MONEY;
	msg_trade_money.Head.usSize = sizeof(MSG_TRADE_MONEY);

	msg_trade_additem.Head.usType = _MSG_TRADE_ADDITEM;
	msg_trade_additem.Head.usSize = sizeof(MSG_TRADE_ADDITEM);

	msg_trade_item.Head.usType = _MSG_TRADE_ITEM;
	msg_trade_item.Head.usSize = sizeof(MSG_TRADE_ITEM);

	msg_trade_removeitem.Head.usType = _MSG_TRADE_REMOVEITEM;
	msg_trade_removeitem.Head.usSize = sizeof(MSG_TRADE_REMOVEITEM);

	msg_npc_storagedata.Head.usType = _MSG_NPCSTORAGEDATA;
	msg_npc_storagedata.Head.usSize = sizeof(msg_npc_storagedata);

	msg_status_disappear.Head.usType = _MSG_STATUS;
	msg_status_disappear.Head.usSize = sizeof(msg_status_disappear);

	msg_syn_gamedata.Head.usType = _MSG_SYN_GAMEDATA;
	msg_syn_gamedata.Head.usSize = sizeof(msg_syn_gamedata);

	m_pMap = new CMap;

	m_lMonsterQueue = 0;
	m_lMonsterCount = 0;
	for (int i = 0; i < MAX_REGION_MONSTER; i++)
	{
		m_pMonsterQueue[i] = NULL;
	}

	m_lstMgcRgnActive.clear();
	m_lstMgcRgnFree.clear();

	for (int i = 0; i < MAX_MAGIC_REGION; ++i)
		m_lstMgcRgnFree.push_back(&m_MagicRegion[i]);
}

CRegion::~CRegion(void)
{
	m_listPlayer.clear();

	m_listNPC.clear();
	m_listMonster.clear();

	SAFE_DELETE(m_pMap);

	if (m_listConvey.size() != 0)
	{
		for (SceneConveyIterator it = m_listConvey.begin(); it != m_listConvey.end(); it++)
		{
			tarSceneConvey* p = *it;
			SAFE_DELETE(p);
		}
		m_listConvey.clear();
	}

}


long CRegion::Init(CWorld* pWorld)
{
	m_pWorld = pWorld;
	m_listPlayer.clear();
	m_listNPC.clear();
	m_listMonster.clear();

	for (int i = 0; i < REGION_SIZE; i++)
	{
		for (int j = 0; j < REGION_SIZE; j++)
		{
			m_Player[i][j].clear();
			m_NPC[i][j].clear();
			m_Monster[i][j].clear();
		}
	}

	return 0;
}

void CRegion::RemoveMonster(CMonster* pMonster)
{
	if (pMonster == NULL)
		return;
	pMonster->GetCurrentCell()->RemoveObj(pMonster);
	m_listMonster.remove(pMonster);
	m_listObject.remove(pMonster);

	// post message to other player
	msg_leave.uiID = pMonster->GetID();
	msg_leave.Head.usSize = sizeof(msg_leave);

	SendAreaMsgOneToOther(pMonster->GetCurrentCell(), &msg_leave);
}

void CRegion::RemoveNPC(CNPC* pNPC)
{
	if (pNPC == NULL)
		return;

	pNPC->GetCurrentCell()->RemoveObj(pNPC);
	m_listNPC.remove(pNPC);
	m_listObject.remove(pNPC);

	msg_leave.uiID = pNPC->GetID();
	msg_leave.Head.usSize = sizeof(msg_leave);

	SendAreaMsgOneToOther(pNPC->GetCurrentCell(), &msg_leave);
}

void CRegion::RemoveObject(long lID)
{
	if (lID < ACADEMIC_PLAYER_MEM_ID + ACTUAL_PLAYER_ONLINE)
	{
		RemovePlayer((CPlayer*)(m_pWorld->GetObject(lID)));
		return;
	}
	if (lID < ACADEMIC_NPC_MEM_ID + ACTUAL_NPC_ONLINE)
	{
		RemoveNPC((CNPC*)(m_pWorld->GetObject(lID)));
		return;
	}
	if (lID < ACADEMIC_MONSTER_MEM_ID + ACTUAL_MONSTER_ONLINE)
	{
		RemoveMonster((CMonster*)(m_pWorld->GetObject(lID)));
		return;
	}
}


long CRegion::AI(void)
{

	for (PlayerIterator it = m_listPlayer.begin(); it != m_listPlayer.end(); )
	{
		CPlayer* p = *it;
		it++;
		p->Run();
		p->AI();
		if (p->IsMove())
		{
			printf("%s退出游戏！\n", p->GetName());
			break;
		}
	}

	if (m_listNPC.size() != 0)
	{
		for (NPCIterator it = m_listNPC.begin(); it != m_listNPC.end(); it++)
		{
			CNPC* pNPC = *it;
			pNPC->AI();
		}
	}

	if (m_lMonsterQueue > 0)
	{
		int temp = m_lMonsterCount;
		for (int i = temp; i < temp + 100; i++)
		{
			if (i >= m_lMonsterQueue)
			{
				m_lMonsterCount = 0;
				break;
			}
			m_pMonsterQueue[i]->AI();
			m_lMonsterCount++;
		}

		for (int i = 0; i < m_lMonsterQueue; i++)
		{
			m_pMonsterQueue[i]->Run();
		}
	}


	m_GMctr->CrtAI();
	return 0;
}

void CRegion::SendAreaMsgOneToOther(std::vector<CCell*> VecCell, void* buf)
{
	std::vector<CCell*>::iterator itor = VecCell.begin();
	for (; itor != VecCell.end(); itor++)
	{
		CCell* pCell = *itor;
		MapPlayer& PlayerMap = pCell->GetPlayerMap();
		MapPlayer::iterator iter = PlayerMap.begin();
		for (; iter != PlayerMap.end(); iter++)
		{
			CPlayer* pPlayer = iter->second;
			m_pWorld->SendMsgToClient(buf, pPlayer->GetSocket());
		}

	}
}

void CRegion::SendCellMsg(CCell* pCell, void* buf, CPlayer* pMySelf)
{
	if (NULL == pCell)
		return;

	MapPlayer& PlayerMap = pCell->GetPlayerMap();
	MapPlayer::iterator iter = PlayerMap.begin();
	for (; iter != PlayerMap.end(); iter++)
	{
		CPlayer* pPlayer = iter->second;
		if (NULL == pPlayer)
		{
			continue;
		}

		if (pMySelf != NULL && pMySelf == pPlayer)
		{
		}
		else
			m_pWorld->SendMsgToClient(buf, pPlayer->GetSocket());
	}
}

void CRegion::SendAreaMsgOneToOther(CCell* pCell, void* buf, CPlayer* pMySelf)
{
	if (NULL == pCell)
	{
		return;
	}
	std::vector< BYTE >  VecCell = pCell->GetArea();
	std::vector< BYTE >::iterator itor = VecCell.begin();
	for (; itor != VecCell.end(); itor++)
	{
		CCell* pCurCell = m_pMap->GetCell(*itor);

		SendCellMsg(pCurCell, buf, pMySelf);
	}
}




long CRegion::Join(CPlayer* pPlayer)
{
	if (pPlayer == NULL)
		return -1;

	pPlayer->ClearPath();
	pPlayer->SetRegion(this);
	pPlayer->SetPos(pPlayer->GetPosX(), 0.0f, pPlayer->GetPosZ(), true);


	msg.uiType = _OBJECT_TYPE_PLAYER;
	msg.usMapID = m_lID;
	msg.uiID = pPlayer->GetID();
	msg.X = pPlayer->GetPosX();
	msg.Y = pPlayer->GetPosY();
	msg.Z = pPlayer->GetPosZ();
	msg.ucGender = pPlayer->GetGender();
	msg.ucCamp = pPlayer->GetlFaction();
	msg.ucProfession = pPlayer->GetlClass();
	msg.ucBelief = pPlayer->GetusBelief();
	msg.ucHairStyle = pPlayer->GetcHairStyle();
	msg.ucFaceShap = pPlayer->GetcPupilColor();
	msg.uiHP = pPlayer->GetlHP();
	msg.uiMaxHP = pPlayer->GetMaxHP();
	msg.ucLV = pPlayer->GetRank();
	strcpy(msg.cName, pPlayer->GetName());
	msg.fAtan2 = pPlayer->GetAtan2();
	msg.iStatusCount = (UINT)pPlayer->GetStatus(msg.iStautsDis);
	int Index = 0;
	msg.Head.usSize = sizeof(MSG_OBJECTINFO);
	msg.cEquipPos = 0;

	for (int a = 0; a < MAX_EQUIP_COUNT; a++)
	{
		if (pPlayer->GetEquip()[a].GetItemData().Base != -1)
		{
			msg.cEquipPos |= (1 << a);
			const ItemData& Data = pPlayer->GetEquip()[a].GetItemData();
			memcpy(&msg.SEquip[Index], &Data, sizeof(Equip));
			Index++;
		}
	}

	msg2other.uiType = _OBJECT_TYPE_PLAYER;
	msg2other.usMapID = m_lID;
	msg2other.uiID = 0;
	msg2other.X = 0.0f;
	msg2other.Y = 0.0f;
	msg2other.Z = 0.0f;
	msg2other.fAtan2 = 0.0f;


	std::vector<BYTE> Area = pPlayer->GetCurrentCell()->GetArea();
	std::vector<BYTE>::iterator  AreaItor = Area.begin();
	for (; AreaItor != Area.end(); AreaItor++)
	{
		CCell* pCurCell = m_pMap->GetCell(*AreaItor);
		{
			{
				MapPlayer& PlayerMap = pCurCell->GetPlayerMap();
				MapPlayer::iterator iter = PlayerMap.begin();
				for (; iter != PlayerMap.end(); iter++)
				{
					CPlayer* p = iter->second;
					msg.DB_id = pPlayer->GetDBID();
					msg.uiID = pPlayer->GetID();
					msg.Head.usType = _MSG_OBJECTINFO;
					if (p != pPlayer)
					{
						m_pWorld->SendMsgToClient(&msg, p->GetSocket());
					}

					msg2other.DB_id = p->GetDBID();
					msg2other.uiID = p->GetID();
					msg2other.Stall = p->GetStall()->IsStart();
					memcpy(msg2other.StallText, p->GetStall()->GetText(), MAX_NAME);
					msg2other.X = p->GetPosX();
					msg2other.Y = p->GetPosY();
					msg2other.Z = p->GetPosZ();
					msg2other.fAtan2 = p->GetAtan2();
					msg2other.uiHP = p->GetlHP();
					msg2other.uiMaxHP = p->GetMaxHP();
					msg2other.ucGender = p->GetGender();
					msg2other.ucCamp = p->GetlFaction();
					msg2other.ucProfession = p->GetlClass();
					msg2other.ucBelief = p->GetusBelief();
					msg2other.ucHairStyle = p->GetcHairStyle();
					msg2other.ucFaceShap = p->GetcPupilColor();
					msg2other.ucLV = p->GetRank();
					strcpy(msg2other.cName, p->GetName());
					msg2other.iStatusCount = (UINT)p->GetStatus(msg2other.iStautsDis);
					int Index = 0;
					msg2other.cEquipPos = 0;
					msg2other.Head.usSize = sizeof(MSG_OBJECTINFO);

					for (int a = 0; a < MAX_EQUIP_COUNT; a++)
					{
						if (p->GetEquip()[a].GetItemData().Base != -1)
						{
							msg2other.cEquipPos |= (1 << a);
							const ItemData& Data = p->GetEquip()[a].GetItemData();
							memcpy(&msg2other.SEquip[Index], &Data, sizeof(Equip));
							Index++;
						}
					}

					if (p != pPlayer)
					{
						m_pWorld->SendMsgToClient(&msg2other, pPlayer->GetSocket());
					}

				}
			}

			{
				MapMonster& MonsterMap = pCurCell->GetMonsterMap();
				MapMonster::iterator MonsterItor = MonsterMap.begin();
				{
					for (; MonsterItor != MonsterMap.end(); MonsterItor++)
					{
						CMonster* p = MonsterItor->second;
						if (p->GetState() != MONSTER_STATE_LOST)
						{
							if (p->GetState() != MONSTER_STATE_DEAD)
							{
								msg_monster.uiID = p->GetID();
								msg_monster.uiType = _OBJECT_TYPE_MONSTER;
								msg_monster.usMapID = m_lID;
								msg_monster.X = p->GetPosX();
								msg_monster.Y = p->GetPosY();
								msg_monster.Z = p->GetPosZ();
								msg_monster.fAtan2 = p->GetAtan2();
								msg_monster.uiHP = p->GetlHP();
								msg_monster.uiMaxHP = p->GetMaxHP();
								msg_monster.ucLV = p->GetRank();
								msg_monster.ucCamp = p->GetlFaction();
								sprintf(msg_monster.cName, "%s", p->GetName());
								m_pWorld->SendMsgToClient(&msg_monster, pPlayer->GetSocket());

							}
						}
					}

				}
			}

			{
				MapNPC& NPCMap = pCurCell->GetNPCMap();
				MapNPC::iterator NPCItor = NPCMap.begin();

				for (; NPCItor != NPCMap.end(); NPCItor++)
				{
					CNPC* p = NPCItor->second;
					msg_npc.uiID = p->GetID();
					msg_npc.uiType = _OBJECT_TYPE_NPC;
					msg_npc.usMapID = m_lID;
					msg_npc.ucCamp = p->GetlFaction();
					msg_npc.Y = p->GetPosY();
					msg_npc.Z = p->GetPosZ();
					msg_npc.fAtan2 = p->GetAtan2();
					msg_npc.ucLV = p->GetRank();
					sprintf(msg_npc.cName, "%s", p->GetName());
					msg_npc.X = p->GetPosX();
					msg_npc.Head.usSize = sizeof(MSG_OBJECTINFO);
					msg_npc.Head.usType = _MSG_OBJECTINFO;

					m_pWorld->SendMsgToClient(&msg_npc, pPlayer->GetSocket());

					p->GetQuestList(pPlayer);
				}
			}
		}
	}


	pPlayer->GetCurrentCell()->AddObj(pPlayer);
	pPlayer->SetJoinFlag(true);

	m_listObject.push_back((CGameObject*)pPlayer);
	sbase::CSingleLock xLock1(&m_xLock);
	m_listPlayer.push_back(pPlayer);
	return 0;
}


long CRegion::RemovePlayer(CPlayer* pPlayer)
{
	if (pPlayer == NULL)
		return -1;

	pPlayer->GetCurrentCell()->RemoveObj(pPlayer);

	sbase::CSingleLock xLock1(&m_xLock);
	m_listPlayer.remove(pPlayer);
	m_listObject.remove(pPlayer);
	msg_leave.uiID = pPlayer->GetID();

	EndFight(pPlayer);

	if (pPlayer->GetTrade()->IsTrading())
	{
		pPlayer->GetTrade()->GetTrader()->GetTrade()->EndTrade();
		pPlayer->GetTrade()->EndTrade();

		pPlayer->GetTrade()->GetTrader()->GetTrade()->ClearTrade();
		pPlayer->GetTrade()->ClearTrade();
	}

	SendAreaMsgOneToOther(pPlayer->GetCurrentCell(), &msg_leave, pPlayer);



	return 0;
}


long CRegion::GetOnline(void)
{
	return (long)m_listPlayer.size();
}


eError CRegion::Talk_Local(CPlayer* _pPlayer, MSG_TALK* _msg)
{
	if (!_pPlayer)
	{
		return	NO_MSG_ERRO;
	}
	for (PlayerIterator it = m_listPlayer.begin(); it != m_listPlayer.end(); it++)
	{
		CPlayer* p = *it;
		if (p->ucChannel & defLOCAL)
			m_pWorld->SendMsgToClient(_msg, p->GetSocket());
	}

	return	NO_MSG_ERRO;
}
eError CRegion::Talk_Faction(CPlayer* _pPlayer, MSG_TALK* _msg)
{
	if (!_pPlayer)
	{
		return	NO_MSG_ERRO;
	}

	for (int i = 0; i < ACTUAL_PLAYER_ONLINE; i++)
	{
		if (m_pWorld->g_pPlayer[i].IsActive() && (m_pWorld->g_pPlayer[i].GetlFaction() == _pPlayer->GetlFaction())
			&& m_pWorld->g_pPlayer[i].IsEnterWorld())
		{
			if (m_pWorld->g_pPlayer[i].ucChannel & defCAMP)
				m_pWorld->SendMsgToClient(_msg, m_pWorld->g_pPlayer[i].GetSocket());
		}
	}
	return	NO_MSG_ERRO;
}

eError CRegion::Talk_Union(CPlayer* _pPlayer, MSG_TALK* _msg)
{
	if (!_pPlayer)
	{
		return	NO_MSG_ERRO;
	}

	if (!_pPlayer->GetConsortia())
	{
		wstring unFound = L"对不起，你没有加入任何公会";
		MSG_TALK	msg_talk;
		msg_talk.ucAimPlayerNameSize = 0;
		msg_talk.ucResPlayerNameSize = 0;
		msg_talk.ucTextSize = (INT)((unFound.size() + 1) * 2);
		msg_talk.ucType = defSYSTEM;
		msg_talk.uiUserID = -1;
		wmemset(msg_talk.wcContent, 0, 256);
		wcscpy(msg_talk.wcContent, unFound.c_str());
		msg_talk.Head.usType = _MSG_TALK;
		msg_talk.Head.usSize = sizeof(msg_talk.Head) + 4 + 4 + 4;
		if ((msg_talk.ucTextSize % 4) != 0)
			msg_talk.Head.usSize += msg_talk.ucTextSize + 2;
		else
			msg_talk.Head.usSize += msg_talk.ucTextSize;

		m_pWorld->SendMsgToClient(&msg_talk, _pPlayer->GetSocket());

		return NO_MSG_ERRO;
	}

	_pPlayer->BroadcastConsortiaMsg(_msg);

	return NO_MSG_ERRO;
}

eError CRegion::Talk_World(CPlayer* _pPlayer, MSG_TALK* _msg)
{
	if (!_pPlayer || !_msg)
	{
		return NO_MSG_ERRO;
	}

	if (_pPlayer->UseItem(4101, 1))
	{
		_msg->ucType = defWORLD;
		for (int i = 0; i < ACTUAL_PLAYER_ONLINE; i++)
		{
			if (m_pWorld->g_pPlayer[i].IsActive() && m_pWorld->g_pPlayer[i].IsEnterWorld())
			{
				m_pWorld->SendMsgToClient(_msg, m_pWorld->g_pPlayer[i].GetSocket());
			}
		}
	}
	else
	{
		wstring unFound = L"对不起，您的世界频道暂未开通";
		MSG_TALK	msg_talk;
		msg_talk.ucAimPlayerNameSize = 0;
		msg_talk.ucResPlayerNameSize = 0;
		msg_talk.ucTextSize = (INT)((unFound.size() + 1) * 2);
		msg_talk.ucType = defSYSTEM;
		msg_talk.uiUserID = -1;
		wmemset(msg_talk.wcContent, 0, 256);
		wcscpy(msg_talk.wcContent, unFound.c_str());
		msg_talk.Head.usType = _MSG_TALK;
		msg_talk.Head.usSize = sizeof(msg_talk.Head) + 4 + 4 + 4;
		if ((msg_talk.ucTextSize % 4) != 0)
			msg_talk.Head.usSize += msg_talk.ucTextSize + 2;
		else
			msg_talk.Head.usSize += msg_talk.ucTextSize;

		m_pWorld->SendMsgToClient(&msg_talk, _pPlayer->GetSocket());
		return NO_MSG_ERRO;
	}
}

eError CRegion::Talk_GM(CPlayer* _pPlayer, MSG_TALK* _msg)
{
	if (!_pPlayer || !_msg)
	{
		return NO_MSG_ERRO;
	}
	if (_pPlayer->IsGM())
	{
		_msg->ucType = defGM;
		for (int i = 0; i < ACTUAL_PLAYER_ONLINE; i++)
		{
			if (m_pWorld->g_pPlayer[i].IsActive())
			{
				m_pWorld->SendMsgToClient(_msg, m_pWorld->g_pPlayer[i].GetSocket());
			}
		}
		return NO_MSG_ERRO;
	}
	return NO_MSG_ERRO;
}
eError CRegion::Talk_Team(CPlayer* _pPlayer, MSG_TALK* _msg)
{
	if (!_pPlayer)
	{
		return NO_MSG_ERRO;
	}
	if (NULL != _pPlayer->m_pTeamLeader)
	{
		if (_pPlayer->m_pTeamLeader->ucChannel & defTEAM)
			m_pWorld->SendMsgToClient(_msg, _pPlayer->m_pTeamLeader->GetSocket());
		for (vector<CPlayer*>::iterator iter = _pPlayer->m_pTeamLeader->m_Teammates.begin();
			iter != _pPlayer->m_pTeamLeader->m_Teammates.end();
			iter++)
		{
			if ((*iter)->ucChannel & defTEAM)
				m_pWorld->SendMsgToClient(_msg, (*iter)->GetSocket());
		}
		return NO_MSG_ERRO;
	}
	else
	{
		wstring unFound = L"对不起，您还没有加入任何队伍";
		MSG_TALK	msg_talk;
		msg_talk.ucAimPlayerNameSize = 0;
		msg_talk.ucResPlayerNameSize = 0;
		msg_talk.ucTextSize = (INT)((unFound.size() + 1) * 2);
		msg_talk.ucType = defSYSTEM;
		msg_talk.uiUserID = -1;
		wmemset(msg_talk.wcContent, 0, 256);
		wcscpy(msg_talk.wcContent, unFound.c_str());
		msg_talk.Head.usType = _MSG_TALK;
		msg_talk.Head.usSize = sizeof(msg_talk.Head) + 4 + 4 + 4;
		if ((msg_talk.ucTextSize % 4) != 0)
			msg_talk.Head.usSize += msg_talk.ucTextSize + 2;
		else
			msg_talk.Head.usSize += msg_talk.ucTextSize;

		m_pWorld->SendMsgToClient(&msg_talk, _pPlayer->GetSocket());
		return NO_MSG_ERRO;
	}
}

long CRegion::Avatar(CPlayer* pPlayer, int nPos, int nAvatarID)
{
	return 0;
}


eError CRegion::Skill(CGameObject* pPlayer, CGameObject* pTarget, float x, float, float z, UINT magicID, float x1, float, float z1)
{
	CPlayer* player = (CPlayer*)pPlayer;
	if (pPlayer->IsDead())
		return NO_MSG_ERRO;

	if (GetID() != pPlayer->GetRegion()->GetID())
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception, Accounts : %s, Role : %s, [_MSG_SKILL_1]", player->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}

	if (player->GetStall()->IsStart())
	{
		return MSG_ERRO_006A;
	}
	const MagicData* pMagic = pPlayer->s_World->g_pSkillManager->GetMagic(magicID);
	if (!pMagic)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception, Accounts : %s, Role : %s, [_MSG_SKILL_2]", player->GetAccounts(), pPlayer->GetName());
		return NO_MSG_ERRO;
	}
	if (!pPlayer->FindActiveSkill(magicID) && !pMagic->bIsEquipUse)
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception, Accounts : %s, Role : %s, [_MSG_SKILL_3]", player->GetAccounts(), pPlayer->GetName());
		return MSG_ERRO_006F;
	}

	if (!static_cast<CPlayer*>(pPlayer)->IsEquipThis(pMagic))
	{
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception, Accounts : %s, Role : %s, [_MSG_SKILL_4]", player->GetAccounts(), pPlayer->GetName());
		return MSG_ERRO_006E;
	}

	if (!pPlayer->IsPublicCDOK())
		return MSG_ERRO_006D;

	if (pPlayer->IsActiveSkillCooling(magicID))
	{
		int i = 5;
		i++;
		return MSG_ERRO_006D;
	}

	if (pPlayer->GetlMP() - pMagic->sMP < 0)
		return MSG_ERRO_006C;

	if (static_cast<CPlayer*>(pPlayer)->IsGodSkillTimeOut(magicID))
	{
		MSG_MAGIC_PERFORM mgicPerform;
		mgicPerform.Head.usSize = sizeof(MSG_MAGIC_PERFORM);
		mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
		mgicPerform.uiID = pPlayer->GetID();
		mgicPerform.x = pPlayer->GetPosX();
		mgicPerform.z = pPlayer->GetPosZ();
		mgicPerform.uiObjectID = pPlayer->GetID();
		mgicPerform.ucMagicID = magicID;
		SendAreaMsgOneToOther(pPlayer->GetCurrentCell(), &mgicPerform);

		return NO_MSG_ERRO;
	}

	if (pPlayer != pTarget)
	{
		float fPX = 0.0f;
		float fPZ = 0.0f;
		float fDist = 0.0f;
		if (pTarget) {
			fPX = x - pTarget->GetPosX();
			fPZ = z - pTarget->GetPosZ();
		}
		else {
			fPX = x - x1;
			fPZ = z - z1;
		}

		fDist = fPX * fPX + fPZ * fPZ;
		if (fDist > pMagic->ucDistance* pMagic->ucDistance + 0.5)
		{
			return MSG_ERRO_006B;
		}
	}

	if (!pPlayer->SetActiveSkillStartTime(magicID))
		return NO_MSG_ERRO;




	msg_skill.uiID = pPlayer->GetID();
	msg_skill.Head.usSize = sizeof(msg_skill);
	msg_skill.cType = magicID;
	if (pMagic->iProperty == PROPERTY_PHY)
		msg_skill.cStep = 1;
	else
		msg_skill.cStep = 0;
	msg_skill.x1 = x1;
	msg_skill.z1 = z1;
	msg_skill.x = x;
	msg_skill.z = z;
	if (pTarget)
		msg_skill.uiObjectID = pTarget->GetID();
	else
		msg_skill.uiObjectID = -1;

	pPlayer->SetCurActiveSkillID(magicID);
	pPlayer->m_eState = OBJECT_CAST;
	pPlayer->ResetTimer(pMagic->usCoolingTime, TIME_STYLE_COOL);
	pPlayer->ResetTimer(pMagic->ucIntonateTime, TIME_STYLE_CAST);
	pPlayer->SetTarget(pTarget);
	pPlayer->SetSkillTarget(pTarget);

	pPlayer->ChangeActiveSkillStatus(pPlayer->GetCurActiveSkillID(), TIME_STYLE_COOL);

	if (msg_skill.cStep == 0)
		SendAreaMsgOneToOther(pPlayer->GetCurrentCell(), &msg_skill);
	else
		SendAreaMsgOneToOther(pPlayer->GetCurrentCell(), &msg_skill, (CPlayer*)pPlayer);

	pPlayer->StartPublicCD();

	return NO_MSG_ERRO;
}

long CRegion::Effects(CPlayer* pPlayer, void* Msg)
{

	MSG_EFFECT* temp = static_cast<MSG_EFFECT*>(Msg);
	memcpy(&msg_effect, temp, sizeof(MSG_EFFECT));

	msg_effect.Head.usSize = sizeof(msg_effect);
	SendAreaMsgOneToOther(pPlayer->GetCurrentCell(), &msg_effect);
	return 0;

}

long CRegion::LearnSkill(CPlayer* pPlayer, unsigned char cType)
{
	const SKillData* pSkill = pPlayer->s_World->g_pSkillManager->GetSkill(cType, 0);
	IF_NOT(pSkill)
		return 0;


	if (pPlayer->FindPassiveSkill(cType))
	{
		return 0;
	}

	switch (cType)
	{
	case 0:
	{

	}
	break;
	case 1:
	case 2:
	case 3:
	{
		if (pPlayer->GetcRank() < 4)
		{
			return 0;
		}

	}
	break;
	case 4:
	{
		if (pPlayer->GetcRank() < 6)
		{
			return 0;
		}

	}
	break;
	case 5:
	{
		if (pPlayer->GetcRank() < 10)
		{
			return 0;
		}

	}
	break;
	case 6:
	{
		if (pPlayer->GetcRank() < 16)
		{
			return 0;
		}

	}
	break;
	case 7:
	{
		if (pPlayer->GetcRank() < 20)
		{
			return 0;
		}

	}
	break;
	case 8:
	{
		if (pPlayer->GetcRank() < 25)
		{
			return 0;
		}

	}
	break;
	case 9:
	{
		if (pPlayer->GetcRank() < 30)
		{
			return 0;
		}

	}
	break;
	case 10:
	{

	}
	break;
	case 11:
	{
		if (pPlayer->GetcRank() < 4)
		{
			return 0;
		}
	}
	break;
	case 12:
	{
		if (pPlayer->GetcRank() < 4)
		{
			return 0;
		}
	}
	break;
	case 13:
	{
		if (pPlayer->GetcRank() < 4)
		{
			return 0;
		}
	}
	break;
	case 14:
	{
		if (pPlayer->GetcRank() < 4)
		{
			return 0;
		}
	}
	break;
	case 15:
	{
		if (pPlayer->GetcRank() < 10)
		{
			return 0;
		}
	}
	break;
	case 16:
	{
		if (pPlayer->GetcRank() < 16)
		{
			return 0;
		}
	}
	break;
	case 17:
	{
		if (pPlayer->GetcRank() < 20)
		{
			return 0;
		}
	}
	break;
	case 18:
	{
		if (pPlayer->GetcRank() < 25)
		{
			return 0;
		}
	}
	break;
	case 19:
	{
		if (pPlayer->GetcRank() < 30)
		{
			return 0;
		}
	}
	break;


	default:
		break;
	}

	pPlayer->AddPassiveSkill(cType);

	if (pSkill == NULL)
	{
		msg_skill_learn.bIsSucced = false;
	}
	else
	{
		msg_skill_learn.bIsSucced = true;
	}

	msg_skill_learn.cType = cType;
	msg_skill_learn.cKinds = 0;
	msg_skill_learn.uiID = pPlayer->GetID();

	SendAreaMsgOneToOther(pPlayer->GetCurrentCell(), &msg_skill_learn);
	return 0;
}

long CRegion::LearnMagic(CPlayer* pPlayer, unsigned char cType, BYTE Flag)
{
	const  MagicData* pMagic = pPlayer->s_World->g_pSkillManager->GetMagic(cType);
	if (pMagic == NULL)
	{
		return 0;
	}
	if (pPlayer->FindActiveSkill(cType))
	{
		return 0;
	}

	switch (cType)
	{
	case 0:
	{

	}
	break;
	case 1:
	case 2:
	case 3:
	{
		if (pPlayer->GetcRank() < 4)
		{
			return 0;
		}

	}
	break;
	case 4:
	{
		if (pPlayer->GetcRank() < 6)
		{
			return 0;
		}

	}
	break;
	case 5:
	{
		if (pPlayer->GetcRank() < 10)
		{
			return 0;
		}

	}
	break;
	case 6:
	{
		if (pPlayer->GetcRank() < 16)
		{
			return 0;
		}

	}
	break;
	case 7:
	{
		if (pPlayer->GetcRank() < 20)
		{
			return 0;
		}

	}
	break;
	case 8:
	{
		if (pPlayer->GetcRank() < 25)
		{
			return 0;
		}

	}
	break;
	case 9:
	{
		if (pPlayer->GetcRank() < 30)
		{
			return 0;
		}

	}
	break;
	case 10:
	{

	}
	break;
	case 11:
	{
		if (pPlayer->GetcRank() < 4)
		{
			return 0;
		}
	}
	break;
	case 12:
	{
		if (pPlayer->GetcRank() < 4)
		{
			return 0;
		}
	}
	break;
	case 13:
	{
		if (pPlayer->GetcRank() < 4)
		{
			return 0;
		}
	}
	break;
	case 14:
	{
		if (pPlayer->GetcRank() < 4)
		{
			return 0;
		}
	}
	break;
	case 15:
	{
		if (pPlayer->GetcRank() < 10)
		{
			return 0;
		}
	}
	break;
	case 16:
	{
		if (pPlayer->GetcRank() < 16)
		{
			return 0;
		}
	}
	break;
	case 17:
	{
		if (pPlayer->GetcRank() < 20)
		{
			return 0;
		}
	}
	break;
	case 18:
	{
		if (pPlayer->GetcRank() < 25)
		{
			return 0;
		}
	}
	break;
	case 19:
	{
		if (pPlayer->GetcRank() < 30)
		{
			return 0;
		}
	}
	break;


	default:
		break;
	}
	pPlayer->AddActiveSkill(cType);

	msg_skill_learn.cType = cType;
	msg_skill_learn.cKinds = Flag;
	msg_skill_learn.uiID = pPlayer->GetID();

	if (pMagic == NULL)
	{
		msg_skill_learn.bIsSucced = false;
	}
	else
	{
		msg_skill_learn.bIsSucced = true;
	}

	SendAreaMsgOneToOther(pPlayer->GetCurrentCell(), &msg_skill_learn);
	return -1;
}



long CRegion::SendMsgToPlayer(void* buf)
{
	for (PlayerIterator it = m_listPlayer.begin(); it != m_listPlayer.end(); it++)
	{
		CPlayer* p = *it;
		m_pWorld->SendMsgToClient(buf, p->GetSocket());
	}
	return 0;
}

eError CRegion::PickupDrops(CPlayer* pPlayer, CMonster* pMonster, int index)
{
	if (MAX_DROPS <= index)
		return NO_MSG_ERRO;

	Item* p = &pMonster->m_DropItems[index];

	if (!pMonster->IsOwner(pPlayer->GetName()))
		return NO_MSG_ERRO;

	if (pPlayer->PickupDrops(p))
	{
		msg_pickup_drops.PickupIndex = index;

		pMonster->m_DropItems[index].Clear();
		pMonster->m_Drops--;

		if (!pMonster->m_Drops)
			pMonster->ResetCorpseTimer();

		msg_pickup_drops.uiID = pPlayer->GetID();
		msg_pickup_drops.uiMonsterID = pMonster->GetID();

		for (int i = 0; i < MAX_OWNER; i++)
		{
			if (pMonster->m_ItemOwner[i])
				m_pWorld->SendMsgToClient(&msg_pickup_drops, ((CPlayer*)pMonster->m_ItemOwner[i])->GetSocket());
		}
	}

	return NO_MSG_ERRO;
}

eError CRegion::UseOrEquip(CPlayer* pPlayer, int index, int num)
{
	Item* pitem = pPlayer->GetItem(index, num);

	if (!pitem)
		return MSG_ERRO_00E3;

	if (pitem->IsClear())
		return MSG_ERRO_00F2;

	Item temp = *pitem;

	if (pPlayer->UseOrEquip(index, num))
	{
		msg_useorequip_item.Index = (USHORT)index;
		msg_useorequip_item.Num = (USHORT)num;
		msg_useorequip_item.uiID = pPlayer->GetID();
		msg_useorequip_item.False = false;

		msg_useorequip_item.Base = temp.GetItemBaseAttribute()->ID;
		for (int i = 0; i < MAX_EQUIPAPPEND_COUNT; i++)
		{
			if (temp.GetItemAppendAttribute(i))
			{
				msg_useorequip_item.Append[i] = temp.GetItemAppendAttribute(i)->ID;
			}
			else
			{
				msg_useorequip_item.Append[i] = -1;
			}
		}

		msg_useorequip_item.BaseLevel = temp.BaseLevel;
		msg_useorequip_item.AppendLevel = temp.AppLevel;

		SendAreaMsgOneToOther(pPlayer->GetCurrentCell(), &msg_useorequip_item);
	}
	else
	{
		msg_useorequip_item.Index = (USHORT)index;
		msg_useorequip_item.Num = (USHORT)num;
		msg_useorequip_item.uiID = pPlayer->GetID();
		msg_useorequip_item.False = true;

		m_pWorld->SendMsgToClient(&msg_useorequip_item, pPlayer->GetSocket());
	}

	return NO_MSG_ERRO;
}

long CRegion::AddRelation(CPlayer* pPlayer, CPlayer* pOpsite)
{
	__asm int 3;
	memcpy(msg_responses.cName_Request, pPlayer->GetName(), sizeof(msg_responses.cName_Request));
	msg_responses.IsAgree = true;
	m_pWorld->SendMsgToClient(&msg_responses, pOpsite->GetSocket());
	return 0;
}

CGameObject* CRegion::GetNearestPartner(CGameObject* obj)
{
	CGameObject* nearest = NULL;

	int distance = 100;

	for (MonsterIterator iter = m_listMonster.begin(); iter != m_listMonster.end(); iter++)
	{
		float fPX = obj->m_fX - (*iter)->GetPosX();
		float fPZ = obj->m_fZ - (*iter)->GetPosZ();

		float dis = sqrt(fPX * fPX + fPZ * fPZ);

		if (strcmp((*iter)->GetName(), obj->GetName()) == 0 && *iter != obj
			&& !(*iter)->m_bIsFight && (*iter)->m_eState != OBJECT_DEAD)
		{
			if (distance > dis)
			{
				nearest = *iter;

				distance = (int)dis;
			}

		}
	}

	return nearest;
}

void CRegion::StartFight(CGameObject* obj, CGameObject* target)
{
	if (!obj->GetFight())
	{
		msg_fight.Head.usSize = sizeof(msg_fight);
		msg_fight.Fight = 1;
		msg_fight.uiID = obj->GetID();

		SendAreaMsgOneToOther(obj->GetCurrentCell(), &msg_fight);
		if (obj->GetType() == OBJECTTYPE_MONSTER)
			((CMonster*)obj)->SetFightPosition();
	}

	obj->StartFight(target);
}

void CRegion::EndFight(CGameObject* obj)
{
	msg_fight.Head.usSize = sizeof(msg_fight);
	msg_fight.Fight = 0;
	msg_fight.uiID = obj->GetID();

	SendAreaMsgOneToOther(obj->GetCurrentCell(), &msg_fight);

	for (EnmityIterator iter = obj->m_listEnmity.begin(); iter != obj->m_listEnmity.end(); iter++)
	{
		iter->pObject->RemoveEnmity(obj);

		if (iter->pObject->IsClearEnmity())
			iter->pObject->GetRegion()->EndFight(iter->pObject);
	}

	obj->EndFight();
}
void CRegion::CollectGameObjectSphere(CGameObject* caster, ObjectList* objlst, float vCenter[3],
	const MagicData* magicData, int max)
{
	int flags = magicData->ucAffectObj;
	float		x, z, squareLength;
	const float squareRadius = magicData->ucRange * magicData->ucRange;
	eObjectType eType;


	for (ObjectIterator it = m_listObject.begin(); it != m_listObject.end(); it++)
	{
		CGameObject* p = *it;

		if (p->IsDead())
			continue;

		eType = p->GetType();

		if (eType != OBJECTTYPE_MONSTER &&
			eType != OBJECTTYPE_PLAYER)
		{
			continue;
		}

		if (eType == OBJECTTYPE_MONSTER && (caster->GetlFaction() == p->GetlFaction()))
			continue;

		if (p->GetlHP() <= 0)
			continue;

		x = p->GetPosX();
		z = p->GetPosZ();

		x -= vCenter[0];
		z -= vCenter[2];

		squareLength = x * x + z * z;
		if (squareLength < squareRadius)
		{
			if (caster->GetType() == OBJECTTYPE_MONSTER)
			{
				if (eType == OBJECTTYPE_PLAYER)
					objlst->push_back(p);
				continue;
			}

			if (NULL != CollectTarget(caster, p, flags, magicData))
			{
				objlst->push_back(p);
				continue;
			}
		}
		if ((int)objlst->size() == max)
			break;
	}
}

CGameObject* CRegion::CollectTarget(CGameObject* pCaster, CGameObject* pTarget, UINT flags, const MagicData* magicData)
{
	if (NULL == pTarget)
		return NULL;

	bool bRightTarget = false;

	if (flags & TARGET_SELF && pTarget->GetlHP() > 0)
	{
		if (pCaster == pTarget && pTarget->GetlHP() > 0)
		{
			bRightTarget = true;
		}
	}
	if (flags & TARGET_SAME_FACTION && pTarget->GetlHP() > 0)
	{
		if (pCaster->GetlFaction() == pTarget->GetlFaction() && pCaster != pTarget
			&& (pTarget->GetcRank() > MAX_LEVEL_LIMIT || magicData->ucStyle == 1))
		{
			bRightTarget = true;
		}
	}
	if (flags & TARGET_DIFF_FACTION && pTarget->GetlHP() > 0)
	{
		if ((pCaster->GetlFaction() != pTarget->GetlFaction()) && pTarget->GetcRank() > MAX_LEVEL_LIMIT&& pCaster->GetcRank() > MAX_LEVEL_LIMIT)
		{
			bRightTarget = true;
		}
	}
	if (flags & TARGET_MONSTER && pTarget->GetlHP() > 0)
	{
		if (pTarget->GetType() == OBJECTTYPE_MONSTER)
		{
			bRightTarget = true;
		}
	}

	return bRightTarget ? pTarget : NULL;
}

void CRegion::CollectGameObjectAABB(CPlayer* player, ObjectList* objlst, float vMin[3], float vMax[3], int flags, int max)
{
	float x, z;
	eObjectType eType;

	for (ObjectIterator it = m_listObject.begin(); it != m_listObject.end(); it++)
	{
		CGameObject* p = *it;

		eType = p->GetType();

		if (eType != OBJECTTYPE_MONSTER &&
			eType != OBJECTTYPE_PLAYER)
		{
			continue;
		}

		x = p->GetPosX();
		z = p->GetPosZ();

		if (x < vMax[0] && x > vMin[0] && z < vMax[2] && z > vMin[2])
		{
			if (flags & TARGET_SELF) {
				if (player == p && p->GetlHP() > 0)
				{
					objlst->push_back(p);
					continue;
				}
			}
			if (flags & TARGET_SAME_FACTION)
			{
				if (player->GetlFaction() == p->GetlFaction() && player != p)
				{
					objlst->push_back(p);
					continue;
				}
			}
			if (flags & TARGET_DIFF_FACTION)
			{
				if (player->GetlFaction() != p->GetlFaction())
				{
					objlst->push_back(p);
					continue;
				}
			}
			if (flags & TARGET_MONSTER && p->GetlHP() > 0)
			{
				if (p->GetType() == OBJECTTYPE_MONSTER)
				{
					objlst->push_back(p);
					continue;
				}
			}
		}

		if ((int)objlst->size() == max)
			break;
	}
}
void	CRegion::AddMagicRegion(int playerID, int magicID, float vCenter[3], float radius, int flags, int targetID)
{
	if (m_lstMgcRgnFree.size())
	{
		MgcRgnIterator Iter = m_lstMgcRgnFree.begin();
		MagicRegion* mgcRgn = *Iter;

		mgcRgn->flags = flags;
		mgcRgn->playerID = playerID;
		mgcRgn->magicID = magicID;
		mgcRgn->vCenter[0] = vCenter[0];
		mgcRgn->vCenter[1] = vCenter[1];
		mgcRgn->vCenter[2] = vCenter[2];
		mgcRgn->fRadius = radius;
		mgcRgn->iCount = 0;
		mgcRgn->lastTime = timeGetTime();
		mgcRgn->targetID = targetID;

		m_lstMgcRgnActive.push_back(mgcRgn);
		m_lstMgcRgnFree.erase(Iter);
	}
	else {
	}
}
void	CRegion::FreeMagicRegion(MgcRgnIterator Iter)
{
	m_lstMgcRgnFree.push_back(*Iter);
	m_lstMgcRgnActive.erase(Iter);
}
void	CRegion::UpdateMagicRegion(void)
{
	for (MgcRgnIterator MgcIter = m_lstMgcRgnActive.begin(); MgcIter != m_lstMgcRgnActive.end(); )
	{
		ObjectList				objlst;
		CGameObject* pTarget;
		MagicRegion* mgcRgn = *MgcIter;
		CGameObject* caster = m_pWorld->GetObject(mgcRgn->playerID);
		const MagicData* magicData = caster->s_World->g_pSkillManager->GetMagic(mgcRgn->magicID);

		if (caster == NULL)
		{
			MgcRgnIterator tIter = MgcIter;
			MgcIter++;
			FreeMagicRegion(tIter);
			continue;
		}

		if (timeGetTime() - mgcRgn->lastTime > (magicData->fTimeInterval * 1000))
		{
			mgcRgn->lastTime += (int)magicData->fTimeInterval * 1000;
			mgcRgn->iCount++;

			if (mgcRgn->iCount >= magicData->iEffectTurn) {
				MgcRgnIterator tIter = MgcIter;
				MgcIter++;
				FreeMagicRegion(tIter);
				continue;
			}
		}
		else
		{
			MgcIter++;
			continue;
		}

		CollectGameObjectSphere(caster, &objlst, mgcRgn->vCenter, magicData);

		int count = (int)objlst.size();

		if (((magicData->ucTarget & TARGET_POS) == TARGET_POS) || (magicData->ucRange > 0))
		{
			int size = int(sizeof(MsgHead) + sizeof(int) + 2 * sizeof(UINT) + sizeof(BYTE) + count * sizeof(MAGIC_DAMAGE));
			MSG_MAGIC_CLUSTER_DAMAGE  tmpDamage;
			MSG_MAGIC_CLUSTER_DAMAGE* clstDamager = &tmpDamage;

			if (clstDamager == NULL)
				break;

			clstDamager->uiID = mgcRgn->playerID;
			clstDamager->iCount = count;
			clstDamager->Head.usSize = (USHORT)size;
			clstDamager->Head.usType = _MSG_MAGIC_CLUSTER_DAMAGE;

			int  i = 0;
			for (ObjectIterator Iter = objlst.begin(); Iter != objlst.end(); Iter++)
			{
				MAGIC_DAMAGE* mgicDamage = &clstDamager->magicDamage[i];
				++i;

				pTarget = (CGameObject*)*Iter;

				mgicDamage->uiObjectID = pTarget->GetID();

				pTarget->ClearDamageInfo();
				ExcScript(caster, pTarget, magicData->ucID);
				DAMAGE_INFO damageInfo = pTarget->GetDamageInfo();

				if (damageInfo.HP) {
					mgicDamage->ucType = 0;
					mgicDamage->iPower = damageInfo.HP;
					mgicDamage->Attack_Type = 1;
				}
				else {
					mgicDamage->ucType = 0;
					mgicDamage->iPower = damageInfo.MP;
					mgicDamage->Attack_Type = 1;

				}
				if (pTarget->GetType() == OBJECTTYPE_MONSTER || pTarget->GetType() == OBJECTTYPE_PLAYER)
				{
					if (pTarget->GetlHP() <= 0)
					{
						pTarget->Dead(caster);
					}
				}
			}
			SendAreaMsgOneToOther(caster->GetCurrentCell(), clstDamager);
			objlst.clear();

			MgcIter++;

		}
		else
		{
			int size = int(sizeof(MSG_MAGIC_DAMAGE));
			MSG_MAGIC_DAMAGE  tmpDamage;
			MSG_MAGIC_DAMAGE* singleDamager = &tmpDamage;

			if (singleDamager == NULL)
				break;

			singleDamager->Head.usSize = (USHORT)size;
			singleDamager->Head.usType = _MSG_MAGIC_DAMAGE;

			pTarget = (CGameObject*)m_pWorld->GetObject(mgcRgn->targetID);
			if (!pTarget)
				return;
			if (!pTarget)
				return;

			singleDamager->uiObjectID = mgcRgn->targetID;

			pTarget->ClearDamageInfo();
			ExcScript(caster, pTarget, caster->GetCurActiveSkillID());
			DAMAGE_INFO damageInfo = pTarget->GetDamageInfo();

			if (damageInfo.HP) {
				singleDamager->bType = 0;
				singleDamager->iPower = damageInfo.HP;
			}
			else {
				singleDamager->bType = 0;
				singleDamager->iPower = damageInfo.MP;
			}

			if (pTarget->GetType() == OBJECTTYPE_MONSTER || pTarget->GetType() == OBJECTTYPE_PLAYER)
			{
				if (pTarget->GetlHP() <= 0)
				{
					pTarget->Dead(caster);
				}
			}
			SendAreaMsgOneToOther(caster->GetCurrentCell(), singleDamager);
			objlst.clear();

			MgcIter++;
		}
	}

}

void CRegion::BackUpPlayerData()
{
	sbase::CSingleLock xLock(&m_xLock);
	for (PlayerIterator it = m_listPlayer.begin(); it != m_listPlayer.end(); it++)
	{
		CPlayer* p1 = *it;
		p1->BackUp();
	}
}

BYTE CRegion::GetCellID(CGameObject* pObj)
{
	return (BYTE)m_pMap->GetCell(pObj->GetPosX(), pObj->GetPosZ())->GetCellID();
}

void  CRegion::RefreshInitiativeMonster(CPlayer* player)
{
	CCell* pCell = player->GetCurrentCell();
	if (NULL == pCell)
	{
		return;
	}

	std::vector< BYTE >  VecCell = pCell->GetArea();
	std::vector< BYTE >::iterator itor = VecCell.begin();
	for (; itor != VecCell.end(); itor++)
	{
		CCell* pCurCell = m_pMap->GetCell(*itor);
		MapMonster& MonsterMap = pCurCell->GetMonsterMap();
		MapMonster::iterator iter = MonsterMap.begin();

		for (; iter != MonsterMap.end(); iter++)
		{
			CMonster* pMonster = iter->second;

			if (pMonster->GetlFaction() != player->GetlFaction() && pMonster->GetMode() == MONSTER_MODE_INITIATIVE && !pMonster->IsDead())
			{
				if (!pMonster->GetFight() && !pMonster->IsLeaveFight() && pMonster->GetDist(player->GetPosX(), player->GetPosZ()) < pMonster->m_fEyeshot)
				{
					StartFight(player, pMonster);
					StartFight(pMonster, player);
				}
			}
		}
	}
}

bool CRegion::WriteDataToDB()
{
	for (PlayerIterator it = m_listPlayer.begin(); it != m_listPlayer.end(); it++)
	{
		CPlayer* p = *it;
		p->UpDateDB();
	}
	return true;
}

bool CRegion::SendNoteToClient(const char* Note)
{
	for (PlayerIterator it = m_listPlayer.begin(); it != m_listPlayer.end(); it++)
	{
		CPlayer* p = *it;
		p->SendNoteToClient(Note);
	}
	return true;
}

bool CRegion::SendSysNoteToClient(const char* Note)
{
	for (PlayerIterator it = m_listPlayer.begin(); it != m_listPlayer.end(); it++)
	{
		CPlayer* p = *it;
		p->SendSysNoteToClient(Note);
	}
	return true;
}
bool CRegion::SendMidNoteToClient(const char* Note)
{
	for (PlayerIterator it = m_listPlayer.begin(); it != m_listPlayer.end(); it++)
	{
		CPlayer* p = *it;
		p->SendMidNoteToClient(Note);
	}
	return true;
}
void CRegion::AddGMControl(ActionElem& elem)
{
	m_GMctr->Push(elem);
}
void CRegion::DelActionCtr(GM_MSG_TYPE eType)
{
	m_GMctr->DelActionCtr(eType);
}
bool CRegion::GM_Handler_placard(ActionElem& elem)
{
	if (elem.Head.usType == ACTION_SYS_PLACARD)
	{
		switch (elem.PlacardType)
		{
		case 0:
		{
			CRegion* pRegion = static_cast<CRegion*>(elem.Obj);
			if (elem.SendType == 0)
			{
				pRegion->SendSysNoteToClient(elem.PlacardInfo);
			}
			else if (elem.SendType == 1)
			{
				pRegion->SendMidNoteToClient(elem.PlacardInfo);
			}
			else if (elem.SendType == 2)
			{
				pRegion->SendNoteToClient(elem.PlacardInfo);
			}
			return true;
		}
		break;
		case 1:
		{
			if (time(NULL) >= elem.OriginalTime + elem.EndTime || elem.EndTime < 0)
			{
				return true;
			}
			if (time(NULL) > elem.OriginalTime + elem.StartTime && time(NULL) < elem.OriginalTime + elem.EndTime)
			{
				if (elem.HaveRepeat > elem.Repeat)
					return true;
				else if ((time(NULL) - elem.OriginalTime) > elem.HaveRepeat* elem.IntervalTime)
				{
					CRegion* pRegion = static_cast<CRegion*>(elem.Obj);
					if (elem.SendType == 0)
					{
						pRegion->SendSysNoteToClient(elem.PlacardInfo);
					}
					else if (elem.SendType == 1)
					{
						pRegion->SendMidNoteToClient(elem.PlacardInfo);
					}
					else if (elem.SendType == 2)
					{
						pRegion->SendNoteToClient(elem.PlacardInfo);
					}
					elem.HaveRepeat++;
				}
			}

		}
		break;
		}
	}
	return false;
}

bool CRegion::GM_Handler_ShutDown(ActionElem& elem)
{

	if (elem.Head.usType == ACTION_SYS_SHUTDOWN)
	{
		if (elem.OriginalTime + elem.StartTime > time(NULL))
		{
			if (elem.OriginalTime + elem.StartTime - SERVER_SHOWDOWN_START < time(NULL))
			{
				if (elem.StartTime > SERVER_SHOWDOWN_START)
				{
					if ((time(NULL) - elem.OriginalTime - elem.StartTime + SERVER_SHOWDOWN_START) > elem.HaveRepeat* elem.IntervalTime)
					{
						if (elem.HaveRepeat > elem.Repeat)
						{
							ExitProcess(0);
							return true;
						}
						char note[256];
						memset(note, 0L, sizeof(note));
						int minute = (elem.OriginalTime + elem.StartTime - time(NULL)) / 60;
						if (minute == 0)
						{
							sprintf(note, "即将关闭服务器,请大家及时退出游戏", minute);
						}
						else
						{
							sprintf(note, "%d分钟后关闭服务器,请大家及时退出游戏", minute);
						}

						strcpy(elem.PlacardInfo, note);
						CRegion* pRegion = static_cast<CRegion*>(elem.Obj);
						pRegion->SendNoteToClient(elem.PlacardInfo);
						elem.HaveRepeat++;
					}
				}
				else
				{
					if ((time(NULL) - elem.OriginalTime) > elem.HaveRepeat* elem.IntervalTime)
					{
						if (elem.HaveRepeat > elem.Repeat)
						{
							ExitProcess(0);
							return true;
						}
						char note[256];
						memset(note, 0L, sizeof(note));
						int minute = (elem.OriginalTime + elem.StartTime - time(NULL)) / 60;
						if (minute == 0)
						{
							sprintf(note, "即将关闭服务器,请大家及时退出游戏", minute);
						}
						else
						{
							sprintf(note, "%d分钟后关闭服务器,请大家及时退出游戏", minute);
						}

						strcpy(elem.PlacardInfo, note);
						CRegion* pRegion = static_cast<CRegion*>(elem.Obj);
						pRegion->SendNoteToClient(elem.PlacardInfo);
						elem.HaveRepeat++;
					}
				}
			}
		}
		else
		{
			ExitProcess(0);
			return true;
		}
	}

	return false;
}
