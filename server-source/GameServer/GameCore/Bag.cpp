#include "stdafx.h"
#include "Bag.h"
#include "Item.h"
#include "Storage.h"


Bag::Bag()
:	m_Active(false)
{
}

Bag::~Bag()
{
}

int Bag::UseQuestItem(int questid)
{
	if (!m_Active)
		return -1;

	for (int i = 0 ; i < MAX_BAG_GRID ; i++)
	{
		if (!m_Grids[i].IsClear() && !m_Grids[i].m_Lock)
		{
			if (m_Grids[i].GetItemBaseAttribute()->QuestID == questid)
			{
				if ((--m_Grids[i].m_Overlap) == 0)
					m_Grids[i].Clear();
				return i;
			}
		}
	}

	return -1;
}

int Bag::GetItemFreeOverlap(Item* item)
{
	if (!m_Active)
		return 0;

	int num = 0;

	for (int i = 0 ; i < MAX_BAG_GRID ; i++)
	{
		if (!m_Grids[i].IsClear() && !m_Grids[i].m_Lock)
		{
			if (m_Grids[i].GetItemBaseAttribute()->ID == item->GetItemBaseAttribute()->ID && m_Grids[i].m_Binding == item->m_Binding)
				if (m_Grids[i].GetItemBaseAttribute()->Overlap > m_Grids[i].m_Overlap)
				{
					num = m_Grids[i].GetItemBaseAttribute()->Overlap - m_Grids[i].m_Overlap;
				}
		}
	}

	return num;
}

void Bag::Clear()
{
	for (int i = 0 ; i < MAX_BAG_GRID ; i++)
		m_Grids[i].Clear();
}

Item* Bag::GetItem(int index)
{
	if (!m_Active)
		return NULL;

	if (index >= MAX_BAG_GRID || index < 0)
	{
		return NULL;
	}

	return &m_Grids[index];
}

Item* Bag::GetEmptyItem()
{
	if (!m_Active)
		return NULL;

	for (int i = 0 ; i < MAX_BAG_GRID ; i++)
	{
		if (m_Grids[i].IsClear())
			return &m_Grids[i];
	}

	return NULL;
}

bool Bag::GetActive() const
{
	return m_Active;
}

void Bag::SetActive(bool b)
{
	m_Active = b;
}

bool Bag::IsEmpty(int index)
{
#ifdef _DEBUG
	ASSERT(index < MAX_BAG_GRID);
#else
	if (index >= MAX_BAG_GRID || index < 0)
	{
		return false;
	}
#endif

	return m_Grids[index].IsClear();
}

bool Bag::Push(Item* p)
{
	if (p->IsClear())
		return false;

	if (!m_Active)
		return false;

	int OverLap = p->m_Overlap;

	if (p->GetItemBaseAttribute()->Overlap == 1)
	{
		for (int i = 0 ; i < MAX_BAG_GRID ; i++)
		{
			if (m_Grids[i].IsClear())
			{
				m_Grids[i] = *p;

				return true;
			}
		}

		return false;
	}

	for (int i = 0 ; i < MAX_BAG_GRID ; i++)
	{
		if (!m_Grids[i].IsClear())
		{
			if (m_Grids[i].GetItemBaseAttribute()->ID == p->GetItemBaseAttribute()->ID &&
				!m_Grids[i].m_Lock &&
				m_Grids[i].m_Binding == p->m_Binding)
				if (m_Grids[i].GetItemBaseAttribute()->Overlap > m_Grids[i].m_Overlap)
				{
					if (m_Grids[i].m_Overlap + OverLap > m_Grids[i].GetItemBaseAttribute()->Overlap)
					{
						OverLap = m_Grids[i].m_Overlap + OverLap - m_Grids[i].GetItemBaseAttribute()->Overlap;

						m_Grids[i].m_Overlap = m_Grids[i].GetItemBaseAttribute()->Overlap;
					
						p->m_Overlap = (char)OverLap;

						continue;
					}
					else
					{
						m_Grids[i].m_Overlap = m_Grids[i].m_Overlap + (char)OverLap;
					}

					return true;
				}
		}

	}

	for (int i = 0 ; i < MAX_BAG_GRID ; i++)
	{
		if (m_Grids[i].IsClear())
		{
			if (OverLap > p->GetItemBaseAttribute()->Overlap)
			{
				p->m_Overlap -= p->GetItemBaseAttribute()->Overlap;
				m_Grids[i] = *p;
				m_Grids[i].m_Overlap = (char)p->GetItemBaseAttribute()->Overlap;
				continue;
			}
			else
			{
				m_Grids[i] = *p;
				m_Grids[i].m_Overlap = (char)OverLap;
			}
		
			return true;
		}
	}

	return false;
}

bool Bag::Push(Item* p , int index)
{
#ifdef _DEBUG
	if (index >= MAX_BAG_GRID || !p)
		ASSERT(0);
#else
	if (index >= MAX_BAG_GRID || !p || index < 0)
	{
		return false;
	}
#endif

	if (!m_Active)
		return false;

	m_Grids[index] = *p;

	return true;
}

bool Bag::Erase(int index)
{
	if (!m_Active)
		return false;

#ifdef _DEBUG
	if (index >= MAX_BAG_GRID)
		ASSERT(0);
#else
	if (index >= MAX_BAG_GRID || index < 0)
	{
		return false;
	}
#endif

	if (!m_Active)
		return false;

	if (m_Grids[index].m_Lock)
		return false;

	m_Grids[index].Clear();

	return true;
}
bool Bag::Move(Bag* other , int index , int otherindex )
{
#ifdef _DEBUG
	if (index >= MAX_BAG_GRID || otherindex >= MAX_BAG_GRID || !other)
		ASSERT(0);
#else
	if (index >= MAX_BAG_GRID || otherindex >= MAX_BAG_GRID || !other ||
		index < 0 || otherindex < 0)
	{
		return false;
	}
#endif

	if (!m_Active)
		return false;


	if (other == this && index == otherindex)
		return false;

	if (!m_Grids[index].IsClear())
		if (m_Grids[index].m_Lock)
			return false;

	if (!other->m_Grids[otherindex].IsClear())
		if (other->m_Grids[otherindex].m_Lock)
			return false;

	if (!m_Grids[index].IsClear()
		&&!other->m_Grids[otherindex].IsClear())
	{
		if (m_Grids[index].GetItemBaseAttribute()->ID == other->m_Grids[otherindex].GetItemBaseAttribute()->ID
			&& m_Grids[index].m_Binding == other->m_Grids[otherindex].m_Binding 
			&&m_Grids[index].GetItemBaseAttribute()->Overlap > 1
			&& other->m_Grids[otherindex].GetItemBaseAttribute()->Overlap > 1)
		{
			int allOverlay = m_Grids[index].m_Overlap + other->m_Grids[otherindex].m_Overlap;
			if( allOverlay <= m_Grids[index].GetItemBaseAttribute()->Overlap)
			{
				other->m_Grids[otherindex]=m_Grids[index];
				other->m_Grids[otherindex].m_Overlap=allOverlay;
				m_Grids[index].Clear();
			}
			else
			{
				m_Grids[index].m_Overlap=allOverlay-m_Grids[index].GetItemBaseAttribute()->Overlap;
				other->m_Grids[otherindex]=m_Grids[index];
				other->m_Grids[otherindex].m_Overlap=m_Grids[index].GetItemBaseAttribute()->Overlap;
			}
		} 
		else
		{
			Item temp = m_Grids[index];
			m_Grids[index] = other->m_Grids[otherindex];
			other->m_Grids[otherindex] = temp;

		}
	}
	else
	{
		Item temp = m_Grids[index];
		m_Grids[index] = other->m_Grids[otherindex];
		other->m_Grids[otherindex] = temp;
	}

	return true;
}
bool Bag::Break(Bag* other , int index , int otherindex,int overlay)
{
#ifdef _DEBUG
	if (index >= MAX_BAG_GRID || otherindex >= MAX_BAG_GRID || !other)
		ASSERT(0);
#else
	if (index >= MAX_BAG_GRID || otherindex >= MAX_BAG_GRID || !other ||
		index < 0 || otherindex < 0)
	{
		return false;
	}
#endif

	if (!m_Active)
		return false;


	if (other == this && index == otherindex)
		return false;

	if (!other->m_Grids[otherindex].IsClear())
		return false;

	if ( overlay < m_Grids[index].m_Overlap && m_Grids[index].m_Overlap-overlay > 0)
	{	
		if (!other->m_Grids[otherindex].IsClear())
		{
			return false;

		} 
		else
		{
			if (m_Grids[index].m_Lock)
			{
				m_Grids[index].m_Lock = false;
			}
			m_Grids[index].m_Overlap-=overlay;
			other->m_Grids[otherindex]=m_Grids[index];
			other->m_Grids[otherindex].m_Overlap=overlay;

		}
	}
	else
	{
		return false;

	}


	return true;
}
bool Bag::Move(Storage* storage , int index , int storageindex)
{
	if (!m_Active)
		return false;

	if (index < 0 || index >= MAX_BAG_GRID)
		return false;

	if (storageindex < 0 || storageindex >= storage->GetActive())
		return false;

	if (m_Grids[index].IsClear())
		return false;

	if (m_Grids[index].m_Lock)
	{
		return false;
	}

	Item item = *storage->GetItem(storageindex);
	storage->SetItem(&m_Grids[index] , storageindex);
	m_Grids[index] = item;

	return true;
}

bool Bag::MoveBag(Storage* storage , int index,int CurNum)
{
	if (!m_Active)
		return false;

	if (index < 0 || index >= MAX_BAG_GRID)
		return false;

	if (m_Grids[index].IsClear())
		return false;

	if (m_Grids[index].m_Lock)
		return false;

	if (storage->GetItemFreeOverlap(&m_Grids[index]) == 0)
		return false;

	int Full = 0;
	int CmpId = 0;
	for (int i = 0 ; i < CurNum; i++)
	{	
		if (storage->GetItem(i))
		{ 
			if( storage->GetItem(i)->GetItemBaseAttribute()!=NULL )
			{ 
				if(storage->GetItem(i)->GetItemBaseAttribute()->ID == m_Grids[index].GetItemBaseAttribute()->ID)
				{
					CmpId = 1; 
				}
				Full++;
			}
		}
	}

	if(Full == CurNum && !CmpId)
	{
		return false;
	}

	for (int i = 0 ; i < CurNum; i++)
	{
		if (!storage->GetItem(i))
			continue;

		if (storage->GetItem(i)->IsClear())
		{
			storage->SetItem(&m_Grids[index] , i);
			m_Grids[index].Clear();

			return true;
		}
		else if (storage->GetItem(i)->Push(&m_Grids[index]))
		{

			return true;
		}
	}

	return true;
}