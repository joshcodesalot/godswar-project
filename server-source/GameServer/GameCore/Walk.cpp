#include "StdAfx.h"
#include "Region.h"
#include "GameObject.h"
#include "Player.h"
#include "NPC.h"
#include "Monster.h"
#include "Map.h"

#include "..\..\Common\MsgDefine.h"
#include "..\..\Common\MsgTypes.h"
#include "..\errormessage.h"

extern MSG_WALK_BEGIN msg_walk_begin;
extern MSG_WALK_END msg_walk_end;
extern MSG_WALK msg_walk;


// =============================
//   �ƶ�����
// =============================



long CRegion::Walk( CPlayer* pPlayer, float x, float z, float fatan, long lState )
{
	tarWalk* p = new tarWalk;
	p->x = x;
	p->z = z;
	p->fatan = fatan;

	pPlayer->m_fatan2 = fatan;

	pPlayer->m_queueWalk.push_back( p );

	msg_walk.uiID = pPlayer->GetID();
	msg_walk.usMapID = pPlayer->GetRegion()->GetID();
	msg_walk.fatan = fatan;
	msg_walk.x = x;
	msg_walk.z = z;
	msg_walk.lState = lState;
	SendAreaMsgOneToOther( pPlayer->GetCurrentCell(), &msg_walk );

	// ��ֹ���
	static float fPX = 0.0f;
	static float fPZ = 0.0f;
	static int loop = 0;
	if( loop > 100 )
	{
		// ����ȱ�ݣ���ʱ�������ȷ������ȶ����ٽ����޸�
		// ȱ�ݣ�m_queueWalk��size��Ϊ1��ʱ���ж���������ģ�����˲�Ƶ���ҿ���
		WalkIterator it = pPlayer->m_queueWalk.end();
		it--;
		fPX = x - (*it)->x;
		fPZ = z - (*it)->z;
		float Dist = sqrt( fPX*fPX + fPZ*fPZ );
		if( Dist > 0.3f )
		{
			pPlayer->AddDanger();
			sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_WALK] Type:Walk.", pPlayer->GetAccounts(), pPlayer->GetName() );
		}
		loop = 0;
	}
	else
		loop++;

	return 0;
}






eError CRegion::WalkBegin( CPlayer* pPlayer, float x, float , float z, float offset_x, float /*offset_y*/, float offset_z, unsigned int tick )
{
	return NO_MSG_ERRO;

	if (pPlayer->m_eState == OBJECT_DEAD)
		return NO_MSG_ERRO;

	PathIterator it;
	tarPath* pPath = new tarPath;

	pPath->x = x;
	pPath->y = 0.0f;
	pPath->z = z;
	pPath->end_x = x;
	pPath->end_y = 0.0f;
	pPath->end_z = z;
	pPath->nTick = 0;
	pPath->nEndTick = 0;
	pPath->OffsetX = offset_x;		// ��Ҫ��֤
	pPath->OffsetZ = offset_z;

	tarPath* pPerPath = NULL;

	if( tick == 0 )
	{
	}
	else if( pPlayer->m_queuePath.size() == 1 )		// ��һ��·��
	{
		it = pPlayer->m_queuePath.begin();
		pPerPath = *it;

		if( tick > pPerPath->nTick )
		{
			// ����
			pPerPath->nEndTick = tick;
			pPath->end_x = x;
			pPath->end_z = z;
		}
		else if( tick == pPerPath->nTick )
		{
			// �պý���������״̬�����ʼ��ͣ�
			pPlayer->SetPos( x, 0.0f, z );

			pPlayer->m_queuePath.remove( pPerPath );
			SAFE_DELETE( pPerPath );
		}
		else	// lag
		{
			// ���л���
			float temp_x, temp_z;
			temp_x = pPlayer->GetPosX();
			temp_z = pPlayer->GetPosZ();
			for( unsigned int i=0; i<(pPerPath->nTick-tick); i++ )
			{
				temp_x -= pPerPath->OffsetX;
				temp_z -= pPerPath->OffsetZ;
			}

			//pPlayer->SetPos( temp_x, 0.0f, temp_z );

			pPlayer->SetPos( x, 0.0f, z );

			pPlayer->m_queuePath.remove( pPerPath );
			SAFE_DELETE( pPerPath );
		}
	}
	else if( pPlayer->m_queuePath.size() > 1 )	// ������������·��
	{
		it = pPlayer->m_queuePath.end();
		it--;
		pPerPath = *it;
		pPerPath->nEndTick = tick;
		pPerPath->end_x = x;
		pPerPath->end_z = z;

		if ((*it)->nEndTick == 0)
		{
			//����������EndTick = 0 ʱ���ᵱ���µ�·���ߣ�֮ǰ��EndTick = 0���û��ɾ���ͻ����
			//ɾ��֮ǰEndTick = 0��·��
			pPlayer->m_queuePath.erase(it);
		}
	}
	else
	{
		pPlayer->m_fatan2 = atan2( offset_x, offset_z );
		if( pPlayer->m_queuePath.size() == 0 )
		{
			SAFE_DELETE( pPath );
			return NO_MSG_ERRO;
		}
	}


	pPlayer->m_queuePath.push_back( pPath );

	if( pPlayer->m_eState == OBJECT_IDLE )
		pPlayer->m_eState = OBJECT_RUN;


	if( pPlayer->m_queuePath.size() == 1 )
	{
		// ������Ϣ��ȷ���ǽ�ɫ�Ӿ�ֹ��ʼ�ŷ���
		// post message to other player
		msg_walk_begin.X = pPath->x;//pPlayer->GetPosX();
		msg_walk_begin.Y = 0.0f;
		msg_walk_begin.Z = pPath->z;//pPlayer->GetPosZ();
		msg_walk_begin.uiTick = tick;
		msg_walk_begin.OffsetX = pPath->OffsetX;
		msg_walk_begin.OffsetY = 0.0f;
		msg_walk_begin.OffsetZ = pPath->OffsetZ;
		msg_walk_begin.uiID = pPlayer->GetID();
		msg_walk_begin.usMapID = (USHORT)this->GetID();
		SendAreaMsgOneToOther( pPlayer->GetCurrentCell(), &msg_walk_begin );
	}

	return NO_MSG_ERRO;
}

// �ƶ���ֹ���л���ɫ״̬								   
long CRegion::WalkEnd( CPlayer* pPlayer, float x, float , float z, float atan2, unsigned int tick, unsigned int )
{
	return 0;

	PathIterator it;
	tarPath* pPerPath = NULL;

	/*if( tick == 0 )
	{
		// tick=0����������δ���ؼ���Ϣ
	}
	else */if( pPlayer->m_queuePath.size() == 1 )		// ��һ��·��
	{
		it = pPlayer->m_queuePath.begin();
		pPerPath = *it;

		if( tick > pPerPath->nTick )
		{
			// ����
			pPerPath->nEndTick = tick;
			pPerPath->end_x = x;
			pPerPath->end_z = z;
//			OutputDebugString( "tick >>>> nTick\n" );
		}
		else if( tick == pPerPath->nTick )
		{
//			OutputDebugString( "tick ==== nTick\n" );
			// �պý���������״̬�����ʼ��ͣ�
			pPlayer->m_queuePath.remove( pPerPath );
			SAFE_DELETE( pPerPath );
			if( pPlayer->m_eState == OBJECT_RUN )
				pPlayer->m_eState = OBJECT_IDLE;

			// ��������
//			if( abs(abs(pPlayer->GetPosX())-abs(x)) < 0.1f && abs(abs(pPlayer->GetPosZ())-abs(z)) < 0.1f )

			pPlayer->SetPos( x, 0.0f, z );

			// post message to other player
			msg_walk_end.X = pPlayer->GetPosX();
			msg_walk_end.Y = 0.0f;
			msg_walk_end.Z = pPlayer->GetPosZ();
			msg_walk_end.uiTick = tick;
			msg_walk_end.fAtan2 = pPlayer->m_fatan2;
			msg_walk_end.uiID = pPlayer->GetID();
			msg_walk_end.usMapID = (USHORT)this->GetID();
			SendAreaMsgOneToOther( pPlayer->GetCurrentCell(), &msg_walk_end );
		}
		else	// lag
		{
//			OutputDebugString( "tick <<<< nTick\n" );
			// ���л���
			float temp_x, temp_z;
			temp_x = pPlayer->GetPosX();
			temp_z = pPlayer->GetPosZ();
			for( unsigned int i=0; i<(pPerPath->nTick-tick); i++ )
			{
				temp_x -= pPerPath->OffsetX;
				temp_z -= pPerPath->OffsetZ;
			}
			//pPlayer->SetPos( temp_x, 0.0f, temp_z );			�Լ���

			// �ͻ���λ�� ������
			pPlayer->SetPos( x, 0.0f, z );

			// ��������
//			if( abs(abs(pPlayer->GetPosX())-abs(x)) < 0.1f && abs(abs(pPlayer->GetPosZ())-abs(z)) < 0.1f )
//				pPlayer->SetPos( x, 0.0f, z );

			pPlayer->m_queuePath.remove( pPerPath );
			SAFE_DELETE( pPerPath );
			if( pPlayer->m_eState == OBJECT_RUN )
				pPlayer->m_eState = OBJECT_IDLE;

			// post message to other player
			msg_walk_end.X = pPlayer->GetPosX();
			msg_walk_end.Y = 0.0f;
			msg_walk_end.Z = pPlayer->GetPosZ();
			msg_walk_end.uiTick = tick;
			msg_walk_end.fAtan2 = pPlayer->m_fatan2;
			msg_walk_end.uiID = pPlayer->GetID();
			msg_walk_end.usMapID = (USHORT)this->GetID();
			SendAreaMsgOneToOther( pPlayer->GetCurrentCell(), &msg_walk_end );
		}
	}
	else if( pPlayer->m_queuePath.size() > 1 )	// ������������·��
	{
		it = pPlayer->m_queuePath.end();
		it--;
		pPerPath = *it;
		pPerPath->nEndTick = tick;
		pPerPath->end_x = x;
		pPerPath->end_z = z;
	}
	else	// �쳣����·����ֹͣ���󣬸ı䳯��
	{
//		pPlayer->m_eState = OBJECT_IDLE;
		pPlayer->m_fatan2 = pPlayer->m_fDest = atan2;
	}

	// post message to other player
/*
	msg_walk_end.X = x;//pPlayer->GetPosX();
	msg_walk_end.Y = 0.0f;
	msg_walk_end.Z = z;//pPlayer->GetPosZ();
	msg_walk_end.uiTick = tick;
	msg_walk_end.fAtan2 = pPlayer->m_fatan2;
	msg_walk_end.uiID = pPlayer->GetID();
	msg_walk_end.usMapID = (USHORT)this->GetID();
	SendAreaMsgOneToOther( pPlayer->GetCurrentCell(), &msg_walk_end );
*/

	return 0;
}



/*



long CRegion::Walk( CPlayer* pPlayer, float x, float y, float z )
{

	// ��ʱ������ (Z2-Z1) (X2-X1)
	float fPX = 0.0f;
	float fPZ = 0.0f;

	pPlayer->m_fDestinationX = x;
	pPlayer->m_fDestinationY = 0;// ��������߶ȣ��̶�Ϊ0
	pPlayer->m_fDestinationZ = z;

	// �ƶ�Ŀ�ĵ�ƫ����
	pPlayer->m_fOffsetX = pPlayer->GetPosX();
	pPlayer->m_fOffsetZ = pPlayer->GetPosZ();

	fPX = x - pPlayer->GetPosX();
	fPZ = z - pPlayer->GetPosZ();

	// ��ǰλ����Ŀ��λ�þ���
	pPlayer->m_fDist = sqrt( fPX*fPX + fPZ*fPZ );

	pPlayer->m_fOffsetX = fPX / pPlayer->m_fDist * pPlayer->GetfSpeed();
	pPlayer->m_fOffsetZ = fPZ / pPlayer->m_fDist * pPlayer->GetfSpeed();

	// �ƶ��沿����
	pPlayer->m_fDest = pPlayer->m_fatan2 = atan2( x-pPlayer->GetPosX(), z-pPlayer->GetPosZ() );

	// ��ɫ״̬
	//	if( pPlayer->m_eState == OBJECT_IDLE )
	{
		pPlayer->m_eState = OBJECT_RUN;
	}

	// post message to other player
	msg_walk.X = x;
	msg_walk.Y = y;
	msg_walk.Z = z;
	msg_walk.StartX = pPlayer->m_fX;
	msg_walk.StartY = pPlayer->m_fY;
	msg_walk.StartZ = pPlayer->m_fZ;
	msg_walk.uiID = pPlayer->GetID();
	msg_walk.usMapID = (USHORT)this->GetID();
	/ *
	for( PlayerIterator it=m_listPlayer.begin(); it!=m_listPlayer.end(); it++ )
	{
	CPlayer* p = *it;
	Loader.SendMsgToClient( &msg_walk, p->GetSocket() );
	}
	* /
	int gx = int(CONVERT_REGION_WIDTH_POS(pPlayer->GetPosX()));
	int gy = int(CONVERT_REGION_HEIGHT_POS(pPlayer->GetPosZ()));
	// ���ĸ�
	WalkMsgRegionSizeOperate( gx, gy, pPlayer );

	// ����
	if( gx > 0 && gy > 0 )
		WalkMsgRegionSizeOperate( gx-1, gy-1, pPlayer );

	// ��
	if( gy > 0 )
		WalkMsgRegionSizeOperate( gx, gy-1, pPlayer );

	// ����
	if( gx < (REGION_SIZE-1) && gy > 0 )
		WalkMsgRegionSizeOperate( gx+1, gy-1, pPlayer );

	// ��
	if( gx > 0 )
		WalkMsgRegionSizeOperate( gx-1, gy, pPlayer );

	// ��
	if( gx < (REGION_SIZE-1) )
		WalkMsgRegionSizeOperate( gx+1, gy, pPlayer );

	// ����
	if( gx > 0 && gy < (REGION_SIZE-1) )
		WalkMsgRegionSizeOperate( gx-1, gy+1, pPlayer );

	// ��
	if( gy < (REGION_SIZE-1) )
		WalkMsgRegionSizeOperate( gx, gy+1, pPlayer );

	// ����
	if( gx < (REGION_SIZE-1) && gy < (REGION_SIZE-1) )
		WalkMsgRegionSizeOperate( gx+1, gy+1, pPlayer );
	return 0;
}

*/
