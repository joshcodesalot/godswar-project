#pragma once

#include "Item.h"
#include "../../Common/Include/PyScript/ScriptSystem.h"
#include "NPC.h"

class CPlayer;


class CMall
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

	CMall(void);
	~CMall(void);

public:

	static CMall* Instance();
	void Release();
	void Init();
	////////////////////游戏商城//////////////////////////
	eError  GetAthensMallData(CPlayer* player);	//获取雅典商城的物品列表
	eError  GetSpartaMallData(CPlayer* player);	//获取斯巴达商城的物品列表

	eError  AthensMallSell(CPlayer* player , int index , int num , int count , int base);  //雅典贩卖物品
	eError  SpartaMallSell(CPlayer* player , int index , int num , int count , int base);  //斯巴达贩卖物品

	bool CheckMallConsume(CPlayer* player,Item* pItem);	//判读玩家是否有足够的货币
	void UpdataMallConsume(CPlayer* player,Item* pItem); //更新玩家身上的货币

	////////////工会商城////////////////
	eError  GetAthensAssociationData(CPlayer* player);	//获取雅典工会商城的物品列表
	eError  GetSpartaAssociationData(CPlayer* player);	//获取斯巴达工会商城的物品列表

	eError  AthensAssociatonSell(CPlayer* player , int index , int num , int count , int base);	//雅典贩卖工会商城物品
	eError  SpartaAssociatonSell(CPlayer* player , int index , int num , int count , int base);	//斯巴达贩卖工会商城物品

	bool CheckAssociaConsume(CPlayer* player,Item* pItem);	//判读玩家是否有足够的货币
	void UpdataAssociaConsume(CPlayer* player,Item* pItem);

	void Clear();

private:
	static CMall* g_pMall;
	void LoadAthensMallItemList( const char* name );	//读取雅典商城物品脚本
	void LoadSpartaMallItemList( const char* name );	//读取斯巴达商城物品脚本

	void LoadAthensAssociationItemList( const char* name ); //读取雅典公会商城物品脚本
	void LoadSpartaAssociationItemList( const char* name ); //读取斯巴达公会商城物品脚本

	void LoadAthensMallItemListType(const char* name); //读取雅典商城标签脚本
	void LoadSpartaMallItemListType(const char* name); //读取斯巴达商城标签脚本

	void LoadAssociationItemListType(const char* name); //目前公会只有一个标签，预留接口
	//游戏商城
	vector< vector<NPCItemData > > m_AthensMallItems;
	vector<char> m_AthensMallSell;
	vector< vector<NPCItemData > > m_SpartaMallItems;
	vector<char> m_SpartaMallSell;

	PyObject* m_MallModule;

	//工会商城
	vector<NPCItemData> m_AthensAssociationItems;
	vector<char> m_AthensAssociationSell;
	vector<NPCItemData> m_SpartaAssociationItems;
	vector<char> m_SpartaAssociationSell;

	PyObject* m_AssociationModule;


};

