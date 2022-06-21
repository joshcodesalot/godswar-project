#include "stdafx.h"
#include "QuestManager.h"
#include "ItemManager.h"
#include "../GameCore/RepRegion.h"
#include "../GameCore/Player.h"
#include "../DataCore.h"
#include "../World.h"

QuestManager* QuestManager::m_This = NULL;

QuestManager::QuestManager()
{

}

QuestManager::~QuestManager()
{

}

void QuestManager::Init(CWorld* pWorld)
{
	m_pWorld = pWorld;

	_get_timezone(&m_TimeZone);

	m_Timer.Startup(5);
}

void QuestManager::Release()
{
	delete this;

	m_This = NULL;
}

QuestManager* QuestManager::Instance()
{
	if (!m_This)
		m_This = new QuestManager();

	return m_This;
}


void QuestManager::InsertQuest(int id , QuestInfo* quest)
{
	if (m_MapQuestInfo.find(id) == m_MapQuestInfo.end())
	{
		m_MapQuestInfo[id] = quest;

		if (quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_TIMED))
		{
			m_TimeQuests.push_back(quest);

			time_t timenow = GetLocalTime();

			//上一次刷新时间
			if (quest->TimeUpdata == 0)
			{
				quest->TimeUpdata = timenow / quest->TimeStatus * quest->TimeStatus + quest->MinTime;

				if (quest->TimeUpdata > timenow)
					quest->TimeUpdata -= quest->TimeStatus;
			}


			//当前时间大于上一次刷新1个周期
			if ((timenow - quest->TimeUpdata) > quest->TimeStatus)
				quest->TimeUpdata += quest->TimeStatus;

			//时间超时，5秒延迟时间
			if (timenow - quest->TimeUpdata > quest->MaxTime - quest->MinTime + 5)
				quest->TimeOver = true;
		}
	}
	else
		sbase::ConsoleWriteStr( 1,2,"Quest ID %d is already define." , id);
}

void QuestManager::EraseQuest(int id)
{
	map<int , QuestInfo*>::iterator iter = m_MapQuestInfo.find(id);

	if (iter != m_MapQuestInfo.end())
	{
		vector<QuestInfo*>::iterator itertime = std::find(m_TimeQuests.begin() , m_TimeQuests. end() , iter->second);

		if (itertime != m_TimeQuests.end())
			m_TimeQuests.erase(itertime);

		m_MapQuestInfo.erase(id);
	}
	else
		sbase::ConsoleWriteStr( 1,2,"Quest ID %d is not have. " , id);
}

void QuestManager::InsertQuestReward(int id , QuestReward* quest)
{
	if (m_MapQuestReward.find(id) == m_MapQuestReward.end())
	{
		m_MapQuestReward[id] = quest;
	}
	else
		sbase::ConsoleWriteStr( 1,2,"QuestReward ID %d is already define." , id);
}

void QuestManager::EraseQuestReward(int id)
{
	if (m_MapQuestReward.find(id) != m_MapQuestReward.end())
		m_MapQuestReward.erase(id);
	else
		sbase::ConsoleWriteStr( 1,2,"QuestReward ID %d is not have. " , id);
}

QuestInfo* QuestManager::FindQuest(int id)
{
	map<int , QuestInfo*>::iterator iter = m_MapQuestInfo.find(id);

	if (iter != m_MapQuestInfo.end())
		return iter->second;

	return NULL;
}

QuestReward* QuestManager::FindQuestReward(int id)
{
	map<int , QuestReward*>::iterator iter = m_MapQuestReward.find(id);

	if (iter != m_MapQuestReward.end())
		return iter->second;

	return NULL;
}

void QuestManager::UpData()
{
	if (!m_Timer.IsExpire())
		return;

	m_Timer.Update();

	int timenow = GetLocalTime();

//	printf( "%d\n", m_TimeQuests.size() );
	for (vector<QuestInfo*>::iterator iter = m_TimeQuests.begin() ; iter != m_TimeQuests.end() ; iter++)
	{
		//当前时间大于上一次刷新1个周期

		if ((timenow - (*iter)->TimeUpdata) > (*iter)->TimeStatus)
		{
			//任务刷新

			(*iter)->TimeUpdata += (*iter)->TimeStatus;
			(*iter)->TimeOver = false;

			//同步刷新
 			for (int i = 0 ; i < MAX_REGION ; i++)
 			{
 				for (PlayerIterator j = m_pWorld->GetRegion(i)->GetPlayerList().begin() ; j != m_pWorld->GetRegion(i)->GetPlayerList().end() ; j++)
 				{
 					(*j)->TimeQuestUpdate((*iter)->QuestID);
 				}
 			}
		}

		//超时
		if (timenow - (*iter)->TimeUpdata > (*iter)->MaxTime - (*iter)->MinTime + 5 && !(*iter)->TimeOver)
		{
			(*iter)->TimeOver = true;

			//同步刷新
			for (int i = 0 ; i < MAX_REGION ; i++)
			{
				for (PlayerIterator j = m_pWorld->GetRegion(i)->GetPlayerList().begin() ; j != m_pWorld->GetRegion(i)->GetPlayerList().end() ; j++)
				{
					(*j)->TimeOverQuestUpdate((*iter)->QuestID);
				}
			}
		}
	}
}


void QuestManager::Check()
{
	int Athens = 0;
	int	Sparta = 0;
	int	All = 0;

	for (map<int , QuestInfo*>::iterator iter = m_MapQuestInfo.begin() ; iter != m_MapQuestInfo.end() ; iter++)
	{
		map<int , QuestReward*>::iterator qriter = m_MapQuestReward.find(iter->second->QuestID);

		if (qriter == m_MapQuestReward.end())
			sbase::ConsoleWriteStr( 1,2,"Quest ID %d is not have QuestReward. " , iter->second->QuestID);

		for (int i = 0 ; i < QUEST_REWARDS_COUNT ; i++)
		{
			Item item;

			if (iter->second->RewardItem[i].Base != -1)
			{
				ItemManager::Instance()->CreateItem(&iter->second->RewardItem[i] , &item);

				if (item.IsClear())
					sbase::ConsoleWriteStr( 1,2,"Quest ID %d 's RewardItem %d IsClear. " , iter->second->QuestID , iter->second->RewardItem[i].Base);
			}
		}

		if (iter->second->PrevQuestId == -1){}
		else if (m_MapQuestInfo.find(iter->second->PrevQuestId) == m_MapQuestInfo.end())
			sbase::ConsoleWriteStr( 1,2,"Quest ID %d is not have PrevQuestId %d. " , iter->second->QuestID , iter->second->PrevQuestId);
		else
		{
			QuestInfo* qi = m_MapQuestInfo[iter->second->PrevQuestId];
			
			bool have = false;

			if (qi->NextQuestId == iter->second->QuestID)
				have = true;

			for (size_t i = 0 ; i < qi->NextQuestsID.size() ; i++)
				if (qi->NextQuestsID[i] == iter->second->QuestID)
					have = true;

			if (!have)
			{
				sbase::ConsoleWriteStr( 1,2,"Quest ID %d `s PrevQuestId %d is not have This. " , iter->second->QuestID , iter->second->PrevQuestId);
				qi->NextQuestsID.push_back(iter->second->QuestID);
			}
		}

		if (iter->second->NextQuestId == -1){}
		else if (m_MapQuestInfo.find(iter->second->NextQuestId) == m_MapQuestInfo.end())
			sbase::ConsoleWriteStr( 1,2,"Quest ID %d is not have NextQuestId %d. " , iter->second->QuestID , iter->second->NextQuestId);
		else
		{
			QuestInfo* qi = m_MapQuestInfo[iter->second->NextQuestId];

			if (qi->PrevQuestId != iter->second->QuestID)
				sbase::ConsoleWriteStr( 1,2,"Quest ID %d `s NextQuestId %d `s PrevQuestId != This. " , iter->second->QuestID , iter->second->NextQuestId);
		}

		for (size_t i = 0 ; i < iter->second->NextQuestsID.size() ; i++)
		{
			if (iter->second->NextQuestsID[i] == -1){}
			else if (m_MapQuestInfo.find(iter->second->NextQuestsID[i]) == m_MapQuestInfo.end())
				sbase::ConsoleWriteStr( 1,2,"Quest ID %d is not have NextQuestsID %d. " , iter->second->QuestID , iter->second->NextQuestsID[i]);
			else
			{
				QuestInfo* qi = m_MapQuestInfo[iter->second->NextQuestsID[i]];

				if (qi->PrevQuestId != iter->second->QuestID )
					sbase::ConsoleWriteStr( 1,2,"Quest ID %d `s NextQuestsId %d `s PrevQuestId != This. " , iter->second->QuestID , iter->second->NextQuestsID[i]);
			}
		}

		if (iter->second->RequiredFaction == QUEST_FACTION_NONE)
			All++;
		else if (iter->second->RequiredFaction == QUEST_FACTION_ATHENS)
			Athens++;
		else if (iter->second->RequiredFaction == QUEST_FACTION_SPARTA)
			Sparta++;
	}

	sbase::ConsoleWriteStr( 1,2,"Quests Count %d . \nAthens Quests %d . Sparta Quests %d . Common Quests %d " , m_MapQuestInfo.size() , Athens , Sparta , All);
}
