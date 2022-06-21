#include "stdafx.h"
#include "./DropItemMgr.h"


CDropItemMgr::CDropItemMgr()
{

}

CDropItemMgr::~CDropItemMgr()
{

}

bool CDropItemMgr::Init( char* DropFileIndex )
{ 
	if ( !strcmp(DropFileIndex,"NULL") )
		return false;

	char Drop[64];
	sprintf( Drop, "./Drop/Drop%s.ini", DropFileIndex );

	sbase::CIni DropIni( Drop, false );
	int Num = DropIni.GetData("sys","Count" );

	for ( int a = 0; a < Num; a++ )
	{
		DropItemElem  Elem;
		char  base[16];
		sprintf( base, "%d", a );
		Elem.iBaseID = DropIni.GetData( base, "BaseID" );
		Elem.iProbability = DropIni.GetData( base, "Probability" );

		string Append = DropIni.GetString( base, "Append" );

		int ValuesIndex = (int)Append.find_first_of(',');
		Elem.m_AppendCount = 0;
		while ( ValuesIndex > 0  )
		{
			std::string ValuesTempstr;
			ValuesTempstr.assign( Append, 0, ValuesIndex );
			Elem.arrAppend[Elem.m_AppendCount] = atoi( ValuesTempstr.c_str() );
			Append.erase( 0, ValuesIndex+1 );
			ValuesIndex = (int)Append.find_first_of(',');
			Elem.m_AppendCount++;
		}

		if ( Append.c_str() != "" )
		{
			Elem.arrAppend[Elem.m_AppendCount] = atoi( Append.c_str() );
			Elem.m_AppendCount++;
		}
		Elem.AppLV = DropIni.GetData( base, "AppLV" );

		this->Push( Elem );
	}

	return true;
}

UINT CDropItemMgr::FindThisProbability( int Persent )
{ 
   std::vector<DropItemElem>::iterator itor = Begin();
   for ( ; itor != End(); itor++ )
   {
	  if ( (*itor).iProbability >= 100 - Persent  )
		  return (*itor).iBaseID;
   }

   return 0;
};
///
int CDropItemMgr::GetAppendValues( UINT BaseID, int AppIndex )
{
	std::vector<DropItemElem>::iterator itor = Begin();

	for ( ; itor != End(); itor++ )
	{
	   if ( (*itor).iBaseID == BaseID )
		   return (*itor).arrAppend[AppIndex];
	}

	return 0;
}

int CDropItemMgr::GetAppendLV( UINT BaseID )
{
	std::vector<DropItemElem>::iterator itor = Begin();

	for ( ; itor != End(); itor++ )
	{
		if ( (*itor).iBaseID == BaseID )
			return (*itor).AppLV;
	}

	return 0;
}

int CDropItemMgr::GetBaseLV( UINT BaseID )
{
	std::vector<DropItemElem>::iterator itor = Begin();

	for ( ; itor != End(); itor++ )
	{
		if ( (*itor).iBaseID == BaseID )
			return (*itor).BaseLV;
	}

	return 0;
}

int CDropItemMgr::GetAppCount( UINT BaseID )
{
	std::vector<DropItemElem>::iterator itor = Begin();

	for ( ; itor != End(); itor++ )
	  if ( (*itor).iBaseID == BaseID )
         return (int)(*itor).m_AppendCount;

	return 0;
}
