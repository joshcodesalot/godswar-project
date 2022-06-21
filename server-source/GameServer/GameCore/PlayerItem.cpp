#include "stdafx.h"
#include "Player.h"
#include "..\..\Common\MsgDefine.h"
#include "..\..\Common\MsgTypes.h"
#include "..\DataCore.h"
#include ".\ItemManager.h"
#include "..\ServiceLoader.h"
#include "..\ErrorMessage.h"
#include ".\SkillManager.h"
#include ".\Obstacle.h"
#include "../World.h"


extern MSG_MOVE_ITEM msg_move_item;
extern MSG_BREAK_ITEM msg_break_item;
extern MSG_SELL msg_sell;
extern MSG_MAGIC_DAMAGE msg_magic;
extern MSG_WALK_BEGIN msg_walk_begin;
extern MSG_WALK_END msg_walk_end;
extern MSG_ACTIVESKILL_INFO  msg_activeskill_info;
extern MSG_USEOREQUIP_ITEM msg_useorequip_item;


extern void ExcScript(CGameObject* p1, CGameObject* p2, int MagicID);
void CPlayer::AI()
{
	OnRead();

	if (m_RunTimer.IsExpire())
	{
		m_RunTimer.Startup(PLAYER_AI_TIMER);
		OnSend();
	}
	else
		return;

	IsRefresh(TIME_STYLE_COOL);

	if ((m_ObjectData.m_lHP < GetMaxHP() || m_ObjectData.m_lMP < GetMaxMP()))
	{
		if (m_Resume_Timer.IsActive() && m_Resume_Timer.IsExpire() && !IsDead())
		{
			m_ObjectData.m_lHP += GetAlllResumeHP();
			m_ObjectData.m_lMP += GetAlllResumeMP();

			if (m_ObjectData.m_lHP > GetMaxHP())
			{
				m_ObjectData.m_lHP = GetMaxHP();
			}

			if (m_ObjectData.m_lMP > GetMaxMP())
			{
				m_ObjectData.m_lMP = GetMaxMP();
			}
			MSG_RESUNE msg_Resume;
			msg_Resume.Head.usSize = sizeof(MSG_RESUNE);
			msg_Resume.Head.usType = _MSG_RESUNE;
			msg_Resume.ulHP = m_ObjectData.m_lHP;
			msg_Resume.ulMP = m_ObjectData.m_lMP;
			CGameObject::s_World->SendMsgToClient(&msg_Resume, GetSocket());
			ResetResumeTimer();
		}
	}

	if (m_Relation_Timer.IsActive() && m_Relation_Timer.IsExpire() && !IsDead())
	{
		RefreshRelation();
		ResetRelationTimer();
	}

	if (m_Trade.IsTrading())
	{
		float fPX = 0.0f;
		float fPZ = 0.0f;

		fPX = m_Trade.GetTrader()->GetPosX() - GetPosX();
		fPZ = m_Trade.GetTrader()->GetPosZ() - GetPosZ();

		int dist = sqrt(fPX * fPX + fPZ * fPZ);

		if (dist > 20)
		{
			m_Trade.GetTrader()->GetTrade()->EndTrade();
			m_Trade.EndTrade();

			m_Trade.GetTrader()->GetTrade()->ClearTrade();
			m_Trade.ClearTrade();
		}
	}

	if (!this->IsDead())
		this->GetRegion()->RefreshInitiativeMonster(this);

	__super::AI();


	if (GetTimeStart())
	{
		Updata_SurplusTime();
	}
	ClearExpiredItem();

	QuestUpdata();

	if (GetRegion()->ChangeRegion(this, GetPosX(), 0.0f, GetPosZ()))
	{
	}

	if ((NULL != m_pTeamLeader) && m_Team_Timer.IsActive() && m_Team_Timer.IsExpire())
	{
		m_pTeamLeader->UpdateTeam_New(true);
		m_Team_Timer.Update();
	}

	m_GMctr->CrtAI();

	if (GetDanger() >= 3)
		s_World->KickPlayer(this);

}

int CPlayer::CheckBlock(int Mid, float x, float z)
{
	return CGameObject::s_World->g_pObstacle->IsObstacleList(GetRegion()->GetID(), x, z);
}

void CPlayer::Run(void)
{
	if (m_WorldStatus == eSCENECHANGE)
	{
		return;
	}

	if (m_queueWalk.size() > 0)
	{
		WalkIterator it = m_queueWalk.begin();
		tarWalk* pWalk = *it;

		SetPos(pWalk->x, 0.0f, pWalk->z);
		m_fatan2 = pWalk->fatan;

		m_queueWalk.remove(pWalk);
		SAFE_DELETE(pWalk);
	}


	if (m_queuePath.size() > 0)
	{
		PathIterator it = m_queuePath.begin();
		tarPath* pPath = *it;
		if (pPath->nTick >= 60)
		{
			char Ifbuf[128];
			_stprintf(Ifbuf, "Íæ¼Ò: %s ¿¨Î»ÁË£¡", GetName());
			sbase::ConsoleWriteColorText(FOREGROUND_RED, Ifbuf);
		}
		else
		{
			pPath->nTick++;
			SetPos(GetPosX() + pPath->OffsetX, 0, GetPosZ() + pPath->OffsetZ);

		}

		if (pPath == NULL)
		{
			if (m_queuePath.size() > 0)
			{
				it = m_queuePath.begin();
				pPath = *it;
			}
		}


		if (m_queuePath.size() > 0 && pPath->nEndTick != 0)
		{
			if (pPath->nTick >= pPath->nEndTick)
			{
				SetPos(pPath->end_x, 0, pPath->end_z);
				m_queuePath.remove(pPath);
				if (m_queuePath.size() > 0)
				{
					it = m_queuePath.begin();
					tarPath* pNextPath = *it;
					SetPos(pNextPath->x, 0, pNextPath->z);
					m_fatan2 = atan2(pNextPath->OffsetX, pNextPath->OffsetZ);

					msg_walk_begin.X = GetPosX();
					msg_walk_begin.Y = 0.0f;
					msg_walk_begin.Z = GetPosZ();
					msg_walk_begin.uiTick = pPath->nEndTick;
					msg_walk_begin.OffsetX = pNextPath->OffsetX;
					msg_walk_begin.OffsetY = 0.0f;
					msg_walk_begin.OffsetZ = pNextPath->OffsetZ;
					msg_walk_begin.uiID = GetID();
					msg_walk_begin.usMapID = (USHORT)this->GetID();
					this->GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg_walk_begin);

				}
				else
				{
					if (m_eState == OBJECT_RUN)
						m_eState = OBJECT_IDLE;

					msg_walk_end.X = GetPosX();
					msg_walk_end.Y = 0.0f;
					msg_walk_end.Z = GetPosZ();
					msg_walk_end.uiTick = pPath->nEndTick;
					msg_walk_end.fAtan2 = m_fatan2;
					msg_walk_end.uiID = GetID();
					msg_walk_end.usMapID = (USHORT)this->GetID();
					this->GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg_walk_end);

				}
				SAFE_DELETE(pPath);
			}
		}
		else
		{
		}
	}

	Explorer();

	switch (m_eState)
	{
	case OBJECT_RUN:
	{
	}
	break;
	case OBJECT_CAST:
	{
		if (IsRefresh(TIME_STYLE_CAST))
		{
			m_eState = OBJECT_PERFORM;
		}
	}
	break;
	case OBJECT_PERFORM:
	{
		if (IsDead())
			break;
		const MagicData* magicData = CGameObject::s_World->g_pSkillManager->GetMagic(GetCurActiveSkillID());
		CGameObject* pTarget = GetRegion()->CollectTarget(this, GetSkillTarget(), magicData->ucTarget, magicData);
		if (NULL == pTarget)
		{
			if (!((magicData->ucTarget & TARGET_POS) == TARGET_POS))
			{
				ResetCastState();
				return;
			}
		}

		bool FirstDead = false;
		if (pTarget)
			FirstDead = pTarget->IsDead();

		if (magicData->ucRange == 0)
		{

			if (FirstDead)
			{
				ResetCastState();
				break;
			}

			if (magicData->iEffectTurn > 1)
			{
				float fPosition[3];
				fPosition[0] = pTarget->GetPosX();
				fPosition[1] = pTarget->GetPosY();
				fPosition[2] = pTarget->GetPosZ();
				GetRegion()->AddMagicRegion(GetID(), GetCurActiveSkillID(), fPosition, magicData->ucRange, magicData->ucAffectObj, pTarget->GetID());
			}
			if (magicData->ucStyle == 1)
			{
				std::vector<int>::const_iterator iter = magicData->m_Status.begin();
				for (; iter != magicData->m_Status.end(); iter++)
				{
					if ((*iter) < 0)
					{
						pTarget->DelStatus(CGameObject::s_World->g_pStatusManager->GetStatus((-1) * (*iter)));
					}
					else
					{
						pTarget->AddStatus(CGameObject::s_World->g_pStatusManager->GetStatus(*iter));
					}

				}

				pTarget->CoagentEnmityList(this, magicData->iEnmity);

				ExcScript(this, pTarget, GetCurActiveSkillID());

			}
			else
			{
				if (magicData->ucEffect == 1)
				{
					if (JudgeAppendStatusStyle(pTarget, magicData) == ATTACK_HIT)
					{
						std::vector<int>::const_iterator iter = magicData->m_Status.begin();
						for (; iter != magicData->m_Status.end(); iter++)
						{
							if ((*iter) < 0)
							{
								pTarget->DelStatus(CGameObject::s_World->g_pStatusManager->GetStatus((-1) * (*iter)));
							}
							else
							{
								pTarget->AddStatus(CGameObject::s_World->g_pStatusManager->GetStatus(*iter));
							}

						}
						msg_magic.Status_Type = ATTACK_HIT;


					}
					else
					{
						msg_magic.Status_Type = ATTACK_MISS;
					}

					MSG_MAGIC_PERFORM mgicPerform;
					mgicPerform.Head.usSize = sizeof(MSG_MAGIC_PERFORM);
					mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
					mgicPerform.uiID = GetID();
					mgicPerform.x = m_SkillMsg.x1;
					mgicPerform.z = m_SkillMsg.z1;
					mgicPerform.uiObjectID = pTarget->GetID();
					mgicPerform.ucMagicID = GetCurActiveSkillID();


					if (!FirstDead)
					{
					}

					GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &mgicPerform);
				}
				else
				{
					if (pTarget->GetlFaction() == this->GetlFaction())
						return;

					if (JudgeAppendStatusStyle(pTarget, magicData) == ATTACK_HIT)
					{
						std::vector<int>::const_iterator iter = magicData->m_Status.begin();
						for (; iter != magicData->m_Status.end(); iter++)
						{
							if ((*iter) < 0)
							{
								pTarget->DelStatus(CGameObject::s_World->g_pStatusManager->GetStatus((-1) * (*iter)));
							}
							else
							{
								pTarget->AddStatus(CGameObject::s_World->g_pStatusManager->GetStatus(*iter));
							}
						}
						msg_magic.Status_Type = ATTACK_HIT;
					}


					if (pTarget && (pTarget->GetType() == OBJECTTYPE_MONSTER || pTarget->GetType() == OBJECTTYPE_PLAYER))
					{
						pTarget->ClearDamageInfo();

						ExcScript(this, pTarget, GetCurActiveSkillID());
						DAMAGE_INFO damageInfo = pTarget->GetDamageInfo();


						if (pTarget->GetlHP() <= 0)
						{
							pTarget->Dead(this);
						}

						if (pTarget)
							msg_magic.uiObjectID = pTarget->GetID();
						else
							msg_magic.uiObjectID = -1;
						msg_magic.uiID = GetID();


						if (damageInfo.HP)
						{
							msg_magic.bType = 0;
							msg_magic.iPower = damageInfo.HP;
						}
						else
						{
							msg_magic.bType = 1;
							msg_magic.iPower = damageInfo.MP;
						}

						MSG_MAGIC_PERFORM mgicPerform;

						mgicPerform.Head.usSize = sizeof(MSG_MAGIC_PERFORM);
						mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
						mgicPerform.uiID = GetID();
						mgicPerform.x = m_SkillMsg.x1;
						mgicPerform.z = m_SkillMsg.z1;
						mgicPerform.uiObjectID = pTarget->GetID();
						mgicPerform.ucMagicID = GetCurActiveSkillID();


						if (!FirstDead)
						{
							msg_magic.ucMagicID = GetCurActiveSkillID();
							msg_magic.Attack_Type = damageInfo.Type;
							msg_magic.x1 = m_SkillMsg.x1;
							msg_magic.z1 = m_SkillMsg.z1;

							GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg_magic);
						}

						GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &mgicPerform);
					}

				}

			}
		}
		else
		{
			ObjectList objlst;
			float	   center[3];
			center[0] = m_SkillMsg.x1;
			center[1] = 0;
			center[2] = m_SkillMsg.z1;

			GetRegion()->CollectGameObjectSphere(this, &objlst, center, magicData);

			size_t count = objlst.size();

			int size = int(sizeof(MsgHead) + sizeof(int) + 2 * sizeof(UINT) + sizeof(BYTE) + count * sizeof(MAGIC_DAMAGE));
			MSG_MAGIC_CLUSTER_DAMAGE  tmpDamage;
			MSG_MAGIC_CLUSTER_DAMAGE* clstDamager = &tmpDamage;

			if (magicData->iEffectTurn > 1)
			{
				GetRegion()->AddMagicRegion(GetID(), GetCurActiveSkillID(), center, magicData->ucRange, magicData->ucAffectObj);
			}

			if (clstDamager == NULL)
				break;

			clstDamager->iCount = int(count);
			clstDamager->uiID = GetID();
			clstDamager->Head.usSize = size;
			clstDamager->Head.usType = _MSG_MAGIC_CLUSTER_DAMAGE;
			clstDamager->ucMagicID = magicData->ucID;
			::ZeroMemory(clstDamager->magicDamage, sizeof(clstDamager->magicDamage));

			int  i = 0;
			for (ObjectIterator Iter = objlst.begin(); Iter != objlst.end(); Iter++)
			{
				MAGIC_DAMAGE* mgicDamage = &clstDamager->magicDamage[i];
				++i;

				pTarget = (CGameObject*)*Iter;


				if (magicData->ucStyle == 1)
				{
					if (FirstDead)
						break;

					std::vector<int>::const_iterator iter = magicData->m_Status.begin();
					for (; iter != magicData->m_Status.end(); iter++)
					{
						if ((*iter) < 0)
						{
							pTarget->DelStatus(pTarget->s_World->g_pStatusManager->GetStatus((-1) * (*iter)));
						}
						else
						{
							pTarget->AddStatus(pTarget->s_World->g_pStatusManager->GetStatus(*iter));
						}

					}
				}
				else
				{
					if (magicData->ucEffect == 1)
					{
						if (JudgeAppendStatusStyle(pTarget, magicData) == ATTACK_HIT)
						{
							std::vector<int>::const_iterator iter = magicData->m_Status.begin();
							for (; iter != magicData->m_Status.end(); iter++)
							{
								if ((*iter) < 0)
								{
									pTarget->DelStatus(pTarget->s_World->g_pStatusManager->GetStatus((-1) * (*iter)));
								}
								else
								{
									pTarget->AddStatus(pTarget->s_World->g_pStatusManager->GetStatus(*iter));
								}

							}
						}
					}
					else
					{
						if (JudgeAppendStatusStyle(pTarget, magicData) == ATTACK_HIT)
						{
							std::vector<int>::const_iterator iter = magicData->m_Status.begin();
							for (; iter != magicData->m_Status.end(); iter++)
							{
								if ((*iter) < 0)
								{
									pTarget->DelStatus(pTarget->s_World->g_pStatusManager->GetStatus((-1) * (*iter)));
								}
								else
								{
									pTarget->AddStatus(pTarget->s_World->g_pStatusManager->GetStatus(*iter));
								}
							}
						}
					}
				}




				mgicDamage->uiObjectID = pTarget->GetID();

				pTarget->ClearDamageInfo();
				ExcScript(this, pTarget, GetCurActiveSkillID());
				DAMAGE_INFO damageInfo = pTarget->GetDamageInfo();

				if (damageInfo.HP)
				{
					mgicDamage->ucType = 0;
					mgicDamage->iPower = damageInfo.HP;
					mgicDamage->Attack_Type = 1;
				}
				else
				{
					mgicDamage->ucType = 1;
					mgicDamage->iPower = damageInfo.MP;
					mgicDamage->Attack_Type = 1;
				}

				mgicDamage->Attack_Type = damageInfo.Type;

				if (pTarget->GetlHP() <= 0)
				{
					pTarget->Dead(this);
				}
			}

			MSG_MAGIC_PERFORM mgicPerform;
			mgicPerform.Head.usSize = sizeof(MSG_MAGIC_PERFORM);
			mgicPerform.Head.usType = _MSG_MAGIC_PERFORM;
			mgicPerform.uiID = GetID();
			mgicPerform.x = m_SkillMsg.x1;
			mgicPerform.z = m_SkillMsg.z1;
			mgicPerform.uiObjectID = -1;
			mgicPerform.ucMagicID = GetCurActiveSkillID();

			GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &mgicPerform);
			GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), clstDamager);
		}
		AddMP(-magicData->sMP);

		MSG_UPDATE_MP	msg;
		msg.Head.usSize = sizeof(MSG_UPDATE_MP);
		msg.Head.usType = _MSG_UPDATE_MP;
		msg.mp = GetlMP();
		msg.playerID = GetID();
		GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg);
	}

	SetState(OBJECT_IDLE);

	break;
	case OBJECT_IDLE:
	{
	}
	break;
	default:
		break;
	}

}

bool CPlayer::PickupDrops(Item* p)
{
	if (!IsMsgOK(MSG_KITBAG_FLAG))
		return false;

	for (int i = 0; i < MAX_BAGS; i++)
	{
		if (m_Bags[i].Push(p))
		{
			if (!p->GetItemBaseAttribute())
			{
				char Ifbuf[64];
				_stprintf(Ifbuf, "CPlayer::PickupDrops() : pItem->GetItemBaseAttribute()");
				sbase::SysLogSave(Ifbuf);
			}
			return true;
		}
	}

	return false;
}

Item* CPlayer::GetEmpty()
{
	for (int i = 0; i < MAX_BAGS; i++)
	{
		if (!m_Bags[i].GetActive())
			return NULL;

		Item* pitem = m_Bags[i].GetEmptyItem();

		if (pitem)
			return pitem;
	}

	return NULL;
}

Item* CPlayer::GetItem(int index, int num)
{
	if (index >= MAX_BAGS || index < 0 || num < 0)
		return NULL;

	if (!m_Bags[index].GetActive())
		return NULL;


	return m_Bags[index].GetItem(num);
}

void CPlayer::AddItem(Item* item, int index, int num)
{

	m_Bags[index].Push(item, num);
}

void CPlayer::AddItem(Item* item, int count)
{
	vector<Item*> free;

	for (int i = 0; i < MAX_BAGS; i++)
	{
		for (int j = 0; j < MAX_BAG_GRID; j++)
		{
			if (m_Bags[i].GetItem(j))
			{
				if (m_Bags[i].GetItem(j)->IsClear())
					free.push_back(m_Bags[i].GetItem(j));
			}

		}
	}

	for (int i = 0; i < count; i++)
	{
		*free[i] = item[i];
	}
}


bool CPlayer::IsGoldBoxExist(int keyid)
{
	Item* temp_item = NULL;
	bool key(false), box(false);
	for (int i = 0; i < MAX_BAGS; i++)
	{
		for (int j = 0; j < MAX_BAG_GRID; j++)
		{
			temp_item = m_Bags[i].GetItem(j);
			if (!temp_item) { return false; }
			ASSERT(temp_item->GetItemBaseAttribute());
			if (!key) { if (temp_item->GetItemBaseAttribute()->KEY == keyid) { key = true; } }
			if (!box) { if (temp_item->GetItemBaseAttribute()->GoldBoxIdx == keyid) { box = true; } }
			if (key && box) { return true; }
		}
	}
	return false;
}
BYTE CPlayer::CanOpenbox(BYTE Iter, UINT base_id, int keyid)
{
	Item* temp_item(NULL), * temp_item2(NULL);
	BYTE index1(0), index2(0);
	index1 = Iter / MAX_BAG_GRID;
	index2 = Iter % MAX_BAG_GRID;

	temp_item = m_Bags[index1].GetItem(index2);
	if (!temp_item) { return -1; }
	if (temp_item->m_Lock) { return -1; }
	ASSERT(temp_item->GetItemBaseAttribute());
	if (temp_item->GetItemBaseAttribute()->ID != base_id)
	{
		return -1;
	}


	for (int i = 0; i < MAX_BAGS; i++)
	{
		for (int j = 0; j < MAX_BAG_GRID; j++)
		{
			temp_item2 = m_Bags[i].GetItem(j);
			if (!temp_item2) { return -1; }
			if (temp_item2->IsClear()) { continue; }
			if (temp_item2->m_Lock) { continue; }


			if (temp_item2->GetItemBaseAttribute()->KEY == keyid)
			{
				return i * MAX_BAG_GRID + j;
			}
		}
	}
	return -1;
}
bool CPlayer::ExpendGoldBoxIter(BYTE IterBox, BYTE IterKey, int base_id, int keyid)
{
	Item* temp_item(NULL), * temp_item2(NULL);
	BYTE index1(0), index2(0), index3(0), index4(0);
	index1 = IterBox / MAX_BAG_GRID;
	index2 = IterBox % MAX_BAG_GRID;
	index3 = IterKey / MAX_BAG_GRID;
	index4 = IterKey % MAX_BAG_GRID;

	temp_item = m_Bags[index1].GetItem(index2);
	if (!temp_item) { return -1; }
	if (temp_item->m_Lock) { return -1; }
	ASSERT(temp_item->GetItemBaseAttribute());
	if (temp_item->GetItemBaseAttribute()->ID != base_id)
	{
		return false;
	}


	temp_item2 = m_Bags[index3].GetItem(index4);
	if (!temp_item2) { return -1; }
	if (temp_item2->m_Lock) { return -1; }
	if (temp_item2->GetItemBaseAttribute()->KEY != keyid)
	{
		return false;
	}

	if (!UseOrEquip(index3, index4))
	{
		return false;
	}

	AddItem(&cur_box_item, index1, index2);

	msg_useorequip_item.Index = (USHORT)index3;
	msg_useorequip_item.Num = (USHORT)index4;
	msg_useorequip_item.uiID = GetID();
	msg_useorequip_item.False = false;

	ASSERT(temp_item->GetItemBaseAttribute());
	msg_useorequip_item.Base = temp_item->GetItemBaseAttribute()->ID;
	for (int i = 0; i < MAX_EQUIPAPPEND_COUNT; i++)
	{
		if (temp_item->GetItemAppendAttribute(i))
		{
			msg_useorequip_item.Append[i] = temp_item->GetItemAppendAttribute(i)->ID;
		}
		else
		{
			msg_useorequip_item.Append[i] = -1;
		}
	}

	msg_useorequip_item.BaseLevel = temp_item->BaseLevel;
	msg_useorequip_item.AppendLevel = temp_item->AppLevel;

	s_World->SendMsgToClient(&msg_useorequip_item, m_pSocket);
	return true;
}



bool CPlayer::ExpendGoldBoxKey(BYTE Iter)
{
	Item* temp_item = NULL;

	BYTE index1(0), index2(0);
	index1 = Iter / MAX_BAG_GRID;
	index2 = Iter % MAX_BAG_GRID;

	temp_item = m_Bags[index1].GetItem(index2);

	if (!temp_item) { return false; }
	if (temp_item->IsClear()) { return false; }
	if (temp_item->m_Lock) { return false; }
	ASSERT(temp_item->GetItemBaseAttribute());
	if (temp_item->GetItemBaseAttribute()->KEY == Iter)
	{
		if (!UseOrEquip(index1, index2))
		{
			return false;
		}

		msg_useorequip_item.Index = (USHORT)index1;
		msg_useorequip_item.Num = (USHORT)index2;
		msg_useorequip_item.uiID = GetID();
		msg_useorequip_item.False = false;

		ASSERT(temp_item->GetItemBaseAttribute());
		msg_useorequip_item.Base = temp_item->GetItemBaseAttribute()->ID;
		for (int i = 0; i < MAX_EQUIPAPPEND_COUNT; i++)
		{
			if (temp_item->GetItemAppendAttribute(i))
			{
				msg_useorequip_item.Append[i] = temp_item->GetItemAppendAttribute(i)->ID;
			}
			else
			{
				msg_useorequip_item.Append[i] = -1;
			}
		}

		msg_useorequip_item.BaseLevel = temp_item->BaseLevel;
		msg_useorequip_item.AppendLevel = temp_item->AppLevel;

		CRegion* pRegion = GetRegion();
		pRegion->SendAreaMsgOneToOther(GetCurrentCell(), &msg_useorequip_item);
		return true;
	}

	return false;
}

bool CPlayer::ExpendGoldBoxId(int gold_box_id, int keyid)
{
	Item* temp_item = NULL;
	bool key(false), box(false);
	for (int i = 0; i < MAX_BAGS; i++)
	{
		for (int j = 0; j < MAX_BAG_GRID; j++)
		{
			if (key && box)
			{
				SetGoldBoxId(0);
				return true;
			}
			temp_item = m_Bags[i].GetItem(j);
			if (!temp_item) { return false; }
			if (temp_item->IsClear()) { continue; }
			if (temp_item->m_Lock) { continue; }
			Item temp = *temp_item;
			if (!key)
			{
				ASSERT(temp_item->GetItemBaseAttribute());
				if (temp_item->GetItemBaseAttribute()->KEY == keyid)
				{
					key = true;
					if (!UseOrEquip(i, j))
					{
						return false;
					}

					msg_useorequip_item.Index = (USHORT)i;
					msg_useorequip_item.Num = (USHORT)j;
					msg_useorequip_item.uiID = GetID();
					msg_useorequip_item.False = false;

					msg_useorequip_item.Base = temp.GetItemBaseAttribute()->ID;
					for (int i = 0; i < MAX_EQUIPAPPEND_COUNT; i++)
					{
						if (temp.GetItemAppendAttribute(i))
						{
							msg_useorequip_item.Append[i] = temp.GetItemAppendAttribute(i)->ID;
						}
						else
						{
							msg_useorequip_item.Append[i] = -1;
						}
					}

					msg_useorequip_item.BaseLevel = temp.BaseLevel;
					msg_useorequip_item.AppendLevel = temp.AppLevel;

					CRegion* pRegion = GetRegion();
					pRegion->SendAreaMsgOneToOther(GetCurrentCell(), &msg_useorequip_item);
					continue;
				}
			}
			if (!box)
			{
				ASSERT(temp_item->GetItemBaseAttribute());
				if (temp_item->GetItemBaseAttribute()->ID == gold_box_id)
				{
					box = true;
					if (!UseOrEquip(i, j))
					{
						return false;
					}
					msg_useorequip_item.Index = (USHORT)i;
					msg_useorequip_item.Num = (USHORT)j;
					msg_useorequip_item.uiID = GetID();
					msg_useorequip_item.False = false;

					msg_useorequip_item.Base = temp.GetItemBaseAttribute()->ID;
					for (int i = 0; i < MAX_EQUIPAPPEND_COUNT; i++)
					{
						if (temp.GetItemAppendAttribute(i))
						{
							msg_useorequip_item.Append[i] = temp.GetItemAppendAttribute(i)->ID;
						}
						else
						{
							msg_useorequip_item.Append[i] = -1;
						}
					}

					msg_useorequip_item.BaseLevel = temp.BaseLevel;
					msg_useorequip_item.AppendLevel = temp.AppLevel;

					CRegion* pRegion = GetRegion();
					pRegion->SendAreaMsgOneToOther(GetCurrentCell(), &msg_useorequip_item);

				}
			}
		}
	}
	return false;
}

bool CPlayer::IsDoubleItem()
{
	switch (++GoldOpendNum)
	{
	case 20:
	{return true; }
	case 35:
	{return true; }
	case 45:
	{GoldOpendNum = 0; return true; }
	default:
		return false;
	}

}

bool CPlayer::AddItem(Item* item)
{
	if (item->GetItemBaseAttribute()->EType == ItemBaseAttribute::ConsumeItem)
	{
		if (GetFreeItemCount() == 0)
		{
			int num = 0;

			for (int i = 0; i < MAX_BAGS; i++)
			{
				num += m_Bags[i].GetItemFreeOverlap(item);
			}

			if (num < item->m_Overlap)
				return false;
		}

		for (int i = 0; i < MAX_BAGS; i++)
		{
			if (m_Bags[i].Push(item))
				return true;
		}

		return false;
	}

	int free = GetFreeItemCount();

	if (!free)
		return false;

	for (int i = 0; i < MAX_BAGS; i++)
	{
		for (int j = 0; j < MAX_BAG_GRID; j++)
		{
			if (m_Bags[i].GetItem(j))
				if (m_Bags[i].GetItem(j)->IsClear())
				{
					m_Bags[i].Push(item, j);

					return true;
				}
		}
	}

	return false;
}

Item* CPlayer::FindItem(int id, int* bag, int* num)
{
	for (int i = 0; i < MAX_BAGS; i++)
	{
		for (int j = 0; j < MAX_BAG_GRID; j++)
		{
			Item* pItem = m_Bags[i].GetItem(j);
			if (pItem)
				if (!pItem->IsClear())
					if (pItem->GetItemBaseAttribute()->ID == id)
					{
						*bag = i;
						*num = j;
						return pItem;
					}
		}
	}
	return NULL;
}

bool  CPlayer::DelItem(int index, int num)
{
	if (index >= MAX_BAGS || index < 0 || num >= 24 || num < 0)
	{
		return false;
	}

	Item* pItem = m_Bags[index].GetItem(num);
	if (pItem && !pItem->m_Lock)
	{
		pItem->Clear();
		return true;
	}

	return false;
}

eError CPlayer::SellItem(int index, int num)
{
	Item* item = this->GetItem(index, num);

	if (!item)
	{
		return NO_MSG_ERRO;
	}

	if (item->IsClear())
	{
		return NO_MSG_ERRO;
	}

	if (item->m_Lock)
	{

		return NO_MSG_ERRO;
	}

	if (m_Trade.IsTrading())
	{
		return NO_MSG_ERRO;
	}

	if (item->GetItemBaseAttribute()->Money == 0 &&
		item->GetItemBaseAttribute()->Bijou == 0)
	{
		return NO_MSG_ERRO;
	}

	this->AddMoney(item->GetItemBaseAttribute()->Money * item->m_Overlap * 0.2);
	this->AddBijou(item->GetItemBaseAttribute()->Bijou * item->m_Overlap * 0.2);

	item->Clear();

	msg_sell.Index = index;
	msg_sell.Num = num;

	CGameObject::s_World->SendMsgToClient(&msg_sell, GetSocket());

	return NO_MSG_ERRO;
}

int	CPlayer::GetFreeItemCount()
{
	int count = 0;

	for (int i = 0; i < MAX_BAGS; i++)
	{
		for (int j = 0; j < MAX_BAG_GRID; j++)
		{
			if (m_Bags[i].GetItem(j))
			{
				if (m_Bags[i].GetItem(j)->IsClear())
					count++;
			}
		}
	}

	return count;
}

int CPlayer::GetFreeItemOverlap(Item* p)
{
	if (!p)
		return 0;

	if (p->IsClear())
		return 0;

	int count = 0;

	for (int i = 0; i < MAX_BAGS; i++)
	{
		for (int j = 0; j < MAX_BAG_GRID; j++)
		{
			if (m_Bags[i].GetItem(j))
			{
				if (m_Bags[i].GetItem(j)->IsClear())
					count += p->GetItemBaseAttribute()->Overlap;
				else if (m_Bags[i].GetItem(j)->GetItemBaseAttribute()->ID == p->GetItemBaseAttribute()->ID && m_Bags[i].GetItem(j)->m_Binding == p->m_Binding)
					count += p->GetItemBaseAttribute()->Overlap - p->m_Overlap;
			}
		}
	}

	return count;
}


bool CPlayer::MoveEquip(int index, int indexaim, int numaim)
{
	if (IsGM())
		return false;

	if (index >= EquipCount)
	{
		return false;
	}

	if (indexaim >= MAX_BAGS)
	{
		return false;
	}

	if (indexaim < 0 || numaim < 0)
	{
		m_Equips[index].Decrease(&m_EquipData);
		m_Equips[index].Clear();

		SynGameData(true);
		SynClientPassiveSkillData();

		return true;
	}

	Item* pitem = m_Bags[indexaim].GetItem(numaim);

	if (!pitem)
	{
		return false;
	}

	if (!pitem->IsClear())
	{
		if (pitem->m_Lock)
			return false;

		msg_move_item.Base = pitem->GetItemBaseAttribute()->ID;
		msg_move_item.AppendLevel = pitem->AppLevel;
		msg_move_item.BaseLevel = pitem->BaseLevel;


		for (int i = 0; i < MAX_EQUIPAPPEND_COUNT; i++)
		{
			//TODO: FIX
			if (pitem->GetItemAppendAttribute(i))
			{
				msg_move_item.Append[i] = pitem->GetItemAppendAttribute(i)->ID;
			}
			else
			{
				msg_move_item.Append[i] = -1;
			}
		}

		if (!SetEquip(indexaim, numaim, index))
			return false;
	}
	else
	{
		if (m_Equips[index].IsClear())
		{
			return false;
		}

		m_Bags[indexaim].Push(&m_Equips[index], numaim);

		m_Equips[index].Decrease(&m_EquipData);

		if (m_Equips[index].GetItemBaseAttribute()->EType == ItemBaseAttribute::Weapon)
		{
			m_Equips[index].Clear();
			ChangeAttackSpeed();
		}
		else
			m_Equips[index].Clear();

		ChangeHp();
		SynGameData(true);
		SynClientPassiveSkillData();

		msg_move_item.Base = -1;
	}

	msg_move_item.uiID = this->GetID();
	msg_move_item.Index = index;
	msg_move_item.Num = -1;
	msg_move_item.IndexAim = indexaim;
	msg_move_item.NumAim = numaim;

	GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg_move_item);
	//TODO
	s_World->SendMsgToClient(&msg_move_item, GetSocket());

	return true;
}

int  CPlayer::MoveItem(int index, int num, int indexaim, int numaim)
{
	if (index < 0)
	{
		return NO_MSG_ERRO;
	}

	if (num < 0)
	{
		return MoveEquip(index, indexaim, numaim);
	}

	if (index >= MAX_BAGS || indexaim >= MAX_BAGS)
	{
		return NO_MSG_ERRO;
	}

	if (indexaim < 0 || numaim < 0)
	{
		if (m_Bags[index].Erase(num))
		{
			msg_move_item.uiID = this->GetID();
			msg_move_item.Index = index;
			msg_move_item.Num = num;
			msg_move_item.IndexAim = indexaim;
			msg_move_item.NumAim = numaim;

			CGameObject::s_World->SendMsgToClient(&msg_move_item, GetSocket());
		}

		return NO_MSG_ERRO;
	}

	if (m_Bags[index].Move(&m_Bags[indexaim], num, numaim))
	{
		msg_move_item.uiID = this->GetID();
		msg_move_item.Index = index;
		msg_move_item.Num = num;
		msg_move_item.IndexAim = indexaim;
		msg_move_item.NumAim = numaim;
		CGameObject::s_World->SendMsgToClient(&msg_move_item, GetSocket());
	}

	return NO_MSG_ERRO;
}
int  CPlayer::BreakItem(int index, int num, int indexaim, int numaim, int overlay)
{
	Item* item = GetItem(index, num);

	if (m_Bags[index].Break(&m_Bags[indexaim], num, numaim, overlay))
	{
		msg_break_item.Index = index;
		msg_break_item.Num = num;
		msg_break_item.IndexAim = indexaim;
		msg_break_item.NumAim = numaim;
		msg_break_item.Overlap = overlay;
		msg_break_item.Flags = 2;

		CGameObject::s_World->SendMsgToClient(&msg_break_item, GetSocket());
	}
	else
	{
		if (item->m_Lock)
		{
			item->m_Lock = false;
		}

		msg_break_item.Index = index;
		msg_break_item.Num = num;
		msg_break_item.IndexAim = -1;
		msg_break_item.NumAim = -1;
		msg_break_item.Flags = 1;

		CGameObject::s_World->SendMsgToClient(&msg_break_item, GetSocket());

	}

	return NO_MSG_ERRO;
}

bool CPlayer::SetEquip(int index, int num, int indexaim)
{
	Item* pitem = m_Bags[index].GetItem(num);

	if (pitem->GetItemBaseAttribute()->Equip)
	{
		if (pitem->BaseLevel > m_ObjectData.m_cRank ||
			pitem->GetItemBaseAttribute()->EType > ItemBaseAttribute::Ring)
		{
			return false;
		}
		if (!pitem->HasClass(m_ObjectData.m_lClass))
			return false;

		if (pitem->GetItemBaseAttribute()->Faction != m_ObjectData.m_lFaction &&
			pitem->GetItemBaseAttribute()->Faction != -1)
			return false;

		Item equip = *pitem;
		Item temp;
		Item temp2;

		int main = EquipMainHand1;
		int Auxiliary = EquipAuxiliaryHand1;

		pitem->m_Binding = true;

		switch (indexaim)
		{
		case EquipMainHand1:
		{
			if (pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Weapon)
				return false;

			switch (pitem->GetItemBaseAttribute()->EType)
			{
			case ItemBaseAttribute::Weapon:
			{
				switch (pitem->GetItemBaseAttribute()->eEquipHand)
				{
				case ItemBaseAttribute::OneHand:
				{
					if (!m_Equips[Auxiliary].IsClear())
					{
						if (m_Equips[Auxiliary].GetItemBaseAttribute()->EType == ItemBaseAttribute::Weapon)
							if (m_Equips[Auxiliary].GetItemBaseAttribute()->eEquipHand != pitem->GetItemBaseAttribute()->eEquipHand)
								return false;
					}

					temp = m_Equips[main];
					m_Equips[main] = *pitem;
				}
				break;
				case ItemBaseAttribute::TwoHand:
				{
					if (GetFreeItemCount() == 0 && !m_Equips[main].IsClear() && !m_Equips[Auxiliary].IsClear())
						return false;

					temp = m_Equips[main];
					temp2 = m_Equips[Auxiliary];

					m_Equips[main] = *pitem;
					m_Equips[Auxiliary].Clear();
				}
				break;
				case ItemBaseAttribute::MainHand:
				{
					if (!m_Equips[Auxiliary].IsClear())
					{
						if (m_Equips[Auxiliary].GetItemBaseAttribute()->EType == ItemBaseAttribute::Weapon)
							if (m_Equips[Auxiliary].GetItemBaseAttribute()->eEquipHand != pitem->GetItemBaseAttribute()->eEquipHand)
								return false;
					}

					temp = m_Equips[main];
					m_Equips[main] = *pitem;
				}
				break;
				case ItemBaseAttribute::AuxiliaryHand:
				{
					return false;
				}
				break;
				default:
					ASSERT(0);
					return false;
				}
			}
			break;
			}

			ChangeAttackSpeed();
		}
		break;
		case EquipAuxiliaryHand1:
		{
			if (pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Weapon &&
				pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Shield)
				return false;

			switch (pitem->GetItemBaseAttribute()->EType)
			{
			case ItemBaseAttribute::Weapon:
			{
				switch (pitem->GetItemBaseAttribute()->eEquipHand)
				{
				case ItemBaseAttribute::OneHand:
				{
					if (!m_Equips[main].IsClear())
					{
						if (m_Equips[main].GetItemBaseAttribute()->eEquipHand != pitem->GetItemBaseAttribute()->eEquipHand)
							return false;
					}

					temp = m_Equips[Auxiliary];
					m_Equips[Auxiliary] = *pitem;
				}
				break;
				case ItemBaseAttribute::TwoHand:
				{
					return false;
				}
				break;
				case ItemBaseAttribute::MainHand:
				{
					return false;
				}
				break;
				case ItemBaseAttribute::AuxiliaryHand:
				{
					if (!m_Equips[main].IsClear())
					{
						if (m_Equips[main].GetItemBaseAttribute()->eEquipHand != pitem->GetItemBaseAttribute()->eEquipHand)
							return false;
					}

					temp = m_Equips[Auxiliary];
					m_Equips[Auxiliary] = *pitem;
				}
				break;
				default:
					ASSERT(0);
					return false;
				}

				ChangeAttackSpeed();
			}
			break;
			case ItemBaseAttribute::Shield:
			{
				if (m_Equips[main].IsClear() && m_Equips[Auxiliary].IsClear())
				{
					temp = m_Equips[Auxiliary];
					m_Equips[Auxiliary] = *pitem;
				}
				else if (m_Equips[main].IsClear() && !m_Equips[Auxiliary].IsClear())
				{
					temp = m_Equips[Auxiliary];
					m_Equips[Auxiliary] = *pitem;
				}
				else
				{
					if (m_Equips[main].GetItemBaseAttribute()->eEquipHand == ItemBaseAttribute::TwoHand)
					{
						if (GetFreeItemCount() == 0 && !m_Equips[main].IsClear() && !m_Equips[Auxiliary].IsClear())
							return false;

						temp = m_Equips[Auxiliary];
						temp2 = m_Equips[main];

						m_Equips[Auxiliary] = *pitem;
						m_Equips[main].Clear();
					}
					else if (pitem->GetItemBaseAttribute()->EType == ItemBaseAttribute::Weapon &&
						m_Equips[main].GetItemBaseAttribute()->eEquipHand == pitem->GetItemBaseAttribute()->eEquipHand)
					{
						temp = m_Equips[Auxiliary];
						m_Equips[Auxiliary] = *pitem;
					}
					else if (pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Weapon)
					{
						temp = m_Equips[Auxiliary];
						m_Equips[Auxiliary] = *pitem;
					}
					else
						return false;
				}

				ChangeAttackSpeed();
			}
			break;
			}
		}
		break;
		case EquipHead:
			if (pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Head)
				return false;

			temp = m_Equips[EquipHead];
			m_Equips[EquipHead] = *pitem;
			break;
		case EquipAmulet:
			if (pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Amulet)
				return false;

			temp = m_Equips[EquipAmulet];
			m_Equips[EquipAmulet] = *pitem;
			break;
		case EquipGlove:
			if (pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Glove)
				return false;

			temp = m_Equips[EquipGlove];
			m_Equips[EquipGlove] = *pitem;
			break;
		case EquipArmor:
			if (pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Armor &&
				pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Cloth)
				return false;

			temp = m_Equips[EquipArmor];
			m_Equips[EquipArmor] = *pitem;
			break;
		case EquipCuff:
			if (pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Cuff)
				return false;

			temp = m_Equips[EquipCuff];
			m_Equips[EquipCuff] = *pitem;
			break;
		case EquipGirdle:
			if (pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Girdle)
				return false;

			temp = m_Equips[EquipGirdle];
			m_Equips[EquipGirdle] = *pitem;
			break;
		case EquipShoes:
			if (pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Shoes)
				return false;

			temp = m_Equips[EquipShoes];
			m_Equips[EquipShoes] = *pitem;
			break;
		case EquipLeggins:
			if (pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Leggins)
				return false;

			temp = m_Equips[EquipLeggins];
			m_Equips[EquipLeggins] = *pitem;
			break;
		case EquipRing1:
			if (pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Ring)
				return false;

			temp = m_Equips[EquipRing1];
			m_Equips[EquipRing1] = *pitem;
			break;
		case EquipRing2:
			if (pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Ring)
				return false;

			temp = m_Equips[EquipRing2];
			m_Equips[EquipRing2] = *pitem;
			break;
		default:
			return false;
		}

		m_Bags[index].Push(&temp, num);

		equip.Increase(&m_EquipData);
		temp.Decrease(&m_EquipData);

		if (!temp2.IsClear())
		{
			AddItem(&temp2);
			temp2.Decrease(&m_EquipData);
		}

		ChangeHp();
		ChangeAttackSpeed();

		SynGameData(true);
		SynClientPassiveSkillData();

		return true;
	}
	else
	{
		return false;
	}
}

void CPlayer::SendEquipTimeEnd(int index, int type, int bagnum)
{
	if (!type)
	{
		m_Equips[index].Decrease(&m_EquipData);

		if (m_Equips[index].GetItemBaseAttribute()->EType == ItemBaseAttribute::Weapon)
		{
			m_Equips[index].Clear();
			ChangeAttackSpeed();
		}
		else
			m_Equips[index].Clear();

		ChangeHp();
		SynGameData(true);
		SynClientPassiveSkillData();

		msg_move_item.Base = -1;
		msg_move_item.uiID = this->GetID();
		msg_move_item.Index = index;
		msg_move_item.Num = -1;
		msg_move_item.IndexAim = -1;
		msg_move_item.NumAim = -1;

		GetRegion()->SendAreaMsgOneToOther(GetCurrentCell(), &msg_move_item);
	}
	else
	{
		Item* pItem = m_Bags[index].GetItem(bagnum);
		ASSERT(pItem);
		if (pItem)
		{
			ItemBaseAttribute* pItemBaseInfo = pItem->GetItemBaseAttribute();
			ASSERT(pItemBaseInfo);
			if (pItemBaseInfo)
			{
				if (pItemBaseInfo->UndoSkillID)
				{
					if (GetRegion()->Skill(this, this, GetPosX(), 0, GetPosZ(), pItemBaseInfo->UndoSkillID, 0, 0, 0) == NO_MSG_ERRO)
					{
						;
					}
				}
			}
		}
		msg_move_item.uiID = this->GetID();
		msg_move_item.Index = index;
		msg_move_item.Num = bagnum;
		msg_move_item.IndexAim = -1;
		msg_move_item.NumAim = -1;

		s_World->SendMsgToClient(&msg_move_item, GetSocket());

	}
}

int	CPlayer::GetNumOfItem(const int _ID)
{
	int	count = 0;
	ItemBaseAttribute* pItemBaseInfo = NULL;
	Item* pItem = NULL;
	for (int i = 0; i < MAX_BAGS; i++)
	{
		for (int j = 0; j < MAX_BAG_GRID; j++)
		{
			pItem = m_Bags[i].GetItem(j);
			if (!pItem)
				continue;
			pItemBaseInfo = pItem->GetItemBaseAttribute();
			if (!pItemBaseInfo)
				continue;
			if (_ID == pItemBaseInfo->ID)
				count += pItem->m_Overlap;
		}
	}
	return	count;
}

bool	CPlayer::UseItem(const int _ID, int _Num)
{
	ItemBaseAttribute* pItemBaseInfo = NULL;
	Item* pItem = NULL;
	for (int i = 0; i < MAX_BAGS; i++)
	{
		for (int j = 0; j < MAX_BAG_GRID; j++)
		{
			pItem = m_Bags[i].GetItem(j);
			if (!pItem)
				continue;
			pItemBaseInfo = pItem->GetItemBaseAttribute();
			if (!pItemBaseInfo)
				continue;
			if (_ID == pItemBaseInfo->ID)
			{
				(pItem->m_Overlap)--;
				if (pItem->m_Overlap <= 0)
					pItem->Clear();

				return	true;
			}
		}
	}
	return	false;
}

void CPlayer::ClearExpiredItem()
{
	ItemBaseAttribute* ItemBaseInfo = NULL;
	for (int i = 0; i < EquipCount; i++)
	{
		ItemBaseInfo = m_Equips[i].GetItemBaseAttribute();
		if (!ItemBaseInfo)
			continue;

		if (0 == ItemBaseInfo->Mode && m_Equips[i].IsActivated)
		{
			if (IsExpired(&(m_Equips[i].ExpiredDate)))
			{
				SendEquipTimeEnd(i, 0, 0);
			}
		}
	}
	Item* pItem = NULL;
	for (int i = 0; i < MAX_BAGS; i++)
	{
		for (int j = 0; j < MAX_BAG_GRID; j++)
		{
			pItem = m_Bags[i].GetItem(j);
			if (!pItem)
				continue;

			ItemBaseInfo = pItem->GetItemBaseAttribute();
			if (!ItemBaseInfo)
				continue;

			if (0 == ItemBaseInfo->Mode && pItem->IsActivated)
			{
				if (IsExpired((&pItem->ExpiredDate)))
				{
					SendEquipTimeEnd(i, 1, j);
				}
			}
		}
	}

	vector<Item>::iterator iter = MallItemInUseList.begin();
	vector<Item>::iterator iter_end = MallItemInUseList.end();
	for (; iter != iter_end; iter++)
	{
		if (IsExpired(&(iter->ExpiredDate)))
		{
			if (0 == iter->GetItemBaseAttribute()->UndoSkillID)
			{
				MallItemInUseList.erase(iter);
				iter = MallItemInUseList.begin();
				iter_end = MallItemInUseList.end();
				if (iter == iter_end)
					break;
			}
			else if (GetRegion()->Skill(this, this, GetPosX(), 0, GetPosZ(), iter->GetItemBaseAttribute()->UndoSkillID, 0, 0, 0) == NO_MSG_ERRO)
			{
				MallItemInUseList.erase(iter);
				iter = MallItemInUseList.begin();
				iter_end = MallItemInUseList.end();
				if (iter == iter_end)
					break;;
			}
		}
	}

}
bool	CPlayer::IsExpired(tm* _pTime)
{
	time_t	timep;
	time(&timep);
	tm* tm_buf = localtime(&timep);
	timep = _mkgmtime(tm_buf);
	if (timep > _mkgmtime(_pTime))
		return	true;
	return	false;
}
void	CPlayer::SetItemActivated(Item* _pItem, bool _IsActivated)
{
	if (!_pItem)
		return;
	Item ItemBuf = *_pItem;
	_pItem = &ItemBuf;
	ItemBaseAttribute* ItemBaseInfo = NULL;
	ItemBaseInfo = _pItem->GetItemBaseAttribute();
	if (!ItemBaseInfo)
		return;
	if (_pItem->IsActivated)
		return;
	time_t	time_buf;
	time(&time_buf);
	_pItem->ExpiredDate = *localtime(&time_buf);
	_pItem->ExpiredDate.tm_sec += ItemBaseInfo->ExpiredTime.tm_sec;
	_pItem->ExpiredDate.tm_min += ItemBaseInfo->ExpiredTime.tm_min;
	_pItem->ExpiredDate.tm_hour += ItemBaseInfo->ExpiredTime.tm_hour;
	_pItem->ExpiredDate.tm_mday += ItemBaseInfo->ExpiredTime.tm_mday;
	_pItem->ExpiredDate.tm_mon += ItemBaseInfo->ExpiredTime.tm_mon;
	_pItem->ExpiredDate.tm_year += ItemBaseInfo->ExpiredTime.tm_year;
	time_buf = _mkgmtime(&(_pItem->ExpiredDate));

	_pItem->IsActivated = true;

	vector<Item>::iterator itor = MallItemInUseList.begin();
	vector<Item>::iterator itor_end = MallItemInUseList.end();
	for (; itor != itor_end; itor++)
	{
		if ((*itor).GetItemBaseAttribute()->ID == ItemBaseInfo->ID)
		{
			MallItemInUseList.erase(itor);
			break;
		}
	}
	MallItemInUseList.push_back(*_pItem);
	int i = MallItemInUseList.size();

	return;
}

bool CPlayer::UseOrEquip(int index, int num)
{

	if (IsDead())
		return false;

	Item* pitem = m_Bags[index].GetItem(num);


	if (pitem->m_Lock)
		return false;

	if (pitem->GetEquipLV() > m_ObjectData.m_cRank)
	{
		return false;
	}

	if (!pitem->HasClass(m_ObjectData.m_lClass))
		return false;

	if (pitem->GetItemBaseAttribute()->Faction != m_ObjectData.m_lFaction &&
		pitem->GetItemBaseAttribute()->Faction != -1)
		return false;

	if (pitem->GetItemBaseAttribute()->Equip)
	{
		if (IsGM())
			return false;

		if (pitem->GetItemBaseAttribute()->EType > ItemBaseAttribute::Ring)
		{
			return false;
		}

		Item equip = *pitem;
		Item temp;
		Item temp2;


		int main = EquipMainHand1;
		int Auxiliary = EquipAuxiliaryHand1;

		pitem->m_Binding = true;

		if (pitem->GetItemBaseAttribute()->Mode >= 0)
			SetItemActivated(pitem, true);

		switch (pitem->GetItemBaseAttribute()->EType)
		{
		case ItemBaseAttribute::Weapon:
		{
			switch (pitem->GetItemBaseAttribute()->eEquipHand)
			{
			case ItemBaseAttribute::OneHand:
			{
				if (m_Equips[main].IsClear())
				{
					temp = m_Equips[main];
					m_Equips[main] = *pitem;
				}
				else if (!m_Equips[main].IsClear() && m_Equips[Auxiliary].IsClear())
				{
					if (m_Equips[main].GetItemBaseAttribute()->eEquipHand != pitem->GetItemBaseAttribute()->eEquipHand)
					{
						temp = m_Equips[main];
						m_Equips[main] = *pitem;
					}
					else
					{
						temp = m_Equips[Auxiliary];
						m_Equips[Auxiliary] = *pitem;
					}
				}
				else
				{
					if (m_Equips[Auxiliary].GetItemBaseAttribute()->EType != ItemBaseAttribute::Weapon)
					{
						temp = m_Equips[main];
						m_Equips[main] = *pitem;
					}
					else if (m_Equips[main].GetItemBaseAttribute()->eEquipHand == pitem->GetItemBaseAttribute()->eEquipHand)
					{
						temp = m_Equips[main];
						m_Equips[main] = *pitem;
					}
					else
						return false;
				}
			}
			break;
			case ItemBaseAttribute::TwoHand:
			{
				if (GetFreeItemCount() == 0 && !m_Equips[main].IsClear() && !m_Equips[Auxiliary].IsClear())
					return false;

				temp = m_Equips[main];
				temp2 = m_Equips[Auxiliary];

				m_Equips[main] = *pitem;
				m_Equips[Auxiliary].Clear();
			}
			break;
			case ItemBaseAttribute::MainHand:
			{
				if (m_Equips[main].IsClear() && m_Equips[Auxiliary].IsClear())
				{
					temp = m_Equips[main];
					m_Equips[main] = *pitem;
				}
				else if (!m_Equips[main].IsClear() && m_Equips[Auxiliary].IsClear())
				{
					temp = m_Equips[main];
					m_Equips[main] = *pitem;
				}
				else
				{
					if (m_Equips[Auxiliary].GetItemBaseAttribute()->EType == ItemBaseAttribute::Weapon &&
						m_Equips[Auxiliary].GetItemBaseAttribute()->eEquipHand == pitem->GetItemBaseAttribute()->eEquipHand)
					{
						temp = m_Equips[main];
						m_Equips[main] = *pitem;
					}
					else if (m_Equips[Auxiliary].GetItemBaseAttribute()->EType != ItemBaseAttribute::Weapon)
					{
						temp = m_Equips[main];
						m_Equips[main] = *pitem;
					}
					else
						return false;
				}
			}
			break;
			case ItemBaseAttribute::AuxiliaryHand:
			{
				if (m_Equips[main].IsClear() && m_Equips[Auxiliary].IsClear())
				{
					temp = m_Equips[Auxiliary];
					m_Equips[Auxiliary] = *pitem;
				}
				else if (m_Equips[main].IsClear() && !m_Equips[Auxiliary].IsClear())
				{
					temp = m_Equips[Auxiliary];
					m_Equips[Auxiliary] = *pitem;
				}
				else
				{
					if (m_Equips[main].GetItemBaseAttribute()->eEquipHand == ItemBaseAttribute::TwoHand)
					{
						if (GetFreeItemCount() == 0 && !m_Equips[main].IsClear() && !m_Equips[Auxiliary].IsClear())
							return false;

						temp = m_Equips[Auxiliary];
						temp2 = m_Equips[main];

						m_Equips[Auxiliary] = *pitem;
						m_Equips[main].Clear();
					}
					else if (pitem->GetItemBaseAttribute()->EType == ItemBaseAttribute::Weapon &&
						m_Equips[main].GetItemBaseAttribute()->eEquipHand == pitem->GetItemBaseAttribute()->eEquipHand)
					{
						temp = m_Equips[Auxiliary];
						m_Equips[Auxiliary] = *pitem;
					}
					else if (pitem->GetItemBaseAttribute()->EType != ItemBaseAttribute::Weapon)
					{
						temp = m_Equips[Auxiliary];
						m_Equips[Auxiliary] = *pitem;
					}
					else
						return false;
				}
			}
			break;
			default:
				ASSERT(0);
				return false;
				break;
			}
		}
		break;
		case ItemBaseAttribute::Shield:
		{
			if (m_Equips[main].IsClear() && m_Equips[Auxiliary].IsClear())
			{
				temp = m_Equips[Auxiliary];
				m_Equips[Auxiliary] = *pitem;
			}
			else if (m_Equips[main].IsClear() && !m_Equips[Auxiliary].IsClear())
			{
				temp = m_Equips[Auxiliary];
				m_Equips[Auxiliary] = *pitem;
			}
			else
			{
				if (m_Equips[main].GetItemBaseAttribute()->eEquipHand == ItemBaseAttribute::TwoHand)
				{
					if (GetFreeItemCount() == 0 && !m_Equips[main].IsClear() && !m_Equips[Auxiliary].IsClear())
						return false;

					temp = m_Equips[Auxiliary];
					temp2 = m_Equips[main];

					m_Equips[Auxiliary] = *pitem;
					m_Equips[main].Clear();
				}
				else
				{
					temp = m_Equips[Auxiliary];
					m_Equips[Auxiliary] = *pitem;
				}
			}
		}
		break;
		case ItemBaseAttribute::Head:
			temp = m_Equips[EquipHead];
			m_Equips[EquipHead] = *pitem;
			break;
		case ItemBaseAttribute::Amulet:
			temp = m_Equips[EquipAmulet];
			m_Equips[EquipAmulet] = *pitem;
			break;
		case ItemBaseAttribute::Glove:
			temp = m_Equips[EquipGlove];
			m_Equips[EquipGlove] = *pitem;
			break;
		case ItemBaseAttribute::Armor:
			temp = m_Equips[EquipArmor];
			m_Equips[EquipArmor] = *pitem;
			break;
		case ItemBaseAttribute::Cloth:
			temp = m_Equips[EquipArmor];
			m_Equips[EquipArmor] = *pitem;
			break;
		case ItemBaseAttribute::Cuff:
			temp = m_Equips[EquipCuff];
			m_Equips[EquipCuff] = *pitem;
			break;
		case ItemBaseAttribute::Girdle:
			temp = m_Equips[EquipGirdle];
			m_Equips[EquipGirdle] = *pitem;
			break;
		case ItemBaseAttribute::Shoes:
			temp = m_Equips[EquipShoes];
			m_Equips[EquipShoes] = *pitem;
			break;
		case ItemBaseAttribute::Leggins:
			temp = m_Equips[EquipLeggins];
			m_Equips[EquipLeggins] = *pitem;
			break;
		case ItemBaseAttribute::Ring:
			if (m_Equips[EquipRing1].IsClear())
				m_Equips[EquipRing1] = *pitem;
			else if (m_Equips[EquipRing2].IsClear())
				m_Equips[EquipRing2] = *pitem;
			else
			{
				temp = m_Equips[EquipRing1];
				m_Equips[EquipRing1] = *pitem;
			}
			break;

		default:
			ASSERT(0);
			return false;
			break;
		}



		m_Bags[index].Push(&temp, num);


		equip.Increase(&m_EquipData);
		temp.Decrease(&m_EquipData);

		if (!temp2.IsClear())
		{
			AddItem(&temp2);
			temp2.Decrease(&m_EquipData);
		}

		ChangeHp();
		ChangeAttackSpeed();

		SynGameData(true);
		SynClientPassiveSkillData();



		return true;
	}
	else
	{

		TempScriptItem = pitem;
		TempScriptIndex = index;
		TempScriptNum = num;

		if (pitem->GetItemBaseAttribute()->Overlap)
		{
			if ((pitem->GetItemBaseAttribute()->GoldBoxIdx != -1)
				|| (pitem->GetItemBaseAttribute()->KEY != -1))
			{
				if ((--pitem->m_Overlap) == 0)
					pitem->Clear();

				return true;
			}

			if (pitem->GetItemBaseAttribute()->SkillID == -1)
				return false;

			if (pitem->GetItemBaseAttribute()->SkillBook)
			{
				if (pitem->GetItemBaseAttribute()->PrevSkillID != -1 &&
					!FindActiveSkill(pitem->GetItemBaseAttribute()->PrevSkillID))
					return false;

				if (GetRegion()->Skill(this, this, GetPosX(), 0, GetPosZ(), pitem->GetItemBaseAttribute()->SkillID, 0, 0, 0) == NO_MSG_ERRO)
				{


					if (!AddActiveSkill(pitem->GetItemBaseAttribute()->SkillLearnID))
					{
						return false;
					}

					SynClientActiveSkillData();

					if ((--pitem->m_Overlap) == 0)
						pitem->Clear();

					return true;
				}

				return false;
			}

			if (0 <= pitem->GetItemBaseAttribute()->Mode)
			{
				SetItemActivated(pitem, true);
				if (GetRegion()->Skill(this, this, GetPosX(), 0, GetPosZ(), pitem->GetItemBaseAttribute()->SkillID, 0, 0, 0) == NO_MSG_ERRO)
				{
					if ((--pitem->m_Overlap) == 0)
						pitem->Clear();

					return	true;
				}
				return	false;
			}


			if (GetRegion()->Skill(this, this, GetPosX(), 0, GetPosZ(), pitem->GetItemBaseAttribute()->SkillID, 0, 0, 0) == NO_MSG_ERRO)
			{
				if ((--pitem->m_Overlap) == 0)
					pitem->Clear();

			}
			else
				return false;
		}

		return true;
	}

}


bool CPlayer::StorageToBag(int index)
{
	return m_Storage.Move(index, GetuiBagNum());
}

bool CPlayer::StorageToBag(int index, int bagindex, int bagnum)
{
	if (bagindex < 0 || bagindex >= MAX_BAGS)
	{
		return false;
	}

	return m_Storage.Move(index, bagindex, bagnum);
}

bool CPlayer::BagToStoragePly(int bagindex, int bagnum, int num)
{
	if (bagindex < 0 || bagindex >= MAX_BAGS)
	{
		return false;
	}

	return m_Bags[bagindex].MoveBag(&m_Storage, bagnum, num);
}

bool CPlayer::BagToStorage(int bagindex, int bagnum, int index)
{
	if (bagindex < 0 || bagindex >= MAX_BAGS)
	{
		return false;
	}

	return m_Bags[bagindex].Move(&m_Storage, bagnum, index);
}

