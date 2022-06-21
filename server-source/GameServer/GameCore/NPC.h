#pragma once
#include "GameObject.h"
#include "Item.h"
#include "QuestDef.h"

#include "../../Common/Include/PyScript/ScriptSystem.h"

class CPlayer;

enum NPCFlag
{
	NPC_FLAG_NONE = 0,			
	NPC_FLAG_QUESTGIVER = 1,
	NPC_FLAG_QUESTRESPONDER = 2,
	NPC_FLAG_VENDOR = 4,
	NPC_FLAG_TRAINER = 8,
	NPC_FLAG_GUARD = 16,
	NPC_FLAG_BANKER = 32,
	NPC_FLAG_AUCTIONEER = 64,
	NPC_FLAG_CONSORTIA = 128,
	NPC_FLAG_CONSORTIA_LEVELUP = 256,
    NPC_FLAG_SYS = 512,
};

struct  LargessElem 
{

};


struct  NPCItemData :  public ItemData
{
	enum ITEM_SELL_TYPE{ ITEM_TRADE_PRESTIGE,ITEM_TRADE_CONSORTIA_CONTRIBUTE,ITEM_TRADE_MONEY,ITEM_TRADE_BIJOU=3 };
	std::map<ITEM_SELL_TYPE,int> Trade;
	NPCItemData():ItemData()
	{
        Trade.clear();
	}
};


class CNPC : public CGameObject
{
	//消费类型
	enum 
	{
		ITEM_TRADE_PRESTIGE=0,			//声望
		ITEM_TRADE_CONSORTIA_CONTRIBUTE,//公会贡献
		ITEM_TRADE_MONEY,				//银币
		ITEM_TRADE_BIJOU,				//宝石
	};
public:

	CNPC(void);
	~CNPC(void);

public:

	void ReloadSellingList();
	void ReloadQuests();

	virtual bool InitObjData(long ID);		//初始化内部数据
    virtual UINT GetRank(){ return 0; };
	eError Talk(CPlayer* player);
	void SysList( CPlayer* player, int SysFlag, const int *Oper );

	void SetScriptName(const char* name);
	const char* GetScriptName();

	void GetData(CPlayer* player);

	eError Sell(CPlayer* player , int index , int num , int count , int base);
	bool CheckNpcConsume(CPlayer* player,Item* pItem);
	void UpdataNpcConsume(CPlayer* player,Item* pItem);

	void Clear();

	//计算攻击力
	virtual             UINT                CalculateAttack() ;
	//物理攻击的时候计算各个技能熟练度
	bool CalculatePassivePracticeDegree( PhysiacalAttack Type, ATTACK_TYPE eAttackType, bool IsDead = false );
	//魔法攻击时候计算各个技能熟练度
	void CalculatePassivePracticeDegree(  int MagicID, ATTACK_TYPE eAttackType, bool IsDead = false );

	//改变技能熟练度
	virtual             bool                ChanegeDegree( int ucSkillID, int Buffer);
	//判断技能升级
	virtual             bool                PassiveSkillUpgrade(UINT  SkillID, int *Rank = NULL )  ;
	//添加被动技能等级或者改变已有技能登记
	virtual             void                AddPassiveSkill( int ucSkillID, int iRank = 1, int EXP = 0) ;
	//计算物理攻击伤害
	virtual             INT                CalculateAttackDamage(  CGameObject& pObj , float iPower = 0 ) ;
	//计算魔法攻击伤害
	virtual             INT                CalculateAttackDamage( int MagicID ) ;
	//将其他对象信息同步给自己
	virtual void AynObjToObj( CCell *pOldCell, CCell *pNewCell );
	//将自己的信息同步给其他玩家
	virtual void AynMeToOther( CCell *pOldCell, CCell *pNewCell );

	//命中
	virtual ULONG GetHit() ;
	//躲避
	virtual ULONG GetDodge() ;

	virtual void AI(void);

	bool HasFlag(NPCFlag flag);

	void GetQuestList(CPlayer* player);
	void GrantOfficial(CPlayer* player);
	void GrantItem(CPlayer* player);

private:

	void LoadItemList();
	void LoadItemListType();
	void LoadSysGift();

	void LoadQuest();
	void LoadQuestReward();

	void GetType();

	void GetSysGift(CPlayer* player);
	void GetItemList(CPlayer* player);
	void GetStorageData(CPlayer* player);

	int m_Flag;
	int m_SysFlag;

	bool m_Trade;

	//任务
	QuestInfo m_Quests[QUEST_NPCTAKE_COUNT];
	QuestReward m_QuestReward[QUEST_NPCTAKE_COUNT];

	//道具(仅仅买卖用)
	vector< vector<NPCItemData> > m_Items;
	vector<int> m_ItemBases;
	vector<char> m_Overlap;
	vector<char> m_BaseLevel;
	vector<char> m_AppendLevel;
	vector<int>  m_Money;
	vector<char> m_NpcSell;

	string m_ScriptName;
	PyObject* m_Module;
};

