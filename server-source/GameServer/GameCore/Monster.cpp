#include "StdAfx.h"
#include "Monster.h"
#include "Player.h"
#include "Region.h"
#include "Obstacle.h"
#include "ItemManager.h"
#include "..\ServiceLoader.h"
#include "..\DataCore.h"
#include "SkillManager.h"
#include ".\Mat.h"
#include "ObjMgr.h"
#include "../World.h"

#include "..\ServiceLoader.h"


#include "..\..\Common\MsgDefine.h"
#include "..\..\Common\MsgTypes.h"

extern MSG_WALK_BEGIN msg_walk_begin;
extern MSG_WALK_END msg_walk_end;
extern MSG_WALK msg_walk;
extern MSG_OBJECTINFO msg;
extern MSG_ATTACK msg_attack;
extern MSG_MAGIC_DAMAGE msg_magic;
extern MSG_DEAD msg_dead;
extern MSG_DROPS msg_drops;
extern MSG_LEAVE msg_leave;

extern MSG_SYN_GAMEDATA  msg_syn_gamedata;


PARAM  CMonster::Param;
ATTACK_PARAM  CMonster::AttackParam;

CMonster::CMonster(void)
	: m_Drops(0), m_Level(0),
	m_Kidney(MONSTER_MODE_VALOROUS),
	m_Escaped(false), m_Escaping(false),
	m_SkillCount(0), m_SkillRandom(0),
	m_QuestID(-1), m_WalkTime(0)
{
	::ZeroMemory(m_SkillInfos, sizeof(SkillInfo) * 4);
	m_ObjectData.m_lFaction = MONSTER_FACTION;

	m_eType = OBJECTTYPE_MONSTER;

	m_lAIType = AI_TYPE_PASSIVITY;
	m_lRefreshTime = REFRESH_TIME;

	m_lState = MONSTER_STATE_LIVE;
	m_lActivity = MONSTER_ACTIVITY_IDLE;
	m_fOffsetX = 0.0f;
	m_fOffsetZ = 0.0f;

	m_fFightX = 0.0f;
	m_fFightX = 0.0f;
	m_fAngle = 0.0f;

	m_fIniX = m_fIniY = m_fIniZ = 0.0f;

	m_timeFight.SetInterval(FIGHT_TIME);
	m_timeAttack.SetInterval(FIGHT_INTERVAL);
	m_timerAI.SetInterval(MONSTER_AI_TIME);

	for (int i = 0; i < MAX_OWNER; i++)
	{
		m_ItemOwner[i] = NULL;
		m_ItemOwnerName[i] = "";
	}
}

CMonster::~CMonster(void)
{
}

bool CMonster::IsOwner(const char* name)
{
	for (int i = 0; i < MAX_OWNER; i++)
	{
		if (strcmp(name, m_ItemOwnerName[i].c_str()) == 0)
			return true;
	}

	return false;
}

void CMonster::AddEnmity(CGameObject* pObject, long lValue)
{
	if (m_listEnmity.empty() || !m_ItemOwner[0])
	{
		m_ItemOwner[0] = pObject;
		m_ItemOwnerName[0] = pObject->GetName();

		if (((CPlayer*)m_ItemOwner[0])->m_pTeamLeader)
		{
			m_ItemOwner[0] = ((CPlayer*)m_ItemOwner[0])->m_pTeamLeader;
			m_ItemOwnerName[0] = ((CPlayer*)m_ItemOwner[0])->m_pTeamLeader->GetName();
			int i = 1;
			for (vector<CPlayer*>::iterator iter = ((CPlayer*)m_ItemOwner[0])->m_pTeamLeader->m_Teammates.begin();
				iter != ((CPlayer*)m_ItemOwner[0])->m_pTeamLeader->m_Teammates.end();
				iter++)
			{
				m_ItemOwner[i] = (*iter);
				m_ItemOwnerName[i] = (*iter)->GetName();
				i++;
				if (i >= MAX_OWNER)
				{
					break;
				}
			}
		}

	}

	CGameObject::AddEnmity(pObject, lValue);
}

void CMonster::RemoveEnmity(CGameObject* pObject)
{
	CGameObject::RemoveEnmity(pObject);

	for (int i = 0; i < MAX_OWNER; i++)
	{
		if (m_ItemOwner[i] == pObject)
		{
			m_ItemOwner[i] = NULL;
			m_ItemOwnerName[i] = "";
		}
	}
}

void CMonster::Activate(void)
{
	CGameObject::Activate();
}

void CMonster::ResetTimer(void)
{
	m_Timer.Startup(m_lRefreshTime);
	m_lState = MONSTER_STATE_LOST;
}
bool CMonster::IsRefresh(void)
{
	return m_Timer.IsExpire();
}

void CMonster::ResetCorpseTimer(void)
{
	if (m_Drops)
		m_CorpseTimer.Startup(CORPSE_ITEM_TIME);
	else
		m_CorpseTimer.Startup(CORPSE_LOST_TIME);

	m_lState = MONSTER_STATE_DEAD;
}

bool CMonster::IsCorpseTimer(void)
{

	return m_CorpseTimer.IsExpire();
}


void CMonster::Refresh(void)
{

	if (m_StatusMap.size() > 0)
	{
		MSG_STATUS msg_status;
		msg_status.Head.usSize = sizeof(MSG_STATUS);
		msg_status.Head.usType = _MSG_STATUS;
		msg_status.uiCount = 0;
		msg_status.uiID = GetID();
		memcpy(&msg_status.Data, &m_StatusData, sizeof(StatusData));
		GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg_status);

		memset(&m_StatusData, 0L, sizeof(m_StatusData));
		m_StatusMap.clear();
		m_StatusSelfData.clear();
	}

	m_Escaped = false;
	m_Escaping = false;
	for (int i = 0; i < MAX_DROPS; i++)
		m_DropItems[i].Clear();

	m_Drops = 0;

	for (int i = 0; i < MAX_OWNER; i++)
	{
		m_ItemOwner[i] = NULL;
		m_ItemOwnerName[i] = "";
	}

	m_ObjectData.m_lHP = GetBaselMaxHP();
	m_lState = MONSTER_STATE_LIVE;
	m_lActivity = MONSTER_ACTIVITY_IDLE;
	m_eState = OBJECT_IDLE;
	m_fDest = m_fDist = m_fatan2 = 0.0f;

	SetPos(m_fFightX, 0.0f, m_fFightZ);
	m_fOffsetX = 0.0f;
	m_fOffsetZ = 0.0f;
	m_nRunTick = 0;

	m_listEnmity.clear();

	m_timerAI.Update();


	msg_leave.uiID = GetID();
	msg_leave.Head.usSize = sizeof(msg_leave);
	m_pRegion->SendAreaMsgOneToOther(GetCurrentCell(), &msg_leave);


	this->m_fatan2 = this->m_fAngle;

	msg.Head.usSize = sizeof(msg);
	msg.uiID = GetID();
	msg.uiType = _OBJECT_TYPE_MONSTER;
	msg.X = GetPosX();
	msg.Y = GetPosY();
	msg.Z = GetPosZ();
	msg.fAtan2 = GetAtan2();
	msg.uiHP = GetlHP();
	msg.uiMaxHP = GetBaselMaxHP();
	msg.ucLV = GetRank();
	msg.usMapID = GetRegion()->GetID();
	msg.ucCamp = GetlFaction();

	sprintf(msg.cName, "%s", GetName());
	m_pRegion->SendAreaMsgOneToOther(GetCurrentCell(), &msg);
}


void CMonster::LeaveFight(void)
{
	GetRegion()->EndFight(this);

	SetTarget(NULL);
}

bool CMonster::DoSomething()
{
	switch (m_Kidney)
	{
	case MONSTER_MODE_CRAVEN:
	{
		if (GetlHP() <= GetBaselMaxHP() * 0.5)
		{
			CGameObject* obj = GetRegion()->GetNearestPartner(this);

			if (!m_Escaping && !m_Escaped)
			{
				if (sbase::RandGet(10) != 0)
					return false;
			}

			if (!obj || m_Escaped)
				return false;

			if (FollowTo(obj->GetPosX(), 0.0f, obj->GetPosZ()))
			{
				m_Escaping = true;

				return true;
			}
			else
			{
				if (m_eState == OBJECT_RUN)
				{
					StopWalk();

					m_Escaped = true;
					m_Escaping = false;

					obj->ChainEnmityList(this, 0);
					GetRegion()->StartFight(obj, m_targetObj);

					return true;
				}

			}

		}
	}
	break;
	case MONSTER_MODE_VALOROUS:
	{
	}
	break;
	case MONSTER_MODE_DEEP:
	{
	}
	break;
	}

	return false;
}

bool CMonster::IsSkillCooling(int index)
{
	DWORD elspsed = timeGetTime() - m_SkillInfos[index].TimePos;
	const MagicData* magicData = CGameObject::s_World->g_pSkillManager->GetMagic(m_SkillInfos[index].SkillID);

	return (elspsed < magicData->usCoolingTime * 1000);
}

void CMonster::Cooldown(int index)
{
	m_SkillInfos[index].TimePos = timeGetTime();
}

void CMonster::UseSkill(int index)
{
	int skillID = m_SkillInfos[index].SkillID;
	Cooldown(index);

	const MagicData* magicData = CGameObject::s_World->g_pSkillManager->GetMagic(skillID);

	UINT targetFlag = magicData->ucTarget;
	bool bTarSelf = (targetFlag == TARGET_SELF);

	MSG_MAGIC_PERFORM mgicPerform;
	mgicPerform.Head.usSize = sizeof(MSG_MAGIC_PERFORM);
	mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
	mgicPerform.uiID = GetID();
	mgicPerform.x = m_fX;
	mgicPerform.z = m_fZ;
	mgicPerform.uiObjectID = bTarSelf ? GetID() : GetTarget()->GetID();
	mgicPerform.ucMagicID = skillID;
	GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &mgicPerform);

	if (magicData->ucRange != 0)
	{
		if (GetTarget() != NULL)
		{
			float	   center[3];
			center[0] = bTarSelf ? m_fX : GetTarget()->GetPosX();
			center[1] = 0;
			center[2] = bTarSelf ? m_fZ : GetTarget()->GetPosZ();
			if (magicData->iEffectTurn > 1)
			{
				GetRegion()->AddMagicRegion(GetID(), skillID, center, magicData->ucRange,
					magicData->ucAffectObj);
			}

			ObjectList objlst;
			GetRegion()->CollectGameObjectSphere(this, &objlst, center, magicData);

			size_t count = objlst.size();

			int size = int(sizeof(MsgHead) + sizeof(int) + 2 * sizeof(UINT) + sizeof(BYTE) + count * sizeof(MAGIC_DAMAGE));
			MSG_MAGIC_CLUSTER_DAMAGE  tmpDamage;
			MSG_MAGIC_CLUSTER_DAMAGE* clstDamager = &tmpDamage;

			clstDamager->iCount = int(count);
			clstDamager->uiID = GetID();
			clstDamager->Head.usSize = size;
			clstDamager->Head.usType = _MSG_MAGIC_CLUSTER_DAMAGE;
			clstDamager->ucMagicID = magicData->ucID;
			::ZeroMemory(clstDamager->magicDamage, sizeof(clstDamager->magicDamage));

			CGameObject* pTarget = NULL;
			int  i = 0;
			for (ObjectIterator Iter = objlst.begin(); Iter != objlst.end(); Iter++)
			{
				MAGIC_DAMAGE* mgicDamage = &clstDamager->magicDamage[i];
				++i;

				pTarget = (CGameObject*)*Iter;

				if (pTarget->GetID() == GetTarget()->GetID())
				{
					clstDamager->iCount -= 1;
					--i;
					continue;
				}

				mgicDamage->uiObjectID = pTarget->GetID();

				mgicDamage->iPower = CalculateAttackDamage(m_SkillInfos[index].SkillID);
				mgicDamage->ucType = 0;
				mgicDamage->Attack_Type = 1;

				pTarget->AddHP(-mgicDamage->iPower);

				if (pTarget->GetlHP() <= 0)
				{
					pTarget->Dead(this);
				}
			}
			if (clstDamager->iCount >= 1)
				GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), clstDamager);
		}
	}
	else {

	}
}

void CMonster::AI(void)
{
	if (GetState() == MONSTER_STATE_DEAD)
	{
		if (IsCorpseTimer())
		{
			ResetTimer();
			msg_leave.uiID = GetID();
			msg_leave.Head.usSize = sizeof(msg_leave);

			GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg_leave);
		}

		return;

	}
	else if (GetState() == MONSTER_STATE_LOST)
	{
		if (IsRefresh())
			Refresh();

		return;
	}


	if (m_lState == MONSTER_STATE_LEAVEFIGHT)
	{
		if (FollowTo(m_fFightX, 0.0f, m_fFightZ))
		{
		}
		else
		{
			Refresh();
		}

	}

	if (m_bIsFight)
	{
		__super::AI();

		if (IsDead())
		{
			m_lState = MONSTER_STATE_DEAD;
			Dead(NULL);
			return;
		}

		if (GetDist(m_fFightX, m_fFightZ) > m_fActivityRange&& m_timeFight.IsExpire())
		{
			m_lState = MONSTER_STATE_LEAVEFIGHT;

			LeaveFight();
		}

		m_targetObj = GetMaxEnmity();
		int rand = RandomSkill();

		const MagicData* magicData = CGameObject::s_World->g_pSkillManager->GetMagic(m_SkillInfos[rand].SkillID);
		if (GetTarget() != NULL)
		{
			if (DoSomething())
			{
				m_timeAttack.Update();
			}
			else if (FollowTo(GetTarget()->GetPosX(), 0.0f, GetTarget()->GetPosZ(), magicData->ucDistance))
			{
			}
			else
			{
				if (m_eState == OBJECT_RUN)
				{
					StopWalk();
				}
				else if (IsAttack())
				{

					ATTACK_TYPE  attack_type = this->JudgeAttackStyle(m_targetObj);
					msg_attack.ucAttackType = attack_type;

					UseSkill(rand);

					switch (attack_type)
					{
					case ATTACK_FORCE:
					case ATTACK_HIT:
					{
						const MagicData* magicData = CGameObject::s_World->g_pSkillManager->GetMagic(m_SkillInfos[rand].SkillID);
						std::vector<int>::const_iterator iter = magicData->m_Status.begin();
						for (; iter != magicData->m_Status.end(); iter++)
						{
							if ((*iter) < 0)
							{
								m_targetObj->DelStatus(CGameObject::s_World->g_pStatusManager->GetStatus((-1) * (*iter)));
							}
							else
							{
								m_targetObj->AddStatus(CGameObject::s_World->g_pStatusManager->GetStatus(*iter));
							}

						}

						SetRandMagicAttack(m_SkillInfos[rand].Attack);
						int damage = this->CalculateAttackDamage(m_SkillInfos[rand].SkillID);
						if (damage <= 0)
							damage = 1;
						else
						{
							if (OBJECT_CAST == m_targetObj->GetState() && m_targetObj->GetHaltIntonate() > 0.0f)
							{
								m_targetObj->ResetCastState();
							}

							int HPResume = int(damage * m_targetObj->GetReboundDamage() + (int)damage * m_targetObj->GetReturnDamage());
							AddHP(HPResume);

							m_targetObj->SetHaltIntonate(0.00f);
							m_targetObj->SetReboundDamage(0.00f);
						}

						msg_attack.uiID = GetID();
						msg_attack.uiObjectID = GetTarget()->GetID();
						msg_attack.X = m_fX;
						msg_attack.Y = m_fY;
						msg_attack.Z = m_fZ;
						msg_attack.ucAttackType = ATTACK_HIT;
						msg_attack.uiDamage = damage;


						m_targetObj->AddHP(-damage);

						GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg_attack);

						if (m_targetObj->GetlHP() <= 0)
						{
							m_targetObj->Dead(this);
						}

						m_timeAttack.Update();
					}

					break;
					case ATTACK_MISS:
					{
						msg_attack.uiID = GetID();
						msg_attack.uiObjectID = GetTarget()->GetID();
						msg_attack.ucAttackType = ATTACK_MISS;
						msg_attack.X = m_fX;
						msg_attack.Y = m_fY;
						msg_attack.Z = m_fZ;
						msg_attack.uiDamage = -1;

						if (m_targetObj->GetlHP() <= 0)
						{
							m_targetObj->Dead(this);
						}

						GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg_attack);
						m_timeAttack.Update();

					}
					break;
					}


				}

			}
		}
		else
		{
			m_lState = MONSTER_STATE_LEAVEFIGHT;

			LeaveFight();
		}
	}
	else
	{
		if (m_WalkTime == 0)
			return;


		if (m_timerAI.IsExpire())
		{
			switch (m_lActivity)
			{
			case MONSTER_ACTIVITY_IDLE:
			{
				m_lActivity = MONSTER_ACTIVITY_WALK;

				float rand_x = 0.0f;
				float rand_z = 0.0f;
				float top_x, top_y, bottom_x, bottom_y;
				top_x = top_y = bottom_x = bottom_y = 0;

				CCell* pCell = this->GetCurrentCell();
				pCell->GetPointWithThis(top_x, top_y, bottom_x, bottom_y);

				rand_x = top_x + (float)(sbase::RandGet(15, false)) + 0.1f;
				rand_z = top_y + (float)(sbase::RandGet(15, false)) + 0.1f;

				if (rand_x < (m_fIniX - 5.0f))
					rand_x = m_fIniX - (float)(sbase::RandGet(5, false));
				else if (rand_x > (m_fIniX + 5.0f))
					rand_x = m_fIniX + (float)(sbase::RandGet(5, false));

				if (rand_z < (m_fIniZ - 5.0f))
					rand_z = m_fIniZ - (float)(sbase::RandGet(5, false));
				else if (rand_z > (m_fIniZ + 5.0f))
					rand_z = m_fIniZ + (float)(sbase::RandGet(5, false));
				WalkTo(rand_x, 0.0f, rand_z);
			}
			break;
			case MONSTER_ACTIVITY_WALK:
			{
				m_lActivity = MONSTER_ACTIVITY_IDLE;
				WalkTo(m_fIniX, 0.0f, m_fIniZ);
			}
			break;
			default:
				break;
			}

			m_timerAI.Update();
		}


	}


}


void CMonster::Run()
{
	if (IsDead())
		return;

	switch (m_eState)
	{
	case OBJECT_RUN:
	{
		SetPos(GetPosX() + GetOffsetX(), 0, GetPosZ() + GetOffsetZ());
		m_nRunTick++;

		m_fDist -= m_ObjectData.m_fSpeed;

		msg_walk.uiID = GetID();
		msg_walk.usMapID = GetRegion()->GetID();
		msg_walk.fatan = m_fatan2;
		msg_walk.x = GetPosX();
		msg_walk.z = GetPosZ();
		msg_walk.lState = OBJECT_RUN;

		if (m_fDist <= 0.0f)
		{
			StopWalk();
			msg_walk.lState = OBJECT_IDLE;
			m_timerAI.Update();
		}

	}
	break;
	default:
		break;
	}


}

bool CMonster::InitObjData(long)
{
	m_DropItemMgr.Init(m_ConfirmConfig.ItemTable);
	memset(&m_BaseData, 0L, sizeof(m_EquipData));
	memset(&m_Specialty, 0L, sizeof(m_StatusData));
	memset(&m_StatusData, 0L, sizeof(m_StatusData));
	memset(&m_EquipData, 0L, sizeof(m_EquipData));
	memset(&m_AltarData, 0L, sizeof(m_AltarData));

	m_fFightX = m_fIniX;
	m_fFightZ = m_fIniZ;

	m_BaseData.m_lMaxHP = CPeizhi::Instance()->GetMonsterMaxHP(m_Level) * Param.HP;
	m_BaseData.m_lMaxMP = 0;
	m_BaseData.m_lResumeHP = 0;
	m_BaseData.m_lResumeMP = 0;
	m_BaseData.m_lAttack = CPeizhi::Instance()->GetMonsterAttack(m_Level);
	m_BaseData.m_lDefend = CPeizhi::Instance()->GetMonsterPhyDefend(m_Level) * Param.PhyDefend;
	m_BaseData.m_lMagicAttack = 0;
	m_BaseData.m_lMagicDefend = CPeizhi::Instance()->GetMonsterMagicDefend(m_Level) * Param.MagicDefend;
	m_BaseData.m_lHit = CPeizhi::Instance()->GetMonsterHit(m_Level) + Param.Hit;
	m_BaseData.m_lDodge = CPeizhi::Instance()->GetMonsterMiss(m_Level) + Param.Miss;
	m_BaseData.m_lCritAppend = Param.FrenzyHit;
	m_BaseData.m_lCritDefend = Param.FrenzyMiss;
	m_BaseData.m_fPhyDamageAppend = Param.PhyDamage;
	m_BaseData.m_fMagicDamageAppend = Param.MagicDamage;
	m_BaseData.m_lDamageSorb = Param.DamageSorb * CPeizhi::Instance()->GetMonsterDamageSorb(m_Level);
	m_BaseData.m_fBeCure = 0;
	m_BaseData.m_fCure = 0;
	m_BaseData.m_lStatusHit = CPeizhi::Instance()->GetMonsterStatusHit(m_Level) + Param.StatusHit;
	m_BaseData.m_lStatudDodge = CPeizhi::Instance()->GetMonsterStatusMiss(m_Level) + Param.statusMiss;
	m_ObjectData.m_cExp = CPeizhi::Instance()->GetMonsterEXP(m_Level) * Param.Exp;
	m_ObjectData.m_lHP = m_BaseData.m_lMaxHP;
	m_ObjectData.m_lMP = m_BaseData.m_lMaxMP;

	Refresh();

	for (int i = 0; i < MAX_SKILLS; i++)
	{
		const MagicData* pActiveSkill = CGameObject::s_World->g_pSkillManager->GetMagic(m_SkillInfos[i].SkillID);

		if (!pActiveSkill)
			continue;

		m_SkillInfos[i].Attack = CPeizhi::Instance()->GetMonsterAttack(m_Level) * m_SkillInfos[i].SkillCoe;

		m_SkillCount++;
		m_SkillRandom += m_SkillInfos[i].SkillPro;

		AddActiveSkill(m_SkillInfos[i].SkillID);
	}

	return true;
}


bool CMonster::StopWalk(void)
{
	if (m_eState == OBJECT_RUN)
	{
		m_eState = OBJECT_IDLE;
		msg_walk_end.X = GetPosX();
		msg_walk_end.Y = 0.0f;
		msg_walk_end.Z = GetPosZ();
		msg_walk_end.uiTick = m_nRunTick;
		msg_walk_end.fAtan2 = m_fatan2;
		msg_walk_end.uiID = GetID();
		msg_walk_end.usMapID = (USHORT)GetRegion()->GetID();
		GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg_walk_end);

		m_nRunTick = 0;
		return true;
	}

	return false;
}


bool CMonster::WalkTo(float x, float, float z)
{
	if (x == GetPosX() && z == GetPosZ())
		return false;

	float fPX = 0.0f;
	float fPZ = 0.0f;

	m_fDestinationX = x;
	m_fDestinationY = 0.0f;
	m_fDestinationZ = z;

	fPX = x - GetPosX();
	fPZ = z - GetPosZ();

	m_fDist = sqrt(fPX * fPX + fPZ * fPZ);


	m_fOffsetX = fPX / m_fDist * m_ObjectData.m_fSpeed;
	m_fOffsetZ = fPZ / m_fDist * m_ObjectData.m_fSpeed;


	m_fatan2 = atan2(fPX, fPZ);


	if (m_eState == OBJECT_IDLE)
	{
		m_eState = OBJECT_RUN;
	}

	msg_walk_begin.X = GetPosX();
	msg_walk_begin.Y = 0.0f;
	msg_walk_begin.Z = GetPosZ();
	msg_walk_begin.uiTick = 0;
	msg_walk_begin.OffsetX = m_fOffsetX;
	msg_walk_begin.OffsetY = 0.0f;
	msg_walk_begin.OffsetZ = m_fOffsetZ;
	msg_walk_begin.uiID = GetID();
	msg_walk_begin.usMapID = (USHORT)GetRegion()->GetID();
	GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg_walk_begin);

	return true;
}

bool CMonster::FollowTo(float x, float, float z, BYTE distance)
{
	float fPX = 0.0f;
	float fPZ = 0.0f;


	m_fDestinationX = x;
	m_fDestinationY = 0.0f;
	m_fDestinationZ = z;

	fPX = x - GetPosX();
	fPZ = z - GetPosZ();

	m_fDist = sqrt(fPX * fPX + fPZ * fPZ);


	m_fatan2 = atan2(fPX, fPZ);

	if (m_fDist < distance || m_fDist < 2.0f)
	{
		return false;
	}

	m_fOffsetX = fPX / m_fDist * m_ObjectData.m_fSpeed;
	m_fOffsetZ = fPZ / m_fDist * m_ObjectData.m_fSpeed;


	msg_walk_begin.X = GetPosX();
	msg_walk_begin.Y = 0.0f;
	msg_walk_begin.Z = GetPosZ();
	msg_walk_begin.uiTick = m_nRunTick;
	msg_walk_begin.OffsetX = m_fOffsetX;
	msg_walk_begin.OffsetY = 0.0f;
	msg_walk_begin.OffsetZ = m_fOffsetZ;
	msg_walk_begin.uiID = GetID();
	msg_walk_begin.usMapID = (USHORT)GetRegion()->GetID();
	msg_walk_begin.Head.usSize = sizeof(msg_walk_begin);

	GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg_walk_begin);
	m_nRunTick = 0;

	{
		m_eState = OBJECT_RUN;
		return true;
	}

}

float CMonster::GetDistWithTarget()
{
	if (GetTarget() == NULL)
		return 0.0f;

	float fPX = GetTarget()->GetPosX() - GetPosX();
	float fPZ = GetTarget()->GetPosZ() - GetPosZ();

	return sqrt(fPX * fPX + fPZ * fPZ);
}

float CMonster::GetDist(float x, float z)
{
	float fPX = x - GetPosX();
	float fPZ = z - GetPosZ();

	return sqrt(fPX * fPX + fPZ * fPZ);
}

UINT CMonster::CalculateAttack() const
{
	return  0;
}

UINT CMonster::CalculateDefend() const
{
	return CGameObject::CalculateDefend();
}



void CMonster::SetDropConfig(const DropConfig& drop)
{
	m_DropConfig = drop;
}

DropConfig& CMonster::GetDropConfig()
{
	return m_DropConfig;
}

Item* CMonster::GetFreeItem()
{
	for (int i = 0; i < MAX_DROPS; i++)
		if (m_DropItems[i].IsClear())
			return &m_DropItems[i];

	return NULL;
}

CGameObject* CMonster::GetNearestPartner()
{
	return NULL;
}



UINT  CMonster::CalculateAttack()
{
	return CGameObject::CalculateAttack();
}

bool  CMonster::CalculatePassivePracticeDegree(PhysiacalAttack, ATTACK_TYPE, bool)
{

	return false;
}

void CMonster::CalculatePassivePracticeDegree(int, ATTACK_TYPE, bool)
{
	return;
}

bool CMonster::ChanegeDegree(int, int)
{
	return false;
}


bool CMonster::PassiveSkillUpgrade(UINT, int*)
{
	return true;
}

void CMonster::AddPassiveSkill(int, int, int)
{

}

INT CMonster::CalculateAttackDamage(CGameObject& obj, float)
{
	return CGameObject::CalculateAttackDamage(obj);
}

INT  CMonster::CalculateAttackDamage(int MagicID)
{
	if (m_targetObj != NULL)
		return  CGameObject::CalculateAttackDamage(MagicID, *m_targetObj);
}

ULONG CMonster::GetHit()
{
	return m_BaseData.m_lHit;
}

ULONG CMonster::GetDodge()
{
	return m_BaseData.m_lDodge;
}

ULONG	CMonster::GetEXP()
{
	return m_ObjectData.m_cExp;
}

int CMonster::GetMoney()
{
	static int money = 0;

	return money;
}

void CMonster::Dead(CGameObject* pObj)
{
	struct  DropItemElem
	{
		UINT  iBaseID;
		int   iProbability;
		int   arrAppend[5];
	};


	if (m_eState == OBJECT_DEAD)
		return;

	int Count = 0;
	for (int i = 0; i < MAX_OWNER; i++)
	{
		if (m_ItemOwner[i])
			Count++;
	}

	if (NULL != pObj)
	{
		if (pObj->GetRank() <= (UINT)(m_Level + 10))
			pObj->CalculateExp(true);
	}
	else
	{
		if (NULL != m_targetObj)
		{
			if (m_ItemOwner[0]->GetRank() <= (UINT)(m_Level + 10))
				m_targetObj->CalculateExp(true);
		}
	}


	int money = GetMoney();

	money /= Count;

	for (int i = 0; i < MAX_OWNER; i++)
	{
		if (m_ItemOwner[i] && m_ItemOwner[i]->GetType() == OBJECTTYPE_PLAYER)
		{
			((CPlayer*)m_ItemOwner[i])->AddMoney(money);
			msg_dead.playerID[i] = m_ItemOwner[i]->GetID();
			msg_dead.playerMoney[i] = ((CPlayer*)m_ItemOwner[i])->GetMoney();
			msg_dead.Exp[i] = ((CPlayer*)m_ItemOwner[i])->GetEXP();
			msg_dead.SkillExp[i] = ((CPlayer*)m_ItemOwner[i])->GetSkillExp();
			msg_dead.SkillPoint[i] = ((CPlayer*)m_ItemOwner[i])->GetSkillPoint();

			if (((CPlayer*)m_ItemOwner[i])->GetRegion() == this->GetRegion())
			{
				((CPlayer*)m_ItemOwner[i])->QuestKILL(m_QuestID);

				for (int j = 0; j < MAX_DROPS; j++)
					((CPlayer*)m_ItemOwner[i])->QuestCast(m_QuestItem[j]);
			}
		}
		else
			msg_dead.playerID[i] = -1;
	}

	m_Drops = 0;
	m_eState = OBJECT_DEAD;
	m_lState = MONSTER_STATE_DEAD;

	if (pObj && pObj->GetRank() - GetRank() < 30)
	{
		for (int i = 0; i < m_ConfirmConfig.DropNum; i++)
		{
			int rand = sbase::RandGet(100);

			if (m_ConfirmConfig.DropProbability > 100 - rand)
			{
				Item* pItem = NULL;
				if (strcmp(m_ConfirmConfig.ItemTable, "NULL"))
				{
					int Random = sbase::RandGet(100);

					UINT BaseID = m_DropItemMgr.FindThisProbability(Random);
					if (!BaseID)
						continue;

					pItem = ItemManager::Instance()->CreateConfimItem(this);

					if (NULL != pItem)
					{
						int num = m_Drops - 1;
						m_DropItem[num] = pItem->GetItemData();
					}
				}
			}
		}
	}

	for (int i = 0; i < m_DropConfig.DropNum; i++)
	{
		int rand = sbase::RandGet(100);

		if (m_DropConfig.DropProbability > 100 - rand)
		{
			Item* pItem = ItemManager::Instance()->CreateItem(this);

			if (!pItem)
				continue;

			int num = m_Drops - 1;

			m_DropItem[num] = pItem->GetItemData();
		}
	}

	GetRegion()->EndFight(this);

	ResetCorpseTimer();

	StopWalk();

	msg_dead.uiID = GetID();
	msg_dead.Head.usSize = sizeof(msg_dead);
	msg_dead.KillerID = GetID();
	msg_dead.KillerPre = 0;
	GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg_dead);

	msg_drops.uiID = GetID();
	msg_drops.DropNum = m_Drops;
	msg_drops.Head.usSize = sizeof(msg_drops.Head);
	msg_drops.Head.usSize += sizeof(msg_drops.uiID);
	msg_drops.Head.usSize += sizeof(msg_drops.DropNum);
	msg_drops.Head.usSize += (USHORT)sizeof(ItemData) * msg_drops.DropNum;
	memcpy(msg_drops.Drops, m_DropItem, sizeof(ItemData) * msg_drops.DropNum);

	for (int i = 0; i < MAX_OWNER; i++)
	{
		if (m_ItemOwner[i])
			CGameObject::s_World->SendMsgToClient(&msg_drops, ((CPlayer*)m_ItemOwner[i])->GetSocket());
	}
}

void CMonster::SetFightPosition()
{
	m_fFightX = GetPosX();
	m_fFightZ = GetPosZ();
}

void CMonster::EndFight()
{
	CGameObject::EndFight();

	m_lState = MONSTER_STATE_LEAVEFIGHT;
}

int CMonster::RandomSkill()
{
	int random = sbase::RandGet(m_SkillRandom);

	for (int i = 0; i < m_SkillCount; i++)
	{
		if (random < m_SkillInfos[i].SkillPro && !IsSkillCooling(i))
			return i;
		else
			random -= m_SkillInfos[i].SkillPro;
	}

	return 0;
}

void CMonster::AynObjToObj(CCell* pOldCell, CCell* pNewCell)
{
	msg_leave.uiID = GetID();
	std::vector<CCell*>  NewCell = GetRegion()->GetMap()->AreaCollisionTest(pNewCell, pOldCell);
	GetRegion()->SendAreaMsgOneToOther(NewCell, &msg_leave);

	AynMeToOther(pOldCell, pNewCell);
}

void CMonster::AynMeToOther(CCell* pOldCell, CCell* pNewCell)
{
	CGameObject::AynMeToOther(pOldCell, pNewCell);
}


void   CMonster::SetRandMagicAttack(UINT MagicAttack)
{
	m_BaseData.m_lAttack = MagicAttack;
	m_BaseData.m_lMagicAttack = MagicAttack;
}


void  CMonster::SynGameData(bool  IsSelf)
{
	msg_syn_gamedata.Head.usSize = sizeof(msg_syn_gamedata);
	msg_syn_gamedata.Head.usType = _MSG_SYN_GAMEDATA;
	msg_syn_gamedata.iUserID = GetID();
	memcpy(&msg_syn_gamedata.sGameData, &m_ObjectData, sizeof(m_ObjectData));
	msg_syn_gamedata.sGameData.m_lMP = GetlMP();
	msg_syn_gamedata.sGameData.m_lHP = GetlHP();
	msg_syn_gamedata.sGameData.m_lRegionID = CGameObject::s_World->GetRegionFromLogicID(GetlRegionID())->GetID();
	msg_syn_gamedata.sBaseData.m_lMaxHP = GetAlllMaxHP();
	msg_syn_gamedata.sBaseData.m_lMaxMP = GetAlllMaxMP();
	msg_syn_gamedata.sBaseData.m_lResumeHP = GetAlllResumeHP();
	msg_syn_gamedata.sBaseData.m_lResumeMP = GetAlllResumeMP();
	msg_syn_gamedata.sBaseData.m_lAttack = GetAlllAttack();
	msg_syn_gamedata.sBaseData.m_lDefend = GetAlllDefend();
	msg_syn_gamedata.sBaseData.m_lMagicAttack = GetAlllMagicAttack();
	msg_syn_gamedata.sBaseData.m_lMagicDefend = GetAlllMagicDefend();
	msg_syn_gamedata.sBaseData.m_lHit = GetAlllHit();
	msg_syn_gamedata.sBaseData.m_lDodge = GetAlllDodge();
	msg_syn_gamedata.sBaseData.m_lCritAppend = GetAlllCritAppend();
	msg_syn_gamedata.sBaseData.m_lCritDefend = GetAlllCritDefend();
	msg_syn_gamedata.sBaseData.m_fPhyDamageAppend = GetAllfPhyDamageAppend();
	msg_syn_gamedata.sBaseData.m_fMagicDamageAppend = GetAllfMagicDamageAppend();
	msg_syn_gamedata.sBaseData.m_lDamageSorb = GetAlllDamageSorb();
	msg_syn_gamedata.sBaseData.m_fBeCure = GetAllfBeCure();
	msg_syn_gamedata.sBaseData.m_fCure = GetAllfCure();
	msg_syn_gamedata.sBaseData.m_lStatusHit = GetAlllStatusHit();
	msg_syn_gamedata.sBaseData.m_lStatudDodge = GetAlllStatudDodge();
	msg_syn_gamedata.iAttackDistance = GetAttackDistance();
	msg_syn_gamedata.iAttackSpeed = GetAttackSpeed();

	GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg_syn_gamedata);
}
