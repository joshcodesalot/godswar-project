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
	//��������
	enum 
	{
		ITEM_TRADE_PRESTIGE=0,			//����
		ITEM_TRADE_CONSORTIA_CONTRIBUTE,//���ṱ��
		ITEM_TRADE_MONEY,				//����
		ITEM_TRADE_BIJOU,				//��ʯ
	};
public:

	CNPC(void);
	~CNPC(void);

public:

	void ReloadSellingList();
	void ReloadQuests();

	virtual bool InitObjData(long ID);		//��ʼ���ڲ�����
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

	//���㹥����
	virtual             UINT                CalculateAttack() ;
	//��������ʱ������������������
	bool CalculatePassivePracticeDegree( PhysiacalAttack Type, ATTACK_TYPE eAttackType, bool IsDead = false );
	//ħ������ʱ������������������
	void CalculatePassivePracticeDegree(  int MagicID, ATTACK_TYPE eAttackType, bool IsDead = false );

	//�ı似��������
	virtual             bool                ChanegeDegree( int ucSkillID, int Buffer);
	//�жϼ�������
	virtual             bool                PassiveSkillUpgrade(UINT  SkillID, int *Rank = NULL )  ;
	//��ӱ������ܵȼ����߸ı����м��ܵǼ�
	virtual             void                AddPassiveSkill( int ucSkillID, int iRank = 1, int EXP = 0) ;
	//�����������˺�
	virtual             INT                CalculateAttackDamage(  CGameObject& pObj , float iPower = 0 ) ;
	//����ħ�������˺�
	virtual             INT                CalculateAttackDamage( int MagicID ) ;
	//������������Ϣͬ�����Լ�
	virtual void AynObjToObj( CCell *pOldCell, CCell *pNewCell );
	//���Լ�����Ϣͬ�����������
	virtual void AynMeToOther( CCell *pOldCell, CCell *pNewCell );

	//����
	virtual ULONG GetHit() ;
	//���
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

	//����
	QuestInfo m_Quests[QUEST_NPCTAKE_COUNT];
	QuestReward m_QuestReward[QUEST_NPCTAKE_COUNT];

	//����(����������)
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

