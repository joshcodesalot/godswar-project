#include "stdafx.h"
#include "Trade.h"
#include "Player.h"
#include "Error.h"
#include "ItemBackup.h"

#include "..\ServiceLoader.h"
#include "..\..\Common\MsgDefine.h"
#include "..\..\Common\MsgTypes.h"
#include "../World.h"

extern MSG_TRADE msg_trade;
extern MSG_TRADE_MONEY msg_trade_money;
extern MSG_TRADE_ADDITEM msg_trade_additem;
extern MSG_TRADE_ITEM msg_trade_item;
extern MSG_TRADE_REMOVEITEM msg_trade_removeitem;

extern unsigned __int64 g_TradeID;

Trade::Trade()
:	m_State(TradeIdle) ,
m_Trader(NULL) , m_This(NULL) ,
m_Money(0) , m_Bijou(0) ,
m_ItemCount(0)
{
	for (int i = 0 ; i < MAX_TRADE_ITEMS ; i++)
		m_Items[i] = NULL;
}

Trade::~Trade()
{

}

bool Trade::IsTrading()
{
	return m_State != TradeIdle;
}

void Trade::ClearTrade()
{
	m_State = TradeIdle;
	m_Trader = NULL;
	m_Money = 0;
	m_Bijou = 0;
	m_ItemCount = 0;

	for (int i = 0 ; i < MAX_TRADE_ITEMS ; i++)
		m_Items[i] = NULL;
}

void Trade::SetTradeState(TradeState state)
{
	m_State = state;
}

TradeState Trade::GetTradeState() const
{
	return m_State;
} 

eError Trade::StartTrade(CPlayer* p)
{
	//这里还要加距离限制
	if (m_State != TradeIdle || p->GetTrade()->GetTradeState() != TradeIdle ||
		m_Trader != NULL || p->GetTrade()->GetTrader() != NULL || p == m_This)
	{
		//SendServerError(PlayerIsBusy , m_This);

		return MSG_ERRO_024B;
	}

	if (!m_This->IsMsgOK(MSG_KITBAG_FLAG) ||
		!p->IsMsgOK(MSG_KITBAG_FLAG))
	{
		//SendServerError(PlayerIsBusy , m_This);

		return MSG_ERRO_024B;
	}

	if (m_This->IsDead() || p->IsDead())
	{
		//SendServerError(PlayerIsBusy , m_This);

		return MSG_ERRO_024B;
	}


	//同步状态,

#ifdef _DEBUG
	ASSERT(m_Money == 0 && m_Bijou == 0 && m_ItemCount == 0);
#else

	if (m_Money != 0 || m_Bijou != 0 || m_ItemCount != 0)
		return NO_MSG_ERRO;

#endif

	m_State = TradeStart;
	p->GetTrade()->SetTradeState(TradeStart);

	m_Trader = p;
	p->GetTrade()->SetTrader(m_This);

	msg_trade.uiID = m_This->GetID();
	msg_trade.uiObjectID = p->GetID();
	msg_trade.State = TradeStart;
	msg_trade.TraderState = TradeStart;

	strcpy(msg_trade.chName,m_This->GetName());
	strcpy(msg_trade.chObjectName,p->GetName());

	p->s_World->SendMsgToClient(&msg_trade , m_This->GetSocket());
	p->s_World->SendMsgToClient(&msg_trade , p->GetSocket());

	return NO_MSG_ERRO;
}

void Trade::EndTrade()
{
	//发送结束消息,

	msg_trade.uiID = m_This->GetID();
	msg_trade.uiObjectID = m_Trader->GetID();
	msg_trade.State = TradeCancel;

	strcpy(msg_trade.chName,m_This->GetName());
	strcpy(msg_trade.chObjectName,m_Trader->GetName());

	m_This->s_World->SendMsgToClient(&msg_trade , m_This->GetSocket());

	for (int i = 0 ; i < MAX_TRADE_ITEMS ; i++)
	{
		if (m_Items[i])
			m_Items[i]->m_Lock = false;
	}
}

eError Trade::LockTrade(CPlayer* pPlayer)
{
	if (!m_Trader)
	{
		//外挂,,不对,有可能对方取消了,但是没同步过去

		return NO_MSG_ERRO;
	}

	if (m_State == TradeIdle ||
		m_Trader->GetTrade()->GetTradeState() == TradeIdle ||
		m_Trader->GetTrade()->GetTrader() != m_This)
	{
		//外挂
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_TRADE]",pPlayer->GetAccounts(), pPlayer->GetName() );
		return NO_MSG_ERRO;
	}

	m_State = TradeLock;

	//同步锁定状态,
	msg_trade.uiID = m_This->GetID();
	msg_trade.uiObjectID = m_Trader->GetID();
	msg_trade.State = TradeLock;
	msg_trade.TraderState = m_Trader->GetTrade()->GetTradeState();

	strcpy(msg_trade.chName,m_This->GetName());
	strcpy(msg_trade.chObjectName,m_Trader->GetName());

	m_This->s_World->SendMsgToClient(&msg_trade , m_This->GetSocket());
	m_This->s_World->SendMsgToClient(&msg_trade , m_Trader->GetSocket());

	if (m_Trader->GetTrade()->GetTradeState() == TradeLock)
	{
		CompleteTrade();
	}

	return NO_MSG_ERRO;
}

eError Trade::CompleteTrade()
{
	if (m_ItemCount > m_Trader->GetFreeItemCount() + m_Trader->GetTrade()->GetItemCount() ||
		m_Trader->GetTrade()->GetItemCount() > m_This->GetFreeItemCount() + m_ItemCount)
	{
		//道具栏不够,交易结束
		
			
			MSG_MANAGE_RETURN  error;
			error.Head.usSize = sizeof(MSG_MANAGE_RETURN);
			error.Head.usType = _MSG_MANAGE_RETURN;
			error.ucDisc = MSG_ERRO_024C;
			m_This->s_World->SendMsgToClient( &error, m_This->GetSocket() );
			m_This->s_World->SendMsgToClient( &error, m_Trader->GetSocket() );

			m_Trader->GetTrade()->EndTrade();
			EndTrade();

			m_Trader->GetTrade()->ClearTrade();
			ClearTrade();

			return NO_MSG_ERRO;

		//SendErrorInfo(MSG_ERRO_024C , m_This->GetSocket());
		//SendErrorInfo(MSG_ERRO_024C , m_Trader->GetSocket());

		// 		m_Trader->GetTrade()->ClearTrade();
		// 		ClearTrade();

	}

	if (m_This->GetMoney() < m_Money ||
		m_Trader->GetMoney() < m_Trader->GetTrade()->GetMoney())
	{
		//金钱不够

		//SendServerError(MSG_ERRO_024D , m_This->GetSocket());
		//SendServerError(MSG_ERRO_024D , m_Trader->GetSocket());

		// 		m_Trader->GetTrade()->ClearTrade();
		// 		ClearTrade();

		return NO_MSG_ERRO;
	}

	if (m_This->GetBijou() < m_Bijou ||
		m_Trader->GetBijou() < m_Trader->GetTrade()->GetBijou())
	{
		//宝石

		//SendServerError(MSG_ERRO_024E , m_This->GetSocket());
		//SendServerError(MSG_ERRO_024E , m_Trader->GetSocket());

		// 		m_Trader->GetTrade()->ClearTrade();
		// 		ClearTrade();

		return NO_MSG_ERRO;
	}

	//备份
	BackupTrade(m_This->GetDBID() , m_Items , m_Money , m_Bijou , g_TradeID);
	BackupTrade(m_Trader->GetDBID() , m_Trader->GetTrade()->m_Items , 
		m_Trader->GetTrade()->GetMoney() , m_Trader->GetTrade()->GetBijou() , g_TradeID);

	g_TradeID++;

	m_This->AddMoney(-m_Money);
	m_This->AddMoney(m_Trader->GetTrade()->GetMoney());
	m_This->AddBijou(-m_Bijou);
	m_This->AddBijou(m_Trader->GetTrade()->GetBijou());

	m_Trader->AddMoney(-m_Trader->GetTrade()->GetMoney());
	m_Trader->AddMoney(m_Money);
	m_Trader->AddBijou(-m_Trader->GetTrade()->GetBijou());
	m_Trader->AddBijou(m_Bijou);

	vector<Item> tempThis;
	vector<Item> tempTrader;

	for (int i = 0 ; i < MAX_TRADE_ITEMS ; i++)
	{
		if (m_Items[i])
		{
#ifdef _DEBUG
			ASSERT(m_Items[i]->m_Lock);
#else
			if (!m_Items[i]->m_Lock)
			{
				m_Trader->GetTrade()->EndTrade();
				EndTrade();
			}
#endif
			m_Items[i]->m_Lock = false;

			tempThis.push_back(*m_Items[i]);

			m_Items[i]->Clear();
		}

	}

	for (int i = 0 ; i < MAX_TRADE_ITEMS ; i++)
	{
		if (m_Trader->GetTrade()->m_Items[i])
		{
#ifdef _DEBUG
			ASSERT( m_Trader->GetTrade()->m_Items[i]->m_Lock);
#else
			if (!m_Trader->GetTrade()->m_Items[i]->m_Lock)
			{
				m_Trader->GetTrade()->EndTrade();
				EndTrade();
			}
#endif

			m_Trader->GetTrade()->m_Items[i]->m_Lock = false;

			tempTrader.push_back(*m_Trader->GetTrade()->m_Items[i]);

			m_Trader->GetTrade()->m_Items[i]->Clear();
		}

	}

	if (m_Trader->GetTrade()->GetItemCount())
		m_This->AddItem(&tempTrader[0] , m_Trader->GetTrade()->GetItemCount());

	if (m_ItemCount)
		m_Trader->AddItem(&tempThis[0] , m_ItemCount);

	//同步玩家消息,
	msg_trade.uiID = m_This->GetID();
	msg_trade.uiObjectID = m_Trader->GetID();
	msg_trade.State = TradeComplete;
	msg_trade.TraderState = TradeComplete;
	strcpy(msg_trade.chName,m_This->GetName());
	strcpy(msg_trade.chObjectName,m_Trader->GetName());

	m_This->s_World->SendMsgToClient(&msg_trade , m_This->GetSocket());
	m_This->s_World->SendMsgToClient(&msg_trade , m_Trader->GetSocket());

	m_Trader->GetTrade()->ClearTrade();	
	ClearTrade();

	return NO_MSG_ERRO;
}

void Trade::SetTrader(CPlayer* trader)
{
	m_Trader = trader;
}

CPlayer* Trade::GetTrader()
{
	return m_Trader;
}

eError Trade::SetItem(int index , Item* item,CPlayer* pPlayer)
{
	if (!m_Trader)
	{
		//外挂

		return NO_MSG_ERRO;
	}

	if (m_State == TradeIdle ||
		m_Trader->GetTrade()->GetTradeState() == TradeIdle ||
		m_Trader->GetTrade()->GetTrader() != m_This)
	{
		//外挂
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_TRADE_ADDITEM]",pPlayer->GetAccounts(), pPlayer->GetName() );
		return NO_MSG_ERRO;
	}

	if (index < 0 || index >= MAX_TRADE_ITEMS || item->m_Lock || item->IsClear() || item->m_Binding)
	{
		//外挂
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_TRADE_ADDITEM]",pPlayer->GetAccounts(), pPlayer->GetName() );
		return NO_MSG_ERRO;
	}

	if (m_Items[index])
	{
		//如果已存在

		m_Items[index]->m_Lock = false;
	}
	else
		m_ItemCount++;

	item->m_Lock = true;
	m_Items[index] = item;

	m_Trader->GetTrade()->SetTradeState(TradeStart);
	m_State = TradeStart;

	//同步,

	msg_trade_item.uiID = m_This->GetID();
	msg_trade_item.Index = index;
	strcpy(msg_trade_item.chName,m_This->GetName());

	msg_trade_item.BaseID = item->GetItemBaseAttribute()->ID;
	msg_trade_item.BaseLevel = item->BaseLevel;
	msg_trade_item.AppendLevel = item->AppLevel;

	for (int i = 0 ; i < MAX_EQUIPAPPEND_COUNT ; i++)
	{
		if (item->GetItemAppendAttribute(i))
			msg_trade_item.Append[i] = item->GetItemAppendAttribute(i)->ID;
		else
			msg_trade_item.Append[i] = -1;
	}

	msg_trade_item.Overlap = item->m_Overlap;

	m_This->s_World->SendMsgToClient(&msg_trade_item , m_Trader->GetSocket());

	return NO_MSG_ERRO;
}

eError Trade::SetMoney(int money , int bijou ,CPlayer* pPlayer)
{
	if (money < 0 || bijou < 0)
	{
		return NO_MSG_ERRO;
	}

	if (!m_Trader)
	{
		//外挂

		return NO_MSG_ERRO;
	}

	if (m_State == TradeIdle ||
		m_Trader->GetTrade()->GetTradeState() == TradeIdle ||
		m_Trader->GetTrade()->GetTrader() != m_This)
	{
		//外挂
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_TRADE_ADDITEM]",pPlayer->GetAccounts(), pPlayer->GetName() );
		return NO_MSG_ERRO;
	}

	//检测玩家金钱,宝石
	if (m_This->GetMoney() < m_Money )
	{
		return MSG_ERRO_024D;
	}
	if (m_This->GetBijou() < m_Bijou)
	{
		return MSG_ERRO_024E;
	}

	m_Money = money;
	m_Bijou = bijou;

	m_Trader->GetTrade()->SetTradeState(TradeStart);
	m_State = TradeStart;

	msg_trade_money.uiID = m_This->GetID();
	msg_trade_money.Money = m_Money;
	msg_trade_money.Bijou = m_Bijou;
	strcpy(msg_trade_money.chName,m_This->GetName());

	//同步

	m_This->s_World->SendMsgToClient(&msg_trade_money , m_Trader->GetSocket());

	return NO_MSG_ERRO;
}

int Trade::GetMoney() const
{
	return m_Money;
}

int Trade::GetBijou() const
{
	return m_Bijou;
}

eError Trade::RemoveItem(int index ,CPlayer* pPlayer)
{
	if (!m_Trader || index >= MAX_TRADE_ITEMS || index < 0)
	{
		//外挂
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_TRADE_ADDITEM]",pPlayer->GetAccounts(), pPlayer->GetName() );
		return NO_MSG_ERRO;
	}

	Item* item = m_Items[index];

	if (!item ||
		m_State == TradeIdle ||
		m_Trader->GetTrade()->GetTradeState() == TradeIdle ||
		m_Trader->GetTrade()->GetTrader() != m_This)
	{
		//外挂
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_TRADE_ADDITEM]",pPlayer->GetAccounts(), pPlayer->GetName() );
		return NO_MSG_ERRO;
	}

	if (item->IsClear() || !item->m_Lock)
	{
		//外挂
		pPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_TRADE_ADDITEM]",pPlayer->GetAccounts(), pPlayer->GetName() );
		return NO_MSG_ERRO;
	}

	item->m_Lock = false;
	m_Items[index] = NULL;

	m_Trader->GetTrade()->SetTradeState(TradeStart);
	m_State = TradeStart;

	m_ItemCount--;

	//同步,
	msg_trade_removeitem.uiID = m_This->GetID();
	msg_trade_removeitem.Index = index;
	strcpy(msg_trade_removeitem.chName,m_This->GetName());

	m_This->s_World->SendMsgToClient(&msg_trade_removeitem , m_Trader->GetSocket());

	return NO_MSG_ERRO;
}

int Trade::GetItemCount() const
{
	return m_ItemCount;
}

void Trade::SetThis(CPlayer* p)
{
	m_This = p;
}

CPlayer* Trade::GetThis()
{
	return m_This;
}
