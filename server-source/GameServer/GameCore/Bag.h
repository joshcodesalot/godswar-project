#ifndef _BAG_H_
#define _BAG_H_

#include "Item.h"

class Storage;

class Bag
{
public:

	Bag();
	~Bag();

	Item* GetItem(int index);

	int UseQuestItem(int questid);
	int GetItemFreeOverlap(Item* item);

	bool Push(Item* p);
	bool Push(Item* p , int index);
	bool Erase(int index);

	bool IsEmpty(int index);

	bool Move(Bag* other , int index , int otherindex/* , int overlay=0 */);
	//物品的拆分
	bool Break(Bag* other , int index , int otherindex , int overlay=0 );

	bool Move(Storage* storage , int index , int storageindex);
	bool MoveBag(Storage* storage , int index,int CurNum);

	bool GetActive() const;
	void SetActive(bool b);

	Item* GetEmptyItem();

	void Clear();

protected:
private:

	bool m_Active;

	Item m_Grids[MAX_BAG_GRID];
};

#endif

