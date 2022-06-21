#include "stdafx.h"
#include "Player.h"
#include "..\ServiceLoader.h"

int CPlayer::CreateTeam_New()
{
	if (m_pTeamLeader != NULL)
	{
		return -1;
	}
	else
	{
		m_pTeamLeader = this;

		MSG_TIP msg_tip;
		msg_tip.Head.usSize = sizeof(MSG_TIP);
		msg_tip.Head.usType = _MSG_TEAM_TIP;
		wmemset(msg_tip.chName, 0, 64);
		wmemset(msg_tip.tip, 0, 128);
		std::wstring buf = L"\0";
		std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
		buf = L"你创建了一支队伍";

		std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
		CGameObject::s_World->SendMsgToClient(&msg_tip, (this)->GetSocket());
		m_Team_Timer.Clear();
		m_Team_Timer.Startup(1);
		return 1;
	}
}

int CPlayer::AddTeammate_New(CPlayer* _pPlayer)
{
	if (!_pPlayer)
	{
		return -1;
	}

	if (_pPlayer->m_pTeamLeader != NULL)
	{
		MSG_TIP msg_tip;
		msg_tip.Head.usSize = sizeof(MSG_TIP);
		msg_tip.Head.usType = _MSG_TEAM_TIP;
		wmemset(msg_tip.chName, 0, 64);
		wmemset(msg_tip.tip, 0, 128);
		WCHAR wcBuf[64];
		wmemset(wcBuf, 0, 64);
		MultiByteToWideChar(CP_ACP, 0, _pPlayer->GetName(), -1, wcBuf, 64);
		std::wstring buf = wcBuf;
		std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
		buf = L"已经有自己的队伍";
		std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
		CGameObject::s_World->SendMsgToClient(&msg_tip, (this)->GetSocket());
		return 0;
	}

	if (m_pTeamLeader == NULL)
	{
		CreateTeam_New();
		return AddTeammate_New(_pPlayer);
	}

	if (this == m_pTeamLeader)
	{
		if (m_Teammates.size() < Max_TEAMMATES_NUM)
		{
			MSG_TIP msg_tip;
			msg_tip.Head.usSize = sizeof(MSG_TIP);
			msg_tip.Head.usType = _MSG_TEAM_TIP;
			wmemset(msg_tip.chName, 0, 64);
			wmemset(msg_tip.tip, 0, 128);
			WCHAR wcBuf[64];
			wmemset(wcBuf, 0, 64);
			MultiByteToWideChar(CP_ACP, 0, _pPlayer->GetName(), -1, wcBuf, 64);
			std::wstring buf = wcBuf;
			std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
			buf = L"加入了队伍";
			std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());

			for (vector<CPlayer*>::iterator iter = m_Teammates.begin();
				iter != m_Teammates.end();
				iter++)
			{
				CGameObject::s_World->SendMsgToClient(&msg_tip, (*iter)->GetSocket());
			}
			CGameObject::s_World->SendMsgToClient(&msg_tip, (this)->GetSocket());
			_pPlayer->m_pTeamLeader = this;
			m_Teammates.push_back(_pPlayer);

			wmemset(msg_tip.chName, 0, 64);
			wmemset(msg_tip.tip, 0, 128);
			buf = L"\0";
			std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
			buf = L"你已加入队伍";
			std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
			CGameObject::s_World->SendMsgToClient(&msg_tip, (_pPlayer)->GetSocket());
			UpdateTeam_New(false);
		}
		else
		{
			MSG_TIP msg_tip;
			msg_tip.Head.usSize = sizeof(MSG_TIP);
			msg_tip.Head.usType = _MSG_TEAM_TIP;
			wmemset(msg_tip.chName, 0, 64);
			wmemset(msg_tip.tip, 0, 128);
			std::wstring buf = L"\0";
			std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
			buf = L"队伍已满";
			std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
			CGameObject::s_World->SendMsgToClient(&msg_tip, (this)->GetSocket());
			return -1;
		}
	}
	else
	{
		MSG_TIP msg_tip;
		msg_tip.Head.usSize = sizeof(MSG_TIP);
		msg_tip.Head.usType = _MSG_TEAM_TIP;
		wmemset(msg_tip.chName, 0, 64);
		wmemset(msg_tip.tip, 0, 128);
		std::wstring buf = L"\0";
		std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
		buf = L"你不是队长，没有权限添加队员";
		std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
		CGameObject::s_World->SendMsgToClient(&msg_tip, (this)->GetSocket());
		return -1;
	}

	return 0;
}

bool CPlayer::IsTeammate_New(const CPlayer* _pPlayer)
{
	if (!_pPlayer)
		return false;
	if (m_pTeamLeader)
	{
		vector<CPlayer*>::iterator iter;
		for (iter = m_pTeamLeader->m_Teammates.begin();
			iter != m_pTeamLeader->m_Teammates.end();
			iter++)
		{
			if (_pPlayer == (*iter))
			{
				return true;
			}
		}
	}

	return false;
}

int CPlayer::RemoveTeammate_New(CPlayer* _pPlayer)
{
	if (!_pPlayer)
		return 0;
	if (this == m_pTeamLeader)
	{
		vector<CPlayer*>::iterator iter;

		if (1 == m_Teammates.size())
		{
			iter = m_Teammates.begin();
			if ((*iter) == _pPlayer)
			{
				ClearTeam_New();
				return 1;
			}

		}

		for (iter = m_pTeamLeader->m_Teammates.begin();
			iter != m_pTeamLeader->m_Teammates.end();
			iter++)
		{
			if (_pPlayer == (*iter))
			{
				(*iter)->m_pTeamLeader = NULL;

				MSG_TEAM_DESTROY msg;
				msg.Head.usSize = sizeof(MSG_TEAM_DESTROY);
				msg.Head.usType = _MSG_TEAM_DESTROY;
				MSG_TIP msg_tip;
				msg_tip.Head.usSize = sizeof(MSG_TIP);
				msg_tip.Head.usType = _MSG_TEAM_TIP;
				wmemset(msg_tip.chName, 0, 64);
				wmemset(msg_tip.tip, 0, 128);
				std::wstring buf = L"\0";
				std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
				buf = L"你已离开队伍";
				std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());

				CGameObject::s_World->SendMsgToClient(&msg, (*iter)->GetSocket());
				CGameObject::s_World->SendMsgToClient(&msg_tip, (*iter)->GetSocket());

				m_pTeamLeader->m_Teammates.erase(iter);

				WCHAR wcBuf[64];
				MultiByteToWideChar(CP_ACP, 0, _pPlayer->GetName(), -1, wcBuf, 64);
				buf = wcBuf;
				wmemset(msg_tip.chName, 0, 64);
				wmemset(msg_tip.tip, 0, 128);
				std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
				buf = L"已离开队伍";
				std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
				CGameObject::s_World->SendMsgToClient(&msg_tip, (this)->GetSocket());
				for (vector<CPlayer*>::iterator iter = m_Teammates.begin();
					iter != m_Teammates.end();
					iter++)
				{
					CGameObject::s_World->SendMsgToClient(&msg_tip, (*iter)->GetSocket());
				}

				UpdateTeam_New(false);
				return 1;
			}
		}
	}

	return 0;
}

int CPlayer::ClearTeam_New()
{
	if (this == m_pTeamLeader)
	{
		MSG_TEAM_DESTROY msg;
		msg.Head.usSize = sizeof(MSG_TEAM_DESTROY);
		msg.Head.usType = _MSG_TEAM_DESTROY;

		MSG_TIP msg_tip;
		msg_tip.Head.usSize = sizeof(MSG_TIP);
		msg_tip.Head.usType = _MSG_TEAM_TIP;
		wmemset(msg_tip.chName, 0, 64);
		wmemset(msg_tip.tip, 0, 128);
		std::wstring buf = L"\0";
		std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
		buf = L"队伍解散";
		std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());


		vector<CPlayer*>::iterator iter;
		for (iter = m_pTeamLeader->m_Teammates.begin();
			iter != m_pTeamLeader->m_Teammates.end();
			iter++)
		{
			(*iter)->m_pTeamLeader = NULL;
			CGameObject::s_World->SendMsgToClient(&msg, (*iter)->GetSocket());
			CGameObject::s_World->SendMsgToClient(&msg_tip, (*iter)->GetSocket());
		}
		this->m_pTeamLeader = NULL;
		this->m_Teammates.clear();
		CGameObject::s_World->SendMsgToClient(&msg, this->GetSocket());
		CGameObject::s_World->SendMsgToClient(&msg_tip, (this)->GetSocket());

	}
	return -1;
}
bool CPlayer::ChangeLeader_New(CPlayer* _pPlayer)
{
	if (!_pPlayer)
		return false;
	if (this == m_pTeamLeader)
	{
		if (IsTeammate_New(_pPlayer))
		{
			vector<CPlayer*>::iterator iter = std::find(m_pTeamLeader->m_Teammates.begin(), m_pTeamLeader->m_Teammates.end(), _pPlayer);
			if (iter != m_pTeamLeader->m_Teammates.end())
			{
				this->m_Teammates.erase(iter);
			}

			for (iter = m_pTeamLeader->m_Teammates.begin();
				iter != m_pTeamLeader->m_Teammates.end();
				iter++)
			{
				(*iter)->m_pTeamLeader = _pPlayer;
			}

			_pPlayer->m_pTeamLeader = _pPlayer;
			_pPlayer->m_Teammates = m_Teammates;
			this->m_Teammates.clear();
			this->m_pTeamLeader = _pPlayer;
			this->m_Team_Timer.Clear();

			_pPlayer->m_Teammates.push_back(this);
			_pPlayer->UpdateTeam_New(false);

			MSG_TIP msg_tip;
			msg_tip.Head.usSize = sizeof(MSG_TIP);
			msg_tip.Head.usType = _MSG_TEAM_TIP;
			wmemset(msg_tip.chName, 0, 64);
			wmemset(msg_tip.tip, 0, 128);
			WCHAR wcBuf[64];
			wmemset(wcBuf, 0, 64);
			MultiByteToWideChar(CP_ACP, 0, _pPlayer->GetName(), -1, wcBuf, 64);
			std::wstring buf = wcBuf;
			std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
			buf = L"成为了队长";
			std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
			for (iter = _pPlayer->m_Teammates.begin();
				iter != _pPlayer->m_Teammates.end();
				iter++)
			{
				CGameObject::s_World->SendMsgToClient(&msg_tip, (*iter)->GetSocket());
			}

			wmemset(msg_tip.chName, 0, 64);
			wmemset(msg_tip.tip, 0, 128);
			buf = L"\0";
			std::wcsncpy(msg_tip.chName, buf.c_str(), buf.length());
			buf = L"你已成为了队长";
			std::wcsncpy(msg_tip.tip, buf.c_str(), buf.length());
			CGameObject::s_World->SendMsgToClient(&msg_tip, (_pPlayer)->GetSocket());
			(_pPlayer)->m_Team_Timer.Clear();
			(_pPlayer)->m_Team_Timer.Startup(0.5);


			return true;
		}
	}

	return false;
}

bool CPlayer::LeaveTeam_New()
{
	if (m_pTeamLeader)
	{
		if (this != m_pTeamLeader)
		{
			m_pTeamLeader->RemoveTeammate_New(this);
			return true;
		}
		else
		{
			if (ChangeLeader_New(this->m_Teammates.at(0)))
			{
				m_pTeamLeader->RemoveTeammate_New(this);
				return true;
			}
		}
	}
	return false;
}

int CPlayer::UpdateTeam_New(bool _bIsDateUpdate)
{
	if (this == m_pTeamLeader)
	{
		MSG_TEAM_REFLASH msg;

		msg.Head.usSize = sizeof(MSG_TEAM_REFLASH);
		msg.Head.usType = _MSG_TEAM_REFLASH;

		msg.lTeamLeaderID = this->GetID();
		msg.uiTeamLeader_HP = this->GetlHP();
		msg.uiTeamLeader_MaxHP = this->GetMaxHP();
		msg.uiTeamLeader_Lv = this->GetRank();
		msg.ucTeamLeader_Class = this->GetlClass();
		strcpy(msg.chTeamLeader_Name, this->GetName());
		msg.usTeamLeader_MapID = this->GetRegion()->GetID();
		msg.ufTeamLeader_X = this->GetPosX();
		msg.ufTeamLeader_Y = this->GetPosY();
		msg.ufTeamLeader_Z = this->GetPosZ();

		msg.lTeammate0_ID = m_pTeamLeader->m_Teammates.at(0)->GetID();
		msg.uiTeammate0_HP = m_pTeamLeader->m_Teammates.at(0)->GetlHP();
		msg.uiTeammate0_MaxHP = m_pTeamLeader->m_Teammates.at(0)->GetMaxHP();
		msg.uiTeammate0_Lv = m_pTeamLeader->m_Teammates.at(0)->GetRank();
		msg.ucTeammate0_Class = m_pTeamLeader->m_Teammates.at(0)->GetlClass();
		strcpy(msg.chTeammate0_Name, m_pTeamLeader->m_Teammates.at(0)->GetName());
		msg.usTeammate0_MapID = m_pTeamLeader->m_Teammates.at(0)->GetRegion()->GetID();
		msg.ufTeammate0_X = m_pTeamLeader->m_Teammates.at(0)->GetPosX();
		msg.ufTeammate0_Y = m_pTeamLeader->m_Teammates.at(0)->GetPosY();
		msg.ufTeammate0_Z = m_pTeamLeader->m_Teammates.at(0)->GetPosZ();

		if (2 <= m_Teammates.size())
		{
			msg.lTeammate1_ID = m_pTeamLeader->m_Teammates.at(1)->GetID();
			msg.uiTeammate1_HP = m_pTeamLeader->m_Teammates.at(1)->GetlHP();
			msg.uiTeammate1_MaxHP = m_pTeamLeader->m_Teammates.at(1)->GetMaxHP();
			msg.uiTeammate1_Lv = m_pTeamLeader->m_Teammates.at(1)->GetRank();
			msg.ucTeammate1_Class = m_pTeamLeader->m_Teammates.at(1)->GetlClass();
			strcpy(msg.chTeammate1_Name, m_pTeamLeader->m_Teammates.at(1)->GetName());
			msg.usTeammate1_MapID = m_pTeamLeader->m_Teammates.at(1)->GetRegion()->GetID();
			msg.ufTeammate1_X = m_pTeamLeader->m_Teammates.at(1)->GetPosX();
			msg.ufTeammate1_Y = m_pTeamLeader->m_Teammates.at(1)->GetPosY();
			msg.ufTeammate1_Z = m_pTeamLeader->m_Teammates.at(1)->GetPosZ();

		}
		else
		{
			msg.lTeammate1_ID = -1;
			msg.uiTeammate1_HP = 0;
			msg.uiTeammate1_MaxHP = 1;
			msg.uiTeammate1_Lv = 0;
			msg.ucTeammate1_Class = 0;
			memset(msg.chTeammate1_Name, 0, 64);
			msg.usTeammate1_MapID = -1;
			msg.ufTeammate1_X = 0;
			msg.ufTeammate1_Y = 0;
			msg.ufTeammate1_Z = 0;

		}

		if (3 <= m_Teammates.size())
		{
			msg.lTeammate2_ID = m_pTeamLeader->m_Teammates.at(2)->GetID();
			msg.uiTeammate2_HP = m_pTeamLeader->m_Teammates.at(2)->GetlHP();
			msg.uiTeammate2_MaxHP = m_pTeamLeader->m_Teammates.at(2)->GetMaxHP();
			msg.uiTeammate2_Lv = m_pTeamLeader->m_Teammates.at(2)->GetRank();
			msg.ucTeammate2_Class = m_pTeamLeader->m_Teammates.at(2)->GetlClass();
			strcpy(msg.chTeammate2_Name, m_pTeamLeader->m_Teammates.at(2)->GetName());
			msg.usTeammate2_MapID = m_pTeamLeader->m_Teammates.at(2)->GetRegion()->GetID();
			msg.ufTeammate2_X = m_pTeamLeader->m_Teammates.at(2)->GetPosX();
			msg.ufTeammate2_Y = m_pTeamLeader->m_Teammates.at(2)->GetPosY();
			msg.ufTeammate2_Z = m_pTeamLeader->m_Teammates.at(2)->GetPosZ();

		}
		else
		{
			msg.lTeammate2_ID = -1;
			msg.uiTeammate2_HP = 0;
			msg.uiTeammate2_MaxHP = 1;
			msg.uiTeammate2_Lv = 0;
			msg.ucTeammate2_Class = 0;
			memset(msg.chTeammate2_Name, 0, 64);
			msg.usTeammate2_MapID = -1;
			msg.ufTeammate2_X = 0;
			msg.ufTeammate2_Y = 0;
			msg.ufTeammate2_Z = 0;
		}

		if (4 <= m_Teammates.size())
		{
			msg.lTeammate3_ID = m_pTeamLeader->m_Teammates.at(3)->GetID();
			msg.uiTeammate3_HP = m_pTeamLeader->m_Teammates.at(3)->GetlHP();
			msg.uiTeammate3_MaxHP = m_pTeamLeader->m_Teammates.at(3)->GetMaxHP();
			msg.uiTeammate3_Lv = m_pTeamLeader->m_Teammates.at(3)->GetRank();
			msg.ucTeammate3_Class = m_pTeamLeader->m_Teammates.at(3)->GetlClass();
			strcpy(msg.chTeammate3_Name, m_pTeamLeader->m_Teammates.at(3)->GetName());
			msg.usTeammate3_MapID = m_pTeamLeader->m_Teammates.at(3)->GetRegion()->GetID();
			msg.ufTeammate3_X = m_pTeamLeader->m_Teammates.at(3)->GetPosX();
			msg.ufTeammate3_Y = m_pTeamLeader->m_Teammates.at(3)->GetPosY();
			msg.ufTeammate3_Z = m_pTeamLeader->m_Teammates.at(3)->GetPosZ();
		}
		else
		{
			msg.lTeammate3_ID = -1;
			msg.uiTeammate3_HP = 0;
			msg.uiTeammate3_MaxHP = 1;
			msg.uiTeammate3_Lv = 0;
			msg.ucTeammate3_Class = 0;
			memset(msg.chTeammate3_Name, 0, 64);
			msg.usTeammate3_MapID = -1;
			msg.ufTeammate3_X = 0;
			msg.ufTeammate3_Y = 0;
			msg.ufTeammate3_Z = 0;
		}

		msg.bIsDateUpdate = _bIsDateUpdate;

		CGameObject::s_World->SendMsgToClient(&msg, this->GetSocket());
		vector<CPlayer*>::iterator iter;
		for (iter = m_Teammates.begin();
			iter != m_Teammates.end();
			iter++)
		{
			CGameObject::s_World->SendMsgToClient(&msg, (*iter)->GetSocket());
		}
	}



	return 1;
}
bool CPlayer::InitTeam_New()
{
	if (m_pTeamLeader)
	{
		m_Teammates.clear();
		m_pTeamLeader = NULL;
		return true;
	}
	return false;
}

void CPlayer::GetTeamState(int& _count, bool& _IfCouples)
{
	_IfCouples = false;
	_count = 0;
	if (m_pTeamLeader)
	{
		_count = m_pTeamLeader->m_Teammates.size();
		_count++;
		if (2 == _count)
		{
			if (m_pTeamLeader->GetGender() != m_pTeamLeader->m_Teammates.at(0)->GetGender())
			{
				_IfCouples = true;
				return;
			}
		}
	}

	return;
}
