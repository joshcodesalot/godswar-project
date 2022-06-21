//========================================================
//
//    Copyright (c) 2007,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� ChangeRegion.cpp
//    ժ    Ҫ �� �����л�ģ��
//    
//    ��ǰ�汾 �� 1.00
//    ��    �� �� �ֵ»�
//    ������� �� 2007-06-11
//
//========================================================

#include "StdAfx.h"
#include "Region.h"
#include "GameObject.h"
#include "Player.h"
#include "NPC.h"
#include "Monster.h"
#include "..\DataCore.h"
#include "..\ServiceLoader.h"
#include "../World.h"


long CRegion::ChangeRegion( CPlayer* pPlayer, float x, float , float z )
{
	// �����л�����
	if( m_listConvey.size() != 0 )
	{
		for( SceneConveyIterator it=m_listConvey.begin(); it!=m_listConvey.end(); it++ )
		{
			tarSceneConvey* p = *it;
			if( x > p->rect_left && x < p->rect_right && z > p->rect_bottom && z < p->rect_top )
			{
				tarSceneChange* pSceneChange = new tarSceneChange;
				pSceneChange->pPlayer = pPlayer;
				pSceneChange->lFromSceneID = this->GetID();
				pSceneChange->lToSceneID = m_pWorld->GetRegionFromName( p->toScene.c_str() )->GetID();
				pSceneChange->x = p->toX;
				pSceneChange->y = 0.0f;
				pSceneChange->z = p->toZ;
				m_pWorld->g_listSceneChange.push_back( pSceneChange );
//				pPlayer->ClearPath();
				pPlayer->m_eState = OBJECT_IDLE;
				pPlayer->SetWorldStatus(eSCENECHANGE);
				//pPlayer->SetPos( p->toX, 0.0f, p->toZ );
				return 1;
			}
		}
	}

	return 0;
}
