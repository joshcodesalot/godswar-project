#ifndef _STALL_H_
#define _STALL_H_

#include "Item.h"
#include "..\errormessage.h"
//I AM MISSING MY SISTER...
//GIVE BACK MY SISTER!!!!!!!!!

#define MAX_GOODS 6
#define MAX_POCKETS 3

class CPlayer;

extern unsigned __int64 g_StallID;

struct Goods
{
	Item* item;
	int money;
	int bijou;

	unsigned __int64 StallID;

	bool SetGoods(Item* item , int money , int bijou)
	{
		if (item->m_Lock)
		{
			//ASSERT(0);
			return false;
		}

		this->item = item;
		this->money = money;
		this->bijou = bijou;

		item->m_Lock = true;

		StallID = g_StallID; 
		g_StallID++;

		return true;
	}

	Goods()
		:item(NULL)
	{

	}

	void Clear()
	{
		item = NULL;

		money = 0;
		bijou = 0;
		StallID = 0;
	}
};

class Stall
{
public:

	Stall();
	~Stall();

	void Clear();

	eError SetGoods(int index , int pocket , int bagindex , int bagnum , int money , int bijou);

	eError DelGoods(int index , int pocket);

	Goods* GetGoods(int index , int pocket = 0);

	void GetGoods(CPlayer* player);

	bool IsStart();

	void SetState(bool b);

	eError BuyGoods(CPlayer* player , __int64& ID , int index , int pocket = 0);

	void Init(CPlayer* player);

	void SetText(wchar_t* text);

	wchar_t* GetText()
	{
		return m_Text;
	}

protected:
private:

	wchar_t m_Text[MAX_NAME];

	CPlayer* m_This;

	bool HaveGoods(__int64& ID , int index , int pocket = 0);

	bool m_Start;
	Goods m_Goods[MAX_POCKETS][MAX_GOODS];
};



#endif
