#ifndef _QUESTDEF_H_
#define _QUESTDEF_H_

#include <vector>
#include "GameData.h"
#include "Item.h"
#pragma  warning(disable:4702)
class CPlayer;

enum QuestFaction
{
	QUEST_FACTION_NONE = -1,
	QUEST_FACTION_SPARTA,
	QUEST_FACTION_ATHENS,

};
enum QuestBelief
{
	QUEST_BELIEF_NONE = -1,

};

enum QuestSort
{
	QUEST_SORT_STORY = 0,
	QUEST_SORT_STORIES,
	QUEST_SORT_REWARD,
	QUEST_SORT_CLASS,
	QUEST_SORT_FACTION,
	QUEST_SORT_DAYS,
	QUEST_SORT_LOOP,		
	QUEST_SORT_RANDOM,		
	QUEST_SORT_CONSORTIA,	

	QUEST_SORT_COUNT
};

enum QuestStatus
{
	QUEST_STATUS_NONE = 0,
	QUEST_STATUS_AVAILABLE,
	QUEST_STATUS_UNAVAILABLE,
	QUEST_STATUS_COMPLETE,
	QUEST_STATUS_INCOMPLETE,
	QUEST_STATUS_FAILD,
	QUEST_STATUS_OVER,


	MAX_QUEST_STATUS
};

enum QuestTimeStatus
{
	QUEST_TIMESTATUS_NONE = 0,
	QUEST_TIMESTATUS_DAY = 86400,
	QUEST_TIMESTATUS_WEEK = 604800,

	MAX_QUESTTIME_STATUS
};

enum QuestGiverStatus
{
	DIALOG_STATUS_NONE = 0,
	DIALOG_STATUS_UNAVAILABLE,
	DIALOG_STATUS_CHAT,
	DIALOG_STATUS_INCOMPLETE,
	DIALOG_STATUS_REWARD_REP,
	DIALOG_STATUS_AVAILABLE,
	DIALOG_STATUS_REWARD_OLD,
	DIALOG_STATUS_REWARD,
};

enum QuestSpecialFlags
{
	QUEST_SPECIAL_FLAGS_NONE			= 0,
	QUEST_SPECIAL_FLAGS_DELIVER			= 1,
	QUEST_SPECIAL_FLAGS_MAP_EXPOLORER	= 2,
	QUEST_SPECIAL_FLAGS_SPEAKTO			= 4,

	QUEST_SPECIAL_FLAGS_KILL_OR_CAST	= 8,
	QUEST_SPECIAL_FLAGS_TIMED			= 16,
	QUEST_SPECIAL_FLAGS_REPEATABLE		= 32,

	QUEST_SPECIAL_FLAGS_REPUTATION		= 64,

	QUEST_SPECIAL_FLAGS_LOOP			= 128,
};

struct ExplorerInfo
{
	int MapID;
	int PosX;
	int PosY;
	int PosZ;
	int Radius;
};

struct QuestInfo
{
	int NPCID;

	int QuestID;			
	int QuestSort;			
	int MinLevel;			
	int MaxLevel;			

	int RequiredLevel;		
	std::vector<int> RequiredClass;		
	int RequiredBelief;		
	int RequiredFaction;	

	int TeamLimit;			

	int TimeStatus;			

	time_t TimeUpdata;		
	time_t MinTime;			
	time_t MaxTime;			

	int	MaxNum;				
	int Repeat;				

	int SpecialFlags;		
	std::vector<ExplorerInfo*> ExplorerInfos;

	int Kill;				
	int QuestCount;			

	int PrevQuestId;		
	int NextQuestId;		
	std::vector<int> NextQuestsID;	

	int Money;				
	int Bijou;				
	int	RewardMoney;		
	int RewardBijou;		

	int UionBijou;		
	int UionMoney;		
	int ReqConsortiaLV;    

	bool TimeOver;			

	short Item[QUEST_OBJECTIVES_COUNT];
	short Creature[QUEST_OBJECTIVES_COUNT];

	short ItemCount[QUEST_OBJECTIVES_COUNT];
	short CreatureCount[QUEST_OBJECTIVES_COUNT];

	short ItemProbability[QUEST_OBJECTIVES_COUNT];

	ItemData RewardItem[QUEST_REWARDS_COUNT];

	void Clear()
	{
		NPCID = -1;

		TimeOver = false;

		TimeStatus = 0;

		QuestID = -1; QuestSort = 0;
		MinLevel = 0; MaxLevel = 0;
		RequiredBelief = -1; RequiredFaction = -1; RequiredLevel = -1;

		RequiredClass.clear();

		MaxNum = 0;
		SpecialFlags = 0;
		PrevQuestId = 0; NextQuestId = 0;
		Money = 0; Bijou = 0; RewardMoney = 0; RewardBijou = 0;

		Repeat = 0;
		TimeUpdata = 0; MinTime = 0; MaxTime = 0;

		memset(Item , 0 , QUEST_OBJECTIVES_COUNT * sizeof(short));
		memset(Creature , 0 , QUEST_OBJECTIVES_COUNT * sizeof(short));
		memset(ItemCount , 0 , QUEST_OBJECTIVES_COUNT * sizeof(short));
		memset(CreatureCount , 0 , QUEST_OBJECTIVES_COUNT * sizeof(short));
		memset(ItemProbability , 0 , QUEST_OBJECTIVES_COUNT * sizeof(short));
		memset(RewardItem , 0 , QUEST_REWARDS_COUNT * sizeof(ItemData));
	}

	QuestInfo()
	:	NPCID(-1) ,
		QuestID(-1) , QuestSort(0) , 
		MinLevel(0) , MaxLevel(0) ,
		RequiredBelief(-1) , RequiredFaction(-1) , RequiredLevel(-1) ,
		TimeStatus(0) ,
		TimeUpdata(0) , MinTime(0) , MaxTime(0) ,
		MaxNum(0) , 
		SpecialFlags(0) , 
		PrevQuestId(0) , NextQuestId(0) , 
		Money(0) , Bijou(0) , RewardMoney(0) , RewardBijou(0) , 
		Repeat(0) ,
		TimeOver(false) ,
		TeamLimit(-1),
		UionBijou(0),
		UionMoney(0),
		ReqConsortiaLV(0)

	{
		for (int i = 0 ; i < QUEST_OBJECTIVES_COUNT ; i++)
		{
			ItemCount[i] = 0;
			CreatureCount[i] = 0;

			memset(ItemCount , 0 , QUEST_OBJECTIVES_COUNT * sizeof(short));
			memset(CreatureCount , 0 , QUEST_OBJECTIVES_COUNT * sizeof(short));
		}
	}

	bool HasSpecialFlag( int flag ) const { return (SpecialFlags & flag ) != 0; }
};

struct QuestReward
{
	QuestReward()
	:	NPCID(-1) ,
		QuestID(-1) ,
		Credit(0) , Contribute(0) , EXP(0) , SkillPoint(0) ,
		SpecialFlags(0) , SpecialCount(0),
		UionContribute(0)
	{
	}

	void Clear()
	{
		NPCID = -1;
		QuestID = -1;

		Credit = 0; Contribute = 0; 
		EXP = 0; SkillPoint = 0;
		SpecialFlags = 0; SpecialCount = 0;		
	}

	int NPCID;

	int QuestID;

	int Credit;			
	int Contribute;		
	int EXP;			
	int SkillPoint;		

	int SpecialFlags;	
	int SpecialCount;	

	int UionContribute;	
};


struct SQuestStatus
{
	SQuestStatus()
	:	Quest(NULL) , Reward(NULL) ,
		Status(QUEST_STATUS_NONE) ,
		Completed(0) , TimeUpdata(0) , TimeLimit(0)
	{
		memset(ItemCount,    0, QUEST_OBJECTIVES_COUNT * sizeof(short));
		memset(CreatureCount, 0, QUEST_OBJECTIVES_COUNT * sizeof(short));
		memset(ExplorerComp,    0, QUEST_EXPLORER_COUNT * sizeof(bool));
		
		m_Timer.Startup(1);
	}

	bool IsFinishedExplorer()
	{
		std::vector<ExplorerInfo*>::iterator it = Quest->ExplorerInfos.begin();
		bool bFinished = true;

		for ( int index=0; it != Quest->ExplorerInfos.end(); it++, index++)
		{
			if ( !ExplorerComp[index])
			{
				bFinished = false;
				break;
			}
		}

		return bFinished;
	}

	int Explorer(int mapid, float posx, float posz)
	{
		if(!Quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_MAP_EXPOLORER))
			return false;

		std::vector<ExplorerInfo*>::iterator it = Quest->ExplorerInfos.begin();
		
		for (int index = 0; it != Quest->ExplorerInfos.end(); it++, index++)
		{
			ExplorerInfo* info = (ExplorerInfo*)(*it);
			if ( !ExplorerComp[index] && mapid == info->MapID )
			{
				static UINT RADIUS = 10;
				UINT length = (info->PosX - posx ) * (info->PosX -posx ) + (info->PosZ - posz ) * (info->PosZ - posz );
				if( length < (UINT) (RADIUS * RADIUS))
				{
					ExplorerComp[index] = true;
					return index;
				}
			}
		}
		return -1;
	}

	bool Kill(int id)
	{
		if (id == -1 && Status != QUEST_STATUS_INCOMPLETE)
			return false;

		if (!Quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_KILL_OR_CAST))
			return false;

		for (int i = 0 ; i < QUEST_OBJECTIVES_COUNT ; i++)
		{
			if (Quest->Creature[i] == id)
			{
				if (CreatureCount[i] >= Quest->CreatureCount[i])
					return false;
				else
				{
					CreatureCount[i]++;
					return true;
				}
			}
		}

		return false;
	}

	bool Cast(int id)
	{
		if (id == -1 && Status != QUEST_STATUS_INCOMPLETE)
			return false;

		if (!Quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_KILL_OR_CAST))
			return false;

		for (int i = 0 ; i < QUEST_OBJECTIVES_COUNT ; i++)
		{
			if (Quest->Item[i] == id)
			{
				if (sbase::RandGet(100) < Quest->ItemProbability[i])
				{
					if (ItemCount[i] >= Quest->ItemCount[i])
						return false;
					else
					{
						ItemCount[i]++;
						return true;
					}
				}
				else
					return false;
			}
		}

		return false;
	}

	void Clear()
	{
		TimeLimit = 0;

		memset(ItemCount,    0, QUEST_OBJECTIVES_COUNT * sizeof(short));
		memset(CreatureCount, 0, QUEST_OBJECTIVES_COUNT * sizeof(short));
		memset(ExplorerComp,    0, QUEST_EXPLORER_COUNT * sizeof(bool));
	}

	bool UpdataTime()
	{
		TimeUpdata = (int)Quest->TimeUpdata + Quest->TimeStatus;

		if (Status == QUEST_STATUS_OVER)
			Status = QUEST_STATUS_AVAILABLE;

		if (Completed)
		{
			Completed = 0;
			return true;
		}

		return false;
	}

	void Release()
	{
		Quest = NULL;
		Reward = NULL;

		Status = QUEST_STATUS_NONE;

		Completed = 0;
		TimeLimit = 0;

		memset(ItemCount,    0, QUEST_OBJECTIVES_COUNT * sizeof(short));
		memset(CreatureCount, 0, QUEST_OBJECTIVES_COUNT * sizeof(short));
		memset(ExplorerComp,    0, QUEST_EXPLORER_COUNT * sizeof(bool));
	}

	bool Cancel()
	{
		if (!Quest)
			return false;

		memset(ItemCount,    0, QUEST_OBJECTIVES_COUNT * sizeof(short));
		memset(CreatureCount, 0, QUEST_OBJECTIVES_COUNT * sizeof(short));
		memset(ExplorerComp,    0, QUEST_EXPLORER_COUNT * sizeof(bool));

		if (Quest->TimeOver)
		{
			Status = QUEST_STATUS_UNAVAILABLE;
			return false;
		}

		Status = QUEST_STATUS_AVAILABLE;

		return true;
	}

	bool Accept()
	{
		if (Status != QUEST_STATUS_AVAILABLE)
			return false;

		Status = QUEST_STATUS_INCOMPLETE;

		return true;
	}

	bool CanAccept()
	{
		return Status == QUEST_STATUS_AVAILABLE;
	}

	bool Complete()
	{
		Clear();

		Completed++;

		if (Quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_REPEATABLE))
		{
			if (Quest->Repeat > 0 && Completed >= Quest->Repeat)
				Status = QUEST_STATUS_OVER;
			else if ( Quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_LOOP))
			{
				Status = QUEST_STATUS_UNAVAILABLE;
			}
			else
				Status = QUEST_STATUS_AVAILABLE;
		}
		else if ( Quest->QuestSort == QUEST_SORT_RANDOM )
		{
			Status = QUEST_STATUS_AVAILABLE;
		}
		else
		{
			Status = QUEST_STATUS_OVER;
		}

		return true;
	}

	int UpData(int level)
	{
		if (!m_Timer.IsExpire())
			return QUEST_STATUS_NONE;

		m_Timer.Update();

		if (!Quest)
			return QUEST_STATUS_NONE;

		if (Status == QUEST_STATUS_COMPLETE || Status == QUEST_STATUS_FAILD)
			return QUEST_STATUS_NONE;

		if (Quest->RequiredLevel > level)
			return QUEST_STATUS_NONE;

		if (Quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_SPEAKTO))
		{
			Status = QUEST_STATUS_COMPLETE;

			return QUEST_STATUS_COMPLETE;
		}

		if (Quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_KILL_OR_CAST))
		{
			for (int i = 0 ; i < QUEST_OBJECTIVES_COUNT ; i++)
			{
				if (ItemCount[i] < Quest->ItemCount[i])
					return QUEST_STATUS_NONE;
				if (CreatureCount[i] < Quest->CreatureCount[i])
					return QUEST_STATUS_NONE;

				Status = QUEST_STATUS_COMPLETE;

				return QUEST_STATUS_COMPLETE;
			}
		}

		if (Quest->HasSpecialFlag(QUEST_SPECIAL_FLAGS_MAP_EXPOLORER))
		{
			if( IsFinishedExplorer() )
			{
				Status = QUEST_STATUS_COMPLETE;

				return QUEST_STATUS_COMPLETE;
			}
		}

		return QUEST_STATUS_NONE;
	}

	sbase::CTimer m_Timer;

	QuestInfo *Quest;
	QuestReward* Reward;

	QuestStatus Status;

	time_t	TimeLimit;									

	time_t TimeUpdata;									

	char Completed;										

	short ItemCount[QUEST_OBJECTIVES_COUNT];
	short CreatureCount[QUEST_OBJECTIVES_COUNT];
	bool ExplorerComp[QUEST_EXPLORER_COUNT];
};

#endif





