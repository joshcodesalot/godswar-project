#pragma once

#include ".\GameCore\GameData.h"
#include "..\Common\MsgTypes.h"
#include "./GameCore/player.h"
class CDial
{
public:
	CDial();
	~CDial();

	struct DialItemData
	{

		ItemData item_data;						//实际的物品数据
		UINT DialID;							//属于哪种宝箱
		BYTE Nonsuch;							//是极品么？
		UINT RandInDial;						//入围几率
		UINT Probability;						//生成几率
		UINT ItemID;							//物品DNA
		int LV; 						//物品等级
	};
	class DialDefaultItemData:public ItemData
	{
	public:
		UINT DialID;							//属于哪种宝箱
		UINT ItemID;							//物品DNA
	};
	void Init();								//初始化
	static  CDial* Instance();						//实例
	void RandomItemInDial(UINT base_id);					//生成入围道具
	//del warning
	int RandomItem(UINT base_id);						//生成随机到的物品
	UINT DefaultItem(UINT base_id);						//生成默认物品
	DialDefaultItemData& CDial::GetDefaultItemData(UINT base_id);		//获取默认物品
	UINT* GetInDialItemID();						//获取入围物品ID
	void SetPlayer(CPlayer* pPlayer){pLocalPlayer = pPlayer;}		//设置当前玩家
	UINT SecondItem(UINT base_id);						//获得额外的物品
	eError  isGoldBoxIDExist(BYTE Iter,UINT base_id);			//是否存在该宝箱
	bool  isMsgWrong(MSG_GOLD_BOX* pGoldBoxMsg);				//验证消息的合法性
	std::map<UINT,DialDefaultItemData>m_DefaultItemDataMap;			//宝箱默认物品

private:
//	UINT m_InDialItemID[DIALITEMCOUNT];					//入围物品ID
	CPlayer* pLocalPlayer;							//当前角色
	std::map<UINT,std::map<UINT,DialItemData>>m_ItemDataMap;		//物品和宝箱对应表
	std::map<UINT,std::map<UINT,DialItemData>>m_NonsuchItemDataMap;		//极品表
	std::map<UINT,UINT> m_SumOfRandInDial;					//入围总几率
	std::map<UINT,UINT> m_SumOfNonsuchInDial;				//极品入围总几率
	DialItemData m_DialItemData[DIALITEMCOUNT];				//入围物品
	static CDial* m_This;

};