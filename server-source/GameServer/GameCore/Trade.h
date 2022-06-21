#ifndef _TRADE_H_
#define _TRADE_H_

#include "..\errormessage.h"

class CPlayer;
class Item;

#define MAX_TRADE_ITEMS 20

enum TradeState
{
	TradeIdle,
	TradeStart,
	TradeLock,
	TradeComplete,
	TradeCancel
};

class Trade
{
public:

	Trade();
	~Trade();

	bool IsTrading();

	void ClearTrade();

	eError StartTrade(CPlayer* p);
	void EndTrade();
	eError LockTrade(CPlayer* pPlayer);
	eError CompleteTrade();

	void SetTrader(CPlayer* trader);
	CPlayer* GetTrader();

	void SetThis(CPlayer* p);
	CPlayer* GetThis();

	TradeState GetTradeState() const;
	void SetTradeState(TradeState state);

	eError SetItem(int index , Item* item ,CPlayer* pPlayer);
	eError RemoveItem(int index ,CPlayer* pPlayer);

	Item* GetItem(int index);

	int GetItemCount() const;

	Item* m_Items[MAX_TRADE_ITEMS];

	eError SetMoney(int money , int bijou ,CPlayer* pPlayer);
	
	int GetMoney() const;
	int GetBijou() const;

protected:
private:

	int	m_Money;
	int m_Bijou;

	int m_ItemCount;

	TradeState m_State;

	CPlayer* m_Trader;
	CPlayer* m_This;
};

#endif

