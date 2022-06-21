//========================================================
//
//    Copyright (c) 2006,JoyConnect studio
//    All rights reserved.
//
//    文件名称 ： Map.cpp
//    摘    要 ： map divide
//    
//    当前版本 ： 1.01
//    作    者 ： Fenjune Li
//    完成日期 ： 2007-07-17
//
//========================================================

#include "stdafx.h"
#include "Map.h"
#include "GameObject.h"
#include "Player.h"
#include "NPC.h"
#include "Monster.h"
#include "..\ServiceLoader.h"
#include ".\GameData.h"


//-------------------------------------------------------------------
CCell::CCell()
{
   m_MapPlayer.clear();
}

//-------------------------------------------------------------------
CCell::CCell( int x, int z )
{
	m_Grid_X = x;
	m_Grid_Z = z;
	m_MapID =( x<<4 )| z ;
	m_MapPlayer.clear();
}


//-------------------------------------------------------------------
CCell::~CCell()
{

}

//-------------------------------------------------------------------
// add object to this map
//-------------------------------------------------------------------
void CCell::AddObj( CGameObject *pObj )
{
   switch( pObj->GetType() )
   {
   case OBJECTTYPE_UNKNOW:
   	break;
   case OBJECTTYPE_PLAYER:
	    m_MapPlayer[ pObj->GetID() ] =  dynamic_cast< CPlayer * > (  pObj );
   	break;
   case OBJECTTYPE_NPC:
	    m_MapNPC[ pObj->GetID() ] =  dynamic_cast< CNPC * > ( pObj );
       break;
   case OBJECTTYPE_MONSTER:
        m_MapMonster[ pObj->GetID() ] =  dynamic_cast< CMonster * > ( pObj );
       break;
   case OBJECTTYPE_BOSS:
	   break;
   case OBJECTTYPE_PET:
	   break;
   case OBJECTTYPE_BUILDING:
	   break;
   case OBJECTTYPE_VIP:
	   break;
   case OBJECTTYPE_GM:
	   break;
   case OBJECTTYPE_GameProgramer:
	   break;
   default:
       break;
   }

}

//-------------------------------------------------------------------
// remove object from this map
//-------------------------------------------------------------------
void CCell::RemoveObj( CGameObject *pObj )
{
	switch( pObj->GetType() )
	{
	case OBJECTTYPE_UNKNOW:
		break;
	case OBJECTTYPE_PLAYER:
		{
 			MapPlayer::iterator itor = m_MapPlayer.find( pObj->GetID() );
 			if ( itor != m_MapPlayer.end() )
 			{
 				m_MapPlayer.erase( itor );
 			}
		}
		break;
	case OBJECTTYPE_NPC:
		{
 			MapNPC::iterator itor = m_MapNPC.find( pObj->GetID() );
 			if ( itor != m_MapNPC.end() )
 			{
 				m_MapNPC.erase( itor );
 			}
		}
		break;
	case OBJECTTYPE_MONSTER:
		{
 			MapMonster::iterator itor = m_MapMonster.find( pObj->GetID() );
 			if ( itor != m_MapMonster.end() )
 			{
 				m_MapMonster.erase( itor );
 			}
		}
		break;
	case OBJECTTYPE_BOSS:
		break;
	case OBJECTTYPE_PET:
		break;
	case OBJECTTYPE_BUILDING:
		break;
	case OBJECTTYPE_VIP:
		break;
	case OBJECTTYPE_GM:
		break;
	case OBJECTTYPE_GameProgramer:
		break;
	default:
		break;
	}

}


//--------------------------------------------------------------------
// find object in the map
//--------------------------------------------------------------------
bool CCell::FindObj( CGameObject * )
{
	//MapObj::iterator  itor = m_MapObj.find( pObj->GetID() );
   // return  ( itor != m_MapObj.end() );
	return false;
}

//--------------------------------------------------------------------
// synchronization all the objects that is new in the cell to one player
//--------------------------------------------------------------------
void CCell::SynOldObjInfoToPlayer( CPlayer * pPlayer )
{ 
  MSG_COLONY_LEAVE MsgLeave, MsgLeaveOne;
  MsgLeave.Head.usSize = sizeof(  MsgLeave.Head ) + 4;
  MsgLeave.Head.usType = _MSG_COLONY_LEAVE;
  MsgLeave.usNum = 0;

  MsgLeaveOne.Head.usSize = sizeof(  MsgLeave.Head ) + 4 + 4;
  MsgLeaveOne.Head.usType = _MSG_COLONY_LEAVE;
  MsgLeaveOne.usNum = 1;
  MsgLeaveOne.uiID[ MsgLeaveOne.usNum-1 ] = pPlayer->GetID();

  MapPlayer::iterator iter = m_MapPlayer.begin();
  for ( ; iter != m_MapPlayer.end(); iter++ )  
  { 
	 //send leave message of mine
	 if ( iter->second !=  pPlayer )
	 {
		 MsgLeave.usNum++;
		 MsgLeave.uiID[ MsgLeave.usNum-1 ] = iter->first;
		 pPlayer->s_World->SendMsgToClient( &MsgLeaveOne , iter->second->GetSocket() );
	 } 
  }

  MapMonster::iterator MonsterItor = m_MapMonster.begin();
  for ( ; MonsterItor != m_MapMonster.end(); MonsterItor++ )
  {
	  MsgLeave.usNum++;
	  MsgLeave.uiID[ MsgLeave.usNum-1 ] = MonsterItor->first;
  }

  MapNPC::iterator NPCItor = m_MapNPC.begin();
  for ( ; NPCItor != m_MapNPC.end(); NPCItor++ )
  {
	  MsgLeave.usNum++;
	  MsgLeave.uiID[ MsgLeave.usNum-1 ] = NPCItor->first;
  }

  if ( MsgLeave.usNum != 0 )
  {
	  MsgLeave.Head.usSize +=  MsgLeave.usNum*4;
	  pPlayer->s_World->SendMsgToClient( &MsgLeave, pPlayer->GetSocket() );
  }

   
}


//--------------------------------------------------------------------
// synchronization all the objects that is old in the cell to one player
//--------------------------------------------------------------------
void  CCell::SynNewObjInfoToPlayer( CPlayer *pPlayer )
{
	MSG_OBJECTINFO SelfInfo,PlayerInfo,MonsterInfo,NPCInfo;
	
	//self information
	//const ObjectData * objData =  pPlayer->GetObjData();
	SelfInfo.Head.usType = _MSG_OBJECTINFO;
	SelfInfo.Head.usSize = 	sizeof(MSG_OBJECTINFO);
	SelfInfo.uiID    = pPlayer->GetID();
	SelfInfo.DB_id = pPlayer->GetDBID();
	SelfInfo.Stall = pPlayer->GetStall()->IsStart();
	wmemcpy(SelfInfo.StallText , pPlayer->GetStall()->GetText() , MAX_NAME); 
	SelfInfo.uiType  = _OBJECT_TYPE_PLAYER;
	SelfInfo.usMapID = pPlayer->GetRegion()->GetID();
	SelfInfo.X       = pPlayer->GetPosX();
	SelfInfo.Y       = pPlayer->GetPosY();
	SelfInfo.Z       = pPlayer->GetPosZ();
	SelfInfo.fAtan2  = pPlayer->GetAtan2();
	SelfInfo.uiHP    = pPlayer->GetlHP();
	SelfInfo.uiMaxHP = pPlayer->GetMaxHP();
	SelfInfo.ucGender= pPlayer->GetGender();
	SelfInfo.ucCamp  = pPlayer->GetlFaction();
	SelfInfo.ucProfession = pPlayer->GetlClass();
	SelfInfo.ucLV = pPlayer->GetRank();
	SelfInfo.ucBelief     = pPlayer->GetusBelief();
	SelfInfo.ucHairStyle  = pPlayer->GetcHairStyle();
	SelfInfo.ucFaceShap = pPlayer->GetcPupilColor();

	//商城道具隐藏名字
	//strcpy( SelfInfo.cName, pPlayer->GetName());
	if(pPlayer->IsFalseName())
		strcpy( SelfInfo.cName, "蒙面客");
	else
		strcpy( SelfInfo.cName, pPlayer->GetName());

	SelfInfo.iStatusCount = (UINT)pPlayer->GetStatus( SelfInfo.iStautsDis );
	int Index = 0;
	SelfInfo.cEquipPos = 0;
	for( int a = 0; a < MAX_EQUIP_COUNT; a++ )
	{ 
		if( pPlayer->GetEquip()[a].GetItemData().Base != -1 )
		{
			SelfInfo.cEquipPos |= ( 1 << a );
			const ItemData& Data = pPlayer->GetEquip()[a].GetItemData();
			memcpy( &SelfInfo.SEquip[Index], &Data, sizeof( Equip) );
			Index++;
		}
	}
	
	//Player information
	MapPlayer::iterator iter = m_MapPlayer.begin();
	for ( ; iter != m_MapPlayer.end(); iter++ )
	{
		CPlayer *p = iter->second;
		p->s_World->SendMsgToClient( &SelfInfo, /*pPlayer*/p->GetSocket() );

		//objData =  p->GetObjData();
		PlayerInfo.Head.usType = _MSG_OBJECTINFO;
		PlayerInfo.Head.usSize = sizeof(MSG_OBJECTINFO);
		PlayerInfo.uiID = p->GetID();
		PlayerInfo.DB_id = p->GetDBID();
		PlayerInfo.Stall = p->GetStall()->IsStart();
		memcpy(PlayerInfo.StallText , p->GetStall()->GetText() , MAX_NAME); 
		PlayerInfo.uiType = _OBJECT_TYPE_PLAYER;
		PlayerInfo.usMapID = p->GetRegion()->GetID();
		PlayerInfo.X = p->GetPosX();
		PlayerInfo.Y = p->GetPosY();
		PlayerInfo.Z = p->GetPosZ();
		PlayerInfo.fAtan2 = p->GetAtan2();
		PlayerInfo.uiHP = p->GetlHP();
		PlayerInfo.uiMaxHP = p->GetMaxHP();
		PlayerInfo.ucGender = p->GetGender();
		PlayerInfo.ucCamp  = p->GetlFaction();
		PlayerInfo.ucProfession = p->GetlClass();
		PlayerInfo.ucLV = p->GetRank();
		PlayerInfo.ucBelief = p->GetusBelief();
		PlayerInfo.ucHairStyle = p->GetcHairStyle();
		PlayerInfo.ucFaceShap = p->GetcPupilColor();

		/*------------------------------------------------------------------------------
		 *作者：C.D
		 *日期：2008-06-17
		 *描述：
		 -----------------------------------------------------------------------------*/
		//商城道具隐藏名字
		//strcpy( PlayerInfo.cName, p->GetName());
		if(p->IsFalseName())
			strcpy( PlayerInfo.cName, "蒙面客");
		else
			strcpy( PlayerInfo.cName, p->GetName());

		PlayerInfo.iStatusCount = (UINT)p->GetStatus( PlayerInfo.iStautsDis );
		int Index = 0;
		PlayerInfo.cEquipPos = 0;
		
		memset( &SelfInfo.SEquip, 0L, MAX_EQUIP_COUNT*sizeof(Equip) );

		for( int a = 0; a < MAX_EQUIP_COUNT; a++ )
		{ 
			if( p->GetEquip()[a].GetItemData().Base != -1 )
			{
				PlayerInfo.cEquipPos |= ( 1 << a );
				const ItemData& Data = p->GetEquip()[a].GetItemData();
				memcpy( &PlayerInfo.SEquip[Index],&Data, sizeof( Equip) );
				Index++;
			}
		}

		pPlayer->s_World->SendMsgToClient( &PlayerInfo, pPlayer->GetSocket() );
	}

	//Monster information
	MonsterInfo.Head.usSize = sizeof(MSG_OBJECTINFO);
	MonsterInfo.Head.usType = _MSG_OBJECTINFO;
	MapMonster::iterator MonsterItor = m_MapMonster.begin();
	for ( ; MonsterItor != m_MapMonster.end(); MonsterItor++ )
	{
		CMonster* p = MonsterItor->second;
		if( p->GetState() != MONSTER_STATE_LOST )
		{
			if( p->GetState() != MONSTER_STATE_DEAD )
			{
				MonsterInfo.uiID = p->GetID();
				MonsterInfo.uiType = _OBJECT_TYPE_MONSTER;
				MonsterInfo.usMapID = p->GetRegion()->GetID();
				MonsterInfo.X = p->GetPosX();
				MonsterInfo.Y = p->GetPosY();
				MonsterInfo.Z = p->GetPosZ();
				MonsterInfo.ucCamp = p->GetlFaction();
				MonsterInfo.fAtan2 = p->GetAtan2();
				MonsterInfo.uiHP = p->GetlHP();
				MonsterInfo.uiMaxHP = p->GetMaxHP();
				MonsterInfo.ucLV = p->GetRank();
				MonsterInfo.iStatusCount = (UINT)p->GetStatus( MonsterInfo.iStautsDis );
				sprintf( MonsterInfo.cName, "%s", p->GetName() );
				pPlayer->s_World->SendMsgToClient( &MonsterInfo, pPlayer->GetSocket() );
			}
		}
	}

	//NPC information
	NPCInfo.Head.usSize = sizeof(MSG_OBJECTINFO);
	NPCInfo.Head.usType = _MSG_OBJECTINFO;
	MapNPC::iterator NPCItor = m_MapNPC.begin();
	for ( ; NPCItor != m_MapNPC.end(); NPCItor++ )
	{
		CNPC* p = NPCItor->second;
		NPCInfo.uiID = p->GetID();
		NPCInfo.uiType = _OBJECT_TYPE_NPC;
		NPCInfo.usMapID = p->GetRegion()->GetID();
		NPCInfo.ucCamp  = p->GetlFaction();
		NPCInfo.X = p->GetPosX();
		NPCInfo.Y = p->GetPosY();
		NPCInfo.Z = p->GetPosZ();
		NPCInfo.fAtan2 = p->GetAtan2();
		NPCInfo.ucLV = p->GetRank();
		sprintf( NPCInfo.cName, "%s", p->GetName() );
		pPlayer->s_World->SendMsgToClient( &NPCInfo, pPlayer->GetSocket() );

		p->GetQuestList(pPlayer);
	}

}
///////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------
//construct method of CMap
//---------------------------------------------------------------------
CMap::CMap()
{
   for ( int i = 0 ; i < NUMBER_OF_REGION_GRID; i++ )
   {
	   for ( int j = 0; j < NUMBER_OF_REGION_GRID; j++ )
	   {
		  m_Cell[i][j].SetCellID( ( i << 4 ) | j );
/*		  int a = ( i << 4 ) | j;*/
		  m_Cell[i][j].SetIndex( j, i );
	   }
   }
}

//---------------------------------------------------------------------
// destruct method of CMap
//---------------------------------------------------------------------
CMap::~CMap()
{

}

//---------------------------------------------------------------------
// get the map by map ID 
//---------------------------------------------------------------------
CCell *CMap::GetCell( int iMapID )
{
  return &m_Cell[( iMapID  & 0xf0 ) >> 4][iMapID  & 0x0f];
}


//---------------------------------------------------------------------
// get the map from the position of object
//---------------------------------------------------------------------
CCell *CMap::GetCell( float pos_x, float pos_z )
{
	float grid_x = ( pos_x + SIZE_OF_REGION ) / SIZE_OF_GRID - 1;
	float grid_z = ( pos_z + SIZE_OF_REGION ) / SIZE_OF_GRID - 1;

	int grid_index_x = ceil( grid_x );
	int grid_index_z = ceil( grid_z );

	if ( grid_index_x < 0 )
	{
	   grid_index_x = 0 ;
	}
	else if ( grid_index_x > NUMBER_OF_REGION_GRID - 1 )
	{
       grid_index_x = NUMBER_OF_REGION_GRID - 1  ;
	}

	if ( grid_index_z < 0 )
	{
		grid_index_z = 0 ;
	}
	else if ( grid_index_z > NUMBER_OF_REGION_GRID - 1 )
	{
		grid_index_z = NUMBER_OF_REGION_GRID - 1  ;
	}


	return &m_Cell[grid_index_z][grid_index_x];
}

//---------------------------------------------------------------------
// switch map from one to other
//---------------------------------------------------------------------
void CMap::SwitchCell( CGameObject *pObj, int MapID  )
{
    CCell *pMap = GetCell( pObj->GetPosX(), pObj->GetPosZ() );
	pMap->RemoveObj( pObj );	
	CCell *pNewMap = GetCell( MapID );
	pNewMap->AddObj( pObj );
} 

//---------------------------------------------------------------------
//Did the object have surpass the borderline of the current map
//---------------------------------------------------------------------
bool CMap::SurpassCellBorderline( CGameObject *pObj, float pos_x, float pos_z )
{

   float grid_x = ( pos_x + SIZE_OF_REGION ) / SIZE_OF_GRID;
   float grid_z = ( pos_z + SIZE_OF_REGION ) / SIZE_OF_GRID;

   int grid_index_x = ceil( grid_x );
   int grid_index_z = ceil( grid_z );
  
   int NextMapID = ( grid_index_x << 4 ) | grid_index_z ;

   if ( pObj->GetRegion()->GetCellID( pObj ) != NextMapID )
   { 
	   SwitchCell( pObj, NextMapID );
	   return true;
   }

   return false;
}

