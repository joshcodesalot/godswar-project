//========================================================
//
//    Copyright (c) 2007,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� SkillManager.cpp
//    ժ    Ҫ �� ���ܹ���������
//    
//    ��ǰ�汾 �� 1.00
//    ��    �� �� ����
//    ������� �� 2007-03-22
//
//========================================================
#include "stdafx.h"
#include "SkillManager.h"
#include "../World.h"
#pragma warning(disable:4800)

CSkillManager::CSkillManager()
{
	m_ucSkillKindsCount  = 0;
	m_ucSkillRankCount   = 0;
	m_ucGodsSkill        = 0;
	m_SkillAll.clear();
	m_MagicList.clear();
	memset(m_Convert,0L,sizeof(m_Convert));
}

CSkillManager::CSkillManager(CWorld* pWorld):CBaseManager(pWorld)
{
	m_ucSkillKindsCount  = 0;
	m_ucSkillRankCount   = 0;
	m_ucGodsSkill        = 0;
	m_SkillAll.clear();
	m_MagicList.clear();
	memset(m_Convert,0L,sizeof(m_Convert));
}

CSkillManager::~CSkillManager()
{
	for ( size_t i =0 ; i < m_SkillAll.size(); i++ )
	{
		SAFE_DELETE(m_SkillAll[(BYTE)i]); 
	}
	m_SkillAll.clear();
	m_MagicList.clear();
}

void CSkillManager::Release()
{
	delete this;
}

// ----------------------------------------------------------------------------------------------
//�Ӽ��������ļ����ؼ�����Ϣ
// ---------------------------------------------------------------------------------------------- 
bool CSkillManager::Init()
{
	bool value = false;
	if ( LoadSkillConfig() )
		value = true;
	if( LoadMagicConfig() )
		value = true;


	return value;

}

// ----------------------------------------------------------------------------------------------
//���ݼ��ܵ�ID�ͼ��ܵȼ������Ӧ�ļ���ָ��
// ---------------------------------------------------------------------------------------------- 
const SKillData* CSkillManager::GetSkill( int ucID, int  ) const
{
	if ( ucID <= m_ucSkillKindsCount -1 )
	{	
		SKILL_LIST::const_iterator itor  =  m_SkillAll.find( ucID );
		if ( itor !=  m_SkillAll.end() )
		{
			return  (*itor).second;
		}
	}
	return NULL;
}


// ------------------------------------------------------------------------------------------------
//��ѯ����(��ѧ��)
// ------------------------------------------------------------------------------------------------
const SKillData* CSkillManager::GetSkill( int ucID ) const
{
	if ( ucID <= m_ucSkillKindsCount -1 )
	{
		//��ֵ����
		return GetSkill( ucID,1 );
	}
	return NULL;
}



// ------------------------------------------------------------------------------------------------
//���ر�����������
// ------------------------------------------------------------------------------------------------
bool  CSkillManager::LoadSkillConfig()
{
	sbase::CIni  Skill_ini( "skill/skill.ini", false );
	m_ucSkillKindsCount = (BYTE)Skill_ini.GetData( "parameter", "Count" );

	char item[32] ;
	memset( item ,0L, sizeof(item));

	//��������״̬
    std::map<std::string,UINT> ALLEffect;
	for ( int i = 0; i < MAX_SKILL_EFFECT; i++ )
	{
		sprintf( item , "Effect%d", i);
		ALLEffect[ Skill_ini.GetString( "Effect", item ) ]= i ; 
	}

	SKILL_LIST templist ;
    char Data[512];
	UINT Effect;
	float EffectValue;
	std::string strSectionData;

	for ( int i =0; i < m_ucSkillKindsCount; i++ )
	{
		sprintf( item , "%d", i);
		if ( !Skill_ini.SearchSection( item ) )
			continue;

		SKillData *Skill = new SKillData;    
		strcpy( Skill->Name, Skill_ini.GetString( item, "Name") );
		Skill->ucID                  =       (BYTE)i;
		Skill->PrefixID              =       (UINT)Skill_ini.GetData( item, "PrefixID" );
		Skill->RrefixRank            =       (UINT)Skill_ini.GetData( item, "RrefixRank" );
		Skill->TotalRank             =       (UINT)Skill_ini.GetData( item, "TotalRank" );

		//װ������
		std::string TempEuipStr = Skill_ini.GetString( item, "EquipRequest") ;
		int c = 0 ;
		Skill->EquipRequest.clear();
		while (  (c = (int)TempEuipStr.find_first_of(',')) > 0 )
		{
			std::string NewStr;
			NewStr.assign(TempEuipStr,0,c);
			TempEuipStr.erase(0,c+1);
			Skill->EquipRequest.push_back((UINT)atoi(NewStr.c_str()));
		}
		if ( TempEuipStr != "-1" ) //-1����û��װ��Ҫ��
		{
			Skill->EquipRequest.push_back(atoi(TempEuipStr.c_str()))	;
		}

        int b =GetPrivateProfileSection( item, Data, 512, "skill/skill.ini" );
		for( int k = 0; k < b; k++ )
		{
			if( Data[k]   !=   '\0' )   
			{   
				strSectionData   =   strSectionData   +   Data[k];   
			}  
			else
			{
				if(strSectionData   !=   "")   
				{   
					char One[128];
					int c = (int)strSectionData.find_first_of('=');
					std::string tempstr;
					tempstr.assign(strSectionData,0,c);

					std::map<std::string,UINT>::iterator itor = ALLEffect.find( tempstr  );
					if ( itor != ALLEffect.end()  )
					{
						GetPrivateProfileString( item, tempstr.c_str(), "",  One, 128, "skill/skill.ini"  );
						sscanf( One, "%d,%f", &Effect, &EffectValue  );
						Skill->Effect[ Effect ] = EffectValue;
						strSectionData.clear();    

					}
				}   
				strSectionData.clear();
			}
		}
	
		m_SkillAll[ i ] = Skill;
		strSectionData.clear();

	}
	return true;
}

// ------------------------------------------------------------------------------------------------
//����������������
// ------------------------------------------------------------------------------------------------
bool CSkillManager::LoadMagicConfig()
{
	sbase::CIni  Magic_ini( "skill/Magic.ini", false );
	m_ucMagicCount     =  Magic_ini.GetData( "parameter", "MagicCount" );
	//m_ucPhysicalCount  =  (BYTE)Magic_ini.GetData( "parameter", "PhysicsCount" );
	//m_ucGodsSkill      =  (BYTE)Magic_ini.GetData( "parameter", "GodsSkill" );

	MagicData magic;
	char item[32];
	memset( item ,0L, sizeof(item));

	//ħ��
	for (int i =0; i < m_ucMagicCount; i++  )
	{
		sprintf( item , "%d", i);
		if ( !Magic_ini.SearchSection( item ) )
			continue;

		strcpy( magic.cName, Magic_ini.GetString( item, "Name" ) );
		magic.ucID            		=  i ;
		magic.ucStyle               =  (BYTE)Magic_ini.GetData( item,"Style");
		magic.ucEffect              =  (BYTE)Magic_ini.GetData(item,"Effect" );
		magic.sMP                   =  (USHORT)Magic_ini.GetData( item, "MP");
		magic.fPower1                =  Magic_ini.GetFloatData( item, "Power1");
		magic.fPower2                =  Magic_ini.GetFloatData( item, "Power2");
		magic.ucDistance            =  (BYTE)Magic_ini.GetData(item,"Distance");
		magic.ucRange               =  (BYTE)Magic_ini.GetData( item, "Range");
		magic.ucRequest_Lv          =  (BYTE)Magic_ini.GetData( item, "Request_LV");
		magic.uiMoney               =  (BYTE)Magic_ini.GetData( item,"Money");
		magic.ucIntonateTime        =  Magic_ini.GetFloatData( item, "IntonateTime");  
		magic.usCoolingTime         =  Magic_ini.GetFloatData( item, "CoolingTime");  
		magic.ucTarget				=  Magic_ini.GetData( item, "Target" );
		magic.ucAffectObj			=  Magic_ini.GetData( item, "AffectObj" );
		magic.iEffectTurn			=  Magic_ini.GetData( item, "EffectTurn" );
		magic.fTimeInterval			=  Magic_ini.GetFloatData( item, "TimeInterval" );
		magic.iProperty				=  Magic_ini.GetData( item, "Property" );
		magic.bIsEquipUse           =  (bool)Magic_ini.GetData( item, "EquipFlag" );
		magic.iEnmity               =  Magic_ini.GetData( item,"Entity" );
		magic.StatusOdds            =  Magic_ini.GetData( item,"StatusOdds" );
	    magic.Kind                  =  Magic_ini.GetData( item, "Kind");
	    magic.Priority              =  Magic_ini.GetData( item, "Priority");

		//����״̬
		std::string TempStr = Magic_ini.GetString( item, "Status") ;
		int c = 0 ;
        magic.m_Status.clear();
		while (  (c = (int)TempStr.find_first_of(',')) > 0 )
		{
			std::string NewStr;
			NewStr.assign(TempStr,0,c);
			TempStr.erase(0,c+1);
			magic.m_Status.push_back((int)atoi(NewStr.c_str()));
		}
		if ( TempStr != "-1" ) //-1����û��״̬
		{
			magic.m_Status.push_back(atoi(TempStr.c_str()))	;
		}

		//װ������
		std::string TempEuipStr = Magic_ini.GetString( item, "EquipRequest") ;
		int b = 0 ;
		magic.EquipRequest.clear();
		while (  (b = (int)TempEuipStr.find_first_of(',')) > 0 )
		{
			std::string NewStr;
			NewStr.assign(TempEuipStr,0,b);
			TempEuipStr.erase(0,b+1);
			magic.EquipRequest.push_back((UINT)atoi(NewStr.c_str()));
		}
		if ( TempEuipStr != "-1" ) //-1����û��װ��Ҫ��
		{
			magic.EquipRequest.push_back(atoi(TempEuipStr.c_str()))	;
		}
		
		strcpy( magic.ScriptFun, Magic_ini.GetString( item, "ScriptID" ) );
		m_MagicList[ i ] = magic;
	}
	return true;
}


// ------------------------------------------------------------------------------------------------
//�õ���������Ч��������
// ------------------------------------------------------------------------------------------------
SkillHandle CSkillManager::GetSkillEffectHandler( UINT EffectID )
{
	static SkillHandle HandlerTable[]=
	{
		&CGameObject::Handler_PS_HPMax,
		&CGameObject::Handler_PS_MPMax,
		&CGameObject::Handler_PS_Hit,
		&CGameObject::Handler_PS_Dodge,
		&CGameObject::Handler_PS_Attack,
		&CGameObject::Handler_PS_Defend,
		&CGameObject::Handler_PS_MagicAttack,
		&CGameObject::Handler_PS_MagicDefend,
		&CGameObject::Handler_PS_CritAppend,
		&CGameObject::Handler_PS_CritDefen,
		&CGameObject::Handler_PS_DamageSorb,
		&CGameObject::Handler_PS_PhyDamageAppend,
		&CGameObject::Handler_PS_MagicDamageAppend,
		&CGameObject::Handler_PS_ResumeHP,
		&CGameObject::Handler_PS_ResumeMP,
		&CGameObject::Handler_PS_OneStatusAppend,
		&CGameObject::Handler_PS_OneStatudDefend,
		&CGameObject::Handler_PS_Cure,
		&CGameObject::Handler_PS_BeCure,
	};
	return HandlerTable[EffectID];
}

// ------------------------------------------------------------------------------------------------
//�õ�ħ���������
// ------------------------------------------------------------------------------------------------
const MagicData* CSkillManager::GetMagic(INT ucID) const
{
	MAGIC_LIST::const_iterator itor = m_MagicList.find( ucID );
	if ( itor != m_MagicList.end() )
	{
		return &((*itor).second);
	}

	return NULL;

}


CStatusManager::CStatusManager()
{
	m_ucStatusCount = 0;

	memset(m_Convert,0L,sizeof(m_Convert));
}

CStatusManager::CStatusManager(CWorld* pWorld):CBaseManager(pWorld)
{
	m_ucStatusCount = 0;

	memset(m_Convert,0L,sizeof(m_Convert));
}

CStatusManager::~CStatusManager()
{

}

//-------------------------------------------------------------------------------------------------
//��ʼ��״̬����
//-------------------------------------------------------------------------------------------------
bool CStatusManager::Init()
{

	return  __super::Init() && LoadStatusConfig();
}

void CStatusManager::Release()
{
	delete this;
}

// ------------------------------------------------------------------------------------------------
//�õ�ħ���������
// ------------------------------------------------------------------------------------------------
bool CStatusManager::LoadStatusConfig()
{
	sbase::CIni  Status_ini( "skill/Status.ini", false );
	m_ucStatusCount = Status_ini.GetData( "parameter", "Count" );

	char item[32];
	//ÿ��״̬Ч��
	for ( int i = 0; i < MAX_STATUS_EFFECT; i++ )
	{
		sprintf( item , "Effect%d", i);
		CElementEffect *pEffect = new CElementEffect( i,Status_ini.GetString("Status", item ) );
		ASSERT(pEffect);
		CGameObject::RouterHandler( &pEffect->Handler, i );
		m_Effect[i] = pEffect;
	}

	//ÿ��״̬��ʼ��
	std::map<std::string,std::string> m_sectionDataList;
	std::string strSectionData;
	for ( int a = 0; a < m_ucStatusCount; a++ )
	{
        sprintf( item , "%d", a);
		if ( !Status_ini.SearchSection( item ) )
			continue;

		m_sectionDataList.clear();
		strSectionData.clear();
		
		CStatus *pStatus = new CStatus( a, Status_ini.GetData( item, "Time" ), Status_ini.GetData( item, "Style" ),Status_ini.GetData( item, "Kind" ),Status_ini.GetData( item, "priority" ) );
		ASSERT(pStatus);
		m_Status[a] = pStatus;

		//�ֽ��
		char KeyName[512]={ 0 };
		int b = GetPrivateProfileSection(item, KeyName, 512, "skill/Status.ini" );
	    std::vector<int> Effect;
	    std::vector<float> Values;
	    std::vector<int> IntervalTime;
		for( int i = 0; i < b; i++ )
		{
			if(KeyName[i]   !=   '\0')   
			{   
				strSectionData   =   strSectionData   +   KeyName[i];   
			}   
			else 
			{   
				if( strSectionData   !=   "")   
				{   
					int c = (int)strSectionData.find_first_of('=');
					std::string tempstr;
					tempstr.assign(strSectionData,0,c);
					strSectionData.erase(0,c+1);
					m_sectionDataList[tempstr] = strSectionData ;
                    if ( tempstr == "Effect" )  //Ч��
                    {  
                       int Index = (int)strSectionData.find_first_of(',');
					   while ( Index > 0  )
					   {
						   std::string tempstr;
						   tempstr.assign(strSectionData,0,Index);
						   Effect.push_back( atof( tempstr.c_str() ) );
						   strSectionData.erase( 0, Index+1 );
						   Index = (int)strSectionData.find_first_of(',');
					   }

					   Effect.push_back( atof( strSectionData.c_str() ) );
                    }
					else if ( tempstr == "Values")//��ֵ
					{
						int ValuesIndex = (int)strSectionData.find_first_of(',');
						while ( ValuesIndex > 0  )
						{
							std::string ValuesTempstr;
							ValuesTempstr.assign(strSectionData,0,ValuesIndex);
							Values.push_back( atof( ValuesTempstr.c_str() ) );
							strSectionData.erase( 0, ValuesIndex+1 );
							ValuesIndex = (int)strSectionData.find_first_of(',');
						}

						Values.push_back( atof( strSectionData.c_str() ) );
					}
					else if ( tempstr == "Interval" )//���ʱ��
					{
						int TimeIndex = (int)strSectionData.find_first_of(',');
						while ( TimeIndex > 0  )
						{
							std::string TimeTempstr;
							TimeTempstr.assign(strSectionData,0,TimeIndex);
							IntervalTime.push_back( atof( TimeTempstr.c_str() ) );
							strSectionData.erase( 0, TimeIndex+1 );
							TimeIndex = (int)strSectionData.find_first_of(',');
						}

						IntervalTime.push_back( atof( strSectionData.c_str() ) );
					}
					strSectionData.clear();    
				}   
			}
		}
 
		for ( size_t n = 0; n<Effect.size(); n++ )
		{
          pStatus->AddEffect( m_Effect[ Effect[n] ], Values[n], IntervalTime[n] );
		}
	}


	return true;

}

IManager::~IManager()
{

}

//-----------------------------------------------------------------
CElementEffect::CElementEffect(BYTE ID , const char* Name ):m_ID(ID),m_Name(Name)
{

}

//-----------------------------------------------------------------
CElementEffect::~CElementEffect()
{

}

//-----------------------------------------------------------------
CStatus::CStatus(int ID, time_t PersistTime, UINT Style, UINT Kind, UINT Priority  ):m_ID(ID),m_PersistTime(PersistTime),m_Style(Style), m_Kind(Kind),m_Priority(Priority)
{

}

//-----------------------------------------------------------------
CStatus::~CStatus()
{

}

//-----------------------------------------------------------------
void CStatus::AddEffect( CElementEffect *pEffect, float value, time_t PersistTime )
{
	m_EffectMap[pEffect] = value;
	m_EffectTimeMap[pEffect] = PersistTime;
}

//-----------------------------------------------------------------
void CStatus::DealWith( CGameObject *pObj, const time_t &StartTime, map<int,int> &PassTime, bool IsRepeated ) 
{
    EFFECT_MAP::iterator Itor = m_EffectMap.begin();
    for ( ; Itor != m_EffectMap.end(); Itor++ )
    {
       CElementEffect *pEffect;
 	   pEffect = Itor->first;
	   if ( m_EffectTimeMap[pEffect] != 0)
	   {   
		   int Num = ( time(NULL) - StartTime ) / m_EffectTimeMap[pEffect];
		   if ( Num > PassTime[pEffect->GetID()] )
		   { 	   
			    (pObj->*(pEffect->Handler))( Num * Itor->second, false, m_ID );
				PassTime[pEffect->GetID()] = Num;
		   }
	   }
	   else
	   {
          if (  !IsRepeated )
          {
			  (pObj->*(pEffect->Handler))( Itor->second, false, m_ID );
          }
	   }
	   


    }
}

void CStatus::Resume( CGameObject *pObj )
{
	EFFECT_MAP::iterator Itor = m_EffectMap.begin();
	for ( ; Itor != m_EffectMap.end(); Itor++ )
	{
		CElementEffect *pEffect;
		pEffect = Itor->first;

		if ( m_EffectTimeMap[pEffect] != 0 )
		{
            (pObj->*(pEffect->Handler))( 0, true, m_ID );
		}
		else
		{
			(pObj->*(pEffect->Handler))( Itor->second, true, m_ID );
		}
	}
}




