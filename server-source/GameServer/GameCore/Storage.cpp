#include "stdafx.h"
#include "Storage.h"
#include "Bag.h"
#include "Player.h"


Storage::Storage()
:	m_This(NULL) , 
	m_Active(DEFAULT_STORAGE_COUNT) , m_Money(0)
{
	Clear();
}

Storage::~Storage()
{

}

void Storage::Init(CPlayer* player)
{
	m_This = player;
}


void Storage::Clear()
{
	m_Active = 0;

	for (int i = 0 ;  i < MAX_STORAGE_GRID ; i++)
	{
		m_Grids[i].Clear();
	}
}


void Storage::SetActive(int a)
{
	if (a < 0 || a > MAX_STORAGE_GRID)
		return;

	m_Active = a;
}

int Storage::GetActive() const
{
	return m_Active;
}

void Storage::SetItem(Item* item , int index)
{
	if (index < 0 || index >= m_Active)
		return;

	m_Grids[index] = *item;
}

Item* Storage::GetItem(int index)
{
	if (index < 0 || index >= m_Active)
		return NULL;

	return m_Grids + index;
}

bool Storage::IsEmpty(int index)
{
	if (index < 0 || index >= m_Active)
		return false;

	return m_Grids[index].IsClear();
}

bool Storage::Switch(int index , int otherindex)
{
	if (index == otherindex)
		return false;

	if (index < 0 || index >= m_Active)
		return false;

	if (otherindex < 0 || otherindex >= m_Active)
		return false;

	Item item = m_Grids[index];
	m_Grids[index] = m_Grids[otherindex];
	m_Grids[otherindex] = item;

	return true;
}

//½»»»
bool Storage::Move(int index , int bagindex , int bagnum)
{
	if (!m_This->IsMsgOK(MSG_KITBAG_FLAG))
		return false;

	if (index < 0 || index >= m_Active)
		return false;

	if (bagindex < 0 || bagindex >= MAX_BAGS)
		return false;

	if (bagnum < 0 || bagnum >= MAX_BAG_GRID)
		return false;

	if(m_This->GetItem(bagindex , bagnum) != NULL)
	{
		if (!m_This->GetItem(bagindex , bagnum)->IsClear())
		{
			if (m_This->GetItem(bagindex , bagnum)->m_Lock)
				return false;
		}
	}
	else
	{
		return false;
	}

	Item item = *m_This->GetItem(bagindex , bagnum);

	m_This->AddItem(&m_Grids[index] , bagindex , bagnum);

	m_Grids[index] = item;

	return true;
}

//ÒÆ¶¯
bool Storage::Move(int index,int curbagnum)
{
	if (!m_This->IsMsgOK(MSG_KITBAG_FLAG))
		return false;

	if (index < 0 || index >= m_Active)
		return false;

	if (m_Grids[index].IsClear())
		return false;

	if (m_This->GetFreeItemOverlap(&m_Grids[index]) == 0)
		return false;

	int NullNum=0;
	for (int i = 0 ; i < curbagnum ; i++)
	{
		for (int j = 0 ; j < MAX_BAG_GRID ; j++)
		{
			if(m_This->GetItem(i , j) != NULL)
			{
				if (m_This->GetItem(i , j)->IsClear()){NullNum++;}
			}
		}
	}
 
	int CmpId = 0;
	for (int i = 0 ; i < curbagnum ; i++)
	{
		for (int j = 0 ; j < MAX_BAG_GRID ; j++)
		{
			if(m_This->GetItem(i , j) != NULL)
			{
				if (!m_This->GetItem(i , j)->IsClear())
				{
					if(m_This->GetItem(i , j)->GetItemBaseAttribute() == NULL || m_Grids[index].GetItemBaseAttribute() ==NULL)
					{
						continue;
					}
					if(m_This->GetItem(i , j)->GetItemBaseAttribute()->ID == m_Grids[index].GetItemBaseAttribute()->ID)
					{
						CmpId = 1; 
					}
				}
			}
		}
	}

	if( !NullNum && !CmpId)
	{ return false; }

	for (int i = 0 ; i < curbagnum ; i++)
	{
		for (int j = 0 ; j < MAX_BAG_GRID ; j++)
		{
			if ( m_This->GetItem(i , j) != NULL )
			{
				if (m_This->GetItem(i , j)->IsClear())
				{
					*m_This->GetItem(i , j) = m_Grids[index];
					m_Grids[index].Clear();

					return true;
				}
				else if (m_This->GetItem(i , j )->Push(&m_Grids[index]))
				{
					return true;
				}
			}
		}
	}

	return true;
}

int Storage::GetItemFreeCount()
{
	int count = 0;

	for (int i = 0 ; i < m_Active ; i++)
	{
		if (m_Grids[i].IsClear())
			++count;
	}

	return count;
}

int Storage::GetItemFreeOverlap(Item* item)
{
	if (!item)
		return 0;

	if (item->IsClear())
		return 0;

	int count = 0;

	for (int i = 0 ; i < m_Active ; i++)
	{
		if (m_Grids[i].IsClear())
			count += item->GetItemBaseAttribute()->Overlap;
		else if (m_Grids[i].GetItemBaseAttribute()->ID == item->GetItemBaseAttribute()->ID)
			count += item->GetItemBaseAttribute()->Overlap - item->m_Overlap;
	}

	return count;
}

int Storage::GetItemOverlap(Item* item)
{
	if (!item)
		return 0;

	if (item->IsClear())
		return 0;

	int count = 0;

	for (int i = 0 ; i < m_Active ; i++)
	{
		if (m_Grids[i].GetItemBaseAttribute()->ID == item->GetItemBaseAttribute()->ID)
			count += item->m_Overlap;
	}

	return count;
}

void Storage::AddMoney(int n)
{
	m_Money += n;
}

void Storage::SetMoney(int n)
{
	m_Money = n;
}

int Storage::GetMoney() const
{
	return m_Money;
}

