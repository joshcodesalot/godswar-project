#include "StdAfx.h"
#include "Region.h"
#include "GameObject.h"
#include "Player.h"
#include "NPC.h"
#include "Monster.h"
#include "ItemManager.h"

#include "..\ServiceLoader.h"

#include "..\..\Common\MsgDefine.h"
#include "..\..\Common\MsgTypes.h"

extern MSG_DEAD msg_dead;
extern MSG_ATTACK msg_attack;
extern MSG_WALK_END msg_walk_end;
extern MSG_DROPS msg_drops;

eError CRegion::Attack( CPlayer* pPlayer, CGameObject* pTarget, float x, float, float z )
{
	if ( pPlayer->IsDead() )                                                    // ���˲��ܴ���
		return NO_MSG_ERRO;

	if( pTarget->IsDead() )												        // ���ܱ�ʬ
		return NO_MSG_ERRO;

	if( pTarget->GetType() == OBJECTTYPE_NPC )									// ���ܹ�������NPC
		return NO_MSG_ERRO;

	if( !pPlayer->IsAttack() )													// ����Ƶ������
		return NO_MSG_ERRO;

	if( pPlayer->GetlFaction() == pTarget->GetlFaction())						// ꇠI�ж�
		return NO_MSG_ERRO;

	//	pPlayer->SetPos( x, 0, z );
	//	pPlayer->m_timeAttack.Startup( pPlayer->m_ObjectData.m_usAttackSpeed - 300 );		// �������(��ʵʱ��-300ms������ʱ)
	pPlayer->m_timeAttack.Startup( pPlayer->GetAttackSpeed()-200 );		// �������

	// ���������Ҿ���
	float fPX = 0.0f;
	float fPZ = 0.0f;
	float fDist = 0.0f;
	fPX = x - pTarget->GetPosX();
	fPZ = z - pTarget->GetPosZ();
	fDist = sqrt( fPX*fPX + fPZ*fPZ );
	if( fDist < 2.8f )
	{
		// ������������
		msg_attack.uiID = pPlayer->GetID();
		msg_attack.uiObjectID = pTarget->GetID();
		msg_attack.ucAttackType = ATTACK_HIT;
		msg_attack.X = 0.0f;
		msg_attack.Y = 0.0f;
		msg_attack.Z = 0.0f;
		// ====================================================================================================
		if( pTarget->GetType() == OBJECTTYPE_MONSTER)			// PVE
		{
			//����������Ҫ����
			//bool UpdateSkill = false;

			ATTACK_TYPE  attack_type = pPlayer->JudgeAttackStyle( pTarget);
			if ( attack_type == ATTACK_MISS  || ((CMonster*)pTarget)->IsLeaveFight())
			{ 
				msg_attack.ucAttackType = (BYTE)attack_type;
				msg_attack.uiDamage = -1; 

				StartFight(pPlayer , pTarget);
				StartFight(pTarget , pPlayer);
				goto SendMsg;
			}

			// ս�����Ը���
			StartFight(pPlayer , pTarget);
			StartFight(pTarget , pPlayer);

			//�����˺�
			int  damage = (int)pPlayer->CalculateAttackDamage( *pTarget );

			if (  attack_type == ATTACK_FORCE )
			{
				damage *=  1.5;
			}

			pTarget->AddHP(-damage);

			//��ʱ���˺������
			pTarget->AddEnmity(pPlayer , damage);
			pTarget->m_timeFight.Update();
			pPlayer->AddEnmity(pTarget , 0);

			//���㾭��͵ȼ�
			pPlayer->CalculateExpAndLevel( *pTarget );

			//���㹥���������������ܾ���
			PhysiacalAttack WeapKind = pPlayer->JudgeWeaponKind();
			//pPlayer->CalculateExp( pTarget->GetlHP() <= 0 );

			msg_attack.Head.usSize = sizeof(msg_attack);


			msg_attack.uiDamage = damage;
			msg_attack.ucAttackType = (BYTE)attack_type;
			msg_attack.ucWeaponType = (BYTE)WeapKind;

			// ������������
			if( pTarget->GetlHP() <= 0 )
			{
				pTarget->Dead( pPlayer );

			}

SendMsg:    // by fengjune SendMsgToPlayer( &msg_attack );
			SendAreaMsgOneToOther( pPlayer->GetCurrentCell(), &msg_attack );

		}
		// ====================================================================================================
		else if ( pTarget->GetType() == OBJECTTYPE_PLAYER)			// PVP
		{
			if (pPlayer == pTarget)
				return NO_MSG_ERRO;

			// �ȼ�����
			if( pTarget->GetRank() <= MAX_LEVEL_LIMIT )
				return NO_MSG_ERRO;
			//���߲�����PK��������by C.D 20080616
			if ( ((CPlayer*)pTarget)->IsPKProtected() )
				return NO_MSG_ERRO;
			if ( pPlayer->IsPKProtected())							// PK����״̬��Ҳ���ܶ����˹���
				return NO_MSG_ERRO;

			ATTACK_TYPE  attack_type = pPlayer->JudgeAttackStyle( pTarget );
			if ( attack_type == ATTACK_MISS  || ((CMonster*)pTarget)->IsLeaveFight())
			{
				msg_attack.ucAttackType = (BYTE)attack_type;
				msg_attack.uiDamage = -1; 
				goto SendMsg;
			}

			pPlayer->SetTarget( pTarget );

			//Ŀǰ�˴���δ����ս��������ս���������⡣

			// ս�����Ը���
			//StartFight(pPlayer , pTarget);
			//StartFight(pTarget , pPlayer);

			//if (pTarget->GetFight())
			//{
			//	//ͬ�����
			//	if (damage > 0)
			//		pPlayer->CoagentEnmityList(pTarget , damage);
			//	else
			//		pPlayer->CoagentEnmityList(pTarget , -damage);
			//}

			int Damage = pPlayer->CalculateAttackDamage( *pTarget );

			pTarget->AddHP(-Damage);

			msg_attack.uiDamage = Damage;

			//pPlayer->CalculateExpAndLevel( *pTarget );
			//by fenjune SendMsgToPlayer( &msg_attack );

			msg_attack.Head.usSize = sizeof(msg_attack);
			SendAreaMsgOneToOther( pPlayer->GetCurrentCell(), &msg_attack );

			// ��ɫ����
			if( pTarget->GetlHP() <= 0 )
			{
				pTarget->Dead( pPlayer );
			}

		}

	}
	return NO_MSG_ERRO;
}
