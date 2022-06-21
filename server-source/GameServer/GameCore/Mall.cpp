#include "StdAfx.h"
#include "Mall.h"
#include "Region.h"
#include "Player.h"
#include "ItemManager.h"
#include "..\ServiceLoader.h"
#include "..\DataCore.h"
#include "..\ServiceLoader.h"

#include "..\..\Common\MsgDefine.h"
#include "..\..\Common\MsgTypes.h"

#include "../GameScript.h"

CMall* CMall::g_pMall = NULL;

extern MSG_MALL_ITEMDATA msg_gamemall_itemdate;
extern MSG_MALL_ITEMDATA msg_associationmall_itemdate;
extern MSG_MALL_SELL msg_mall_sell;
extern MSG_ASSOCIATIONDISCOUNT msg_associationdiscount;
extern MSG_MALL_SELL msg_association_sell;

CMall::CMall(void)
{

}


CMall::~CMall(void)
{

}
CMall* CMall::Instance()
{
	if (!g_pMall)
		g_pMall = new CMall();

	return g_pMall;
}

void CMall::Init()
{
	Clear();
	//LoadMallItemList("Script.GameMall");
	//LoadAssociationItemList("Script.AssociationMall");
	//LoadMallItemListType("Script.GameMall");
	//LoadAssociationItemListType("Script.AssociationMall");

	LoadAthensMallItemList( "Script.Athens_GameMall" );	//读取雅典商城物品脚本
	LoadSpartaMallItemList( "Script.Sparta_GameMall" );	//读取斯巴达商城物品脚本
	LoadAthensMallItemListType("Script.Athens_GameMall"); //读取雅典商城标签脚本
	LoadSpartaMallItemListType("Script.Sparta_GameMall"); //读取斯巴达商城标签脚本

	LoadAthensAssociationItemList( "Script.Athens_AssociationMall"); //读取雅典公会商城物品脚本
	LoadSpartaAssociationItemList( "Script.Sparta_AssociationMall" ); //读取斯巴达公会商城物品脚本


}

eError CMall::GetAthensMallData(CPlayer* player)
{

	//对数据包进行整合， 每个包裹发送的是实际的数据大小
	ASSERT(m_AthensMallSell.size() == m_AthensMallItems.size());
	for (int i=0; i< m_AthensMallItems.size(); i++)
	{
		msg_gamemall_itemdate.Head.usSize = sizeof( msg_gamemall_itemdate.Head ) + 4;

		msg_gamemall_itemdate.TabIndex = i;
		msg_gamemall_itemdate.MaxTabIndex = m_AthensMallItems.size();
		msg_gamemall_itemdate.ItemCount = (char)m_AthensMallItems[i].size();
		ASSERT(16 > msg_gamemall_itemdate.ItemCount);

		for (int j=0; j< (int)m_AthensMallItems[i].size() ; j++)
		{
			MallBase * mallbaseadd = ItemManager::Instance()->GetMallItemAppend( m_AthensMallItems[i][j].Base);
			if ( !mallbaseadd)
			{
				return NO_MSG_ERRO;
			}

			msg_gamemall_itemdate.Itemdata[j].Base = m_AthensMallItems[i][j].Base;
			for (int k=0; k<5; k++)
			{
				msg_gamemall_itemdate.Itemdata[j].Append[k] = m_AthensMallItems[i][j].Append[k];
			}
			std::map<NPCItemData::ITEM_SELL_TYPE,int>::iterator itor = m_AthensMallItems[i][j].Trade.begin();
			int iTypeNum = 0;
			for (;itor != m_AthensMallItems[i][j].Trade.end(); itor++ ,iTypeNum++)
			{
				if (iTypeNum >= 3)
				{
					break;
				}
				msg_gamemall_itemdate.Itemdata[j].BuyType[iTypeNum] = itor->first;
				msg_gamemall_itemdate.Itemdata[j].BuyConsume[iTypeNum] = itor->second;
			}
			msg_gamemall_itemdate.Itemdata[j].BuyTypeNum = m_AthensMallItems[i][j].Trade.size();
			msg_gamemall_itemdate.Itemdata[j].AppendLV = m_AthensMallItems[i][j].AppendLV;
			msg_gamemall_itemdate.Itemdata[j].BaseLV = m_AthensMallItems[i][j].BaseLV;
			msg_gamemall_itemdate.Itemdata[j].Binding = m_AthensMallItems[i][j].Binding;
			msg_gamemall_itemdate.Itemdata[j].Overlap = m_AthensMallItems[i][j].Overlap;
			msg_gamemall_itemdate.Itemdata[j].Sell = m_AthensMallSell.at(i);
			msg_gamemall_itemdate.Itemdata[j].Discount = mallbaseadd->Discount;
			msg_gamemall_itemdate.Itemdata[j].New = mallbaseadd->New;
			msg_gamemall_itemdate.Itemdata[j].Hot = mallbaseadd->Hot;

			msg_gamemall_itemdate.Head.usSize += sizeof(msg_gamemall_itemdate.Itemdata[j]);
		}

		player->s_World->SendMsgToClient(&msg_gamemall_itemdate , player->GetSocket());
	}
	return NO_MSG_ERRO;
}

eError CMall::GetSpartaMallData(CPlayer* player)
{

	//对数据包进行整合， 每个包裹发送的是实际的数据大小
	ASSERT(m_SpartaMallSell.size() == m_SpartaMallItems.size());
	for (int i=0; i< m_SpartaMallItems.size(); i++)
	{
		msg_gamemall_itemdate.Head.usSize = sizeof( msg_gamemall_itemdate.Head ) + 4;

		msg_gamemall_itemdate.TabIndex = i;
		msg_gamemall_itemdate.MaxTabIndex = m_SpartaMallItems.size();
		msg_gamemall_itemdate.ItemCount = (char)m_SpartaMallItems[i].size();
		ASSERT(16 > msg_gamemall_itemdate.ItemCount);

		for (int j=0; j< (int)m_SpartaMallItems[i].size() ; j++)
		{
			MallBase * mallbaseadd = ItemManager::Instance()->GetMallItemAppend( m_SpartaMallItems[i][j].Base);
			if ( !mallbaseadd)
			{
				return NO_MSG_ERRO;
			}

			msg_gamemall_itemdate.Itemdata[j].Base = m_SpartaMallItems[i][j].Base;
			for (int k=0; k<5; k++)
			{
				msg_gamemall_itemdate.Itemdata[j].Append[k] = m_SpartaMallItems[i][j].Append[k];
			}
			std::map<NPCItemData::ITEM_SELL_TYPE,int>::iterator itor = m_SpartaMallItems[i][j].Trade.begin();
			int iTypeNum = 0;
			for (;itor != m_SpartaMallItems[i][j].Trade.end(); itor++ ,iTypeNum++)
			{
				if (iTypeNum >= 3)
				{
					break;
				}
				msg_gamemall_itemdate.Itemdata[j].BuyType[iTypeNum] = itor->first;
				msg_gamemall_itemdate.Itemdata[j].BuyConsume[iTypeNum] = itor->second;
			}
			msg_gamemall_itemdate.Itemdata[j].BuyTypeNum = m_SpartaMallItems[i][j].Trade.size();
			msg_gamemall_itemdate.Itemdata[j].AppendLV = m_SpartaMallItems[i][j].AppendLV;
			msg_gamemall_itemdate.Itemdata[j].BaseLV = m_SpartaMallItems[i][j].BaseLV;
			msg_gamemall_itemdate.Itemdata[j].Binding = m_SpartaMallItems[i][j].Binding;
			msg_gamemall_itemdate.Itemdata[j].Overlap = m_SpartaMallItems[i][j].Overlap;
			msg_gamemall_itemdate.Itemdata[j].Sell = m_SpartaMallSell.at(i);
			msg_gamemall_itemdate.Itemdata[j].Discount = mallbaseadd->Discount;
			msg_gamemall_itemdate.Itemdata[j].New = mallbaseadd->New;
			msg_gamemall_itemdate.Itemdata[j].Hot = mallbaseadd->Hot;

			msg_gamemall_itemdate.Head.usSize += sizeof(msg_gamemall_itemdate.Itemdata[j]);
		}

		player->s_World->SendMsgToClient(&msg_gamemall_itemdate , player->GetSocket());
	}
	return NO_MSG_ERRO;
}

eError CMall::GetAthensAssociationData(CPlayer* player)
{
	//发送工会折扣等级对应表
	std::map<int , AssociationDiscount> ADmap;
	ADmap = ItemManager::Instance()->GetAssociationDiscount();
	ASSERT(30 > ADmap.size());
	msg_associationdiscount.Num = ADmap.size();
	int k=0;
	for (std::map<int,AssociationDiscount>::iterator iter = ADmap.begin(); iter != ADmap.end();iter++,k++)
	{
		if ( k > 29 )
		{
			msg_associationdiscount.Num = 30;
			break;
		}
		msg_associationdiscount.Data[k].level = (*iter).second.level;
		msg_associationdiscount.Data[k].StoneDiscount = (*iter).second.StoneDiscount;
		msg_associationdiscount.Data[k].GloryDiscount = (*iter).second.GloryDiscount;
	}
	player->s_World->SendMsgToClient(&msg_associationdiscount , player->GetSocket());


	//发送工会商城列表
	k=0;
	if (0 == m_AthensAssociationItems.size())
	{
		return NO_MSG_ERRO;
	}
	int index = m_AthensAssociationItems.size() / 16;
	int Num = m_AthensAssociationItems.size() % 16;
	for (int i=0; i< index; i++)
	{
		msg_associationmall_itemdate.TabIndex = i;
		msg_associationmall_itemdate.MaxTabIndex = index;
		msg_associationmall_itemdate.ItemCount = 16 ;

		for (int j=0; j< 16 ; j++)
		{
			MallBase * Associatonbaseadd = ItemManager::Instance()->GetMallItemAppend( m_AthensAssociationItems[k].Base);
			if ( !Associatonbaseadd)
			{
				return NO_MSG_ERRO;
			}

			msg_associationmall_itemdate.Itemdata[j].Base = m_AthensAssociationItems[k].Base;
			for (int m=0; m<5; m++)
			{
				msg_associationmall_itemdate.Itemdata[j].Append[m] = m_AthensAssociationItems[k].Append[m];
			}
			std::map<NPCItemData::ITEM_SELL_TYPE,int>::iterator itor = m_AthensAssociationItems[k].Trade.begin();
			int iTypeNum = 0;
			for (;itor != m_AthensAssociationItems[k].Trade.end(); itor++ ,iTypeNum++)
			{
				if (iTypeNum >= 3)
				{
					break;
				}
				msg_associationmall_itemdate.Itemdata[j].BuyType[iTypeNum] = itor->first;
				msg_associationmall_itemdate.Itemdata[j].BuyConsume[iTypeNum] = itor->second;
			}
			msg_associationmall_itemdate.Itemdata[j].BuyTypeNum = m_AthensAssociationItems[k].Trade.size();
			msg_associationmall_itemdate.Itemdata[j].AppendLV = m_AthensAssociationItems[k].AppendLV;
			msg_associationmall_itemdate.Itemdata[j].BaseLV = m_AthensAssociationItems[k].BaseLV;
			msg_associationmall_itemdate.Itemdata[j].Binding = m_AthensAssociationItems[k].Binding;
			msg_associationmall_itemdate.Itemdata[j].Overlap = m_AthensAssociationItems[k].Overlap;
			msg_associationmall_itemdate.Itemdata[j].Sell = 0;
			msg_associationmall_itemdate.Itemdata[j].Discount = Associatonbaseadd->Discount;
			msg_associationmall_itemdate.Itemdata[j].New = Associatonbaseadd->New;
			msg_associationmall_itemdate.Itemdata[j].Hot = Associatonbaseadd->Hot;
			k++;
		}
		msg_associationmall_itemdate.Head.usSize = sizeof( MSG_MALL_ITEMDATA );
		player->s_World->SendMsgToClient(&msg_associationmall_itemdate , player->GetSocket());
	}

	//如果只有一个包裹 <=16
	if (Num > 0)
	{
		msg_associationmall_itemdate.TabIndex = index;
		msg_associationmall_itemdate.MaxTabIndex = index + 1;
		msg_associationmall_itemdate.ItemCount = Num ;

		for ( int j=0; j< Num ; j++)
		{
			MallBase * Associatonbaseadd = ItemManager::Instance()->GetMallItemAppend( m_AthensAssociationItems[k].Base);
			if ( !Associatonbaseadd)
			{
				return NO_MSG_ERRO;
			}

			msg_associationmall_itemdate.Itemdata[j].Base = m_AthensAssociationItems[k].Base;
			for (int m=0; m<5; m++)
			{
				msg_associationmall_itemdate.Itemdata[j].Append[m] = m_AthensAssociationItems[k].Append[m];
			}
			std::map<NPCItemData::ITEM_SELL_TYPE,int>::iterator itor = m_AthensAssociationItems[k].Trade.begin();
			int iTypeNum = 0;
			for (;itor != m_AthensAssociationItems[k].Trade.end(); itor++ ,iTypeNum++)
			{
				if (iTypeNum >= 3)
				{
					break;
				}
				msg_associationmall_itemdate.Itemdata[j].BuyType[iTypeNum] = itor->first;
				msg_associationmall_itemdate.Itemdata[j].BuyConsume[iTypeNum] = itor->second;
			}
			msg_associationmall_itemdate.Itemdata[j].BuyTypeNum = m_AthensAssociationItems[k].Trade.size();
			msg_associationmall_itemdate.Itemdata[j].AppendLV = m_AthensAssociationItems[k].AppendLV;
			msg_associationmall_itemdate.Itemdata[j].BaseLV = m_AthensAssociationItems[k].BaseLV;
			msg_associationmall_itemdate.Itemdata[j].Binding = m_AthensAssociationItems[k].Binding;
			msg_associationmall_itemdate.Itemdata[j].Overlap = m_AthensAssociationItems[k].Overlap;
			msg_associationmall_itemdate.Itemdata[j].Sell = 0;
			msg_associationmall_itemdate.Itemdata[j].Discount = Associatonbaseadd->Discount;
			msg_associationmall_itemdate.Itemdata[j].New = Associatonbaseadd->New;
			msg_associationmall_itemdate.Itemdata[j].Hot = Associatonbaseadd->Hot;
			k++;
		}
		msg_associationmall_itemdate.Head.usSize = sizeof( MSG_MALL_ITEMDATA );
		player->s_World->SendMsgToClient(&msg_associationmall_itemdate , player->GetSocket());
	}


	return NO_MSG_ERRO;
}

eError CMall::GetSpartaAssociationData(CPlayer* player)
{
	//发送工会折扣等级对应表
	std::map<int , AssociationDiscount> ADmap;
	ADmap = ItemManager::Instance()->GetAssociationDiscount();
	ASSERT(30 > ADmap.size());
	msg_associationdiscount.Num = ADmap.size();
	int k=0;
	for (std::map<int,AssociationDiscount>::iterator iter = ADmap.begin(); iter != ADmap.end();iter++,k++)
	{
		if ( k > 29 )
		{
			msg_associationdiscount.Num = 30;
			break;
		}
		msg_associationdiscount.Data[k].level = (*iter).second.level;
		msg_associationdiscount.Data[k].StoneDiscount = (*iter).second.StoneDiscount;
		msg_associationdiscount.Data[k].GloryDiscount = (*iter).second.GloryDiscount;
	}
	player->s_World->SendMsgToClient(&msg_associationdiscount , player->GetSocket());


	//发送工会商城列表
	k=0;
	if (0 == m_SpartaAssociationItems.size())
	{
		return NO_MSG_ERRO;
	}
	int index = m_SpartaAssociationItems.size() / 16;
	int Num = m_SpartaAssociationItems.size() % 16;
	for (int i=0; i< index; i++)
	{
		msg_associationmall_itemdate.TabIndex = i;
		msg_associationmall_itemdate.MaxTabIndex = index;
		msg_associationmall_itemdate.ItemCount = 16 ;

		for (int j=0; j< 16 ; j++)
		{
			MallBase * Associatonbaseadd = ItemManager::Instance()->GetMallItemAppend( m_SpartaAssociationItems[k].Base);
			if ( !Associatonbaseadd)
			{
				return NO_MSG_ERRO;
			}

			msg_associationmall_itemdate.Itemdata[j].Base = m_SpartaAssociationItems[k].Base;
			for (int m=0; m<5; m++)
			{
				msg_associationmall_itemdate.Itemdata[j].Append[m] = m_SpartaAssociationItems[k].Append[m];
			}
			std::map<NPCItemData::ITEM_SELL_TYPE,int>::iterator itor = m_SpartaAssociationItems[k].Trade.begin();
			int iTypeNum = 0;
			for (;itor != m_SpartaAssociationItems[k].Trade.end(); itor++ ,iTypeNum++)
			{
				if (iTypeNum >= 3)
				{
					break;
				}
				msg_associationmall_itemdate.Itemdata[j].BuyType[iTypeNum] = itor->first;
				msg_associationmall_itemdate.Itemdata[j].BuyConsume[iTypeNum] = itor->second;
			}
			msg_associationmall_itemdate.Itemdata[j].BuyTypeNum = m_SpartaAssociationItems[k].Trade.size();
			msg_associationmall_itemdate.Itemdata[j].AppendLV = m_SpartaAssociationItems[k].AppendLV;
			msg_associationmall_itemdate.Itemdata[j].BaseLV = m_SpartaAssociationItems[k].BaseLV;
			msg_associationmall_itemdate.Itemdata[j].Binding = m_SpartaAssociationItems[k].Binding;
			msg_associationmall_itemdate.Itemdata[j].Overlap = m_SpartaAssociationItems[k].Overlap;
			msg_associationmall_itemdate.Itemdata[j].Sell = 0;
			msg_associationmall_itemdate.Itemdata[j].Discount = Associatonbaseadd->Discount;
			msg_associationmall_itemdate.Itemdata[j].New = Associatonbaseadd->New;
			msg_associationmall_itemdate.Itemdata[j].Hot = Associatonbaseadd->Hot;
			k++;
		}
		msg_associationmall_itemdate.Head.usSize = sizeof( MSG_MALL_ITEMDATA );
		player->s_World->SendMsgToClient(&msg_associationmall_itemdate , player->GetSocket());
	}

	//如果只有一个包裹 <=16
	if (Num > 0)
	{
		msg_associationmall_itemdate.TabIndex = index;
		msg_associationmall_itemdate.MaxTabIndex = index + 1;
		msg_associationmall_itemdate.ItemCount = Num ;

		for ( int j=0; j< Num ; j++)
		{
			MallBase * Associatonbaseadd = ItemManager::Instance()->GetMallItemAppend( m_SpartaAssociationItems[k].Base);
			if ( !Associatonbaseadd)
			{
				return NO_MSG_ERRO;
			}

			msg_associationmall_itemdate.Itemdata[j].Base = m_SpartaAssociationItems[k].Base;
			for (int m=0; m<5; m++)
			{
				msg_associationmall_itemdate.Itemdata[j].Append[m] = m_SpartaAssociationItems[k].Append[m];
			}
			std::map<NPCItemData::ITEM_SELL_TYPE,int>::iterator itor = m_SpartaAssociationItems[k].Trade.begin();
			int iTypeNum = 0;
			for (;itor != m_SpartaAssociationItems[k].Trade.end(); itor++ ,iTypeNum++)
			{
				if (iTypeNum >= 3)
				{
					break;
				}
				msg_associationmall_itemdate.Itemdata[j].BuyType[iTypeNum] = itor->first;
				msg_associationmall_itemdate.Itemdata[j].BuyConsume[iTypeNum] = itor->second;
			}
			msg_associationmall_itemdate.Itemdata[j].BuyTypeNum = m_SpartaAssociationItems[k].Trade.size();
			msg_associationmall_itemdate.Itemdata[j].AppendLV = m_SpartaAssociationItems[k].AppendLV;
			msg_associationmall_itemdate.Itemdata[j].BaseLV = m_SpartaAssociationItems[k].BaseLV;
			msg_associationmall_itemdate.Itemdata[j].Binding = m_SpartaAssociationItems[k].Binding;
			msg_associationmall_itemdate.Itemdata[j].Overlap = m_SpartaAssociationItems[k].Overlap;
			msg_associationmall_itemdate.Itemdata[j].Sell = 0;
			msg_associationmall_itemdate.Itemdata[j].Discount = Associatonbaseadd->Discount;
			msg_associationmall_itemdate.Itemdata[j].New = Associatonbaseadd->New;
			msg_associationmall_itemdate.Itemdata[j].Hot = Associatonbaseadd->Hot;
			k++;
		}
		msg_associationmall_itemdate.Head.usSize = sizeof( MSG_MALL_ITEMDATA );
		player->s_World->SendMsgToClient(&msg_associationmall_itemdate , player->GetSocket());
	}


	return NO_MSG_ERRO;
}

eError CMall::AthensMallSell(CPlayer* player , int index , int num , int count , int base)
{
	if (!player->IsMsgOK(MSG_KITBAG_FLAG))
		return NO_MSG_ERRO;

	//发送错误的物品位置
	if (index < 0 || num < 0 || count <= 0 || base < 0)
	{
		//外挂,
		return NO_MSG_ERRO;
	}

	//没有的物品列表
	if (index >= (int)m_AthensMallItems.size())
	{
		//GetItemList(player);
		return NO_MSG_ERRO;
	}

	//没有的物品
	if (num >= (int)m_AthensMallItems[index].size())
	{
		//GetItemList(player);
		return NO_MSG_ERRO;
	}

	//如果在交易中
	if (player->GetTrade()->IsTrading())
	{
		return MSG_ERRO_0197;
	}

	Item item;
	ItemManager::Instance()->CreateItem(&m_AthensMallItems[index][num] , &item);

	//如果没有这个物品的配置
	if (base != item.GetItemBaseAttribute()->ID)
	{
		//GetItemList(player);
		//外挂
		return NO_MSG_ERRO;
	}

	//对item进行一些必要的赋值
	std::map<NPCItemData::ITEM_SELL_TYPE,int>::iterator itor = m_AthensMallItems[index][num].Trade.begin();
	int iTypeNum = 0;
	for (;itor != m_AthensMallItems[index][num].Trade.end(); itor++ ,iTypeNum++)
	{
		if (iTypeNum >= 3)
		{
			break;
		}
		item.m_BuyType[iTypeNum] = itor->first;
		item.m_BuyConsume[iTypeNum] = itor->second;
	}
	item.m_BuyTypeNum = m_AthensMallItems[index][num].Trade.size();

	//获取打折
	MallBase * mallbaseadd = ItemManager::Instance()->GetMallItemAppend( m_AthensMallItems[index][num].Base);
	if ( !mallbaseadd)
	{
		return NO_MSG_ERRO;
	}
	item.Discount = mallbaseadd->Discount;

	//判断是否有足够的货币
	if ( !CheckMallConsume(player,&item) )
	{
		return MSG_ERRO_0198;
	}

	//判断包裹是否有空位
	int ItemNum = 0;
	ItemNum = player->GetFreeItemOverlap(&item);
	if (ItemNum < count)
	{
		//外挂
		return MSG_ERRO_0199;
	}

	//把物品添加到玩家包裹
	bool isAddSucess = false;
	if (item.GetItemBaseAttribute()->EType == ItemBaseAttribute::ConsumeItem )
	{
		//item.m_Overlap = count;
		isAddSucess = player->AddItem(&item);

	}
	else
	{
		for (int i=0; i<count; i++)
		{
			//item.m_Overlap = item.GetItemBaseAttribute()->Overlap;

			isAddSucess = player->AddItem(&item);
			if ( !isAddSucess)
			{
				break;
			}

		}
	}

	if (isAddSucess)
	{
		UpdataMallConsume(player,&item); //更新玩家货币

		msg_mall_sell.Index = index;
		msg_mall_sell.Num = num;
		msg_mall_sell.Count = count;

		player->s_World->SendMsgToClient(&msg_mall_sell , player->GetSocket());
	}
	else
	{
		//外挂...
	}
	return NO_MSG_ERRO;
}

eError CMall::SpartaMallSell(CPlayer* player , int index , int num , int count , int base)
{
	if (!player->IsMsgOK(MSG_KITBAG_FLAG))
		return NO_MSG_ERRO;

	//发送错误的物品位置
	if (index < 0 || num < 0 || count <= 0 || base < 0)
	{
		//外挂,
		return NO_MSG_ERRO;
	}

	//没有的物品列表
	if (index >= (int)m_SpartaMallItems.size())
	{
		//GetItemList(player);
		return NO_MSG_ERRO;
	}

	//没有的物品
	if (num >= (int)m_SpartaMallItems[index].size())
	{
		//GetItemList(player);
		return NO_MSG_ERRO;
	}

	//如果在交易中
	if (player->GetTrade()->IsTrading())
	{
		return MSG_ERRO_0197;
	}

	Item item;
	ItemManager::Instance()->CreateItem(&m_SpartaMallItems[index][num] , &item);
	//如果没有这个物品的配置
	if (base != item.GetItemBaseAttribute()->ID)
	{
		//GetItemList(player);
		//外挂
		return NO_MSG_ERRO;
	}

	//对item进行一些必要的赋值
	std::map<NPCItemData::ITEM_SELL_TYPE,int>::iterator itor = m_SpartaMallItems[index][num].Trade.begin();
	int iTypeNum = 0;
	for (;itor != m_SpartaMallItems[index][num].Trade.end(); itor++ ,iTypeNum++)
	{
		if (iTypeNum >= 3)
		{
			break;
		}
		item.m_BuyType[iTypeNum] = itor->first;
		item.m_BuyConsume[iTypeNum] = itor->second;
	}
	item.m_BuyTypeNum = m_SpartaMallItems[index][num].Trade.size();

	//获取打折
	MallBase * mallbaseadd = ItemManager::Instance()->GetMallItemAppend( m_SpartaMallItems[index][num].Base);
	if ( !mallbaseadd)
	{
		return NO_MSG_ERRO;
	}
	item.Discount = mallbaseadd->Discount;

	//判断是否有足够的货币
	if ( !CheckMallConsume(player,&item) )
	{
		return MSG_ERRO_0198;
	}

	//判断包裹是否有空位
	int ItemNum = 0;
	ItemNum = player->GetFreeItemOverlap(&item);
	if (ItemNum < count)
	{
		//外挂
		return MSG_ERRO_0199;
	}

	//把物品添加到玩家包裹
	bool isAddSucess = false;
	if (item.GetItemBaseAttribute()->EType == ItemBaseAttribute::ConsumeItem )
	{
		//item.m_Overlap = count;
		isAddSucess = player->AddItem(&item);

	}
	else
	{
		for (int i=0; i<count; i++)
		{
			//item.m_Overlap = item.GetItemBaseAttribute()->Overlap;

			isAddSucess = player->AddItem(&item);
			if ( !isAddSucess)
			{
				break;
			}

		}
	}

	if (isAddSucess)
	{
		UpdataMallConsume(player,&item); //更新玩家货币

		msg_mall_sell.Index = index;
		msg_mall_sell.Num = num;
		msg_mall_sell.Count = count;

		player->s_World->SendMsgToClient(&msg_mall_sell , player->GetSocket());
	}
	else
	{
		//外挂...
	}
	return NO_MSG_ERRO;
}


eError CMall::AthensAssociatonSell(CPlayer* player , int index , int num , int count , int base)
{

	// 	if( !player->GetConsortia() )
	// 	{
	// 		//如果没有公会，不能购买公会商城的东西
	// 		return;
	// 	}

	if (!player->IsMsgOK(MSG_KITBAG_FLAG))
		return NO_MSG_ERRO;

	//发送错误的物品位置
	if (index < 0 || num < 0 || count <= 0 || base < 0)
	{
		//外挂,
		return NO_MSG_ERRO;
	}


	//没有的物品
	if (num >= (int)m_AthensAssociationItems.size())
	{
		//GetItemList(player);
		return NO_MSG_ERRO;
	}

	//如果在交易中
	if (player->GetTrade()->IsTrading())
	{
		return MSG_ERRO_0197;
	}

	Item item;
	ItemManager::Instance()->CreateItem(&m_AthensAssociationItems[num] , &item);

	//如果没有这个物品的配置
	if (base != item.GetItemBaseAttribute()->ID)
	{
		//GetItemList(player);
		//外挂
		return NO_MSG_ERRO;
	}
	//对item进行一些必要的赋值
	std::map<NPCItemData::ITEM_SELL_TYPE,int>::iterator itor = m_AthensAssociationItems[num].Trade.begin();
	int iTypeNum = 0;
	for (;itor != m_AthensAssociationItems[num].Trade.end(); itor++ ,iTypeNum++)
	{
		if (iTypeNum >= 3)
		{
			break;
		}
		item.m_BuyType[iTypeNum] = itor->first;
		item.m_BuyConsume[iTypeNum] = itor->second;
	}
	item.m_BuyTypeNum = m_AthensAssociationItems[num].Trade.size();

	//获取打折
	MallBase * mallbaseadd = ItemManager::Instance()->GetMallItemAppend( m_AthensAssociationItems[num].Base);
	if ( !mallbaseadd)
	{
		return NO_MSG_ERRO;
	}
	item.Discount = mallbaseadd->Discount;


	//如果货币不够
	if ( !CheckAssociaConsume( player ,&item ) )
	{
		return MSG_ERRO_0198;
	}
	// 	AssociationDiscount * ADadd = ItemManager::Instance()->GetAssociationMallItemDiscount( (int)player->GetConsortia()->iLevel);
	// 	if ( ADadd )
	// 	{
	// 		return;
	// 	}


	//如果荣誉不够
	// 	if (  int(mallbaseadd->Glory * item.m_Overlap * ADadd->GloryDiscount /100) > player->GetlPrestige())
	// 	{
	// 		return;
	// 	}

	//判断包裹是否有空位
	int ItemNum = 0;
	ItemNum = player->GetFreeItemOverlap(&item);
	if (ItemNum < count)
	{
		//外挂
		return MSG_ERRO_0199;
	}

	//把物品添加到玩家包裹
	bool isAddSucess = false;
	if (item.GetItemBaseAttribute()->EType == ItemBaseAttribute::ConsumeItem )
	{
		isAddSucess = player->AddItem(&item);

	}
	else
	{
		for (int i=0; i<count; i++)
		{
			isAddSucess = player->AddItem(&item);
			if ( !isAddSucess)
			{
				break;
			}

		}
	}

	if (isAddSucess)
	{
		//扣去宝石跟声望
		UpdataAssociaConsume(player,&item);
		msg_association_sell.Index = index;
		msg_association_sell.Num = num;
		msg_association_sell.Count = count;

		player->s_World->SendMsgToClient(&msg_association_sell , player->GetSocket());
	}
	else
	{
		//外挂...
	}
	return NO_MSG_ERRO;
}

eError CMall::SpartaAssociatonSell(CPlayer* player , int index , int num , int count , int base)
{

	// 	if( !player->GetConsortia() )
	// 	{
	// 		//如果没有公会，不能购买公会商城的东西
	// 		return;
	// 	}

	if (!player->IsMsgOK(MSG_KITBAG_FLAG))
		return NO_MSG_ERRO;

	//发送错误的物品位置
	if (index < 0 || num < 0 || count <= 0 || base < 0)
	{
		//外挂,
		return NO_MSG_ERRO;
	}


	//没有的物品
	if (num >= (int)m_SpartaAssociationItems.size())
	{
		//GetItemList(player);
		return NO_MSG_ERRO;
	}

	//如果在交易中
	if (player->GetTrade()->IsTrading())
	{
		return MSG_ERRO_0197;
	}

	Item item;
	ItemManager::Instance()->CreateItem(&m_SpartaAssociationItems[num] , &item);

	//如果没有这个物品的配置
	if (base != item.GetItemBaseAttribute()->ID)
	{
		//GetItemList(player);
		//外挂
		return NO_MSG_ERRO;
	}
	//对item进行一些必要的赋值
	std::map<NPCItemData::ITEM_SELL_TYPE,int>::iterator itor = m_SpartaAssociationItems[num].Trade.begin();
	int iTypeNum = 0;
	for (;itor != m_SpartaAssociationItems[num].Trade.end(); itor++ ,iTypeNum++)
	{
		if (iTypeNum >= 3)
		{
			break;
		}
		item.m_BuyType[iTypeNum] = itor->first;
		item.m_BuyConsume[iTypeNum] = itor->second;
	}
	item.m_BuyTypeNum = m_SpartaAssociationItems[num].Trade.size();

	//获取打折
	MallBase * mallbaseadd = ItemManager::Instance()->GetMallItemAppend( m_SpartaAssociationItems[num].Base);
	if ( !mallbaseadd)
	{
		return NO_MSG_ERRO;
	}
	item.Discount = mallbaseadd->Discount;


	//如果货币不够
	if ( !CheckAssociaConsume( player ,&item ) )
	{
		return MSG_ERRO_0198;
	}
	// 	AssociationDiscount * ADadd = ItemManager::Instance()->GetAssociationMallItemDiscount( (int)player->GetConsortia()->iLevel);
	// 	if ( ADadd )
	// 	{
	// 		return;
	// 	}


	//如果荣誉不够
	// 	if (  int(mallbaseadd->Glory * item.m_Overlap * ADadd->GloryDiscount /100) > player->GetlPrestige())
	// 	{
	// 		return;
	// 	}

	//判断包裹是否有空位
	int ItemNum = 0;
	ItemNum = player->GetFreeItemOverlap(&item);
	if (ItemNum < count)
	{
		//外挂
		return MSG_ERRO_0199;
	}

	//把物品添加到玩家包裹
	bool isAddSucess = false;
	if (item.GetItemBaseAttribute()->EType == ItemBaseAttribute::ConsumeItem )
	{
		isAddSucess = player->AddItem(&item);

	}
	else
	{
		for (int i=0; i<count; i++)
		{
			isAddSucess = player->AddItem(&item);
			if ( !isAddSucess)
			{
				break;
			}

		}
	}

	if (isAddSucess)
	{
		//扣去宝石跟声望
		UpdataAssociaConsume(player,&item);
		msg_association_sell.Index = index;
		msg_association_sell.Num = num;
		msg_association_sell.Count = count;

		player->s_World->SendMsgToClient(&msg_association_sell , player->GetSocket());
	}
	else
	{
		//外挂...
	}
	return NO_MSG_ERRO;
}


void CMall::Clear()
{
	m_AthensMallItems.clear();
	m_AthensMallSell.clear();
	m_SpartaMallItems.clear();
	m_SpartaMallSell.clear();
	m_MallModule = NULL;

	//工会商城
	m_AthensAssociationItems.clear();
	m_AthensAssociationSell.clear();
	m_SpartaAssociationItems.clear();
	m_SpartaAssociationSell.clear();
	m_AssociationModule = NULL;

}

void CMall::LoadAthensMallItemList( const char* name )
{
	if (!name)
		return;

	if (PyType_Ready(&pyPlayer_type) < 0)
		return;

	PyErr_Clear();

	m_MallModule = ScriptSystem::Instance()->ImportModule(name);

	if (!m_MallModule)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	Py_XINCREF(&pyPlayer_type);
	PyModule_AddObject(m_MallModule , "player", (PyObject*)&pyPlayer_type);

	//////////////
	if (!m_MallModule)
		return;

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;

	pFunc = PyObject_GetAttrString(m_MallModule , "GetItemList");

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
			PyObject* AppendLV = PyDict_GetItemString(obj->cl_dict,"AppendLV");
			PyObject* Overlap = PyDict_GetItemString(obj->cl_dict,"Overlap");
			PyObject* Binding = PyDict_GetItemString(obj->cl_dict,"Binding");
			PyObject* Type = PyDict_GetItemString(obj->cl_dict,"Type");
			PyObject* Consume = PyDict_GetItemString(obj->cl_dict,"Consume");

			data.Base = PyInt_AsLong(Base);
			data.BaseLV = PyInt_AsLong(BaseLV);
			data.AppendLV = PyInt_AsLong(AppendLV);
			data.Overlap = PyInt_AsLong(Overlap);
			data.Binding = PyInt_AsLong(Binding);
			//2008-4-17			data.Money   = PyInt_AsLong(Money);

			for (int z = 0 ; z < MAX_EQUIPAPPEND_COUNT ; z++)
			{
				PyObject* append = PyList_GetItem(Append , z);

				data.Append[z] = PyInt_AsLong(append);
			}

			//消耗类型
			int  Num = PyList_Size(Type);
			for ( int a = 0 ; a < Num ; a++ )
			{
				PyObject* append = PyList_GetItem(Type , a);
				data.Trade[(NPCItemData::ITEM_SELL_TYPE)PyInt_AsLong(append)] = PyInt_AsLong(PyList_GetItem(Consume , a));
			}

			datas.push_back(data);

			Item item;
			ItemManager::Instance()->CreateItem(&data , &item);

			if (item.IsClear())
			{
				printf("Item Error , ID=%d" , data.Base);
				ASSERT(0);
			}
		}

		m_AthensMallItems.push_back(datas);

	}

	Py_XDECREF(pValue);

}

void CMall::LoadSpartaMallItemList( const char* name )
{
	if (!name)
		return;

	if (PyType_Ready(&pyPlayer_type) < 0)
		return;

	PyErr_Clear();

	m_MallModule = ScriptSystem::Instance()->ImportModule(name);

	if (!m_MallModule)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	Py_XINCREF(&pyPlayer_type);
	PyModule_AddObject(m_MallModule , "player", (PyObject*)&pyPlayer_type);

	//////////////
	if (!m_MallModule)
		return;

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;

	pFunc = PyObject_GetAttrString(m_MallModule , "GetItemList");

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
			PyObject* AppendLV = PyDict_GetItemString(obj->cl_dict,"AppendLV");
			PyObject* Overlap = PyDict_GetItemString(obj->cl_dict,"Overlap");
			PyObject* Binding = PyDict_GetItemString(obj->cl_dict,"Binding");
			PyObject* Type = PyDict_GetItemString(obj->cl_dict,"Type");
			PyObject* Consume = PyDict_GetItemString(obj->cl_dict,"Consume");

			data.Base = PyInt_AsLong(Base);
			data.BaseLV = PyInt_AsLong(BaseLV);
			data.AppendLV = PyInt_AsLong(AppendLV);
			data.Overlap = PyInt_AsLong(Overlap);
			data.Binding = PyInt_AsLong(Binding);
			//2008-4-17			data.Money   = PyInt_AsLong(Money);

			for (int z = 0 ; z < MAX_EQUIPAPPEND_COUNT ; z++)
			{
				PyObject* append = PyList_GetItem(Append , z);

				data.Append[z] = PyInt_AsLong(append);
			}

			//消耗类型
			int  Num = PyList_Size(Type);
			for ( int a = 0 ; a < Num ; a++ )
			{
				PyObject* append = PyList_GetItem(Type , a);
				data.Trade[(NPCItemData::ITEM_SELL_TYPE)PyInt_AsLong(append)] = PyInt_AsLong(PyList_GetItem(Consume , a));
			}

			datas.push_back(data);

			Item item;
			ItemManager::Instance()->CreateItem(&data , &item);

			if (item.IsClear())
			{
				printf("Item Error , ID=%d" , data.Base);
				ASSERT(0);
			}
		}

		m_SpartaMallItems.push_back(datas);

	}

	Py_XDECREF(pValue);

}

void CMall::LoadAthensAssociationItemList( const char* name )
{
	if (!name)
		return;

	if (PyType_Ready(&pyPlayer_type) < 0)
		return;

	PyErr_Clear();

	m_AssociationModule = ScriptSystem::Instance()->ImportModule(name);

	if (!m_AssociationModule)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	Py_XINCREF(&pyPlayer_type);
	PyModule_AddObject(m_AssociationModule , "player", (PyObject*)&pyPlayer_type);

	//////////////
	if (!m_AssociationModule)
		return;

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;

	pFunc = PyObject_GetAttrString(m_AssociationModule , "GetItemList");

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
			PyObject* AppendLV = PyDict_GetItemString(obj->cl_dict,"AppendLV");
			PyObject* Overlap = PyDict_GetItemString(obj->cl_dict,"Overlap");
			PyObject* Binding = PyDict_GetItemString(obj->cl_dict,"Binding");
			PyObject* Type = PyDict_GetItemString(obj->cl_dict,"Type");
			PyObject* Consume = PyDict_GetItemString(obj->cl_dict,"Consume");

			data.Base = PyInt_AsLong(Base);
			data.BaseLV = PyInt_AsLong(BaseLV);
			data.AppendLV = PyInt_AsLong(AppendLV);
			data.Overlap = PyInt_AsLong(Overlap);
			data.Binding = PyInt_AsLong(Binding);
			//2008-4-17			data.Money   = PyInt_AsLong(Money);

			for (int z = 0 ; z < MAX_EQUIPAPPEND_COUNT ; z++)
			{
				PyObject* append = PyList_GetItem(Append , z);

				data.Append[z] = PyInt_AsLong(append);
			}

			//消耗类型
			int  Num = PyList_Size(Type);
			for ( int a = 0 ; a < Num ; a++ )
			{
				PyObject* append = PyList_GetItem(Type , a);
				data.Trade[NPCItemData::ITEM_SELL_TYPE(PyInt_AsLong(append))] = PyInt_AsLong(PyList_GetItem(Consume , a));
			}

			m_AthensAssociationItems.push_back(data);

			Item item;
			ItemManager::Instance()->CreateItem(&data , &item);

			if (item.IsClear())
			{
				printf("Item Error , ID=%d" , data.Base);
				ASSERT(0);
			}
		}
	}

	Py_XDECREF(pValue);

}

void CMall::LoadSpartaAssociationItemList( const char* name )
{
	if (!name)
		return;

	if (PyType_Ready(&pyPlayer_type) < 0)
		return;

	PyErr_Clear();

	m_AssociationModule = ScriptSystem::Instance()->ImportModule(name);

	if (!m_AssociationModule)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	Py_XINCREF(&pyPlayer_type);
	PyModule_AddObject(m_AssociationModule , "player", (PyObject*)&pyPlayer_type);

	//////////////
	if (!m_AssociationModule)
		return;

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;

	pFunc = PyObject_GetAttrString(m_AssociationModule , "GetItemList");

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
			PyObject* AppendLV = PyDict_GetItemString(obj->cl_dict,"AppendLV");
			PyObject* Overlap = PyDict_GetItemString(obj->cl_dict,"Overlap");
			PyObject* Binding = PyDict_GetItemString(obj->cl_dict,"Binding");
			PyObject* Type = PyDict_GetItemString(obj->cl_dict,"Type");
			PyObject* Consume = PyDict_GetItemString(obj->cl_dict,"Consume");

			data.Base = PyInt_AsLong(Base);
			data.BaseLV = PyInt_AsLong(BaseLV);
			data.AppendLV = PyInt_AsLong(AppendLV);
			data.Overlap = PyInt_AsLong(Overlap);
			data.Binding = PyInt_AsLong(Binding);
			//2008-4-17			data.Money   = PyInt_AsLong(Money);

			for (int z = 0 ; z < MAX_EQUIPAPPEND_COUNT ; z++)
			{
				PyObject* append = PyList_GetItem(Append , z);

				data.Append[z] = PyInt_AsLong(append);
			}

			//消耗类型
			int  Num = PyList_Size(Type);
			for ( int a = 0 ; a < Num ; a++ )
			{
				PyObject* append = PyList_GetItem(Type , a);
				data.Trade[NPCItemData::ITEM_SELL_TYPE(PyInt_AsLong(append))] = PyInt_AsLong(PyList_GetItem(Consume , a));
			}

			m_SpartaAssociationItems.push_back(data);

			Item item;
			ItemManager::Instance()->CreateItem(&data , &item);

			if (item.IsClear())
			{
				printf("Item Error , ID=%d" , data.Base);
				ASSERT(0);
			}
		}
	}

	Py_XDECREF(pValue);

}

void CMall::LoadAthensMallItemListType(const char* name)
{
	m_MallModule = NULL;

	if (!name)
		return;

	if (PyType_Ready(&pyPlayer_type) < 0)
		return;

	PyErr_Clear();

	m_MallModule = ScriptSystem::Instance()->ImportModule(name);

	if (!m_MallModule)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	Py_XINCREF(&pyPlayer_type);
	PyModule_AddObject(m_MallModule , "player", (PyObject*)&pyPlayer_type);

	//////////////
	if (!m_MallModule)
		return;

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;

	pFunc = PyObject_GetAttrString(m_MallModule , "GetItemListType");

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
		m_AthensMallSell.push_back( Sell );

	}

	Py_XDECREF(pValue);
}

void CMall::LoadSpartaMallItemListType(const char* name)
{
	m_MallModule = NULL;

	if (!name)
		return;

	if (PyType_Ready(&pyPlayer_type) < 0)
		return;

	PyErr_Clear();

	m_MallModule = ScriptSystem::Instance()->ImportModule(name);

	if (!m_MallModule)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	Py_XINCREF(&pyPlayer_type);
	PyModule_AddObject(m_MallModule , "player", (PyObject*)&pyPlayer_type);

	//////////////
	if (!m_MallModule)
		return;

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;

	pFunc = PyObject_GetAttrString(m_MallModule , "GetItemListType");

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
		m_SpartaMallSell.push_back( Sell );

	}

	Py_XDECREF(pValue);
}

void CMall::LoadAssociationItemListType(const char* name)
{
	m_AssociationModule = NULL;

	if (!name)
		return;

	if (PyType_Ready(&pyPlayer_type) < 0)
		return;

	PyErr_Clear();

	m_AssociationModule = ScriptSystem::Instance()->ImportModule(name);

	if (!m_AssociationModule)
	{
		PyErr_Print();
		PyErr_Clear();

		return;
	}

	Py_XINCREF(&pyPlayer_type);
	PyModule_AddObject(m_AssociationModule , "player", (PyObject*)&pyPlayer_type);

	//////////////
	if (!m_AssociationModule)
		return;

	PyObject *pFunc = NULL;
	PyObject *pValue = NULL;

	pFunc = PyObject_GetAttrString(m_AssociationModule , "GetItemListType");

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
		m_AthensAssociationSell.push_back( Sell );
	}

	Py_XDECREF(pValue);
}


bool CMall::CheckMallConsume( CPlayer* player,Item* pItem )
{

	//判断是否有足够的货币(声望,公会贡献,银币,宝石)
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
				//公会贡献
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
				if ( player->GetlStone() <  pItem->m_Overlap * int(pItem->m_BuyConsume[i] * pItem->Discount /100))
				{
					return false;
				}
			}
			break;
		default:
			{
				//如果有不是这些货币类型的，当条件不符合
				return false;
			}
			break;
		}
	}

	return true;
}
void CMall::UpdataMallConsume( CPlayer* player,Item* pItem )
{

	//更新货币(声望,公会贡献,银币,宝石)
	for (int i=0; i< pItem->m_BuyTypeNum; i++)
	{
		switch(pItem->m_BuyType[i])
		{
		case ITEM_TRADE_PRESTIGE:
			{
				player->SetlPrestige(player->GetlPrestige() - pItem->m_BuyConsume[i]); //修改声望
			}
			break;
		case ITEM_TRADE_CONSORTIA_CONTRIBUTE:
			{
				//公会贡献
			}
			break;
		case ITEM_TRADE_MONEY:
			{
				player->SetlMoney( player->GetlMoney() - pItem->m_Overlap * pItem->m_BuyConsume[i] );
			}
			break;
		case ITEM_TRADE_BIJOU:
			{
				player->SetlStone( player->GetlStone() - pItem->m_Overlap * int(pItem->m_BuyConsume[i] * pItem->Discount /100) ); 
			}
			break;
		}
	}
}

bool CMall::CheckAssociaConsume(CPlayer* player,Item* pItem)
{
	//判断是否有足够的货币(声望,公会贡献,银币,宝石)
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
				if (pItem->m_BuyConsume[i] > player->GetlPrestige())  //cdx_gwo 需要家公会等级折扣
				{
					return false;
				}
			}
			break;
		case ITEM_TRADE_CONSORTIA_CONTRIBUTE:
			{
				//公会贡献
			}
			break;
		case ITEM_TRADE_MONEY:
			{
				if ( player->GetlMoney() <  pItem->m_Overlap * pItem->m_BuyConsume[i])
				{
					return false;
				}
			}
			break;
		case ITEM_TRADE_BIJOU:
			{
				if ( player->GetlStone() <  pItem->m_Overlap * int(pItem->m_BuyConsume[i] * pItem->Discount /100)) //cdx_gwo 需要家公会等级折扣
				{
					return false;
				}
			}
			break;
		default:
			{
				//如果有不是这些货币类型的，当条件不符合
				return false;
			}
			break;
		}
	}

	return true;
}
void CMall::UpdataAssociaConsume(CPlayer* player,Item* pItem)
{
	//更新货币(声望,公会贡献,银币,宝石)
	for (int i=0; i< pItem->m_BuyTypeNum; i++)
	{
		switch(pItem->m_BuyType[i])
		{
		case ITEM_TRADE_PRESTIGE:
			{
				player->SetlPrestige(player->GetlPrestige() - pItem->m_BuyConsume[i]); ////cdx_gwo 需要家公会等级折扣
			}
			break;
		case ITEM_TRADE_CONSORTIA_CONTRIBUTE:
			{
				//公会贡献
			}
			break;
		case ITEM_TRADE_MONEY:
			{
				player->SetlMoney( player->GetlMoney() - pItem->m_Overlap * pItem->m_BuyConsume[i] );
			}
			break;
		case ITEM_TRADE_BIJOU:
			{
				player->SetlStone( player->GetlStone() - (UINT)pItem->m_Overlap * (pItem->m_BuyConsume[i] * pItem->Discount /100) ); //cdx_gwo 需要家公会等级折扣
			}
			break;
		}
	}
}

void CMall::Release()
{
	delete this;
}
