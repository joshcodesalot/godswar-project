#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "Item.h"

#define DEFAULT_STORAGE_COUNT 18

class Bag;
class CPlayer;


class Storage
{
public:

	Storage();
	~Storage();

	void SetActive(int a);
	int GetActive() const;

	void SetItem(Item* item , int index);
	Item* GetItem(int index);

	bool IsEmpty(int index);

	bool Move(int index , int bagindex , int bagnum);// Û±ÍÕœ∂Ø
	bool Move(int index,int curbagnum);//”“key“∆∂Ø

	bool Switch(int index , int otherindex);

	int GetItemFreeOverlap(Item* item);
	int GetItemFreeCount();
	int GetItemOverlap(Item* item);

	void Clear();

	void Init(CPlayer* player);

	void AddMoney(int n);
	void SetMoney(int n);
	int GetMoney() const;

protected:
private:

	CPlayer* m_This;

	int m_Active;

	int m_Money;

	Item m_Grids[MAX_STORAGE_GRID];
};

#endif

