#include "stdafx.h"
#include "Player.h"
#include "NPC.h"
#include "Error.h"
#include "ItemManager.h"
#include "QuestManager.h"
#include "../DataCore.h"
#include "../ServiceLoader.h"
#include "..\ErrorMessage.h"
#include "../../Common/MsgDefine.h"
#include "../../Common/MsgTypes.h"
#include "Map.h"
#include "../World.h"

extern MSG_NPC_ACCEPTQUEST msg_npc_acceptquest;
extern MSG_NPC_CANCELQUEST msg_npc_cancelquest;
extern MSG_NPC_COMPLETEQUEST msg_npc_completequest;
extern MSG_NPC_QUESTFAILD msg_npc_questfaild;
extern MSG_NPC_QUESTVIEW msg_npc_questview;
extern MSG_NPC_REWARDQUEST msg_npc_rewardquest;
extern MSG_NPC_QUEST msg_npc_quest;
extern MSG_NPC_QUESTSAVAILABLE msg_npc_questsavailable;
extern MSG_NPC_QUESTSUNAVAILABLE msg_npc_questsunavailable;
extern MSG_NPC_NEXTQUEST msg_npc_nextquest;
extern MSG_NPC_QUESTKILLORCAST msg_npc_questkillorcast;
extern MSG_PLAYER_ACCEPTQUESTS msg_player_acceptquests;
extern MSG_FINDQUESTRESULT msg_findquestresult;
extern MSG_USEOREQUIP_ITEM msg_useorequip_item;
extern MSG_NPC_QUESTEXPLORERRESULT msg_npc_questexplorerresult;


void CPlayer::Explorer()//探索任务验证
{
	for (vector<SQuestStatus*>::iterator iter = m_AcceptQuests.begin() ; iter != m_AcceptQuests.end() ; iter++)
	{

		if ((*iter)->Quest->HasSpecialFlag( QUEST_SPECIAL_FLAGS_MAP_EXPOLORER ) )
		{
			static int retIndex = -1;
			if ( -1 != ( retIndex = (*iter)->Explorer( GetRegion()->GetID() , this->GetPosX(), this->GetPosZ()) ) )
			{
				msg_npc_questexplorerresult.QuestID = (*iter)->Quest->QuestID;
				msg_npc_questexplorerresult.NPCID	= (*iter)->Quest->NPCID;
				msg_npc_questexplorerresult.Index	= retIndex;
	
				s_World->SendMsgToClient(&msg_npc_questexplorerresult , m_pSocket);
			}
		}
	}
}

void CPlayer::FindQuest()
{
	msg_findquestresult.Count = 0;

	for (int i = 0 ; i < QUEST_COUNT ; i++)
	{
		if (m_Quests[i].CanAccept() && m_Quests[i].Quest)
		{		
			if (msg_findquestresult.Count >= QUEST_FIND_COUNT)
			{
				s_World->SendMsgToClient(&msg_findquestresult , m_pSocket);
				return;
			}

			//if (m_Quests[i].Quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED))
			//	continue;
			if ( m_Quests[i].Quest->QuestSort == QUEST_SORT_RANDOM)//随机（任务道具）不可被查找
				continue;
			if ( false == QuestConsortiaLimit(m_Quests[i].Quest->QuestID))//公会长可查看
				continue;

			msg_findquestresult.Quest[msg_findquestresult.Count] = m_Quests[i].Quest->QuestID;
			msg_findquestresult.Count++;
		}
	}

	s_World->SendMsgToClient(&msg_findquestresult , m_pSocket);
}

void CPlayer::ClearQuest()
{
	for (int i = 0 ; i < QUEST_COUNT ; i++)
	{
		m_Quests[i].Quest = NULL;
		m_Quests[i].Release();

		memset(m_Quests[i].ItemCount,    0, QUEST_OBJECTIVES_COUNT * sizeof(short));
		memset(m_Quests[i].CreatureCount, 0, QUEST_OBJECTIVES_COUNT * sizeof(short));
	}

	m_TimeQuestsMap.clear();

	m_AcceptQuests.clear();
	m_AvailableQuests.clear();
	m_UnavailableQuests.clear();
}

bool CPlayer::QuestStatusUpdata(int id)
{
	//id是索引								

	//assert(id < QUEST_COUNT);
	if( id >= QUEST_COUNT )
		return false;

	if (!m_Quests[id].Quest)
		return false;

	if (m_Quests[id].Status != QUEST_STATUS_UNAVAILABLE &&
		m_Quests[id].Status != QUEST_STATUS_NONE)
		return false;

	m_Quests[id].Status = QUEST_STATUS_UNAVAILABLE;

	if (GetcRank() > m_Quests[id].Quest->MaxLevel){return false;}
	if (GetcRank() < m_Quests[id].Quest->MinLevel){return true;}

	if( false == QuestClassLimit(id) )
		return false;//职业限制

	if( false == QuestTeamLimit(id) )
		return false;//队伍限制

	//if( false == QuestConsortiaLimit(id) )
	//	return false;//公会限制

	//超时
	if (m_Quests[id].Quest->TimeOver)
		return true;

	if (m_Quests[id].Quest->RequiredBelief != QUEST_BELIEF_NONE &&
		m_Quests[id].Quest->RequiredBelief != m_ObjectData.m_usBelief)
		return true;

	//判断前序任务
	if (m_Quests[id].Quest->PrevQuestId != -1)
	{
		if (GetQuest(m_Quests[id].Quest->PrevQuestId)->Status != QUEST_STATUS_OVER)
		{
			if ( m_Quests[id].Quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_LOOP) )//链式任务下一个任务查找
			{
				if( GetQuest(m_Quests[id].Quest->PrevQuestId)->Completed <= m_Quests[id].Completed )
					return true;
			}
			else
			{
				return true;
			}
		}
	}
	else
	{
		//if ( m_Quests[id].Quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_LOOP) )//链式任务
		//{
		//	if ( IfLoopQuestAccepted(id))
		//		return true;
		//}
	}

	vector<SQuestStatus*>::iterator iter = std::find(m_UnavailableQuests.begin() , m_UnavailableQuests.end() , m_Quests + id);
	if (iter != m_UnavailableQuests.end())
		m_UnavailableQuests.erase(iter);

	m_Quests[id].Status = QUEST_STATUS_AVAILABLE;

	return true;
}

bool CPlayer::IfLoopQuestAccepted( int m_questid )
{
	if( m_questid < 0 || m_questid >= QUEST_COUNT || !m_Quests[m_questid].Quest )
		return false;

	int qid = m_Quests[m_questid].Quest->QuestID;
	int nqid = m_Quests[m_questid].Quest->NextQuestId;
	int safeTimes = 0;//防止配置填错造成死LOOP

	nqid = (m_ObjectData.m_lFaction == 1 ? nqid - QUEST_COUNT : nqid);

	while ( nqid != qid &&  safeTimes++ < 32 && nqid != -1 )
	{
		//链式任务第一个任务
		for (vector<SQuestStatus*>::iterator iter = m_AcceptQuests.begin() ; iter != m_AcceptQuests.end() ; iter++)
		{
			if( (*iter)->Quest && (*iter)->Quest->QuestID == nqid )
				return true;
		}

		QuestInfo* qi = QuestManager::Instance()->FindQuest(qid);
		if( qi == NULL )
			return false;

		nqid = qi->NextQuestId;
		nqid = (m_ObjectData.m_lFaction == 1 ? nqid - QUEST_COUNT : nqid);
	}

	return false;
}

void CPlayer::AccpetConsortiaQuest(int index, int q_type)
{
	SQuestStatus* sqs = &m_Quests[index];
	switch(q_type)
	{
	case 201://对话
	case 1201:
		if ( sqs->CanAccept() && sqs->Quest 
			&& IsConsortiaQuest( sqs->Quest->QuestID )
			&& sqs->Quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_SPEAKTO) )
		{		
			//del warning
			if (sqs->Quest->MaxLevel < (int)GetRank() )
				return;

			sqs->Accept();
			SendAcceptQuestMsg(sqs);
		}
		break;
	case 202://杀怪
	case 1202:
		if ( sqs->CanAccept() && sqs->Quest 
			&& IsConsortiaQuest( sqs->Quest->QuestID )
			&& sqs->Quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_KILL_OR_CAST) )
		{		
			if (sqs->Quest->MaxLevel < (int)GetRank() )
				return;

			sqs->Accept();
			SendAcceptQuestMsg(sqs);
		}
		break;
	case 103:
		break;
	default:
		break;
	}

}

void CPlayer::H_ConsortiaQuest(int q_type)
{
	if (!IsThisDuty( CONSORTIA_CHAIRMAN ) && !IsThisDuty( CONSORTIA_ASSISTANT_CHAIRMAN ))
		return;

	CastConsortiaQuest();

	int lev = GetConsortiaLevel();

	for (int i = 0 ; i < QUEST_COUNT ; i++)
	{
		stdext::hash_map< ConsortiaElem*, CConsortiaMgr::Member >::iterator itor =  CGameObject::s_World->g_pConsortiaManager->m_ConsortiaMemberMap.find( m_pConsortia );
		if(  itor !=   CGameObject::s_World->g_pConsortiaManager->m_ConsortiaMemberMap.end() )
		{
			if( m_Quests[i].Quest && m_Quests[i].Quest->ReqConsortiaLV > lev )
				continue;//公会级别限制

			CConsortiaMgr::Member member = itor->second;
			for ( CConsortiaMgr::Member::iterator iter = member.begin(); iter != member.end(); iter++ )
			{
				CPlayer *pPlayer = GetPlayerFromRoleName(  iter->first );
				if ( NULL != pPlayer)
					pPlayer->AccpetConsortiaQuest(i,q_type); 	
			}
		}
	}
}

void CPlayer::LoadQuest()
{
	//允许动态增加任务，玩家是看不到的，必须要重启，
	//允许动态删除任务，玩家可能会觉得奇怪。

	for (int i = 0 ; i < QUEST_COUNT ; i++)
	{
		int id = (m_ObjectData.m_lFaction == 1 ? i + QUEST_COUNT : i);

		m_Quests[i].Quest = QuestManager::Instance()->FindQuest(id);
		m_Quests[i].Reward = QuestManager::Instance()->FindQuestReward(id);

		if (!m_Quests[i].Quest || !m_Quests[i].Reward)
			m_Quests[i].Status = QUEST_STATUS_NONE;

		// 		if (m_Quests[i].Status == QUEST_STATUS_NONE)
		// 			continue;

		bool b = QuestStatusUpdata(i);

		if (m_Quests[i].Status == QUEST_STATUS_UNAVAILABLE && b)
			m_UnavailableQuests.push_back(m_Quests + i);

		if (m_Quests[i].Status == QUEST_STATUS_AVAILABLE)
			m_AvailableQuests.push_back(m_Quests + i);

		if (m_Quests[i].Quest)
		{
			//如果不是计时任务
			if (!m_Quests[i].Quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED))
				continue;

			m_TimeQuestsMap[id] = m_Quests + i;

			//如果是刷新过了
			if (m_Quests[i].TimeUpdata > m_Quests[i].Quest->TimeUpdata)
				continue;

			m_Quests[i].UpdataTime();
		}
	}

	int i = 0 ;
	// 	printf("101 %d \n" , (int)m_Quests[101].Status);

	for (vector<SQuestStatus*>::iterator iter = m_AcceptQuests.begin() ; iter != m_AcceptQuests.end() ; iter++)
	{
		msg_player_acceptquests.Quests[i].QuestID = (*iter)->Quest->QuestID;

		msg_player_acceptquests.Quests[i].QuestSort = (*iter)->Quest->QuestSort;
		msg_player_acceptquests.Quests[i].SpecialFlags = (*iter)->Quest->SpecialFlags;

		msg_player_acceptquests.Quests[i].Money = (*iter)->Quest->Money;
		msg_player_acceptquests.Quests[i].Bijou = (*iter)->Quest->Bijou;
		msg_player_acceptquests.Quests[i].RewardMoney = (*iter)->Quest->RewardMoney;
		msg_player_acceptquests.Quests[i].RewardBijou = (*iter)->Quest->RewardBijou;

		msg_player_acceptquests.Quests[i].Status = (char)(*iter)->Status;
		msg_player_acceptquests.Quests[i].NPCID = (*iter)->Quest->NPCID;
		msg_player_acceptquests.Quests[i].RewardNPCID = (*iter)->Reward->NPCID;
		msg_player_acceptquests.Quests[i].Completed = (*iter)->Completed;

		for (int j = 0 ; j < QUEST_OBJECTIVES_COUNT ; j++)
		{
			msg_player_acceptquests.Quests[i].Item[j] = (*iter)->Quest->Item[j];
			msg_player_acceptquests.Quests[i].Creature[j] = (*iter)->Quest->Creature[j];
			msg_player_acceptquests.Quests[i].ItemCount[j] = (*iter)->Quest->ItemCount[j];
			msg_player_acceptquests.Quests[i].CreatureCount[j] = (*iter)->Quest->CreatureCount[j];
			msg_player_acceptquests.Quests[i].ItemCompletedCount[j] = (*iter)->ItemCount[j];
			msg_player_acceptquests.Quests[i].CreatureCompletedCount[j] = (*iter)->CreatureCount[j];
		}
		for (int j = 0 ; j < QUEST_EXPLORER_COUNT ; j++)
		{
			msg_player_acceptquests.Quests[i].ExplorerState[j] = (*iter)->ExplorerComp[j];
		}

		memcpy(msg_player_acceptquests.Quests[i].RewardItem , (*iter)->Quest->RewardItem , sizeof(ItemData) * QUEST_REWARDS_COUNT);

		i++;

		if (i == 3)
		{
			msg_player_acceptquests.Count = i;
			s_World->SendMsgToClient(&msg_player_acceptquests , m_pSocket);
			//数据太大...	

			i = 0;
		}
	}

	msg_player_acceptquests.Count = i;

	if (msg_player_acceptquests.Count)
		s_World->SendMsgToClient(&msg_player_acceptquests , m_pSocket);

	if (this->GetWorldStatus() == eGAMEWORLD)
	{
		//再發一次

		std::vector<BYTE> Area = GetCurrentCell()->GetArea();
		std::vector<BYTE>::iterator  AreaItor = Area.begin();
		for ( ; AreaItor != Area.end(); AreaItor++ )
		{
			CCell  *pCurCell =  GetRegion()->GetMap()->GetCell( *AreaItor );  
			MapNPC &NPCMap = pCurCell->GetNPCMap(); 
			MapNPC::iterator itor = NPCMap.begin(); 

			for (; itor != NPCMap.end(); itor++)
			{
				CNPC* pNPC = itor->second;
				pNPC->GetQuestList(this); 
			} 
		}
	}
	m_MsgFlag[ MSG_QUEST_FLAG ] = true;
}

SQuestStatus* CPlayer::GetAcceptQuest(int id)
{
	for (vector<SQuestStatus*>::iterator iter = m_AcceptQuests.begin() ; iter != m_AcceptQuests.end() ; iter++)
	{
		if ((*iter)->Quest->QuestID == id)
			return (*iter);
	}

	return NULL;
}

SQuestStatus* CPlayer::GetQuest(int id)
{
	int index = m_ObjectData.m_lFaction == 1 ? id - QUEST_COUNT : id;

	//assert(index < QUEST_COUNT);

	if (index >= QUEST_COUNT || index < 0)
		return NULL;

	return m_Quests + index;
}


void CPlayer::ViewQuest(int id)
{
	QuestInfo* qi = QuestManager::Instance()->FindQuest(id);

	if (!qi)
		return;

	msg_npc_questview.QuestSort = qi->QuestSort;
	msg_npc_questview.SpecialFlags = qi->SpecialFlags;

	msg_npc_questview.QuestID = qi->QuestID;
	msg_npc_questview.NPCID = qi->NPCID;
	msg_npc_questview.Money = qi->Money;
	msg_npc_questview.Bijou = qi->Bijou;
	msg_npc_questview.RewardMoney = qi->RewardMoney;
	msg_npc_questview.RewardBijou = qi->RewardBijou;

	for (int i = 0 ; i < QUEST_OBJECTIVES_COUNT ; i++)
	{
		msg_npc_questview.Item[i] = qi->Item[i];
		msg_npc_questview.Creature[i] = qi->Creature[i];
		msg_npc_questview.ItemCount[i] = qi->ItemCount[i];
		msg_npc_questview.CreatureCount[i] = qi->CreatureCount[i];
	}

	memcpy(msg_npc_questview.RewardItem , qi->RewardItem , sizeof(ItemData) * QUEST_REWARDS_COUNT);

	s_World->SendMsgToClient(&msg_npc_questview , m_pSocket);
}

eError CPlayer::AcceptQuest(int id)
{
	int qid = m_ObjectData.m_lFaction == 1 ? id - QUEST_COUNT : id;

	if (qid >= QUEST_COUNT || qid < 0)
	{
		AddDanger();
		sbase::LogException("Message type Exception, Accounts : %s, Role : %s, [_MSG_SKILL]", GetAccounts(), GetName() );
		return NO_MSG_ERRO;//用到其它阵营任务了？
	}
	//已经接受
	if (GetAcceptQuest(id))
		return NO_MSG_ERRO;

	if( QuestConsortiaLimit( id ) == false )
		return MSG_ERRO_016C;

	if( QuestTeamLimit( qid ) == false )
		return MSG_ERRO_016D;
	//接受任务

	SQuestStatus* sqs = GetQuest(id);

	eError ret = ChkQsB4Accept(sqs);
	if (ret != MSG_ERRO_016B)
		return ret;

	SendAcceptQuestMsg(sqs);

	return NO_MSG_ERRO;
}

bool CPlayer::UseQuestItem(int questid)
{
	for (int index=0; index < MAX_BAGS; index++)
	{
		int num = -1;
		num = m_Bags[index].UseQuestItem(questid);
		if( -1 != num )
		{
			msg_useorequip_item.Index = (USHORT)index;
			msg_useorequip_item.Num = (USHORT)num;
			msg_useorequip_item.uiID = GetID();
			msg_useorequip_item.False = false;
			s_World->SendMsgToClient(&msg_useorequip_item , m_pSocket);
			return true;
		}
	}
	return false ;
}

bool CPlayer::CanAcceptQuest(int id , bool db /* = false */)
{
	if (!db)
	{
		SQuestStatus* sqs = GetQuest(id);

		if (!sqs)
			return false;

		if (sqs->CanAccept())
			return true;

		return false;
	}
	else
	{
		//use db

		QuestInfo* qi = QuestManager::Instance()->FindQuest(id);

		if (!qi)
			return false;

		if (qi->RequiredBelief != QUEST_BELIEF_NONE &&
			qi->RequiredBelief != m_ObjectData.m_usBelief)
			return false;

		return true;
	}
}


eError CPlayer::CancelQuest(int id)
{
	//放弃任务
	SQuestStatus* sqs = GetQuest(id);

	vector<SQuestStatus*>::iterator iter = std::find(m_AcceptQuests.begin() , m_AcceptQuests.end() , sqs);

	if (iter == m_AcceptQuests.end())
	{
		//外挂

		return NO_MSG_ERRO;
	}

	sqs->Cancel();

	if (GetcRank() > sqs->Quest->MaxLevel)
		sqs->Status = QUEST_STATUS_UNAVAILABLE;

	if (sqs->Status == QUEST_STATUS_AVAILABLE)
		msg_npc_cancelquest.Available = true;
	else
		msg_npc_cancelquest.Available = false;

	msg_npc_cancelquest.NPCID = sqs->Quest->NPCID;
	msg_npc_cancelquest.RewardNPCID = sqs->Reward->NPCID;

	m_AcceptQuests.erase(iter);

	msg_npc_cancelquest.QuestID = id;
	s_World->SendMsgToClient(&msg_npc_cancelquest , m_pSocket);

	return NO_MSG_ERRO;
}


eError CPlayer::CompleteQuest(int id , int choice)
{
	//完成任务，要报酬

	SQuestStatus* sqs = GetAcceptQuest(id);

	if (!sqs)
		return NO_MSG_ERRO;

	if (!sqs->Quest)
		return NO_MSG_ERRO;

	if (sqs->Status != QUEST_STATUS_COMPLETE)
		return NO_MSG_ERRO;

	CNPC* npc = CGameObject::s_World->GetNPC(sqs->Reward->NPCID);

	if (!npc)
	{
		AddDanger();
		sbase::LogException("Message type Exception, Accounts : %s, Role : %s, [_MSG_SKILL]", GetAccounts(), GetName() );
		return NO_MSG_ERRO;
	}

	float fPX = npc->GetPosX() - GetPosX();
	float fPZ = npc->GetPosZ() - GetPosZ();

	// 当前位置与目标位置距离
	int dist = sqrt( fPX*fPX + fPZ*fPZ );

	if (dist > 30)
		return NO_MSG_ERRO;

	//加报酬
	if (choice >= 0 && choice < QUEST_OBJECTIVES_COUNT)
	{
		Item item;

		ItemManager::Instance()->CreateItem(&sqs->Quest->RewardItem[choice] , &item);

		if (item.IsClear())
			return MSG_ERRO_0200;

		if (GetFreeItemOverlap(&item) >= item.m_Overlap)
		{
			AddItem(&item);
		}
		else
		{
			//SendServerError(ItemFull , this);
			return MSG_ERRO_0201;
		}
	}
	SetSkillPoint( GetSkillPoint() + sqs->Reward->SkillPoint );
	AddExp(sqs->Reward->EXP);
	CalculateExp(true , sqs->Reward->SkillPoint);
	AddMoney(sqs->Quest->RewardMoney);
	AddBijou(sqs->Quest->RewardBijou);
	ConsortiaReward(sqs);

	msg_npc_rewardquest.NPCID = sqs->Quest->NPCID;
	msg_npc_rewardquest.RewardNPCID = sqs->Reward->NPCID;
	msg_npc_rewardquest.QuestID = id;
	msg_npc_rewardquest.Choice = choice;
	msg_npc_rewardquest.Credit = sqs->Reward->Credit;
	msg_npc_rewardquest.Contribute = sqs->Reward->Contribute;
	msg_npc_rewardquest.EXP = GetEXP();
	msg_npc_rewardquest.SkillPoint = GetSkillPoint();

	s_World->SendMsgToClient(&msg_npc_rewardquest , m_pSocket);

	//完成
	sqs->Complete();

	if (sqs->Quest->NextQuestId != -1)
	{
		//后续任务
		int index = m_ObjectData.m_lFaction == 1 ? sqs->Quest->NextQuestId - QUEST_COUNT : sqs->Quest->NextQuestId;
		QuestStatusUpdata(index);

		if (CanAcceptQuest(sqs->Quest->NextQuestId))
		{
			//更新后续任务
			QuestInfo* qi = QuestManager::Instance()->FindQuest(sqs->Quest->NextQuestId);

			msg_npc_nextquest.SpecialFlags = qi->SpecialFlags;
			msg_npc_nextquest.QuestSort = qi->QuestSort;

			msg_npc_nextquest.NPCID = qi->NPCID;
			msg_npc_nextquest.QuestID = qi->QuestID;
			msg_npc_nextquest.Money = qi->Money;
			msg_npc_nextquest.Bijou = qi->Bijou;
			msg_npc_nextquest.RewardMoney = qi->RewardMoney;
			msg_npc_nextquest.RewardBijou = qi->RewardBijou;

			for (int i = 0 ; i < QUEST_OBJECTIVES_COUNT ; i++)
			{
				msg_npc_nextquest.Item[i] = qi->Item[i];
				msg_npc_nextquest.Creature[i] = qi->Creature[i];
				msg_npc_nextquest.ItemCount[i] = qi->ItemCount[i];
				msg_npc_nextquest.CreatureCount[i] = qi->CreatureCount[i];
			}

			memcpy(msg_npc_nextquest.RewardItem , qi->RewardItem , sizeof(ItemData) * QUEST_REWARDS_COUNT);
			s_World->SendMsgToClient(&msg_npc_nextquest , m_pSocket);
		}
	}

	msg_npc_questsavailable.Count = 0;
	for (vector<int>::iterator iter = sqs->Quest->NextQuestsID.begin() ; iter != sqs->Quest->NextQuestsID.end() ; iter++)
	{
		QuestInfo* qi = QuestManager::Instance()->FindQuest(*iter);

		int index = m_ObjectData.m_lFaction == 1 ? qi->QuestID - QUEST_COUNT : qi->QuestID;
		QuestStatusUpdata(index);

		if (CanAcceptQuest(*iter))
		{
			msg_npc_questsavailable.NPCID[msg_npc_questsavailable.Count] = qi->NPCID;
			msg_npc_questsavailable.QuestID[msg_npc_questsavailable.Count] = qi->QuestID;

			msg_npc_questsavailable.Count++;
		}
	}
	if (msg_npc_questsavailable.Count)
		s_World->SendMsgToClient(&msg_npc_questsavailable , m_pSocket);

	if (GetcRank() > sqs->Quest->MaxLevel)
		sqs->Status = QUEST_STATUS_UNAVAILABLE;

	if (sqs->Status == QUEST_STATUS_AVAILABLE)
	{
		//可以重复,刷新数据
		msg_npc_quest.NPCID = sqs->Quest->NPCID;
		msg_npc_quest.Quest.QuestID = sqs->Quest->QuestID;
		msg_npc_quest.Quest.Available = true;

		s_World->SendMsgToClient(&msg_npc_quest , m_pSocket);
	}

	m_AcceptQuests.erase(std::find(m_AcceptQuests.begin() , m_AcceptQuests.end() , sqs));

	return NO_MSG_ERRO;
}

void CPlayer::QuestUpdata()
{
	//更新任务状态

	int q_id = -1;
	for (vector<SQuestStatus*>::iterator iter = m_AcceptQuests.begin() ; iter != m_AcceptQuests.end() ; iter++)
	{
		q_id = ((SQuestStatus*)(*iter))->Quest->QuestID;

		if( IsConsortiaQuest(q_id) && !HaveConsortia())
			continue;

		q_id = m_ObjectData.m_lFaction == 1 ? q_id - QUEST_COUNT : q_id;
		if( QuestTeamLimit(q_id) == false )
			continue;

		switch((*iter)->UpData(this->GetRank()))
		{
		case QUEST_STATUS_COMPLETE:
			{
				//任务完成，不是回报。

				msg_npc_completequest.QuestID = (*iter)->Reward->QuestID;
				msg_npc_completequest.RewardNPCID = (*iter)->Reward->NPCID;

				s_World->SendMsgToClient(&msg_npc_completequest , m_pSocket);
			}
			break;
		case QUEST_STATUS_FAILD:
			{
				//任务失败

				msg_npc_questfaild.QuestID = (*iter)->Reward->QuestID;
				msg_npc_questfaild.RewardNPCID = (*iter)->Reward->NPCID;

				s_World->SendMsgToClient(&msg_npc_questfaild , m_pSocket);
			}
			break;
		default:
			break;
		}

	}

}

void CPlayer::TimeQuestUpdate(int id)
{
	map<int , SQuestStatus*>::iterator iter = m_TimeQuestsMap.find(id);

	if (iter == m_TimeQuestsMap.end())
		return;

	if (iter->second->UpdataTime() && iter->second->CanAccept())
	{
		//完成过
		msg_npc_quest.NPCID = iter->second->Quest->NPCID;
		msg_npc_quest.Quest.QuestID = iter->second->Quest->QuestID;
		msg_npc_quest.Quest.Available = true;

		s_World->SendMsgToClient(&msg_npc_quest , m_pSocket);
	}
}

//超时
void CPlayer::TimeOverQuestUpdate(int id)
{
	map<int , SQuestStatus*>::iterator iter = m_TimeQuestsMap.find(id);

	if (iter == m_TimeQuestsMap.end())
		return;

	iter->second->Status = QUEST_STATUS_UNAVAILABLE;

	msg_npc_quest.NPCID = iter->second->Quest->NPCID;
	msg_npc_quest.Quest.QuestID = iter->second->Quest->QuestID;
	msg_npc_quest.Quest.Available = false;

	s_World->SendMsgToClient(&msg_npc_quest , m_pSocket);
}

void CPlayer::LevelUpdata()
{
	ReflashUnavilableVec();

	ReflashAvilaVec();


}

void CPlayer::QuestKILL(int id)
{
	//杀怪
	for (vector<SQuestStatus*>::iterator iter = m_AcceptQuests.begin() ; iter != m_AcceptQuests.end() ; iter++)
	{
		int q_id = ((SQuestStatus*)(*iter))->Quest->QuestID;
		q_id = m_ObjectData.m_lFaction == 1 ? q_id - QUEST_COUNT : q_id;
		if( QuestTeamLimit(q_id) == false )
			continue;

		if ((*iter)->Kill(id))
		{
			//杀怪计数

			msg_npc_questkillorcast.Kill = true;
			msg_npc_questkillorcast.QuestID = (*iter)->Quest->QuestID;
			msg_npc_questkillorcast.NPCID = (*iter)->Quest->NPCID;
			msg_npc_questkillorcast.KillOrCastID = id;

			s_World->SendMsgToClient(&msg_npc_questkillorcast , m_pSocket);

			//return;
		}
	}
}

void CPlayer::QuestCast(int id)
{
	for (vector<SQuestStatus*>::iterator iter = m_AcceptQuests.begin() ; iter != m_AcceptQuests.end() ; iter++)
	{
		int q_id = ((SQuestStatus*)(*iter))->Quest->QuestID;
		q_id = m_ObjectData.m_lFaction == 1 ? q_id - QUEST_COUNT : q_id;
		if( QuestTeamLimit(q_id) == false )
			continue;

		if ((*iter)->Cast(id))
		{
			//收集计数

			msg_npc_questkillorcast.Kill = false;
			msg_npc_questkillorcast.QuestID = (*iter)->Quest->QuestID;
			msg_npc_questkillorcast.NPCID = (*iter)->Quest->NPCID;
			msg_npc_questkillorcast.KillOrCastID = id;

			s_World->SendMsgToClient(&msg_npc_questkillorcast , m_pSocket);

			//return;
		}
	}
}

bool CPlayer::QuestTeamLimit( int qid )//组队限制检查
{
	int tl = m_Quests[qid].Quest->TeamLimit;
	int num = 1; bool ifCouples = false;

	GetTeamState( num, ifCouples );
	switch( tl )
	{
	case -1://无限制
		return true;
		break;
	case 1://两人用以上任务
		if( num < 2 )
			return false;
		break;
	case 2://五人任务
		if( num < 5 )
			return false;
		break;
	case 3://夫妻任务
		if( !ifCouples )
			return false;
		break;
	default:
		return true;
	}
}

bool CPlayer::QuestClassLimit( int qid )//职业限制检查
{
	if ( !m_Quests[qid].Quest )
	{
		return false;
	}
	std::vector<int> reqCls = m_Quests[qid].Quest->RequiredClass;
	std::vector<int>::iterator it = reqCls.begin();
	bool bChkCls = true;

	for (; it != reqCls.end(); it++)
	{
		bChkCls = false;
		if (this->GetlClass() == (*it))
		{
			bChkCls = true;
			break;
		}
	}

	if (!bChkCls)
		return false;
	else
		return true;
}

bool CPlayer::IsConsortiaQuest( int qid )
{
	qid = m_ObjectData.m_lFaction == 1 ? qid - QUEST_COUNT : qid;
	if(!m_Quests[qid].Quest )
		return false;

	return m_Quests[qid].Quest->QuestSort == QUEST_SORT_CONSORTIA;
}

bool CPlayer::QuestConsortiaLimit( int qid )//公会限制
{
	if( IsConsortiaQuest(qid) )
	{		
		if( IsThisDuty( CONSORTIA_CHAIRMAN ) )
			return true;
		else
			return false;
	}

	return true;
}
void CPlayer::SendQuestToMembers( SQuestStatus* sqs )
{
	//if (IsThisDuty( CONSORTIA_CHAIRMAN ))
	//{
	//	stdext::hash_map< ConsortiaElem*, CConsortiaMgr::Member >::iterator itor =  CGameObject::s_World->g_pConsortiaManager->->m_ConsortiaMemberMap.find( m_pConsortia );
	//	if(  itor !=   CGameObject::s_World->g_pConsortiaManager->->m_ConsortiaMemberMap.end() )
	//	{
	//		CConsortiaMgr::Member member = itor->second;
	//		for ( CConsortiaMgr::Member::iterator iter = member.begin(); iter != member.end(); iter++ )
	//		{
	//			CPlayer *pPlayer = GetPlayerFromRoleName(  iter->first );
	//			if ( NULL != pPlayer)
	//				pPlayer->SendAcceptQuestMsg(sqs); 	
	//		}
	//	}
	//}
	//else
	//{
	//	//不是会长，无法派发
	//}
}




eError CPlayer::ChkQsB4Accept( SQuestStatus* sqs )
{
	if (!sqs)
		return NO_MSG_ERRO;

	if (!sqs->Quest)
		return NO_MSG_ERRO;

	if (!sqs->Accept() && sqs->Quest->QuestSort != QUEST_SORT_RANDOM )
		return NO_MSG_ERRO;

	CNPC* npc = CGameObject::s_World->GetNPC(sqs->Quest->NPCID);

	float fPX = npc->GetPosX() - GetPosX();
	float fPZ = npc->GetPosZ() - GetPosZ();

	// 当前位置与目标位置距离
	int dist = sqrt( fPX*fPX + fPZ*fPZ );

	if (dist > 30 && sqs->Quest->QuestSort != QUEST_SORT_RANDOM )//随机（道具）任务无需NPC判断
		return NO_MSG_ERRO;
	if (sqs->Quest->QuestSort == QUEST_SORT_RANDOM )
	{
		if( false == UseQuestItem(sqs->Quest->QuestID))
			return NO_MSG_ERRO;
	}

	return MSG_ERRO_016B;
}


void CPlayer::SendAcceptQuestMsg( SQuestStatus* sqs )
{
	if( sqs == NULL )
		return;

		QuestInfo* qi = QuestManager::Instance()->FindQuest(sqs->Quest->QuestID);

		msg_npc_acceptquest.QuestSort = qi->QuestSort;
		msg_npc_acceptquest.SpecialFlags = qi->SpecialFlags;

		msg_npc_acceptquest.QuestID = qi->QuestID;
		msg_npc_acceptquest.NPCID = qi->NPCID;
		msg_npc_acceptquest.Money = qi->Money;
		msg_npc_acceptquest.Bijou = qi->Bijou;
		msg_npc_acceptquest.RewardMoney = qi->RewardMoney;
		msg_npc_acceptquest.RewardBijou = qi->RewardBijou;

		for (int i = 0 ; i < QUEST_OBJECTIVES_COUNT ; i++)
		{
			msg_npc_acceptquest.Item[i] = qi->Item[i];
			msg_npc_acceptquest.Creature[i] = qi->Creature[i];
			msg_npc_acceptquest.ItemCount[i] = qi->ItemCount[i];
			msg_npc_acceptquest.CreatureCount[i] = qi->CreatureCount[i];
		}

		memcpy(msg_npc_acceptquest.RewardItem , qi->RewardItem , sizeof(ItemData) * QUEST_REWARDS_COUNT);
	

	m_AcceptQuests.push_back(sqs);

	msg_npc_acceptquest.NPCID = sqs->Quest->NPCID;
	if (sqs->Reward)
		msg_npc_acceptquest.RewardNPCID = sqs->Reward->NPCID;

	msg_npc_acceptquest.QuestID = sqs->Quest->QuestID;
	msg_npc_acceptquest.QuestSort = sqs->Quest->QuestSort;

	s_World->SendMsgToClient(&msg_npc_acceptquest , m_pSocket);
}

void CPlayer::ReflashUnavilableVec()
{
	msg_npc_questsavailable.Count = 0;

	for (size_t i = 0 ; i < m_UnavailableQuests.size() ; i++)
	{
		//先判断前序任务

		if (m_UnavailableQuests[i]->Quest->TimeOver)
			continue;

		if (m_UnavailableQuests[i]->Quest->RequiredBelief != QUEST_BELIEF_NONE &&
			m_UnavailableQuests[i]->Quest->RequiredBelief != m_ObjectData.m_usBelief)
			continue;

		if (m_UnavailableQuests[i]->Quest->PrevQuestId != -1)
		{
			if (GetQuest(m_UnavailableQuests[i]->Quest->PrevQuestId)->Status != QUEST_STATUS_OVER)
				continue;
		}

		if (GetcRank() >= m_UnavailableQuests[i]->Quest->MinLevel && GetcRank() <= m_UnavailableQuests[i]->Quest->MaxLevel)
		{
			int qid = m_UnavailableQuests[i]->Quest->QuestID;
			qid = m_ObjectData.m_lFaction == 1 ? qid - QUEST_COUNT : qid;
			if( false == QuestClassLimit(qid) /*|| false == QuestConsortiaLimit(m_UnavailableQuests[i]->Quest->QuestID)*/)
				continue;
			msg_npc_questsavailable.NPCID[msg_npc_questsavailable.Count] = m_UnavailableQuests[i]->Quest->NPCID;
			msg_npc_questsavailable.QuestID[msg_npc_questsavailable.Count] = m_UnavailableQuests[i]->Quest->QuestID;

			msg_npc_questsavailable.Count++;

			m_UnavailableQuests[i]->Status = QUEST_STATUS_AVAILABLE;

			m_UnavailableQuests.erase(m_UnavailableQuests.begin() + i);i--;		
		}

 		if (msg_npc_questsavailable.Count == QUEST_PLAYER_COUNT)
		{
 			s_World->SendMsgToClient(&msg_npc_questsavailable , m_pSocket);
			msg_npc_questsavailable.Count = 0;
		}
	}

	//ASSERT(msg_npc_questsavailable.Count < QUEST_PLAYER_COUNT * 4);

	if (msg_npc_questsavailable.Count)
		CGameObject::s_World->SendMsgToClient(&msg_npc_questsavailable , m_pSocket);
}

void CPlayer::ReflashAvilaVec()
{
	//仅仅是升级时不可接受

	msg_npc_questsunavailable.Count = 0;

	for (size_t i = 0 ; i < m_AvailableQuests.size() ; i++)
	{
		if (!m_AvailableQuests[i]->CanAccept())
			continue;

		if (GetcRank() > m_AvailableQuests[i]->Quest->MaxLevel)
		{
			msg_npc_questsunavailable.NPCID[msg_npc_questsunavailable.Count] = m_AvailableQuests[i]->Quest->NPCID;
			msg_npc_questsunavailable.QuestID[msg_npc_questsunavailable.Count] = m_AvailableQuests[i]->Quest->QuestID;

			msg_npc_questsunavailable.Count++;

			m_AvailableQuests[i]->Status = QUEST_STATUS_UNAVAILABLE;

			m_AvailableQuests.erase(m_AvailableQuests.begin() + i);i--;
		}

	}

	//ASSERT(msg_npc_questsunavailable.Count < QUEST_PLAYER_COUNT * 4);

	if (msg_npc_questsunavailable.Count)
		CGameObject::s_World->SendMsgToClient(&msg_npc_questsunavailable , m_pSocket);
}

void CPlayer::ConsortiaReward( SQuestStatus* sqs)
{
	if (!HaveConsortia() || !IsConsortiaQuest(sqs->Quest->QuestID))
		return;

	SetuiUionContribute( GetuiUionContribute() + sqs->Reward->UionContribute );
	//GetConsortia()->加金币银币
	SetConsortia_lbijou( sqs->Quest->UionBijou);
	SetConsortia_lfound( sqs->Quest->UionMoney);
	//
}

void CPlayer::CastConsortiaQuest()//公会任务消耗
{
	if (!HaveConsortia())
		return;
	//GetConsortia()->扣金币银币
	//SetConsortia_lbijou( 5);
	//SetConsortia_lfound( 5);
	//CancelConsortiaQuest();
}

void CPlayer::CancelConsortiaQuest()
{
	for( vector<SQuestStatus*>::iterator iter = m_AcceptQuests.begin(); iter != m_AcceptQuests.end();)
	{
		if( NULL == (*iter)->Quest )
			continue;

		SQuestStatus* sqs = (*iter);

		if(sqs->Quest->QuestSort == QUEST_SORT_CONSORTIA )
		{
			sqs->Cancel();

			if (GetcRank() > sqs->Quest->MaxLevel)
				sqs->Status = QUEST_STATUS_UNAVAILABLE;

			if (sqs->Status == QUEST_STATUS_AVAILABLE)
				msg_npc_cancelquest.Available = true;
			else
				msg_npc_cancelquest.Available = false;

			msg_npc_cancelquest.NPCID = sqs->Quest->NPCID;
			msg_npc_cancelquest.RewardNPCID = sqs->Reward->NPCID;

			iter = m_AcceptQuests.erase(iter);

			msg_npc_cancelquest.QuestID = sqs->Quest->QuestID;
			CGameObject::s_World->SendMsgToClient(&msg_npc_cancelquest , m_pSocket);
		}
		else
			++iter;
	}
}