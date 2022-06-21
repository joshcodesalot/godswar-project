#include "stdafx.h"
#include "Player.h"
#include "..\ServiceLoader.h"
#include "..\ErrorMessage.h"
#include ".\GameData.h"
#include "..\DataCore.h"
#include "SkillManager.h"
#include "../World.h"
#include "ItemManager.h"

#include ".\Mat.h"
#include <math.h>

extern MSG_WALK_END msg_walk_end;
extern MSG_DEAD msg_dead;

#define  LINK_MODULUS( X )          X##_modulus
#define  RUCTION_MACRO(Y,Z)		    if ( pSkill->Y )\
{                          \
	if (  TYPE_ADD == Type )\
{\
	m_ObjectData.Z +=  Rank  * ( pSkill->LINK_MODULUS(Y) );    \
}\
																			else if ( TYPE_UPDATE == Type )\
{\
	m_ObjectData.Z -=  ( Rank - 1 )  * ( pSkill->LINK_MODULUS(Y) );    \
	m_ObjectData.Z +=  Rank  * ( pSkill->LINK_MODULUS(Y) );    \
}\
																			else if ( TYPE_DELETE == Type )\
{\
	m_ObjectData.Z -=   Rank  * ( pSkill->LINK_MODULUS(Y)  );\
}\
}


void  CPlayer::UpdateAttributeInfo(UINT  SkillID, AttributeType Type)
{
	typedef void  (CGameObject::* PS_FUN)(SKillData*, AttributeType);
	if (!CGameObject::s_World->g_pSkillManager)
		return;
	const SKillData* pSkill = CGameObject::s_World->g_pSkillManager->GetSkill(SkillID);
	std::map<UINT, float>::iterator  itor = const_cast<SKillData*>(pSkill)->Effect.begin();
	for (; itor != pSkill->Effect.end(); itor++)
	{
		(this->*(CGameObject::s_World->g_pSkillManager->GetSkillEffectHandler(itor->first).Handler))(const_cast<SKillData*>(pSkill), Type);
	}
}

void CPlayer::BePhysicalAttackedPassivePracticeDegree(PhysiacalAttack, ATTACK_TYPE)
{

}



INT  CPlayer::CalculateAttackDamage(CGameObject& obj, float iPower)
{
	return CGameObject::CalculateAttackDamage(obj, iPower);
}


UINT CPlayer::GetHPRecover()
{
	return GetAlllResumeHP();
}

UINT CPlayer::GetMPRecover()
{
	return GetAlllResumeMP();
}

UINT CPlayer::GetNextGradeExp()
{
	return m_NextGradeExp;
}

void CPlayer::SetNextGradeEXP()
{
	m_NextGradeExp = CPeizhi::Instance()->GetPlayerNextGradeExp(GetRank());
}

void CPlayer::AddExp(int exp)
{
	if (GetRank() < PLAYER_MAXLV)
	{
		m_ObjectData.m_cExp += exp;

		while (m_ObjectData.m_cExp >= GetNextGradeExp() && GetRank() < PLAYER_MAXLV)
		{
			m_ObjectData.m_cExp -= GetNextGradeExp();
			m_ObjectData.m_cRank++;
			PlayerUpGrade(true);
		}
	}
}


void	CPlayer::CalculateExpAndLevel(CGameObject& pTarObj)
{
	if (pTarObj.GetlHP() > 0 || pTarObj.GetType() == OBJECTTYPE_PLAYER)
	{
		return;
	}

	float		teamLevelFactor = 1.0f;
	float		memLevelFactor = 1.0f;
	float		factor = 1.0f;
	int			memCount = 0;
	int			teamLevel = 0;
	int			avgteamLevel = 0;
	int			monExp = 0;


	if (NULL != m_pTeamLeader)
	{
		if (GetRegion()->GetMap()->IsTwoCellCross(GetCurrentCell(), m_pTeamLeader->GetCurrentCell()))
		{
			memCount++;
			teamLevel += m_pTeamLeader->GetRank();
		}

		for (vector<CPlayer*>::iterator iter = m_pTeamLeader->m_Teammates.begin();
			iter != m_pTeamLeader->m_Teammates.end();
			iter++)
		{
			if (GetRegion()->GetMap()->IsTwoCellCross(GetCurrentCell(), (*iter)->GetCurrentCell()))
			{
				memCount++;
				teamLevel += (*iter)->GetRank();
			}
		}
	}
	else
	{
		memCount = 1;
		teamLevel = GetRank();
	}

	monExp = pTarObj.GetEXP();
	factor = (memCount - 1) * 0.25f + 1.0f;
	avgteamLevel = teamLevel / memCount;

	teamLevelFactor = 1.0f + (float)(((float)pTarObj.GetRank() - (float)avgteamLevel) / 10.0f);
	if (teamLevelFactor < 0.0f)
		teamLevelFactor = 0.0f;
	if (teamLevelFactor > 1.0f)
		teamLevelFactor = 1.0f;

	memLevelFactor = ((float)m_ObjectData.m_cRank + (float)avgteamLevel) / ((float)teamLevel + (float)(avgteamLevel * memCount));
	if (GetRank() < PLAYER_MAXLV)
	{
		ULONG Exp = GetNextGradeExp();
		float TempEXP = monExp * teamLevelFactor * memLevelFactor * factor;
		m_ObjectData.m_cExp += TempEXP * (1 + m_StatusData.m_GetEXP);
		while (m_ObjectData.m_cExp >= Exp && GetRank() < PLAYER_MAXLV)
		{
			m_ObjectData.m_cRank++;
			m_ObjectData.m_cExp = TempEXP - (Exp - m_ObjectData.m_cExp);
			PlayerUpGrade(true);
			SetlHP(GetMaxHP());
			SetlMP(GetMaxMP());
			Exp = GetNextGradeExp();
		}
	}
	if (memCount > 1)
	{
		if (this != m_pTeamLeader)
		{
			if (GetRegion()->GetMap()->IsTwoCellCross(GetCurrentCell(), m_pTeamLeader->GetCurrentCell()) && m_pTeamLeader->GetRank() < PLAYER_MAXLV)
			{
				memLevelFactor = ((float)m_pTeamLeader->m_ObjectData.m_cRank + (float)avgteamLevel) / ((float)teamLevel + (float)avgteamLevel * (float)memCount);
				ULONG Exp = m_pTeamLeader->GetNextGradeExp();
				float TempEXP = monExp * teamLevelFactor * memLevelFactor * factor;
				m_pTeamLeader->m_ObjectData.m_cExp += TempEXP * (1 + m_pTeamLeader->GetGetEXP()); ;
				while (m_pTeamLeader->m_ObjectData.m_cExp >= Exp && m_pTeamLeader->GetRank() < PLAYER_MAXLV)
				{
					m_pTeamLeader->m_ObjectData.m_cRank++;
					m_pTeamLeader->m_ObjectData.m_cExp = TempEXP - (Exp - m_pTeamLeader->m_ObjectData.m_cExp);
					m_pTeamLeader->PlayerUpGrade(true);
					m_pTeamLeader->SetlHP(GetMaxHP());
					m_pTeamLeader->SetlMP(GetMaxMP());
					Exp = m_pTeamLeader->GetNextGradeExp();
				}
			}
		}

		for (vector<CPlayer*>::iterator iter = m_pTeamLeader->m_Teammates.begin();
			iter != m_pTeamLeader->m_Teammates.end();
			iter++)
		{
			if (this == (*iter))
				continue;
			if (GetRegion()->GetMap()->IsTwoCellCross(GetCurrentCell(), (*iter)->GetCurrentCell()))
			{
				memLevelFactor = ((float)(*iter)->m_ObjectData.m_cRank + (float)avgteamLevel) / ((float)teamLevel + (float)avgteamLevel * (float)memCount);
				ULONG Exp = (*iter)->GetNextGradeExp();
				float TempEXP = monExp * teamLevelFactor * memLevelFactor * factor;
				(*iter)->m_ObjectData.m_cExp += TempEXP * (1 + (*iter)->GetGetEXP());
				while ((*iter)->m_ObjectData.m_cExp >= Exp && (*iter)->GetRank() < PLAYER_MAXLV)
				{
					(*iter)->m_ObjectData.m_cRank++;
					(*iter)->m_ObjectData.m_cExp = TempEXP - (Exp - (*iter)->m_ObjectData.m_cExp);
					(*iter)->PlayerUpGrade(true);
					(*iter)->SetlHP(GetMaxHP());
					(*iter)->SetlMP(GetMaxMP());
					Exp = (*iter)->GetNextGradeExp();
				}
			}
		}
	}
}

void CPlayer::CalculatePrestigeExpLevel()
{
	ULONG Exp = GetNextGradeExp();
	while (GetRank() < PLAYER_MAXLV && m_ObjectData.m_cExp >= Exp)
	{
		m_ObjectData.m_cRank++;
		m_ObjectData.m_cExp -= Exp;
		PlayerUpGrade(true);
		SetlHP(GetMaxHP());
		SetlMP(GetMaxMP());
		Exp = GetNextGradeExp();
	}
}
void	CPlayer::CalculateMoney(CGameObject*)
{
}

void CPlayer::PlayerUpGrade(bool IsUpGrade)
{
	if (IsUpGrade && GetRank() <= PLAYER_MAXLV)
	{
		m_BaseData.m_lMaxHP = CPeizhi::Instance()->GetPlayerMaxHP(GetRank(), GetlClass());
		m_BaseData.m_lMaxMP = CPeizhi::Instance()->GetPlayerMaxMP(GetRank(), GetlClass());
		m_BaseData.m_lMaxHP = m_ObjectData.m_lHP = GetMaxHP();
		m_BaseData.m_lMaxHP = m_ObjectData.m_lMP = GetMaxMP();

		this->SetAtttibute();

		SynGameData(true);

		MSG_UPGRADE  UpGrade;
		UpGrade.Head.usSize = sizeof(MSG_UPGRADE);
		UpGrade.Head.usType = _MSG_UPGRADE;
		UpGrade.ID = GetID();
		UpGrade.Rank = GetRank();
		UpGrade.CurEXP = GetEXP();
		UpGrade.MaxEXP = CPeizhi::Instance()->GetPlayerNextGradeExp(GetRank());
		UpGrade.MAXHP = GetMaxHP();
		UpGrade.CURHP = GetlHP();
		UpGrade.MAXMP = GetMaxMP();
		UpGrade.CURMP = GetlMP();

		GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &UpGrade);


		SetNextGradeEXP();

		LevelUpdata();
	}



}

UINT CPlayer::CalculateAttack()
{
	return __super::CalculateAttack();
}

UINT CPlayer::CalculateMagic()
{
	if (m_PassiveSkill.size() > 0)
	{
		map< UINT, PASSIVEINFO >::iterator itor = m_PassiveSkill.begin();
		for (; itor != m_PassiveSkill.end(); itor++)
		{
		}
	}
	return  0;
}

AttackRadius CPlayer::GetAttackRadius()
{
	if (m_Equips[EquipMainHand1].IsClear())
		return Melee;

	return m_Equips[EquipMainHand1].GetItemBaseAttribute()->eAttackRadius;
}


HandType CPlayer::GetHandType()
{
	int main = EquipMainHand1;
	int Auxiliary = EquipAuxiliaryHand1;

	if (m_Equips[main].IsClear())
		return HTNULL;
	else
	{
		if (m_Equips[Auxiliary].IsClear())
		{
			if (m_Equips[main].GetItemBaseAttribute()->eEquipHand == ItemBaseAttribute::TwoHand)
				return HTTwo;
			else
				return HTOne;
		}
		else
		{
			if (m_Equips[Auxiliary].GetItemBaseAttribute()->EType == ItemBaseAttribute::Weapon)
				return HTDoubule;
			else
				return HTOne;
		}
	}
}


ULONG CPlayer::GetHit()
{
	return  GetAlllHit();
}

ULONG CPlayer::GetDodge()
{
	return  GetAlllDodge();
}

eError CPlayer::Revive(int type)
{
	if (m_ObjectData.m_lHP > 0 || m_ObjectData.m_cRank < 1 || m_ObjectData.m_cRank > 200)
	{
		return NO_MSG_ERRO;
	}

	switch (type)
	{
	case eREVIVE_STONE:
	case eREVIVE_MONEY:
	{
		int consume = ItemManager::Instance()->GetReviveConsum(m_ObjectData.m_cRank, type);
		if (consume == -1)
		{
			ASSERT(consume == -1);
			return NO_MSG_ERRO;
		}

		if (type == eREVIVE_STONE && m_ObjectData.m_lStone >= consume)
		{
			m_ObjectData.m_lStone -= consume;
			m_ObjectData.m_lHP = GetMaxHP();
			m_ObjectData.m_lMP = GetMaxMP();
		}
		else if (type == eREVIVE_MONEY && m_ObjectData.m_lMoney >= consume)
		{
			m_ObjectData.m_lMoney -= consume;
			m_ObjectData.m_lHP = GetMaxHP() / 3;
			m_ObjectData.m_lMP = GetMaxMP() / 3;
		}
		else
		{
			return NO_MSG_ERRO;
		}

		GetRegion()->RemoveObject(GetID());
		tarSceneChange* pSceneChange = new tarSceneChange;
		pSceneChange->pPlayer = this;
		pSceneChange->lFromSceneID = this->GetRegion()->GetID();
		pSceneChange->lToSceneID = this->GetRegion()->GetID();
		pSceneChange->x = this->m_ObjectData.m_fX;
		pSceneChange->y = 0.0f;
		pSceneChange->z = this->m_ObjectData.m_fZ;

		CGameObject::s_World->g_listSceneChange.push_back(pSceneChange);

		this->ClearPath();

	}
	break;
	default:
	{
		GetRegion()->RemoveObject(GetID());
		tarSceneChange* pSceneChange = new tarSceneChange;
		pSceneChange->pPlayer = this;
		pSceneChange->lFromSceneID = this->GetRegion()->GetID();
		if (m_ObjectData.m_lFaction == 1)
		{
			pSceneChange->lToSceneID = CGameObject::s_World->GetRegionFromLogicID(GetRegion()->GetAthensRevival()->MapID)->GetID();
			pSceneChange->x = GetRegion()->GetAthensRevival()->X;
			pSceneChange->y = 0.0f;
			pSceneChange->z = GetRegion()->GetAthensRevival()->Z;
		}
		else if (m_ObjectData.m_lFaction == 0)
		{
			pSceneChange->lToSceneID = CGameObject::s_World->GetRegionFromLogicID(GetRegion()->GetSpartaRevival()->MapID)->GetID();
			pSceneChange->x = GetRegion()->GetSpartaRevival()->X;
			pSceneChange->y = 0.0f;
			pSceneChange->z = GetRegion()->GetSpartaRevival()->Z;
		}

		CGameObject::s_World->g_listSceneChange.push_back(pSceneChange);

		this->ClearPath();

		m_ObjectData.m_lHP = GetMaxHP() / 10;
		m_ObjectData.m_lMP = GetMaxMP() / 10;
	}
	break;
	}

	return NO_MSG_ERRO;
}

void CPlayer::Fly(int id, float x, float y, float z)
{
	int RandFlag = 0;
	if (id < 0)
	{
		id = this->GetRegion()->GetID();
		RandFlag = 1;
	}

	CRegion* pRegion = CGameObject::s_World->GetRegion(id);
	if (NULL == pRegion)
	{
		return;
	}

	tarSceneChange* pSceneChange = new tarSceneChange;
	pSceneChange->pPlayer = this;
	pSceneChange->lFromSceneID = this->GetRegion()->GetID();
	pSceneChange->lToSceneID = id;
	pSceneChange->x = x;
	pSceneChange->y = y;
	pSceneChange->z = z;

	this->ClearPath();

	if (RandFlag > 0)
	{
		while (1)
		{
			float angle = sbase::RandGet(360);
			pSceneChange->x = this->GetPosX() + cos(angle) * 50;
			pSceneChange->z = this->GetPosZ() + sin(angle) * 50;

			if (pSceneChange->x<256 && pSceneChange->x>-256 && pSceneChange->z<256 && pSceneChange->z>-256)
			{
				if (!this->CheckBlock(this->GetRegion()->GetID(), pSceneChange->x, pSceneChange->z))
				{
					break;
				}
			}
		}
	}

	CGameObject::s_World->g_listSceneChange.push_back(pSceneChange);

}

void CPlayer::Dead(CGameObject* pObj)
{
	GetRegion()->EndFight(this);

	if (m_Trade.IsTrading())
	{
		this->GetTrade()->GetTrader()->GetTrade()->EndTrade();
		this->GetTrade()->EndTrade();

		this->GetTrade()->GetTrader()->GetTrade()->ClearTrade();
		this->GetTrade()->ClearTrade();
	}

	msg_walk_end.X = this->GetPosX();
	msg_walk_end.Y = 0.0f;
	msg_walk_end.Z = this->GetPosZ();
	msg_walk_end.uiTick = 1;
	msg_walk_end.fAtan2 = this->m_fatan2;
	msg_walk_end.uiID = this->GetID();
	msg_walk_end.usMapID = GetRegion()->GetID();
	msg_walk_end.Head.usSize = sizeof(msg_walk_end);
	GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg_walk_end);

	GetStall()->SetState(false);

	msg_dead.uiID = this->GetID();
	for (int a = 0; a < 5; a++)
	{
		msg_dead.playerID[a] = -1;
		msg_dead.playerMoney[a] = 0;
		msg_dead.Exp[a] = 0;
	}

	if (pObj)
	{
		int ExpandPre = 0;
		if (pObj->GetType() == OBJECTTYPE_PLAYER && pObj->GetlFaction() != GetlFaction())
		{
			int Dis = pObj->GetRank() - GetRank();
			if ((20 - Dis) > 0)  ExpandPre = 20 - Dis;
			pObj->SetlPrestige(pObj->GetlPrestige() + ExpandPre);
		}

		msg_dead.KillerID = pObj->GetID();
		msg_dead.KillerPre = ExpandPre;
	}
	else
	{
		msg_dead.KillerID = 0;
		msg_dead.KillerPre = 0;
	}


	GetRegion()->SendAreaMsgOneToOther(this->GetCurrentCell(), &msg_dead);

	m_eState = OBJECT_DEAD;

}

void CPlayer::ChangeHp()
{
	if (m_ObjectData.m_lHP > GetMaxHP())
		m_ObjectData.m_lHP = GetMaxHP();

	if (m_ObjectData.m_lMP > GetMaxMP())
		m_ObjectData.m_lMP = GetMaxMP();
}

void CPlayer::ChangeAttackSpeed()
{
	int main = EquipMainHand1;
	int Auxiliary = EquipAuxiliaryHand1;

	if (GetHandType() == ::HTDoubule)
	{
		int attack1 = m_Equips[main].GetItemBaseAttribute()->Attack[m_Equips[main].BaseLevel - 1];
		float appendattack1 = 0.0;
		int attack2 = m_Equips[Auxiliary].GetItemBaseAttribute()->Attack[m_Equips[Auxiliary].BaseLevel - 1];
		float appendattack2 = 0.0;

		for (int i = 0; i < MAX_EQUIPAPPEND_COUNT; i++)
		{
			if (!m_Equips[main].GetItemAppendAttribute(i))
				continue;

			float value = m_Equips[main].GetItemAppendAttribute(i)->LvInfo[m_Equips[main].AppLevel - 1];

			if (m_Equips[main].GetItemAppendAttribute(i)->Type == ItemAppendAttribute::AttackA)
				attack1 += (int)value;

		}

		for (int i = 0; i < MAX_EQUIPAPPEND_COUNT; i++)
		{
			if (!m_Equips[Auxiliary].GetItemAppendAttribute(i))
				continue;

			float value = m_Equips[Auxiliary].GetItemAppendAttribute(i)->LvInfo[m_Equips[Auxiliary].AppLevel - 1];

			if (m_Equips[Auxiliary].GetItemAppendAttribute(i)->Type == ItemAppendAttribute::AttackA)
				attack2 += (int)value;

		}


		m_EquipData.m_AttackSpeed = m_Equips[main].GetItemBaseAttribute()->AttackSpeed[0] * 0.7 * 1000;

		m_timeAttack.SetInterval(m_EquipData.m_AttackSpeed);
	}
	else
	{
		if (!m_Equips[main].IsClear())
		{
			if (!m_Equips[main].GetItemBaseAttribute() || !m_Equips[main].BaseLevel)
			{
				ASSERT(0);
			}

			int attack = m_Equips[main].GetItemBaseAttribute()->Attack[m_Equips[main].BaseLevel - 1];
			float appendattack = 0.0;

			for (int i = 0; i < MAX_EQUIPAPPEND_COUNT; i++)
			{
				if (!m_Equips[main].GetItemAppendAttribute(i))
					continue;

				float value = m_Equips[main].GetItemAppendAttribute(i)->LvInfo[m_Equips[main].AppLevel - 1];

				if (m_Equips[main].GetItemAppendAttribute(i)->Type == ItemAppendAttribute::AttackA)
					attack += (int)value;
			}


			m_EquipData.m_AttackSpeed = m_Equips[main].GetItemBaseAttribute()->AttackSpeed[0] * 1000;

			m_timeAttack.SetInterval(m_EquipData.m_AttackSpeed);
		}
		else if (!m_Equips[Auxiliary].IsClear())
		{
			int attack = m_Equips[Auxiliary].GetItemBaseAttribute()->Attack[m_Equips[Auxiliary].BaseLevel - 1];
			float appendattack = 0.0;

			for (int i = 0; i < MAX_EQUIPAPPEND_COUNT; i++)
			{
				if (!m_Equips[Auxiliary].GetItemAppendAttribute(i))
					continue;

				float value = m_Equips[Auxiliary].GetItemAppendAttribute(i)->LvInfo[m_Equips[Auxiliary].AppLevel - 1];

				if (m_Equips[Auxiliary].GetItemAppendAttribute(i)->Type == ItemAppendAttribute::AttackA)
					attack += (int)value;

			}

			m_EquipData.m_AttackSpeed = 1500;
			m_timeAttack.SetInterval(m_EquipData.m_AttackSpeed);
		}
		else
		{
			m_EquipData.m_AttackSpeed = 1500;
			m_timeAttack.SetInterval(m_EquipData.m_AttackSpeed);
		}

	}

	m_timeAttack.Update();
}

