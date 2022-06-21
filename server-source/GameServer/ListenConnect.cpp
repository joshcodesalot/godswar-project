#include "stdafx.h"
#include "GSMaster.h"
#include "ServiceLoader.h"

#include "DataCore.h"
#include "GameCore\GameObject.h"
#include "GameCore\Player.h"
#include "GameCore\NPC.h"
#include "GameCore\Region.h"
#include "GameCore\KeyWords.h"
#include "..\Common\MsgDefine.h"
#include "..\Common\MsgTypes.h"
#include "..\Common/Include/DB\\Inc\Database.h"
#include "ChatCommand.h"
#include ".\GameCore\RoleRelation.h"
#include "GameCore\Mall.h"

#include "World.h"
extern CWorld* g_pLoader;

//extern MAP_CONSORTIA g_oConsortiaManager;


#define  MAX_LOGIN_QUEUE 20

//void SocketEventHandle::TransactMsgFromClient()
//{	
	/*20080618
	USHORT temp=0;
	static const  MsgHandle* NetEventHandle = GetSocHandleTab( temp );
	if (!m_pPort->IsValid())
		return;

	sbase::IMessage* pPortMsg = NULL;
    pPortMsg = m_pPort->TakeMsg();
	if ( !pPortMsg )
		return;

	//第二个参数暂且为NULL
	eError Err = (this->*NetEventHandle[GetIndex(pPortMsg->GetType())].pfun)( pPortMsg, NULL );
	if ( Err != NO_MSG_ERRO )
      SendErrorInfo( Err,static_cast<snet::CSocket *>(pPortMsg->GetTo()) );
    m_pPort->DiscardMsg(pPortMsg);
	20080618*/
//}


//---------------------------------------------------
//读取数据完成的回调
//---------------------------------------------------
void __cdecl  OnDealWithAttribute( CPlayer *pPlayer,void *Loader )
{
	ASSERT( pPlayer ); 
	if( !(pPlayer->IsActive()) )
		return;

	MSG_SELFINFO msg;
	msg.Head.usType = _MSG_SELFINFO;
	msg.Head.usSize = sizeof(MSG_SELFINFO);
	msg.lDB_id = pPlayer->GetDBID();
	msg.uiUserID = pPlayer->GetID();
	msg.X = pPlayer->GetPosX();
	msg.Y = pPlayer->GetPosY();
	msg.Z = pPlayer->GetPosZ();
	msg.ucGender = pPlayer->GetGender();
	msg.ucProfession = pPlayer->GetlClass();
    msg.ucHairStyle = pPlayer->GetcHairStyle();
	msg.ucFaceShap = pPlayer->GetcPupilColor();
	msg.uiMaxHP = pPlayer->GetMaxHP();
	msg.uiMaxMP = pPlayer->GetMaxMP();
	msg.uiCurHP = pPlayer->GetlHP();
	msg.uiCurMP = pPlayer->GetlMP();
	msg.uiExp = pPlayer->GetcExp();
	msg.ucCamp       = pPlayer->GetlFaction();
	msg.usMapID = pPlayer->s_World->GetRegionFromLogicID( pPlayer->GetlRegionID() )->GetID();
	strcpy( msg.cName, pPlayer->GetName());
	msg.ucBelief = pPlayer->GetusBelief();
	msg.cEquipPos =  0;
	msg.uiMaxExp = pPlayer->GetNextGradeExp( );
	msg.uiSkillPoint = pPlayer->GetSkillPoint();
	msg.uiSkillExp = pPlayer->GetSkillExp();
	
	ItemData EuipData;
	int Index = 0;
	msg.cEquipPos = 0;

	for( int a = 0; a < MAX_EQUIP_COUNT; a++ )
	{
       if( pPlayer->GetEquip()[a].GetItemData().Base != -1 )
	   {
           msg.cEquipPos |= ( 1 << a );
		   const ItemData &Data = pPlayer->GetEquip()[a].GetItemData();
		   memcpy( &msg.SEquip[Index],&Data, sizeof( Equip) );
		   Index++;
	   }
	}
	(static_cast<CWorld *>(Loader))->SendMsgToClient( &msg, pPlayer->GetSocket() );
	//cout<<"Role Info..."<<endl;
} 

void __cdecl OnDealWithPassiveSkill( CPlayer *pPlayer )
{
	ASSERT( pPlayer );
	pPlayer->SendSkillMsg( false );
}

void __cdecl OnDealWithActiveSkill( CPlayer *pPlayer )
{
	ASSERT( pPlayer );
	pPlayer->SendSkillMsg( true );

}

void __cdecl OnDealWithKitBag( CPlayer *pPlayer )
{
	ASSERT( pPlayer );
	pPlayer->SendKitBagMsg(0);
//	CMall::Instance()->GetMallData( pPlayer );
//	CMall::Instance()->GetAssociationData(pPlayer);
}



