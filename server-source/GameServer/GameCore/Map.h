//========================================================
//
//    Copyright (c) 2006,Joyconnect Studio
//    All rights reserved.
//
//    文件名称 ： Map.h
//    摘    要 ： map devide
//    
//    当前版本 ： 1.01
//    作    者 ： Fenjune Li
//    完成日期 ： 2007-07-17
//
//========================================================

#ifndef INC_MAP_H
#define INC_MAP_H

#ifdef _WIN32
#pragma once
#endif

#include <hash_map>
#include "GameData.h"

class CPlayer;
class CNPC;
class CMonster;
class CRegion;
class CGameObject;
class CMap;



typedef stdext::hash_map< UINT, CPlayer*> MapPlayer;
typedef stdext::hash_map< UINT, CNPC*> MapNPC;
typedef stdext::hash_map< UINT, CMonster*> MapMonster;

class CCell 
{   

   friend class CMap;
   struct  Point 
   {
	   float X;
	   float Z;
   };
public:
   void AddObj( CGameObject *pObj );
   void RemoveObj( CGameObject *pObj );
   bool FindObj( CGameObject *pObj );
   void GetPointWithThis( float &topleft_x, float &topleft_z, float &bottomright_x, float &bottomright_z )
   {
        topleft_x = m_LeftTop.X;
		topleft_z = m_LeftTop.Z;
		bottomright_x = m_RightBottom.X;
        bottomright_z = m_RightBottom.Z;
   };
   int  GetCellID(){ return m_MapID; } ;
   void SetCellID( int MapID ){ m_MapID = MapID; };
   void SetIndex( int x, int z )
   { 
	   m_Grid_X = x; 
	   m_Grid_Z = z ; 

	   m_LeftTop.X = m_Grid_X * SIZE_OF_GRID - SIZE_OF_REGION; 
	   m_LeftTop.Z = m_Grid_Z * SIZE_OF_GRID - SIZE_OF_REGION;
	   m_RightBottom.X = ( m_Grid_X + 1) * SIZE_OF_GRID - SIZE_OF_REGION; 
	   m_RightBottom.Z = ( m_Grid_Z + 1)* SIZE_OF_GRID - SIZE_OF_REGION;
   }
   void SynNewObjInfoToPlayer( CPlayer *pPlayer );
   void SynOldObjInfoToPlayer( CPlayer *pPlayer );
   MapPlayer &GetPlayerMap(){ return m_MapPlayer; };
   MapNPC &GetNPCMap(){ return m_MapNPC; };
   MapMonster &GetMonsterMap(){ return m_MapMonster;};
   std::vector<BYTE>  GetArea() const
   {
	   std::vector<BYTE> VeCMap;
	   //left-up
       int x = m_Grid_X - 1;
	   int z = m_Grid_Z - 1;
	   if ( x >= 0  && z >= 0 && x <= NUMBER_OF_REGION_GRID - 1 && z <= NUMBER_OF_REGION_GRID - 1 )
	   {
		  VeCMap.push_back( BYTE(( z << 4 ) | x ) );
	   }

	   //up
	   x = m_Grid_X ;
	   z = m_Grid_Z - 1;
	   if ( x >= 0  && z >= 0 && x <= NUMBER_OF_REGION_GRID - 1 && z <= NUMBER_OF_REGION_GRID - 1 )
	   {
		   VeCMap.push_back( BYTE(( z << 4 ) | x) );
	   }

	   //right-up
	   x = m_Grid_X + 1;
	   z = m_Grid_Z - 1;
	   if ( x >= 0  && z >= 0 && x <= NUMBER_OF_REGION_GRID - 1 && z <= NUMBER_OF_REGION_GRID - 1 )
	   {
		   VeCMap.push_back( BYTE(( z << 4 ) | x) );
	   }

	   //left
	   x = m_Grid_X - 1;
	   z = m_Grid_Z;
	   if ( x >= 0  && z >= 0  && x <= NUMBER_OF_REGION_GRID - 1 && z <= NUMBER_OF_REGION_GRID - 1 )
	   {
		   VeCMap.push_back( BYTE(( z << 4 ) | x) );
	   }

	   //center
	   x = m_Grid_X;
	   z = m_Grid_Z;
	   if ( x >= 0  && z >= 0  && x <= NUMBER_OF_REGION_GRID - 1 && z <= NUMBER_OF_REGION_GRID - 1 )
	   {
		   VeCMap.push_back( BYTE(( z << 4 ) | x) );
	   }

	   //right
	   x = m_Grid_X + 1;
	   z = m_Grid_Z;
	   if ( x >= 0  && z >= 0 && x <= NUMBER_OF_REGION_GRID - 1 && z <= NUMBER_OF_REGION_GRID - 1 )
	   {
		   VeCMap.push_back( BYTE(( z << 4 ) | x) );
	   }

	   //left-down
	   x = m_Grid_X - 1;
	   z = m_Grid_Z + 1;
	   if ( x >= 0  && z >= 0  && x <= NUMBER_OF_REGION_GRID - 1 && z <= NUMBER_OF_REGION_GRID - 1 )
	   {
		   VeCMap.push_back( BYTE(( z << 4 ) | x ) );
	   }

	   //down
	   x = m_Grid_X;
	   z = m_Grid_Z + 1;
	   if ( x >= 0  && z >= 0  && x <= NUMBER_OF_REGION_GRID - 1 && z <= NUMBER_OF_REGION_GRID - 1 )
	   {
		   VeCMap.push_back( BYTE(( z << 4 ) | x) );
	   }

       //right-down
	   x = m_Grid_X + 1;
	   z = m_Grid_Z + 1;
	   if ( x >= 0  && z >= 0 && x <= NUMBER_OF_REGION_GRID - 1 && z <= NUMBER_OF_REGION_GRID - 1 )
	   {
		   VeCMap.push_back( BYTE(( z << 4 ) | x) );
	   }
	   return VeCMap;
   }
   bool operator == ( const CCell& Cell ) const
   {
     return ( m_Grid_X == Cell.m_Grid_X &&  m_Grid_Z == Cell.m_Grid_Z );
   }
   bool operator!=(const CCell &cell) const { return !operator==(cell); }
public:
   int m_Grid_X;
   int m_Grid_Z; 
protected:
private:
  MapPlayer   m_MapPlayer;
  MapNPC      m_MapNPC;
  MapMonster  m_MapMonster;
  int m_MapID;   //max value is 15
  Point m_LeftTop, m_RightBottom;
  CCell();
  explicit CCell( int x, int z );
  ~CCell();     
};

class CMap
{
public:
	CMap();
	~CMap();
   CCell *GetCell( int iMapID );
   CCell *GetCell( float pos_x, float pos_z );

   inline std::vector<CCell*>  AreaCollisionTest( CCell *pSource, CCell *pTarget ) 
    {
	   std::vector<CCell*> VecCell; 
	   VecCell.clear();
 	   std::vector<BYTE> Area1 = pSource->GetArea();
 	   std::vector<BYTE> Area2 = pTarget->GetArea();
	   std::vector<BYTE>::iterator itor = Area2.begin();
	   for ( ; itor != Area2.end(); itor++ )
	   {
		   std::vector<BYTE>::iterator iter = std::find( Area1.begin(), Area1.end(), (*itor) );
		   if ( iter == Area1.end() )
		   {
			   VecCell.push_back( GetCell( (*itor  ) ));
		   }
	   }
       return VecCell;
    }

   inline bool IsTwoCellCross( CCell *pSource, CCell *pTarget )
   { 
	  std::vector<BYTE> Area =  pSource->GetArea();
	  std::vector<BYTE>::iterator iter = std::find( Area.begin(), Area.end(),  pTarget->GetCellID() );
      return iter !=  Area.end();
   }

   void SwitchCell( CGameObject *pObj, int MapID );
   bool SurpassCellBorderline( CGameObject *pObj, float pos_x, float pos_z );  
protected:

private:
   CCell  m_Cell[NUMBER_OF_REGION_GRID][NUMBER_OF_REGION_GRID];

};


#endif