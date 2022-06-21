#include "stdafx.h"
#include "Player.h"
#include "..\ServiceLoader.h"
#include "SkillManager.h"
#include ".\Mat.h"
#include "../World.h"

extern MSG_MAGIC_DAMAGE msg_magic;
extern MSG_SKILL msg_skill;



void CPlayer::AddPassiveSkill(int ucSkillID, int iRank, int)
{
	m_PassiveSkill[ucSkillID].Grade = iRank;
	UpdateAttributeInfo(ucSkillID, TYPE_ADD);
}

bool CPlayer::DelPassiveSkill(int ucSkillID)
{
	UpdateAttributeInfo(ucSkillID, TYPE_DELETE);
	CGameObject::DelPassiveSkill(ucSkillID);
	return true;
}

bool CPlayer::FindPassiveSkill(int ucSkillID, UINT*)
{
	map< UINT, PASSIVEINFO >::const_iterator itor = m_PassiveSkill.find(ucSkillID);
	if (itor != m_PassiveSkill.end())
	{
		return true;
	}
	return false;
}

bool CPlayer::AddActiveSkill(int ucSkillID)
{
	return CGameObject::AddActiveSkill(ucSkillID);
}

void CPlayer::DelActiveSkill(int ucSkillID)
{
	CGameObject::DelActiveSkill(ucSkillID);
}

bool CPlayer::FindActiveSkill(int ucSkillID)
{
	return CGameObject::FindActiveSkill(ucSkillID);
}

bool CPlayer::IsActiveSkillCooling(int ucSkillID)
{
	return (m_ActiveSkill[ucSkillID].Time_style == TIME_STYLE_COOL) ? true : false;

}

void CPlayer::ChangeActiveSkillStatus(int ucSkillID, TIME_STYLE status)
{
	const MagicData* pActiveSkill = CGameObject::s_World->g_pSkillManager->GetMagic(ucSkillID);
	if (pActiveSkill != NULL)
	{
		m_ActiveSkill[ucSkillID].Time_style = status;
		m_ActiveSkill[ucSkillID].Time_interval = (UINT)pActiveSkill->usCoolingTime;
		if (ucSkillID >= GODSKILL_ID && ucSkillID < GODSKILL_ID + 40)
		{
			m_GodLastTime = time(NULL);
		}
	}

}

bool CPlayer::SetActiveSkillStartTime(int ucSkillID)
{
	map<UINT, ACTIVE_INFO> ::iterator itor = m_ActiveSkill.find(ucSkillID);
	if (itor != m_ActiveSkill.end())
	{
		(*itor).second.Time_start = time(NULL);
		return true;
	}
	return false;

}

void CPlayer::SetCurActiveSkillID(int ucActiveSkillID)
{
	m_ucCurActiveSkill = ucActiveSkillID;
}

int	CPlayer::GetCurActiveSkillID()
{
	return (int)m_ucCurActiveSkill;
}

void CPlayer::SetLastMsg(int MsgID)
{
	m_cMsg = MsgID;
};

INT CPlayer::CalculateAttackDamage(int MagicID, CGameObject& pObj)
{
	return CGameObject::CalculateAttackDamage(MagicID, pObj);
}


void CPlayer::UpdateDB_ActiveSkill(CPlayer* pPlayer)
{
	dbtool::IDatabase* DB = CGameObject::s_World->GetDBAddr(DB_NONE);
	DB->ExecuteAsyncSQL(pPlayer->PackActiveSkill(), (void*)pPlayer, &CPlayer::DealWith_Update_ActiveSkill);
	InterlockedIncrement(&pPlayer->SynDBOutStand);
	return;
}
void CPlayer::UpdateDB_PassiveSkill(CPlayer* pPlayer)
{
	dbtool::IDatabase* DB = CGameObject::s_World->GetDBAddr(DB_NONE);
	DB->ExecuteAsyncSQL(pPlayer->PackPassiveSkill(), (void*)pPlayer, &CPlayer::DealWith_Update_PassiveSkill);
	InterlockedIncrement(&pPlayer->SynDBOutStand);
	return;
}

bool CPlayer::IsGodSkillTimeOut(int MagicID)
{
	if (MagicID < GODSKILL_ID)
	{
		return false;
	}
	else if (!IsActiveSkillCooling(MagicID))
	{
		m_ActiveSkill[MagicID].Time_style = TIME_STYLE_COOL;
		m_ActiveSkill[MagicID].Time_start = time(NULL);
		return true;
	}
	return true;
}

bool CPlayer::PassiveSkillUpgrade(UINT SkillID, int* Rank)
{

	switch (SkillID)
	{
	case 1:
	case 2:
	case 3:
	case 4:
		break;
	default:
		break;
	}

	*Rank = 0;
	return false;
}

UINT CPlayer::GetTotalSkillGrade()
{
	UINT a = 0;
	map< UINT, PASSIVEINFO >::const_iterator itor = m_PassiveSkill.begin();
	for (; itor != m_PassiveSkill.end(); itor++)
	{
		a += itor->second.Grade;
	}

	return a;
}

bool CPlayer::IsEquipThis(const MagicData* pMagic)
{
	if (pMagic->EquipRequest.size() == 0)
	{
		return true;
	}

	for (int i = 0; i < EquipCount; i++)
	{
		if (!m_Equips[i].IsClear())
		{
			for (std::vector<UINT>::const_iterator iter = pMagic->EquipRequest.begin(); iter != pMagic->EquipRequest.end(); iter++)
				if (m_Equips[i].GetItemBaseAttribute()->eSkillFlag == int(*iter))
					return true;
		}
	}

	return false;
}

bool CPlayer::IsEquipThis(const SKillData* pSkill)
{
	for (int i = 0; i < EquipCount; i++)
	{
		if (!m_Equips[i].IsClear())
		{
			for (std::vector<UINT>::const_iterator iter = pSkill->EquipRequest.begin(); iter != pSkill->EquipRequest.end(); iter++)
				if (m_Equips[i].GetItemBaseAttribute()->eSkillFlag == int(*iter))
					return true;
		}
	}

	return false;
}

bool CPlayer::ChanegeDegree(int ucSkillID, int)
{
	const SKillData* pskill = CGameObject::s_World->g_pSkillManager->GetSkill(ucSkillID);
	if (NULL == pskill)
	{
		return false;
	}

	if (m_PassiveSkill[ucSkillID].Grade >= 40)
	{
		return false;
	}

	if (!FindPassiveSkill(ucSkillID) || !FindPassiveSkill(pskill->PrefixID))
	{
		return false;
	}

	if (m_PassiveSkill[pskill->PrefixID].Grade < pskill->RrefixRank)
	{
		return false;
	}

	if (pskill->PrefixID != ucSkillID && m_PassiveSkill[pskill->PrefixID].Grade <= m_PassiveSkill[ucSkillID].Grade)
	{
		return false;
	}

	if (GetTotalSkillGrade() < pskill->TotalRank)
	{
		return false;
	}

	UINT RequestPoint = CPeizhi::Instance()->GetPlayerSkillGradePoint(m_PassiveSkill[pskill->ucID].Grade, pskill->ucID);
	if (m_ConvertPoint < RequestPoint)
	{
		return false;
	}

	m_PassiveSkill[ucSkillID].Grade++;
	m_ConvertPoint -= RequestPoint;

	UpdateAttributeInfo(ucSkillID);

	SynGameData(true);

	MSG_SKILL_UPGRADE Skill_UpGrade;
	Skill_UpGrade.Head.usSize = sizeof(MSG_SKILL_UPGRADE);
	Skill_UpGrade.Head.usType = _MSG_SKILL_UPGRADE;
	Skill_UpGrade.uiSkillID = pskill->ucID;
	Skill_UpGrade.uiPlayerID = GetID();
	Skill_UpGrade.uiCurrentSkillGrade = m_PassiveSkill[ucSkillID].Grade;
	Skill_UpGrade.uiNextSkillGradePoint = CPeizhi::Instance()->GetPlayerSkillGradePoint(m_PassiveSkill[ucSkillID].Grade, pskill->ucID);
	Skill_UpGrade.uiSurplusPoint = m_ConvertPoint;
	CGameObject::s_World->SendMsgToClient(&Skill_UpGrade, GetSocket());

	return true;
}


bool CPlayer::CalculatePassivePracticeDegree(PhysiacalAttack, ATTACK_TYPE, bool)
{
	return true;
}


void CPlayer::BeMagicAttackPassivePracticeDegree(UINT, ATTACK_TYPE)
{

}


Magic_Attack CPlayer::JudgeMagicStyle(UINT MagicID)
{
	const MagicData* mgc = CGameObject::s_World->g_pSkillManager->GetMagic(MagicID);

	if (mgc->iProperty == PROPERTY_PHY)
	{
		return MAGIC_PHY;
	}
	else if (mgc->iProperty == PROPERTY_MAG)
	{
		return MAGIC_MAGIC;
	}
	else if (mgc->iProperty == PROPERTY_CURE)
	{
		return MAGIC_IATRIC;
	}
	else
	{
		return MAGIC_FLY;

	}

}

void CPlayer::CalculatePassivePracticeDegree(int, ATTACK_TYPE, bool)
{

}



void CPlayer::ResetTimer(float IntervalTime, TIME_STYLE timeStyle)
{
	if (timeStyle == TIME_STYLE_CAST)
	{
		m_ActiveSkillCast_Timer.Startup(IntervalTime * 1000);
	}
	else if (timeStyle == TIME_STYLE_COOL)
	{
		m_ActiveSkillCool_Timer.Startup(IntervalTime);
	}

}


bool CPlayer::IsRefresh(TIME_STYLE timeStyle)
{

	if (timeStyle == TIME_STYLE_CAST)
	{
		return m_ActiveSkillCast_Timer.IsExpire() ? m_ActiveSkillCast_Timer.TimeOver() : false;

	}
	else if (timeStyle == TIME_STYLE_COOL)
	{
		map< UINT, ACTIVE_INFO > ::iterator itor = m_ActiveSkill.begin();
		for (; itor != m_ActiveSkill.end(); itor++)
		{
			ACTIVE_INFO active_info = itor->second;
			int MagicID = itor->first;
			if (m_ActiveSkillCool_Timer.IsExpire(active_info.Time_start, active_info.Time_interval) && active_info.Time_style == TIME_STYLE_COOL)
			{
				ChangeActiveSkillStatus(MagicID, TIME_STYLE_NONE);
			}
		}
		return true;
	}
	else if (timeStyle == TIME_STYLE_NONE)
	{

	}
	return true;
}

void CPlayer::SendNoteToClient(const char* Note)
{
	MSG_SERVER_NOTE msg_note;
	msg_note.Head.usSize = sizeof(MSG_SERVER_NOTE);
	msg_note.Head.usType = _MSG_SERVER_NOTE;
	strcpy(msg_note.cNote, Note);
	CGameObject::s_World->SendMsgToClient(&msg_note, GetSocket());
}
void CPlayer::SendSysNoteToClient(const char* Note)
{
	WCHAR unFound[256];
	memset(unFound, 0L, sizeof(unFound));
	int len = MultiByteToWideChar(CP_ACP, 0, Note, -1, unFound, 256);
	MSG_TALK	msg_talk;
	msg_talk.ucAimPlayerNameSize = 0;
	msg_talk.ucResPlayerNameSize = 0;
	msg_talk.ucTextSize = (INT)((len + 1) * 2);
	msg_talk.ucType = defSYSTEM;
	msg_talk.uiUserID = -1;
	wmemset(msg_talk.wcContent, 0, 256);
	wcscpy(msg_talk.wcContent, unFound);
	msg_talk.Head.usType = _MSG_TALK;
	msg_talk.Head.usSize = sizeof(msg_talk.Head) + 4 + 4 + 4;
	if ((msg_talk.ucTextSize % 4) != 0)
		msg_talk.Head.usSize += msg_talk.ucTextSize + 2;
	else
		msg_talk.Head.usSize += msg_talk.ucTextSize;

	CGameObject::s_World->SendMsgToClient(&msg_talk, GetSocket());
}
void CPlayer::SendMidNoteToClient(const char* Note)
{
	WCHAR unFound[256];
	memset(unFound, 0L, sizeof(unFound));
	int len = MultiByteToWideChar(CP_ACP, 0, Note, -1, unFound, 256);
	MSG_TALK	msg_talk;
	msg_talk.ucAimPlayerNameSize = 0;
	msg_talk.ucResPlayerNameSize = 0;
	msg_talk.ucTextSize = (INT)((len + 1) * 2);
	msg_talk.ucType = defGM;
	msg_talk.uiUserID = -1;
	wmemset(msg_talk.wcContent, 0, 256);
	wcscpy(msg_talk.wcContent, unFound);
	msg_talk.Head.usType = _MSG_TALK;
	msg_talk.Head.usSize = sizeof(msg_talk.Head) + 4 + 4 + 4;
	if ((msg_talk.ucTextSize % 4) != 0)
		msg_talk.Head.usSize += msg_talk.ucTextSize + 2;
	else
		msg_talk.Head.usSize += msg_talk.ucTextSize;

	CGameObject::s_World->SendMsgToClient(&msg_talk, GetSocket());
}


