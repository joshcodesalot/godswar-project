#include "stdafx.h"
#include "EquipForge.h"
#include "Player.h"
#include "ItemManager.h"
#include "..\..\Common\MsgDefine.h"
#include "..\..\Common\MsgTypes.h"
#include "..\ServiceLoader.h"
#include "..\ErrorMessage.h"

extern MSG_EQUIPFORGE_EQUIP msg_equipforge_equip;
extern MSG_EQUIPFORGE_MATERIAL msg_equipforge_material;
extern MSG_EUQIPFORGE msg_equipforge;


EquipForge::EquipForge()
{
	for (int i = 0; i < 6; i++)
	{
		m_Equip[i] = NULL;
		m_Material[i] = NULL;
	}
}

EquipForge::~EquipForge()
{

}

void EquipForge::Init(CPlayer* player)
{
	m_Player = player;
}

eError EquipForge::ClearEquip()
{
	for (int i = 0; i < 6; i++)
	{
		m_Equip[i] = NULL;
	}

	return NO_MSG_ERRO;
}

eError EquipForge::ClearMaterial()
{
	for (int i = 0; i < 6; i++)
	{
		m_Material[i] = NULL;
	}

	return NO_MSG_ERRO;
}

eError EquipForge::ClearForge()
{
	for (int i = 0; i < 6; i++)
	{
		m_Material[i] = NULL;
		m_Equip[i] = NULL;
	}

	return NO_MSG_ERRO;
}

eError EquipForge::SetEquip(Item* equip, int index, int num, int type)
{

	if (!equip || type < 0 || type>5)
	{
		m_Player->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_EQUIPFORGE_EQUIP]", m_Player->GetAccounts(), m_Player->GetName());
		return MSG_ERRO_0296;
	}

	if (equip->IsClear())
	{
		return MSG_ERRO_0296;
	}

	if (!equip->GetItemBaseAttribute())
	{
		return MSG_ERRO_0296;
	}

	if (!equip->GetItemBaseAttribute()->Equip)
	{
		return MSG_ERRO_0296;
	}

	if (equip->m_Lock)
	{
		return MSG_ERRO_0296;
	}

	m_Equip[type] = equip;

	return NO_MSG_ERRO;
}

eError EquipForge::SetMaterial(Item* material, int index, int num, int type)
{

	if (!material)
	{
		m_Player->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_EQUIPFORGE_MATERIAL]", m_Player->GetAccounts(), m_Player->GetName());
		return MSG_ERRO_02A5;
	}

	if (!material->GetItemBaseAttribute())
	{
		return MSG_ERRO_02A5;
	}

	if (material->IsClear())
	{

	}

	if (material->m_Lock)
	{

		return MSG_ERRO_02A5;
	}



	if (type == 2)
	{
		m_Material[0] = material;
	}

	if (type == 3) { m_Material[1] = material; }
	if (type == 4) { m_Material[2] = material; }
	if (type == 5) { m_Material[3] = material; }
	if (type == 6) { m_Material[4] = material; }

	if (type == 8)
	{
		m_Material[0] = material;
	}
	if (type == 9) { m_Material[1] = material; }
	if (type == 10) { m_Material[2] = material; }
	if (type == 11) { m_Material[3] = material; }
	if (type == 12) { m_Material[4] = material; }
	if (type == 13) { m_Material[5] = material; }

	return NO_MSG_ERRO;
}

void EquipForge::Clear()
{
	for (int i = 0; i < 6; i++)
	{
		if (m_Equip[i] != NULL)
		{
			m_Equip[i] = NULL;
		}

		if (m_Material[i] != NULL)
		{
			m_Material[i] = NULL;
		}
	}
}

eError EquipForge::Forge(int type)
{
	if (m_Player->GetWorldStatus() != eGAMEWORLD)
		return NO_MSG_ERRO;

	if (type == 0)
	{
		if (!m_Equip[0] || !m_Material[0])
		{
			sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[1]", m_Player->GetAccounts(), m_Player->GetName());
			return MSG_ERRO_0287;
		}

		if (m_Equip[0]->IsClear() || m_Material[0]->IsClear())
		{
			sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[2]", m_Player->GetAccounts(), m_Player->GetName());
			return MSG_ERRO_0287;
		}

		if (m_Equip[0]->BaseLevel <= 0 || m_Equip[0]->BaseLevel > 5 || m_Equip[0]->AppLevel <= 0 || m_Equip[0]->AppLevel > 10)
		{
			sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[3]", m_Player->GetAccounts(), m_Player->GetName());
			return MSG_ERRO_0287;
		}

		if (!m_Equip[0]->GetItemBaseAttribute() || !m_Material[0]->GetItemBaseAttribute())
		{
			sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[4]", m_Player->GetAccounts(), m_Player->GetName());
			return MSG_ERRO_0287;
		}

		EquipForgeBase* efb = ItemManager::Instance()->GetEquipForgeBase(m_Material[0]->GetItemBaseAttribute()->ID);
		if (!efb)
		{
			sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[5]", m_Player->GetAccounts(), m_Player->GetName());
			return MSG_ERRO_0287;
		}

		if (efb->MaterialType == 1)
		{
			ForgeBase();
			sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[6]", m_Player->GetAccounts(), m_Player->GetName());
		}
		if (efb->MaterialType == 2)
		{
			ForgeBaseData();
			sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[7]", m_Player->GetAccounts(), m_Player->GetName());
		}
		if (efb->MaterialType == 3)
		{
			ForgeAppend();
			sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[8]", m_Player->GetAccounts(), m_Player->GetName());
		}

		Clear();
	}
	else if (type == 1)
	{
		for (int i = 0; i < 6; i++)
		{
			if (!m_Material[i]) { return NO_MSG_ERRO; }
			else
			{
				if (!m_Material[i]->GetItemBaseAttribute()) { return NO_MSG_ERRO; }

			}
		}

		ForgeMaterial();
	}
	else if (type == 2)
	{
		int Eid[4] = { 0,3,4,5 };

		for (int i = 0; i < 4; i++)
		{
			if (!m_Equip[Eid[i]])
			{
				return NO_MSG_ERRO;
			}
			else
			{
				if (!m_Equip[Eid[i]]->GetItemBaseAttribute())
				{
					return NO_MSG_ERRO;
				}

				if (m_Equip[Eid[i]]->BaseLevel <= 0 || m_Equip[Eid[i]]->BaseLevel > 5 || m_Equip[Eid[i]]->AppLevel <= 0 || m_Equip[Eid[i]]->AppLevel > 10)
				{
					return NO_MSG_ERRO;
				}
			}
		}

		ForgeEquip();
	}
	else
	{
		m_Player->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_EUQIPFORGE]", m_Player->GetAccounts(), m_Player->GetName());
		return NO_MSG_ERRO;
	}

	return NO_MSG_ERRO;
}

void EquipForge::ForgeBase()
{
	EquipForgeBase* efb = ItemManager::Instance()->GetEquipForgeBase(m_Equip[0]->GetItemBaseAttribute()->ID);

	int Money = m_Player->GetMoney();

	if (!efb->Amoney.empty())
	{
		Money -= efb->Amoney[0];
	}

	m_Player->SetMoney(Money);

	char Ifbuf[128];
	_stprintf(Ifbuf, "成本:%d 当前:%d", efb->Amoney, m_Player->GetMoney());
	if (!efb)
	{
		return;
	}

	EquipForgeBase* efbA, * efbC, * efbB1, * efbB2, * efbB3;
	int ProyAdd[5] = { 0,0,0,0,0 };

	if (m_Material[1])
	{
		if (m_Material[1]->GetItemBaseAttribute() != NULL)
		{
			efbC = ItemManager::Instance()->GetEquipForgeBase(m_Material[1]->GetItemBaseAttribute()->ID);
			if (!efbC)
			{
				return;
			}
			ProyAdd[0] = efbC->MaterialProyAdd;
		}

	}
	if (m_Material[2])
	{
		if (m_Material[2]->GetItemBaseAttribute() != NULL)
		{
			efbB1 = ItemManager::Instance()->GetEquipForgeBase(m_Material[2]->GetItemBaseAttribute()->ID);
			if (!efbB1)
			{
				return;
			}
			ProyAdd[1] = efbB1->MaterialProyAdd;
		}
	}
	if (m_Material[3])
	{
		if (m_Material[3]->GetItemBaseAttribute() != NULL)
		{
			efbB2 = ItemManager::Instance()->GetEquipForgeBase(m_Material[3]->GetItemBaseAttribute()->ID);
			if (!efbB2)
			{

				return;
			}
			ProyAdd[2] = efbB2->MaterialProyAdd;
		}
	}
	if (m_Material[4])
	{
		if (m_Material[4]->GetItemBaseAttribute() != NULL)
		{
			efbB3 = ItemManager::Instance()->GetEquipForgeBase(m_Material[4]->GetItemBaseAttribute()->ID);
			if (!efbB3)
			{

				return;
			}
			ProyAdd[3] = efbB3->MaterialProyAdd;
		}
	}

	if (m_Material[0])
	{
		if (m_Material[0]->GetItemBaseAttribute() != NULL)
		{
			efbA = ItemManager::Instance()->GetEquipForgeBase(m_Material[0]->GetItemBaseAttribute()->ID);
			if (!efbA)
			{

				return;
			}
			ProyAdd[4] = efbA->MaterialProyAdd;
		}
	}

	int Modulus1 = 1, Modulus2 = 1, ForgeOk = 0;
	if (m_Material[0])
	{
		EquipForgeBase* efA = ItemManager::Instance()->GetEquipForgeBase(m_Material[0]->GetItemBaseAttribute()->ID);
		if (!efA)
		{

			return;
		}
		if (efA->MaterialType == 1)
		{
			ForgeOk = efb->Probability + ProyAdd[0] + ProyAdd[1] + ProyAdd[2] * Modulus1 + ProyAdd[3] * Modulus2 + ProyAdd[4];
		}
		else if (efA->MaterialType == 2)
		{
			ForgeOk = efb->BaseProyAdd[m_Equip[0]->BaseLevel - 1] + ProyAdd[0] + ProyAdd[1] + ProyAdd[2] * Modulus1 + ProyAdd[3] * Modulus2;
		}
		else if (efA->MaterialType == 3)
		{
			ForgeOk = efb->AppendProyAdd[m_Equip[0]->AppLevel - 1] + ProyAdd[0] + ProyAdd[1] + ProyAdd[2] * Modulus1 + ProyAdd[3] * Modulus2;
		}
	}

	int rand = sbase::RandGet(100);

	_stprintf(Ifbuf, "成功率:%d,%d", rand, ForgeOk);
	if (rand < ForgeOk)
	{
		m_Equip[0]->AppLevel -= 2;
		m_Equip[0]->BaseLevel--;

		if (m_Equip[0]->AppLevel < 1) { m_Equip[0]->AppLevel = 1; }
		if (m_Equip[0]->BaseLevel < 1) { m_Equip[0]->BaseLevel = 1; }

		int Base = efb->NextID;

		ItemData data = m_Equip[0]->GetItemData();
		data.Base = Base;

		ItemManager::Instance()->CreateItem(&data, m_Equip[0]);

		msg_equipforge.Success = true;
	}
	else
	{
		msg_equipforge.Success = false;
	}

	if (m_Material[0]->m_Binding)
		m_Equip[0]->m_Binding = true;

	if (m_Equip[0]->AppLevel == 0)
		m_Equip[0]->AppLevel = 1;

	int NoPunish = 0;
	if (m_Material[1] != NULL) { NoPunish = 1; }

	for (int i = 0; i < 6; i++)
	{
		if (m_Material[i] != NULL)
		{
			m_Material[i]->m_Overlap--;

			if (m_Material[i]->m_Overlap == 0)
				m_Material[i]->Clear();
			m_Material[i] = NULL;
		}
	}

	if (msg_equipforge.Success == false && !NoPunish)
	{
		m_Equip[0] = NULL;
	}

	msg_equipforge.Type = 1;
	m_Player->s_World->SendMsgToClient(&msg_equipforge, m_Player->GetSocket());

}

void EquipForge::ForgeBaseData()
{
	EquipForgeBase* efb = ItemManager::Instance()->GetEquipForgeBase(m_Equip[0]->GetItemBaseAttribute()->ID);

	int Money = m_Player->GetMoney();
	Money -= efb->Bmoney[m_Equip[0]->BaseLevel - 1];
	m_Player->SetMoney(Money);

	char Ifbuf[128];
	_stprintf(Ifbuf, "成本:%d 当前:%d", efb->Bmoney[m_Equip[0]->BaseLevel - 1], m_Player->GetMoney());
	if (!efb)
	{
		return;
	}

	EquipForgeBase* efbA, * efbC, * efbB1, * efbB2, * efbB3;
	int ProyAdd[5] = { 0,0,0,0,0 };

	if (m_Material[1])
	{
		if (m_Material[1]->GetItemBaseAttribute() != NULL)
		{
			efbC = ItemManager::Instance()->GetEquipForgeBase(m_Material[1]->GetItemBaseAttribute()->ID);
			if (!efbC)
			{

				return;
			}
			ProyAdd[0] = efbC->MaterialProyAdd;
		}
	}
	if (m_Material[2])
	{
		if (m_Material[2]->GetItemBaseAttribute() != NULL)
		{
			efbB1 = ItemManager::Instance()->GetEquipForgeBase(m_Material[2]->GetItemBaseAttribute()->ID);
			if (!efbB1)
			{

				return;
			}
			ProyAdd[1] = efbB1->MaterialProyAdd;
		}
	}
	if (m_Material[3])
	{
		if (m_Material[3]->GetItemBaseAttribute() != NULL)
		{
			efbB2 = ItemManager::Instance()->GetEquipForgeBase(m_Material[3]->GetItemBaseAttribute()->ID);
			if (!efbB2)
			{

				return;
			}
			ProyAdd[2] = efbB2->MaterialProyAdd;
		}
	}
	if (m_Material[4])
	{
		if (m_Material[4]->GetItemBaseAttribute() != NULL)
		{
			efbB3 = ItemManager::Instance()->GetEquipForgeBase(m_Material[4]->GetItemBaseAttribute()->ID);
			if (!efbB3)
			{

				return;
			}
			ProyAdd[3] = efbB3->MaterialProyAdd;
		}
	}
	if (m_Material[0])
	{
		if (m_Material[0]->GetItemBaseAttribute() != NULL)
		{
			efbA = ItemManager::Instance()->GetEquipForgeBase(m_Material[0]->GetItemBaseAttribute()->ID);
			if (!efbA)
			{

				return;
			}
			ProyAdd[4] = efbA->MaterialProyAdd;
		}
	}

	int Modulus1 = 1, Modulus2 = 1, ForgeOk = 0;
	if (m_Material[0])
	{
		EquipForgeBase* efA = ItemManager::Instance()->GetEquipForgeBase(m_Material[0]->GetItemBaseAttribute()->ID);
		if (!efA)
		{
			return;
		}
		if (efA->MaterialType == 1)
		{
			ForgeOk = efb->Probability + ProyAdd[0] + ProyAdd[1] + ProyAdd[2] * Modulus1 + ProyAdd[3] * Modulus2 + ProyAdd[4];
		}
		else if (efA->MaterialType == 2)
		{
			ForgeOk = efb->BaseProyAdd[m_Equip[0]->BaseLevel - 1] + ProyAdd[0] + ProyAdd[1] + ProyAdd[2] * Modulus1 + ProyAdd[3] * Modulus2 + ProyAdd[4];
		}
		else if (efA->MaterialType == 3)
		{
			ForgeOk = efb->AppendProyAdd[m_Equip[0]->AppLevel - 1] + ProyAdd[0] + ProyAdd[1] + ProyAdd[2] * Modulus1 + ProyAdd[3] * Modulus2 + ProyAdd[4];
		}
	}

	int rand = sbase::RandGet(100);

	_stprintf(Ifbuf, "成功率:%d,%d", rand, ForgeOk);
	int NoPunish = 0;
	if (m_Material[1] != NULL) { NoPunish = 1; }

	if (rand < ForgeOk)
	{
		if (m_Equip[0]->BaseLevel < 5)
		{
			m_Equip[0]->BaseLevel++;
		}
		msg_equipforge.Success = true;
	}
	else
	{
		msg_equipforge.Success = false;
	}

	if (m_Material[0]->m_Binding)
		m_Equip[0]->m_Binding = true;

	for (int i = 0; i < 6; i++)
	{
		if (m_Material[i] != NULL)
		{
			m_Material[i]->m_Overlap--;

			if (m_Material[i]->m_Overlap == 0)
				m_Material[i]->Clear();
			m_Material[i] = NULL;
		}
	}

	msg_equipforge.Type = 1;
	m_Player->s_World->SendMsgToClient(&msg_equipforge, m_Player->GetSocket());

}

void EquipForge::ForgeAppend()
{
	EquipForgeBase* efb = ItemManager::Instance()->GetEquipForgeBase(m_Equip[0]->GetItemBaseAttribute()->ID);

	int Money = m_Player->GetMoney();
	Money -= efb->Cmoney[m_Equip[0]->AppLevel - 1];
	m_Player->SetMoney(Money);

	char Ifbuf[128];
	_stprintf(Ifbuf, "成本:%d 当前:%d", efb->Cmoney[m_Equip[0]->AppLevel - 1], m_Player->GetMoney());
	if (!efb)
	{
		return;
	}

	EquipForgeBase* efbA, * efbC, * efbB1, * efbB2, * efbB3;
	int ProyAdd[5] = { 0,0,0,0,0 };

	if (m_Material[1])
	{
		if (m_Material[1]->GetItemBaseAttribute() != NULL)
		{
			efbC = ItemManager::Instance()->GetEquipForgeBase(m_Material[1]->GetItemBaseAttribute()->ID);
			if (!efbC)
			{
				return;
			}
			ProyAdd[0] = efbC->MaterialProyAdd;
		}
	}
	if (m_Material[2])
	{
		if (m_Material[2]->GetItemBaseAttribute() != NULL)
		{
			efbB1 = ItemManager::Instance()->GetEquipForgeBase(m_Material[2]->GetItemBaseAttribute()->ID);
			if (!efbB1)
			{

				return;
			}
			ProyAdd[1] = efbB1->MaterialProyAdd;
		}
	}
	if (m_Material[3])
	{
		if (m_Material[3]->GetItemBaseAttribute() != NULL)
		{
			efbB2 = ItemManager::Instance()->GetEquipForgeBase(m_Material[3]->GetItemBaseAttribute()->ID);
			if (!efbB2)
			{

				return;
			}
			ProyAdd[2] = efbB2->MaterialProyAdd;
		}
	}
	if (m_Material[4])
	{
		if (m_Material[4]->GetItemBaseAttribute() != NULL)
		{
			efbB3 = ItemManager::Instance()->GetEquipForgeBase(m_Material[4]->GetItemBaseAttribute()->ID);
			if (!efbB3)
			{

				return;
			}
			ProyAdd[3] = efbB3->MaterialProyAdd;
		}
	}
	if (m_Material[0])
	{
		if (m_Material[0]->GetItemBaseAttribute() != NULL)
		{
			efbA = ItemManager::Instance()->GetEquipForgeBase(m_Material[0]->GetItemBaseAttribute()->ID);
			if (!efbA)
			{

				return;
			}
			ProyAdd[4] = efbA->MaterialProyAdd;
		}
	}

	int Modulus1 = 1, Modulus2 = 1, ForgeOk = 0;
	if (m_Material[0])
	{
		EquipForgeBase* efA = ItemManager::Instance()->GetEquipForgeBase(m_Material[0]->GetItemBaseAttribute()->ID);
		if (!efA)
		{

			return;
		}
		if (efA->MaterialType == 1)
		{
			ForgeOk = efb->Probability + ProyAdd[0] + ProyAdd[1] + ProyAdd[2] * Modulus1 + ProyAdd[3] * Modulus2 + ProyAdd[4];
		}
		else if (efA->MaterialType == 2)
		{
			ForgeOk = efb->BaseProyAdd[m_Equip[0]->BaseLevel - 1] + ProyAdd[0] + ProyAdd[1] + ProyAdd[2] * Modulus1 + ProyAdd[3] * Modulus2 + ProyAdd[4];
		}
		else if (efA->MaterialType == 3)
		{
			ForgeOk = efb->AppendProyAdd[m_Equip[0]->AppLevel - 1] + ProyAdd[0] + ProyAdd[1] + ProyAdd[2] * Modulus1 + ProyAdd[3] * Modulus2 + ProyAdd[4];
		}
	}

	int rand = sbase::RandGet(100);

	_stprintf(Ifbuf, "成功率:%d,%d", rand, ForgeOk);
	int NoPunish = 0;
	if (m_Material[1] != NULL) { NoPunish = 1; }

	if (rand < ForgeOk)
	{
		m_Equip[0]->AppLevel++;

		msg_equipforge.Success = true;
	}
	else
	{
		msg_equipforge.Success = false;
	}

	if (m_Material[0]->m_Binding)
		m_Equip[0]->m_Binding = true;

	for (int i = 0; i < 6; i++)
	{
		if (m_Material[i] != NULL)
		{
			m_Material[i]->m_Overlap--;

			if (m_Material[i]->m_Overlap == 0)
				m_Material[i]->Clear();
			m_Material[i] = NULL;
		}
	}

	msg_equipforge.Type = 1;
	m_Player->s_World->SendMsgToClient(&msg_equipforge, m_Player->GetSocket());

}


void EquipForge::ForgeMaterial()
{
	int Binding = 0;
	for (int i = 0; i < 6; i++)
	{
		if (m_Material[i])
		{
			if (m_Material[i]->m_Binding > 0) { Binding = 1; }
		}
	}

	Item m_TempMaterial = *m_Material[0];

	if (!m_TempMaterial.GetItemBaseAttribute())
	{
		return;
	}

	EquipForgeBase* efb = ItemManager::Instance()->GetEquipForgeBase(m_TempMaterial.GetItemBaseAttribute()->ID);
	if (!efb)
	{
		return;
	}

	int Base = efb->NextID;

	ItemData data = m_TempMaterial.GetItemData();

	data = m_TempMaterial.GetItemData();
	data.Base = Base;
	data.Overlap = 1;
	ItemManager::Instance()->CreateItem(&data, &m_TempMaterial);

	if (Binding > 0) { m_TempMaterial.m_Binding = true; }

	m_Player->AddItem(&m_TempMaterial);

	if (m_TempMaterial.BaseLevel == 0)
	{
		m_TempMaterial.BaseLevel = 1;
	}

	for (int i = 0; i < 6; i++)
	{
		if (m_Material[i] != NULL)
		{
			m_Material[i]->m_Overlap--;

			if (m_Material[i]->m_Overlap == 0)
				m_Material[i]->Clear();
			m_Material[i] = NULL;
		}
	}

	msg_equipforge.Success = true;
	msg_equipforge.Type = 2;
	m_Player->s_World->SendMsgToClient(&msg_equipforge, m_Player->GetSocket());

}

void EquipForge::ForgeEquip()
{
	EquipForgeBase* efb = ItemManager::Instance()->GetEquipForgeBase(m_Equip[0]->GetItemBaseAttribute()->ID);
	if (!efb)
	{
		return;
	}
	ItemData data = m_Equip[0]->GetItemData();
	int Binding = 0;
	if (m_Equip[0]->m_Binding) { Binding = 0; }

	int AppNum[4] = { 0,0,0,0 };
	int AppNumEnd = 0;
	ItemAppendAttribute* attribute = NULL;
	for (int i = 0; i < MAX_EQUIPAPPEND_COUNT; i++)
	{
		attribute = NULL;
		attribute = m_Equip[0]->GetItemAppendAttribute(i);
		if (attribute != NULL)
		{
			AppNum[0] ++;
			attribute = NULL;
		}

		if (m_Equip[3] != NULL)
		{
			attribute = NULL;
			attribute = m_Equip[3]->GetItemAppendAttribute(i);
			if (attribute != NULL)
			{
				AppNum[1] ++;
			}
		}
		else { return; }

		if (m_Equip[4] != NULL)
		{
			attribute = NULL;
			attribute = m_Equip[4]->GetItemAppendAttribute(i);
			if (attribute != NULL)
			{
				AppNum[2] ++;
			}
		}
		else { return; }

		if (m_Equip[5] != NULL)
		{
			attribute = NULL;
			attribute = m_Equip[5]->GetItemAppendAttribute(i);
			if (attribute != NULL)
			{
				AppNum[3] ++;
			}
		}
		else { return; }
	}

	float AppNumEndf = (AppNum[0] + AppNum[1] + AppNum[2] + AppNum[3] + 1) * 1.0f / 4;
	if (AppNumEndf > 4) { AppNumEndf = 4; }

	AppNumEnd = (AppNum[0] + AppNum[1] + AppNum[2] + AppNum[3] + 1) / 4;
	if (AppNumEnd > 4) { AppNumEndf = 4; }

	int Spare = (AppNumEndf - AppNumEnd) * 10;

	int rand = sbase::RandGet(9);
	if (rand < Spare) { AppNumEnd++; }

	char Ifbuf[128];
	_stprintf(Ifbuf, "rand:%d < Spare:%d c附加数:(%d+%d+%d+%d)/4=%d rand:%d Spare:%d", rand, Spare, AppNum[0], AppNum[1], AppNum[2], AppNum[3], AppNumEnd, rand, Spare);

	m_Equip[0]->ClearAppendAttribute();

	ItemManager::Instance()->CreateItemAppend(m_Equip[0], AppNumEnd);

	float TlvF = (m_Equip[0]->BaseLevel + m_Equip[3]->BaseLevel + m_Equip[4]->BaseLevel + m_Equip[5]->BaseLevel + 1) * 1.0f / 4;
	int   TlvI = (m_Equip[0]->BaseLevel + m_Equip[3]->BaseLevel + m_Equip[4]->BaseLevel + m_Equip[5]->BaseLevel + 1) * 1.0f / 4;
	int   TlvSpare1 = (TlvF - TlvI) * 10;
	int   Tlvrand1 = sbase::RandGet(9);
	if (Tlvrand1 < TlvSpare1) { TlvI++; }

	m_Equip[0]->BaseLevel = TlvI;

	TlvF = (m_Equip[0]->AppLevel + m_Equip[3]->AppLevel + m_Equip[4]->AppLevel + m_Equip[5]->AppLevel + 1) * 1.0f / 4;
	TlvI = (m_Equip[0]->AppLevel + m_Equip[3]->AppLevel + m_Equip[4]->AppLevel + m_Equip[5]->AppLevel + 1) * 1.0f / 4;
	int   TlvSpare2 = (TlvF - TlvI) * 10;
	int   Tlvrand2 = sbase::RandGet(9);
	if (Tlvrand2 < TlvSpare2) { TlvI++; }

	m_Equip[0]->AppLevel = TlvI;

	if (m_Equip[0]->BaseLevel < 1) { m_Equip[0]->BaseLevel = 1; }
	else if (m_Equip[0]->BaseLevel > 5) { m_Equip[0]->BaseLevel = 5; }

	if (m_Equip[0]->AppLevel < 1) { m_Equip[0]->AppLevel = 1; }
	else if (m_Equip[0]->AppLevel > 10) { m_Equip[0]->AppLevel = 10; }


	msg_equipforge.BaseLv = m_Equip[0]->BaseLevel;
	msg_equipforge.AppLv = m_Equip[0]->AppLevel;

	_stprintf(Ifbuf, "BaseLv:%d AppLv:%d Tlvrand1:%d<TlvSpare1:%d Tlvrand2:%d<TlvSpare2:%d", m_Equip[0]->BaseLevel, m_Equip[0]->AppLevel, Tlvrand1, TlvSpare1, Tlvrand2, TlvSpare2);

	if (Binding > 0)
	{
		m_Equip[0]->m_Binding = true;
	}

	data = m_Equip[0]->GetItemData();
	m_Equip[0] = NULL;

	for (int i = 0; i < 6; i++)
	{
		if (m_Equip[i] != NULL)
		{
			m_Equip[i]->m_Overlap--;
			if (m_Equip[i]->m_Overlap == 0)
				m_Equip[i]->Clear();
			m_Equip[i] = NULL;
		}
	}

	msg_equipforge.AppId[0] = data.Append[0];
	msg_equipforge.AppId[1] = data.Append[1];
	msg_equipforge.AppId[2] = data.Append[2];
	msg_equipforge.AppId[3] = data.Append[3];
	msg_equipforge.AppId[4] = data.Append[4];
	msg_equipforge.Success = true;
	msg_equipforge.Type = 3;
	m_Player->s_World->SendMsgToClient(&msg_equipforge, m_Player->GetSocket());

	_stprintf(Ifbuf, "附加Id:%d,%d,%d,%d,%d", data.Append[0], data.Append[1], data.Append[2], data.Append[3], data.Append[4]);


}