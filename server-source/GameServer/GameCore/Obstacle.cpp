#include "stdafx.h"
#include "Obstacle.h"
#include "..\..\Common/Include/Base\Inc\Ini.h"

CObstacle::CObstacle()
{
 	m_iFileCount = 0;
 	for(int i=0;i<MAX_OBSTACLE_FILE_COUNT;i++)
 	{
 		m_strFileList[i].clear();
 	}
}

CObstacle::~CObstacle()
{
 	for(size_t i=0;i<m_mapCbstacleList.size();i++)
 	{
 		SAFE_DELETE_ARRAY(m_mapCbstacleList[(int)i]);
 	}
 	m_mapCbstacleList.clear();
}

void CObstacle::Init()
{
	LoadIni();
	ReadObstacleList();
}

void CObstacle::LoadIni()
{
 	sbase::CIni ini( "./Obstacle/obstacle.ini", false );
 	m_iFileCount = ini.GetData( "parameter", "Count" );
 	for(int i=0;i<m_iFileCount;i++)
 	{
 		char acBuffer[20];
 		::ZeroMemory(acBuffer,sizeof(acBuffer));
 		sprintf(acBuffer,"file%d",i);
 		m_strFileList[i] = ini.GetString( "File" , (const char*)acBuffer );
 	}
}

void CObstacle::ReadObstacleList()
{
 	for(int i=0;i<m_iFileCount;i++)
 	{
 		/*DWORD dwCount = 0;*/
 		FILE* fp = fopen(m_strFileList[i].c_str(),"rb");
 		BYTE* pTable = new BYTE[MAX_BLOCK_COUNT];
 		::ZeroMemory(pTable,sizeof(BYTE)*MAX_BLOCK_COUNT);
 		fread(pTable,sizeof(BYTE)*MAX_BLOCK_COUNT,1,fp);
 		m_mapCbstacleList[i] = pTable;
 		fclose(fp);
 	}
}

bool CObstacle::IsObstacleList(int iMapID,float x,float y)
{
  	float x_start =  -( 256.0f * 2.0f * 0.5f );
  	float y_start =  256.0f * 2.0f * 0.5f;
  	float x_end = -x_start;
  	float y_end = -y_start;
  	if(x<x_start||y>y_start||x>x_end||y<y_end)
  		return true;
  	x += 256.0f * 2.0f * 0.5f;
  	y = ( 256.0f * 2.0f * 0.5f - y );
  	x /= (2.0f/8.0f);
  	y /= (2.0f/8.0f);
  	//int  tx = int( x ), ty = int( y );
  	if(*(m_mapCbstacleList[iMapID]+(int)y*256*8+(int)x))
  		return true;
  	else
  		return false;
}
