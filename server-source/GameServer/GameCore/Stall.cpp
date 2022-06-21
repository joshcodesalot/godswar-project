#include "stdafx.h"
#include "Stall.h"
#include "Player.h"
#include "ItemBackup.h"
#include "..\..\Common\MsgDefine.h"
#include "..\..\Common\MsgTypes.h"
#include "..\ServiceLoader.h"
#include "..\errormessage.h"

extern MSG_STALL msg_stall;
extern MSG_STALLITEM msg_stallitem;
extern MSG_STALLBUYITEM msg_stallbuyitem;
extern MSG_WALK_END msg_walk_end;


Stall::Stall()
:	m_Start(false)
{
	wmemset(m_Text , 0 , MAX_NAME);
}

Stall::~Stall()
{

}

bool Stall::IsStart()
{
	return m_Start;
}

void Stall::SetState(bool b)
{
	if (m_This->m_eState != OBJECT_IDLE)
		return;

	if (b == m_Start)
		return;

	m_Start = b;

	if (!b)
		Clear();

	msg_stall.State = b;
	msg_stall.uiID = m_This->GetID();
	int len = wcslen(m_This->GetStall()->GetText());
	wmemset(msg_stall.Text, 0, MAX_NAME);
	if (len < MAX_NAME)
	{		
		wcsncpy(msg_stall.Text,m_This->GetStall()->GetText(), len);
	}
	

	m_This->GetRegion()->SendAreaMsgOneToOther(m_This->GetCurrentCell() , &msg_stall);

	// 发送移动停止的消息
	msg_walk_end.X = m_This->GetPosX();
	msg_walk_end.Y = 0.0f;
	msg_walk_end.Z = m_This->GetPosZ();
	msg_walk_end.uiTick = 1;
	msg_walk_end.fAtan2 = m_This->m_fatan2;
	msg_walk_end.uiID = m_This->GetID();
	msg_walk_end.usMapID = (USHORT)m_This->GetRegion()->GetID();

	m_This->GetRegion()->SendAreaMsgOneToOther(m_This->GetCurrentCell() , &msg_walk_end);
}

void Stall::Init(CPlayer* player)
{
	m_This = player;
}

void Stall::Clear()
{
	m_Start = false;

	for (int i = 0 ; i < MAX_POCKETS ; i++)
		for (int j = 0 ; j < MAX_GOODS ; j++)
		{
			if (m_Goods[i][j].item)
				m_Goods[i][j].item->m_Lock = false;

			m_Goods[i][j].Clear();
		}

		wmemset(m_Text , 0 , MAX_NAME);
}

eError Stall::SetGoods(int index , int pocket , int bagindex , int bagnum , int money , int bijou)
{
	Goods* goods = GetGoods(index , pocket);

	if (!goods)
		return NO_MSG_ERRO;

	Item* item = m_This->GetItem(bagindex , bagnum);

	if (!item)
		return NO_MSG_ERRO;

	if (item->IsClear())
		return NO_MSG_ERRO;

	if (!goods->SetGoods(item , money , bijou))
		return MSG_ERRO_015F;

	return NO_MSG_ERRO;
}

eError Stall::DelGoods(int index , int pocket)
{
	Goods* goods = GetGoods(index , pocket);

	if (!goods)
		return NO_MSG_ERRO;
	if (!goods->item)
	{
		//ASSERT(0);
		return	MSG_ERRO_015A;
	}

	if (!goods->item->m_Lock)
	{
		//ASSERT(0);

		return MSG_ERRO_015F;
	}

	goods->item->m_Lock = false;

	goods->Clear();

	return NO_MSG_ERRO;
}


bool Stall::HaveGoods(__int64& ID , int index , int pocket)
{
	if (m_Goods[pocket][index].StallID != unsigned __int64(ID))
		return false;

	return m_Goods[pocket][index].item != NULL;
}

Goods* Stall::GetGoods(int index , int pocket)
{
	if (pocket < 0 || pocket >= MAX_POCKETS)
		return NULL;

	if (index < 0 || index >= MAX_GOODS)
		return NULL;

	return &m_Goods[pocket][index];
}

void Stall::GetGoods(CPlayer* player)
{
	ItemBaseAttribute* pItemBaseInfo = NULL;
	for (int i = 0 ; i < MAX_POCKETS ; i++)
		for (int j = 0 ; j < MAX_GOODS ; j++)
		{
			if (!m_Goods[i][j].item)
				continue;

			msg_stallitem.uiID = m_This->GetID();
			msg_stallitem.StallID = m_Goods[i][j].StallID;
			msg_stallitem.Index = j;
			msg_stallitem.Pocket = i;
			msg_stallitem.Money = m_Goods[i][j].money;
			msg_stallitem.Bijou = m_Goods[i][j].bijou;

			pItemBaseInfo = m_Goods[i][j].item->GetItemBaseAttribute();
			if (!pItemBaseInfo)
			{
				ASSERT(0);
				return;
			}
			msg_stallitem.BaseID = pItemBaseInfo->ID;
			msg_stallitem.Overlap = m_Goods[i][j].item->m_Overlap;
			msg_stallitem.BaseLevel = m_Goods[i][j].item->BaseLevel;
			msg_stallitem.AppLevel = m_Goods[i][j].item->AppLevel;

			for (int z = 0 ; z < MAX_EQUIPAPPEND_COUNT ; z++)
			{
				if (m_Goods[i][j].item->GetItemAppendAttribute(z))
					msg_stallitem.Append[z] = m_Goods[i][j].item->GetItemAppendAttribute(z)->ID;
				else
					msg_stallitem.Append[z] = -1;
			}

			player->s_World->SendMsgToClient(&msg_stallitem , player->GetSocket());
		}

}

eError Stall::BuyGoods(CPlayer* player , __int64& ID , int index , int pocket )
{

	if (!player)
		return NO_MSG_ERRO;

	if (!player->IsMsgOK(MSG_KITBAG_FLAG))
		return NO_MSG_ERRO;

	if (pocket < 0 || pocket >= MAX_POCKETS)
		return NO_MSG_ERRO;

	if (index < 0 || index >= MAX_GOODS)
		return NO_MSG_ERRO;

	if (!HaveGoods(ID , index , pocket))
		return MSG_ERRO_015C;//该物品已经扯回或被人买走，请向摊主咨询

	if (player->GetMoney() < m_Goods[pocket][index].money ||
		player->GetBijou() < m_Goods[pocket][index].bijou)
		return MSG_ERRO_015D;//金额不足

//  	if (player->GetFreeItemCount() == 0)
//  	{ 		
//  		return MSG_ERRO_0199;
//  	}

	if (!m_Goods[pocket][index].item->m_Lock)
		return NO_MSG_ERRO;

	m_Goods[pocket][index].item->m_Lock = false;

	if (!player->AddItem(m_Goods[pocket][index].item))
		return MSG_ERRO_0199;

	BackupStall(m_This->GetDBID() , player->GetDBID() , m_Goods[pocket][index].item , 
		m_Goods[pocket][index].money , m_Goods[pocket][index].bijou , m_Goods[pocket][index].StallID);

	player->AddMoney(-m_Goods[pocket][index].money);
	m_This->AddMoney(m_Goods[pocket][index].money);

	player->AddBijou(-m_Goods[pocket][index].bijou);
	m_This->AddBijou(m_Goods[pocket][index].bijou);


	m_Goods[pocket][index].item->Clear();
	m_Goods[pocket][index].Clear();

	return MSG_ERRO_015B;//购买成功但是不返回消息
}

void Stall::SetText(wchar_t* text)
{	
	wmemcpy(m_Text , text , MAX_NAME);
}

