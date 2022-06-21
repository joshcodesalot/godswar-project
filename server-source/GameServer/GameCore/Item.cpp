#include "stdafx.h"
#include "Item.h"
#include "ItemManager.h"

#ifdef _DEBUG
#include <assert.h>
#endif

Item::Item()
:	m_BaseAttribute(NULL) ,
m_Overlap(1) , m_Binding(false) , m_Lock(false), IsExpired(false), IsActivated(false)
{
	for (int i = 0 ; i < MAX_EQUIPAPPEND_COUNT ; i++)
	{
		m_ItemAppendAttributes[i] = NULL;
	}	
}

Item::~Item()
{

}

bool Item::HasClass(int n)
{
	if (m_BaseAttribute->Class.empty())
		return true;

	for (vector<int>::iterator iter = m_BaseAttribute->Class.begin() ; 
		iter != m_BaseAttribute->Class.end() ; iter++)
	{
		if (*iter == n)
			return true;
	}

	return false;
}


void Item::Init(int Base , int* Append , int BaseLV, int Appendlevel, int Overlap)
{
	ItemData data;

	data.Base = Base;

	for (int i = 0 ; i < MAX_EQUIPAPPEND_COUNT ; i++)
	{
		data.Append[i] = Append[i];
	}

	BaseLevel = BaseLV;
	AppLevel = Appendlevel;
	m_Overlap = Overlap;

	ItemManager::Instance()->CreateItem(&data , this);
}

void Item::Clear()
{
	m_BaseAttribute = NULL;

	for (int i = 0 ; i < MAX_EQUIPAPPEND_COUNT ; i++)
	{
		m_ItemAppendAttributes[i] = NULL;
	}

	//m_BaseAttribute->Level = 0;

	m_Lock = false;
	m_Overlap = 0;
	m_Binding = false;
	IsExpired = false;
	IsActivated = false;

}

bool Item::IsClear()
{
	bool b = !m_BaseAttribute;

	//for (int i = 0 ; i < MAX_EQUIPAPPEND_COUNT ; i++)
	//	b &= !m_ItemAppendAttributes[i];

	return b;
}

void Item::ClearAppendAttribute()
{
	for (int i = 0 ; i < MAX_EQUIPAPPEND_COUNT ; i++)
	{
		m_ItemAppendAttributes[i] = NULL;
	}
}

void Item::SetItemBaseAttribute(ItemBaseAttribute* attribute)
{
	m_BaseAttribute = attribute;
}

ItemBaseAttribute* Item::GetItemBaseAttribute()
{
	return m_BaseAttribute;
}


void Item::SetItemAppendAttribute(ItemAppendAttribute* attribute , int index)
{
#ifdef _DEBUG
	ASSERT(MAX_EQUIPAPPEND_COUNT > index);
#else
	if (MAX_EQUIPAPPEND_COUNT <= index || index < 0)
		return;
#endif

	m_ItemAppendAttributes[index] = attribute;
}

ItemAppendAttribute* Item::GetItemAppendAttribute(int index)
{
#ifdef _DEBUG
	ASSERT(MAX_EQUIPAPPEND_COUNT > index);
#else
	if (MAX_EQUIPAPPEND_COUNT <= index || index < 0)
		return NULL;
#endif

	return m_ItemAppendAttributes[index];
}

int Item::InsertAppendAttribute(ItemAppendAttribute* attribute)
{
	ItemAppendAttribute** empty = NULL;

	int index = 0;

	for (int i = 0 ; i < MAX_EQUIPAPPEND_COUNT ; i++)
	{
		if (m_ItemAppendAttributes[i])
		{
			if (m_ItemAppendAttributes[i]->Type == attribute->Type)
				return 0;
		}
		else if (!empty)
		{
			empty = &m_ItemAppendAttributes[i];
			index = i;
		}
	}

	if (!empty)
		return 0;

	for (size_t j = 0 ; j < m_BaseAttribute->MainAttribute.size() ; j++)
	{
		if (m_BaseAttribute->MainAttribute[j] == attribute->ID)
		{
			*empty = attribute;

			return 0;
		}
	}
	return 1;
}

ItemData Item::GetItemData()
{
	ItemData data;

	if (m_BaseAttribute)
	{
		data.Base = m_BaseAttribute->ID;
	}
	else
	{
		data.Base = -1;
		data.BaseLV = 0;
		data.AppendLV = 0;
		data.Binding = 0;
		data.Overlap = 0;

		for (int i = 0 ; i < MAX_EQUIPAPPEND_COUNT ; i++)
			data.Append[i] = -1;

		return data;
	}

	ASSERT(data.Base >= -1);

	data.BaseLV= (char)BaseLevel;
	data.AppendLV = (char)AppLevel;
	data.Binding = m_Binding;
	data.Overlap = (char)m_Overlap;

	for (int i = 0 ; i < MAX_EQUIPAPPEND_COUNT ; i++)
	{
		if (m_ItemAppendAttributes[i])
		{
			data.Append[i] = m_ItemAppendAttributes[i]->ID;
		}
		else
		{
			data.Append[i] = -1;
		}
	}

	return data;
}

void Item::Increase(EquipData* data)
{
	if (this->IsClear())
		return;

	int Attack = 0;
	float AppendAttack = 0.0;
	if(BaseLevel<1)BaseLevel = 1;

	if (m_BaseAttribute->EType != ItemBaseAttribute::Weapon)
		Attack = m_BaseAttribute->Attack[BaseLevel-1];


	float AppendDefence = 0.0;


	data->m_lMaxHP             +=  m_BaseAttribute->MAXHP[BaseLevel-1];
	data->m_lMaxMP             +=  m_BaseAttribute->MAXMP[BaseLevel-1];
	data->m_lAttack            +=  m_BaseAttribute->Attack[BaseLevel-1];
	data->m_lDefend            +=  m_BaseAttribute->Defence[BaseLevel-1];        		
	data->m_lMagicAttack       +=  m_BaseAttribute->MagicAk[BaseLevel-1];      
	data->m_lMagicDefend       +=  m_BaseAttribute->MagicRec[BaseLevel-1];   
	data->m_lHit               +=  m_BaseAttribute->Hit[BaseLevel-1];					
	data->m_lDodge             += 	m_BaseAttribute->Miss[BaseLevel-1];	
	data->m_lCritAppend        += 	m_BaseAttribute->FuryAddAk[BaseLevel-1];        
	data->m_lCritDefend        += 	m_BaseAttribute->FuryAddRec[BaseLevel-1];        
	data->m_fPhyDamageAppend   += 	m_BaseAttribute->PhysicalDamage[BaseLevel-1];   
	data->m_fMagicDamageAppend += 	m_BaseAttribute->MagicDamage[BaseLevel-1]; 
	data->m_lDamageSorb        += 	m_BaseAttribute->InjureImbibe[BaseLevel-1];        
	data->m_fBeCure            += 	m_BaseAttribute->AcceptCure[BaseLevel-1];            
	data->m_fCure              += 	m_BaseAttribute->Cure[BaseLevel-1];              
	data->m_lStatusHit         += 	m_BaseAttribute->State[BaseLevel-1];         
	data->m_lStatudDodge       += 	m_BaseAttribute->StateImmunity[BaseLevel-1];       

	for (int i = 0 ; i < MAX_EQUIPAPPEND_COUNT ; i++)
	{
		if (!m_ItemAppendAttributes[i])
			continue;

		float value = m_ItemAppendAttributes[i]->LvInfo[AppLevel-1];

		switch(m_ItemAppendAttributes[i]->Type)
		{
		case ItemAppendAttribute::AttackA:
			data->m_lAttack += (int)value;
			break;
		case ItemAppendAttribute::DefenceA:
			data->m_lDefend += (int)value;
			break;
		case ItemAppendAttribute::AddMagicAk:
			data->m_lMagicAttack += (int)value;
			break;
		case ItemAppendAttribute::AddMagicRec:
			data->m_lMagicDefend += (int)value;
			break;
		case ItemAppendAttribute::Hit:
			data->m_lHit += (int)value;
			break;
		case ItemAppendAttribute::Miss:
			data->m_lDodge += (int)value;
			break;
		case ItemAppendAttribute::FuryAkAdd:
			data->m_lCritAppend +=(int)value;
			break;
		case ItemAppendAttribute::FuryAkRec:
			data->m_lCritDefend += (int)value;
			break;
		case ItemAppendAttribute::PhysicalDamage:
			data->m_fPhyDamageAppend += (float)value;
			break;
		case ItemAppendAttribute::MagicDamage:
			data->m_fMagicDamageAppend += (float)value;
			break;
		case ItemAppendAttribute::InjureImbibe:
			data->m_lDamageSorb += (int)value;
			break;
		case ItemAppendAttribute::State:
			data->m_lStatusHit += (float)value;
			break;
		case ItemAppendAttribute::StateImmunity:
			data->m_lStatudDodge += (float)value;
			break;
		case ItemAppendAttribute::MaxHP:
			data->m_lMaxHP += (int)value;
			break;
		case ItemAppendAttribute::MaxMP:
			data->m_lMaxMP += (int)value;
			break;
		case ItemAppendAttribute::HPRestore:
			data->m_lResumeHP += (int)value;
			break;
		case ItemAppendAttribute::MPRestore:
			data->m_lResumeMP += (int)value;
			break;
		case ItemAppendAttribute::AcceptCure:
			data->m_fBeCure += (float)value;
			break;
		case ItemAppendAttribute::Cure:
			data->m_fCure += (float)value;
			break;
		}
	}
}

void Item::Decrease(EquipData* data)
{
	if (this->IsClear())
		return;

	int Attack = 0;
	float AppendAttack = 0.0;
	if(BaseLevel<1)BaseLevel = 1;
	if (m_BaseAttribute->EType != ItemBaseAttribute::Weapon)
		Attack = m_BaseAttribute->Attack[BaseLevel-1];

	int Defence = m_BaseAttribute->Defence[BaseLevel-1];
	float AppendDefence = 0.0;

	data->m_lMaxHP             -=  m_BaseAttribute->MAXHP[BaseLevel-1];
	data->m_lMaxMP             -=  m_BaseAttribute->MAXMP[BaseLevel-1];
	data->m_lAttack            -=  m_BaseAttribute->Attack[BaseLevel-1];
	data->m_lDefend            -=  m_BaseAttribute->Defence[BaseLevel-1];        		
	data->m_lMagicAttack       -=  m_BaseAttribute->MagicAk[BaseLevel-1];      
	data->m_lMagicDefend       -=  m_BaseAttribute->MagicRec[BaseLevel-1];   
	data->m_lHit               -=  m_BaseAttribute->Hit[BaseLevel-1];					
	data->m_lDodge             -= 	m_BaseAttribute->Miss[BaseLevel-1];	
	data->m_lCritAppend        -= 	m_BaseAttribute->FuryAddAk[BaseLevel-1];        
	data->m_lCritDefend        -= 	m_BaseAttribute->FuryAddRec[BaseLevel-1];        
	data->m_fPhyDamageAppend   -= 	m_BaseAttribute->PhysicalDamage[BaseLevel-1];   
	data->m_fMagicDamageAppend -= 	m_BaseAttribute->MagicDamage[BaseLevel-1]; 
	data->m_lDamageSorb        -= 	m_BaseAttribute->InjureImbibe[BaseLevel-1];        
	data->m_fBeCure            -= 	m_BaseAttribute->AcceptCure[BaseLevel-1];            
	data->m_fCure              -= 	m_BaseAttribute->Cure[BaseLevel-1];              
	data->m_lStatusHit         -= 	m_BaseAttribute->State[BaseLevel-1];         
	data->m_lStatudDodge       -= 	m_BaseAttribute->StateImmunity[BaseLevel-1]; 

	for (int i = 0 ; i < MAX_EQUIPAPPEND_COUNT ; i++)
	{
		if (!m_ItemAppendAttributes[i])
			continue;

		float value = m_ItemAppendAttributes[i]->LvInfo[AppLevel-1];

		switch(m_ItemAppendAttributes[i]->Type)
		{
		case ItemAppendAttribute::AttackA:
			data->m_lAttack -= (int)value;
			break;
		case ItemAppendAttribute::DefenceA:
			data->m_lDefend -= (int)value;
			break;
		case ItemAppendAttribute::AddMagicAk:
			data->m_lMagicAttack -= (int)value;
			break;
		case ItemAppendAttribute::AddMagicRec:
			data->m_lMagicDefend -= (int)value;
			break;
		case ItemAppendAttribute::Hit:
			data->m_lHit -= (int)value;
			break;
		case ItemAppendAttribute::Miss:
			data->m_lDodge -= (int)value;
			break;
		case ItemAppendAttribute::FuryAkAdd:
			data->m_lCritAppend -=(int)value;
			break;
		case ItemAppendAttribute::FuryAkRec:
			data->m_lCritDefend -= (int)value;
			break;
		case ItemAppendAttribute::PhysicalDamage:
			data->m_fPhyDamageAppend -= (float)value;
			break;
		case ItemAppendAttribute::MagicDamage:
			data->m_fMagicDamageAppend -= (float)value;
			break;
		case ItemAppendAttribute::InjureImbibe:
			data->m_lDamageSorb -= (int)value;
			break;
		case ItemAppendAttribute::State:
			data->m_lStatusHit -= (float)value;
			break;
		case ItemAppendAttribute::StateImmunity:
			data->m_lStatudDodge -= (float)value;
			break;
		case ItemAppendAttribute::MaxHP:
			data->m_lMaxHP -= (int)value;
			break;
		case ItemAppendAttribute::MaxMP:
			data->m_lMaxMP -= (int)value;
			break;
		case ItemAppendAttribute::HPRestore:
			data->m_lResumeHP -= (int)value;
			break;
		case ItemAppendAttribute::MPRestore:
			data->m_lResumeMP -= (int)value;
			break;
		case ItemAppendAttribute::AcceptCure:
			data->m_fBeCure -= (float)value;
			break;
		case ItemAppendAttribute::Cure:
			data->m_fCure -= (float)value;
			break;
		}
	}
}

bool Item::IsFull()
{
	return m_BaseAttribute->Overlap ==  m_Overlap;
}

bool Item::Push(Item* item)
{
	if (m_BaseAttribute->ID != item->GetItemBaseAttribute()->ID)
		return false;

	if (item->IsFull())
		return false;

	if (m_Lock)
		return false;

	if (item->m_Binding != this->m_Binding)
		return false;

	if (item->m_Overlap > m_BaseAttribute->Overlap - m_Overlap)
	{
		//物品有剩余

		item->m_Overlap -= (m_BaseAttribute->Overlap - m_Overlap);
		m_Overlap = m_BaseAttribute->Overlap;

		return false;
	}
	else
	{
		//物品无剩余

		m_Overlap += item->m_Overlap;
		item->Clear();

		return true;
	}

}

SkillFalg Item::GetSkillFalg()
{
	if (m_BaseAttribute)
		return (SkillFalg)m_BaseAttribute->eSkillFlag;

	return SF_Count;
}
