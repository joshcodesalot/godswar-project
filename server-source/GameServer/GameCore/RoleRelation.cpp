//========================================================
//
//    Copyright (c) 2006,欢乐连线工作室
//    All rights reserved.
//
//    文件名称 ： RoleRelation.cpp
//    摘    要 ： 角色关系头文件（好友，黑名单）
//    
//
//    修    改 ： 李锋军
//    完成日期 ： 2007-05-09
//
//========================================================

#include "stdafx.h"

#include "RoleRelation.h"
#include "Altar.h"
#include "../GameCore/Player.h"
#include "..\..\Common\MsgDefine.h"
#include "..\..\Common\MsgTypes.h"
#include "..\..\Common/Include/Base\Inc\IPCChannel.h"
#include "../World.h"


//-----------------------------------------
CRelationManager::CRelationManager()
{

}

//-----------------------------------------
CRelationManager::CRelationManager(CWorld* pWorld):CBaseManager(pWorld)
{

}

//-----------------------------------------
CRelationManager::~CRelationManager()
{

}

//-----------------------------------------
bool
CRelationManager::AddRelation(RELATION & relation )
{
	m_Relation[relation.strName.c_str()] = relation;
	return true;
}

//-----------------------------------------
bool
CRelationManager::DeleteRelation(const string* StrName)
{
	std::map< string , RELATION >::iterator itor = m_Relation.find( *StrName );
	if ( itor != m_Relation.end())
	{
		m_Relation.erase( itor );
		return true;
	}
    return false;
}

//-----------------------------------------
bool
CRelationManager::FindRole(const string * Name)
{
	return ( m_Relation.find( *Name ) != m_Relation.end() );
}

RELATION * CRelationManager::GetRelationRole( const string * Name )
{
	std::map< string , RELATION >::iterator itor =  m_Relation.find( *Name );
	if ( itor != m_Relation.end())
	{
		return &itor->second;
	}
	return NULL;
}

//-----------------------------------------
bool
CRelationManager::IsInBlackList(const string &Name)
{
	std::map< string , RELATION >::iterator itor =  m_Relation.find( Name.c_str() );
	if ( itor != m_Relation.end() )
	{
		if ( strcmp( itor->first.c_str(),Name.c_str() ) == 0 )
		{
			return  (itor->second.Type ==  TYPE_BLACKLIST)? true: false;

		}
	}
	return false;	
}

//-----------------------------------------
bool
CRelationManager::IsOverFlow( RELATION_TYPE eType )
{
	int Num = 0;
	std::map< string , RELATION >::iterator  itor = m_Relation.begin();
	for ( ; itor != m_Relation.end(); itor++ )
	{
		if (itor->second.Type == eType )
			Num++;
	}

	return  ( Num >=  MAX_RELATION_NUM ) ? true : false;
}

//-----------------------------------------
bool
CRelationManager::UpdateRelation(const RELATION &relation)
{  
	std::map< string , RELATION >::iterator itor = m_Relation.find( relation.strName.c_str() );
	if ( itor != m_Relation.end())
	{
		(*itor).second = relation;
	}
	return true;

}

//----------------------------------------------------
// 打包好友信息
//----------------------------------------------------
string CRelationManager::PackRelationInfo( RELATION_TYPE Type )
{
    string TempStr="";
   std::map< string , RELATION >::iterator itor = m_Relation.begin();
   for ( ; itor != m_Relation.end(); itor++ )
   {
	   if ( itor->second.Type == Type )
	   {   
		   TempStr += "[";
		   TempStr += itor->first;
		   TempStr += "]#";
	   }
	  
   }
   return TempStr;
}


//-----------------------------------------------------
// 或得关系名单
//-----------------------------------------------------
void  CRelationManager::GetPlayerList(std::vector<string> &NameVec, RELATION_TYPE TYPE )
{
	std::map< string , RELATION >::iterator  itor = m_Relation.begin();
	for ( ; itor != m_Relation.end(); itor++ )
	{
		if (itor->second.Type == TYPE )
		  NameVec.push_back(itor->first);
	}
}


ConsortiaElem::ConsortiaElem()
{
   m_pModuleAltar = ScriptSystem::Instance()->GetModule("Script.Altar");
   m_pAltar.clear();

}

bool ConsortiaElem::LoadAltar( UINT AltarID,UINT level )
{
	map<int,CAltarElem*>::iterator itor = m_pAltar.find( AltarID );
	if ( itor != m_pAltar.end() )
		return false;

	PyObject* Altar = NULL;
	PyObject *UpCondition  = PyObject_GetAttrString(m_pModuleAltar , "AllAltar");
	if ( NULL == UpCondition )
	{
		PyErr_Print();
		PyErr_Clear();
		return false;
	}

	if (PyTuple_Check(UpCondition) == 1)
		Altar = PyTuple_GetItem(UpCondition , AltarID - 1);

	if ( NULL == Altar )
		return false;

	//****Py_XINCREF(Altar);

	CAltarElem *pNewAltar = new CAltarElem( CAltar::ALTAR_TYPE(AltarID),this,Altar,level);
	m_pAltar[ AltarID ] = pNewAltar;

	return true;
}

bool ConsortiaElem::CreateAltar(int AltarID)
{
	map<int,CAltarElem*>::iterator itor = m_pAltar.find( AltarID );
	if ( itor != m_pAltar.end() )
		return false;

	PyObject* Altar = NULL;
	PyObject *UpCondition  = PyObject_GetAttrString(m_pModuleAltar , "AllAltar");
	if (PyTuple_Check(UpCondition) == 1)
		Altar = PyTuple_GetItem(UpCondition , AltarID - 1);

	//****Py_XINCREF(Altar);
	if ( NULL == Altar )
		return false;

	PyObject *pFunc = PyObject_GetAttrString(Altar, "LvValues");
	PyObject *pArg  = Py_BuildValue("(i)",0);
	PyObject *AltarRequst =  PyEval_CallObject(pFunc, pArg);
	Py_XDECREF(pArg);
	int Money = 0,Bijou=0;
	if (PyTuple_Check(AltarRequst) == 1)
	{
		Money  = PyInt_AsLong( PyTuple_GetItem(AltarRequst,0) );
		Bijou  = PyInt_AsLong( PyTuple_GetItem(AltarRequst,1) );
		if ( lfound < Money || lbijou < Bijou )
			return false;
	}
	else
		return false;

	CAltarElem *pNewAltar = new CAltarElem( CAltar::ALTAR_TYPE(AltarID),this,Altar);
	m_pAltar[ AltarID ] = pNewAltar;
	lfound -= Money;
	lbijou -= Bijou;

	Py_XDECREF(AltarRequst);
	return true;
}


bool ConsortiaElem::DelAltar( int AltarID )
{
   map<int,CAltarElem*>::iterator itor = m_pAltar.find( AltarID );
   if( itor != m_pAltar.end() ) 
   {
      delete itor->second;
      m_pAltar.erase(itor);
	  return true;
   }

   return false;
}


//提升祭坛的供奉等级
bool ConsortiaElem::OblationLevelUp( CPlayer* pPlayer,int ID ,int type)
{
	map<int,CAltarElem*>::iterator itor = m_pAltar.find( ID );
	if ( itor == m_pAltar.end() )
		return false;

	return m_pAltar[ID]->Oblation( pPlayer ,ID ,type);

}

bool ConsortiaElem::AltarLevelUp(CPlayer* pPlayer,int ID)
{
	map<int,CAltarElem*>::iterator itor = m_pAltar.find( ID );
	if ( itor == m_pAltar.end() )
		return false;

    return m_pAltar[ID]->UpGrade();
}

vector<int>& ConsortiaElem::GetHaveAltars()
{
	static vector<int> Has;
	Has.clear();
	map<int,CAltarElem*>::iterator itor = m_pAltar.begin();
	for ( ; itor != m_pAltar.end(); itor++ )
	{
		Has.push_back(itor->first);
	}
	return Has;
}

bool ConsortiaElem::LevelUp( CPlayer *pPlayer )
{
	ConsortiaLevelUp *levleup = pPlayer->s_World->g_pConsortiaManager->GetConsortiaLevelUp(iLevel+1);
	if (!levleup)
	{
		return false;
	}

	if (lfound >= levleup->lfound && lbijou >= levleup->lbijou )
	{
       iLevel++; 
	   lfound -= levleup->lfound;
       lbijou -= levleup->lbijou;
       return true;
	}
     
   return false;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void CConsortiaMgr::Init()
{
	__super::Init();
	dbtool::IDatabase* p_DB = m_pWorld->GetDBAddr( DB_NONE );
	if ( NULL != p_DB)
	{
		//step 1:get all the consortia base information from DB
		char szSQL[128];
		sprintf( szSQL, SQL_SELECT_CONSORTIA_BASE_INFO );
		dbtool::IRecordset *pRecordSet = p_DB->CreateRecordset( SQL_SELECT_CONSORTIA_BASE_INFO, dbtool::MODE_EDIT );
		if ( NULL != pRecordSet)
		{
			UINT Count = pRecordSet->CountRecord();
			for ( UINT i = 0; i < Count; i++ )
			{
				pRecordSet->Move(i);
				dbtool::IRecord  *pRec = pRecordSet->GetRecord();
				ConsortiaElem   ConsortiaBase_Info;
				ConsortiaBase_Info.uiID = (UINT)pRec->Field( UINT(0));
				strcpy(ConsortiaBase_Info.acName , (char*)pRec->Field( 1 ) );
				strcpy(ConsortiaBase_Info.acTextInfo, (char*)pRec->Field( 2 ) );
				ConsortiaBase_Info.lfound  =  (UINT)pRec->Field( 9 );
				ConsortiaBase_Info.lbijou  =  (UINT)pRec->Field( 10 );
				ConsortiaBase_Info.iLevel  =  (UINT)pRec->Field( 11 );
				ConsortiaBase_Info.lCredit =  (UINT)pRec->Field( 12 );
				char * DBString = pRec->Field( 13 );
				AddConsortia( ConsortiaBase_Info );
				CreateAltarFromDB(&m_ConsortiaInfoMap[ConsortiaBase_Info.acName],DBString);
				pRec->Release();
			}
		}
		pRecordSet->Release();

		//step 2: get the all member every consortia
		stdext::hash_map< std::string, ConsortiaElem >::iterator itor = m_ConsortiaInfoMap.begin();
		for ( ; itor != m_ConsortiaInfoMap.end(); itor++ )
		{
			Member MemberMap;
			m_ConsortiaMemberMap[ &itor->second ] = MemberMap;

			sprintf( szSQL, SQL_SELECT_CONSORTIA_MEMBER_INFO, itor->second.acName );
			dbtool::IRecordset *pRecordSet = p_DB->CreateRecordset( szSQL, dbtool::MODE_EDIT );
			if ( NULL != pRecordSet )
			{
				UINT Num = pRecordSet->CountRecord();
				for ( UINT i = 0; i < Num; i++ )
				{
					pRecordSet->Move( i );
					dbtool::IRecord *pRec = pRecordSet->GetRecord();
					ConsortiaRelationElem  MemberInfo;
					MemberInfo.strName = (char*)pRec->Field( UINT( 0 ) );
					//						MemberInfo.S_LV = (BYTE)pRec->Field( 1 );
					MemberInfo.LV = (BYTE)pRec->Field( 2 );
					MemberInfo.Job =  pRec->Field(3);
					MemberInfo.lMapID = (BYTE)pRec->Field(4);
					MemberInfo.ucBusiness = (BYTE)pRec->Field(7);
					MemberInfo.uiContribute = (UINT)pRec->Field(8);


					m_ConsortiaMemberMap[ &itor->second ][ MemberInfo.strName ] = MemberInfo;
					memset( &MemberInfo, 0L, sizeof(MemberInfo) );
					pRec->Release();

				}
			}
			pRecordSet->Release();
		}
	}


	LoadConsortiaInfo("./Item/Consortia.xml");
	_get_timezone(&m_TimeZone);
	m_Timer.Startup(10);
	m_IsTimerOut = false;
}

void CConsortiaMgr::UpData()
{
	if (!m_Timer.IsExpire())
		return;

	m_Timer.Update();
	
	int timenow = GetLocalTime();

	if ( CONSORTIA_REFRESH_DAY - (timenow - (timenow / CONSORTIA_REFRESH_DAY * CONSORTIA_REFRESH_DAY)) < 70 && !m_IsTimerOut)
	{
		m_IsTimerOut = true;
		sbase::ConsoleWriteColorText( FOREGROUND_GREEN,"----每日公会消耗----" );
		UpDataConConsume();
		sbase::ConsoleWriteColorText( FOREGROUND_GREEN,"----每日公会消耗----" );
	}
	else if (  CONSORTIA_REFRESH_DAY - (timenow - (timenow / CONSORTIA_REFRESH_DAY * CONSORTIA_REFRESH_DAY)) > 70)
	{
		m_IsTimerOut = false;
	}

}

void CConsortiaMgr::UpDataConConsume()
{

	//公会维护费与惩罚
	stdext::hash_map< std::string, ConsortiaElem >::iterator itor = m_ConsortiaInfoMap.begin();
	for ( ; itor != m_ConsortiaInfoMap.end(); itor++ )
	{
		//扣除公会资源.如果公会资源不够的话就扣除公会等级
		if (itor->second.lfound < m_ConsortiaConsumeMap[itor->second.iLevel].lfound || itor->second.lbijou < m_ConsortiaConsumeMap[itor->second.iLevel].lfound)
		{
			//公会受到惩罚 降一级  如果降到1级就不在惩罚
			if ( itor->second.iLevel > 1)
			{
				itor->second.iLevel -= 1;
			}
		}
		else
		{
			itor->second.lfound -= m_ConsortiaConsumeMap[itor->second.iLevel].lfound;  //公会资金
			itor->second.lbijou -= m_ConsortiaConsumeMap[itor->second.iLevel].lbijou;  //公会宝石
		}

		//写入数据库   (如果公会每分钟保存一次数据的话， 就不需要写数据库)
// 		dbtool::IDatabase* p_DB = g_pLoader->GetDBAddr( DB_NONE );
// 		if ( NULL != p_DB)
// 		{
// 			string TempStr( "");
// 			char convert[32];
// 			std::map<int ,CAltarElem*>::iterator Altar_itor = itor->second.m_pAltar.begin();
// 			for (; Altar_itor != itor->second.m_pAltar.end();Altar_itor++)
// 			{
// 				TempStr += "[";
// 				itoa( Altar_itor->first , convert , 10 );
// 				TempStr += convert + string(",");
// 				itoa( Altar_itor->second->m_LV , convert , 10 );
// 				TempStr += convert + string("]#");
// 			}
// 
// 			static char szSQL[1000];
// 			memset( szSQL,0L,sizeof(szSQL) );
// 			sprintf( szSQL, SQL_UPDATE_CONSORTIA_INFO ,itor->second.uiID, itor->second.acName, itor->second.acTextInfo, "会长","副会长", "理事", "精英", "会员","见习会员",itor->second.lfound, itor->second.lbijou, itor->second.iLevel, itor->second.lCredit,TempStr.c_str() );
// 			p_DB->ExecuteAsyncSQL( szSQL,NULL,&CConsortiaMgr::DealWith_DelConsortia);
// 
// 		}

	}
	
	
	//向所有的公会的公会成员发送更新消息
	stdext::hash_map< std::string, ConsortiaElem >::iterator itor1 = m_ConsortiaInfoMap.begin();
	for ( ; itor1 != m_ConsortiaInfoMap.end(); itor1++ )
	{
 		MSG_CONSORTIA_BASE_INFO BaseInfo;
		BaseInfo.Head.usSize = sizeof( MSG_CONSORTIA_BASE_INFO );
		BaseInfo.Head.usType = _MSG_CONSORTIA_BASE_INFO;
		strcpy( BaseInfo.acName, itor1->second.acName );
		strcpy( BaseInfo.acTextInfo, itor1->second.acTextInfo );
		BaseInfo.iLevel = itor1->second.iLevel;
		BaseInfo.lCredit = itor1->second.lCredit;
		BaseInfo.lfound = itor1->second.lfound;
		BaseInfo.lbijou = itor1->second.lbijou;
		BaseInfo.MaxMember = GetConsortiaMaxMember(&itor1->second);
		BaseInfo.AltarCounts = itor1->second.m_pAltar.size();
		std::map<int ,CAltarElem*>::iterator Altar_itor = itor1->second.m_pAltar.begin();
		int i=0;
		for (; Altar_itor != itor1->second.m_pAltar.end();Altar_itor++)
		{
			BaseInfo.Altar[i].AltarID = Altar_itor->first;
			BaseInfo.Altar[i].Grade = Altar_itor->second->m_LV;
			i++;
		}
		
		stdext::hash_map< ConsortiaElem*, CConsortiaMgr::Member >::iterator itor2 = m_ConsortiaMemberMap.find( &itor1->second );
		if(  itor2 !=  m_ConsortiaMemberMap.end() )
		{
			CConsortiaMgr::Member member = itor2->second;
			for ( CConsortiaMgr::Member::iterator iter = member.begin(); iter != member.end(); iter++ )
			{
				CPlayer *pPlayer = CPlayer::GetPlayerFromRoleName(  iter->first );
				if ( NULL != pPlayer)
					m_pWorld->SendMsgToClient( &BaseInfo, pPlayer->GetSocket() ); 	
			}
		}
	}
	
}

bool CConsortiaMgr::BackUp_Consortia_info()
{
	int SendLength = 0;
	static BACKUP_MSG  Msg;

// 	if ( m_pWorld->g_pBackUpMgr->GetStatus() )
// 	{
		stdext::hash_map< std::string, ConsortiaElem >::iterator itor = m_ConsortiaInfoMap.begin();
		for ( ; itor != m_ConsortiaInfoMap.end(); itor++ )
		{
			memset( &Msg,0L,sizeof(Msg) );

			string TempStr( "");
			char convert[32];
			std::map<int ,CAltarElem*>::iterator Altar_itor = itor->second.m_pAltar.begin();
			for (; Altar_itor != itor->second.m_pAltar.end();Altar_itor++)
			{
				TempStr += "[";
				itoa( Altar_itor->first , convert , 10 );
				TempStr += convert + string(",");
				itoa( Altar_itor->second->m_LV , convert , 10 );
				TempStr += convert + string("]#");
			}

			sprintf(  Msg.SQL, SQL_UPDATE_CONSORTIA_INFO ,itor->second.uiID, itor->second.acName, itor->second.acTextInfo, "会长","副会长", "理事", "精英", "会员","见习会员",itor->second.lfound, itor->second.lbijou, itor->second.iLevel, itor->second.lCredit,TempStr.c_str() );

			Msg.Head.usType = TYPE_MGR_CONSORTIA;
			Msg.Head.usSize = sizeof(MsgHead)+strlen( Msg.SQL );
			//Msg.Head.usSize = Msg.Head.usSize + (4 - Msg.Head.usSize %4);
//			m_pWorld->g_pBackUpMgr->SetSendBuf((void *)&Msg, Msg.Head.usSize );
//			m_pWorld->g_pBackUpMgr->Write((void *)&Msg, Msg.Head.usSize, SendLength );

			m_pWorld->WriteToBackBuff((char*)&Msg, Msg.Head.usSize);

		}
//	}

	return true;
}

void CConsortiaMgr::DealWith_DelConsortia( dbtool::PSQL_RESULT result )
{
	//记得改写存储过程，还回公会的ID号
	static int i=0;
	if ( !result->Falg &&  !result )
	{
		cout<<"DEL  failed! "<<endl;
	}
	else
	{
		cout<<i++   <<"DEL  success! "<<endl;
	}
}

bool CConsortiaMgr::LoadConsortiaInfo(const char* path)
{
	//cdx
	if (!XMLBase::Load(path))
		return false;

	TiXmlElement* root = XMLBase::m_TiXmlDocument.RootElement();

	if (!root)
		return false;

	if (strcmp(root->Value() , "Consortia"))
		return false;

	TiXmlElement* child = root->FirstChildElement();

	while (child)
	{
		if (strcmp(child->Value() , "ConsortiaConsume") == 0)
		{
			TiXmlElement* element = child->FirstChildElement();

			while (element)
			{
				TiXmlAttribute* attr = element->FirstAttribute();

				int level;
				ConsortiaConsume consume;

				while(attr)
				{
					if (strcmp(attr->Name() , "Clevel") == 0)
					{
						level = attr->IntValue();
						consume.level = level;
					}
					else if (strcmp(attr->Name() , "found") == 0)
						consume.lfound = attr->IntValue();	
					else if (strcmp(attr->Name() , "bijou") == 0)
						consume.lbijou = attr->IntValue();	
					attr = attr->Next();
				}

				m_ConsortiaConsumeMap[level] = consume;

				element = element->NextSiblingElement();
			}
		}
		else if(strcmp(child->Value() , "ConsortiaLevelUp") == 0)
		{
			TiXmlElement* element = child->FirstChildElement();

			while (element)
			{
				TiXmlAttribute* attr = element->FirstAttribute();

				int level;
				ConsortiaLevelUp levelup;

				while(attr)
				{
					if (strcmp(attr->Name() , "Clevel") == 0)
					{
						level = attr->IntValue();
						levelup.level = level;
					}
					else if (strcmp(attr->Name() , "found") == 0)
						levelup.lfound = attr->IntValue();	
					else if (strcmp(attr->Name() , "bijou") == 0)
						levelup.lbijou = attr->IntValue();	
					else if (strcmp(attr->Name() , "member") == 0)
						levelup.member = attr->IntValue();
					else if (strcmp(attr->Name() , "altarNum") == 0)
						levelup.altarNum = attr->IntValue();
					attr = attr->Next();
				}

				m_ConsortiaLevelUpMap[level] = levelup;

				element = element->NextSiblingElement();
			}

		}
		child = child->NextSiblingElement();
	}
	return true;
}


ConsortiaLevelUp * CConsortiaMgr::GetConsortiaLevelUp(int level)
{
	std::map<int , ConsortiaLevelUp>::iterator iter = m_ConsortiaLevelUpMap.find(level);

	if (iter != m_ConsortiaLevelUpMap.end())
	{
		return &iter->second;
	}

	return NULL;
}

//获取公会当前等级可以创建的祭坛个数
int CConsortiaMgr::GetConsortiaMaxAltar(int level)
{
	map<int , ConsortiaLevelUp>::iterator itor = m_ConsortiaLevelUpMap.find(level);
	if (itor != m_ConsortiaLevelUpMap.end())
	{
		return itor->second.altarNum;
	}
	return 0;
}

bool CConsortiaMgr::CreateAltarFromDB(ConsortiaElem *pConsrotiaInfo,string DBString)
{
	string    tmpstr;
	string    tmpprop;
	ItemData  prop;
	size_t a = 0;
	size_t b;

	if ( DBString.empty() )
	{
		return  false;
	}

	while ( !DBString.empty() )
	{
		b = 0;
		a = DBString.find_first_of( '#');
		tmpstr.assign( DBString,0,a);
		b = tmpstr.find_first_of( ']' );
		tmpstr.assign( DBString,1,b-1 );
		DBString.erase(0,a+1);
		if ( !tmpstr.empty() )
		{			
			b = tmpstr.find_first_of(',');
			tmpprop.assign( tmpstr,0,b);
			UINT AltarID = atoi( tmpprop.c_str() );
			tmpstr.erase(0, b+1 );
			UINT Grade = atoi( tmpstr.c_str() );

			pConsrotiaInfo->LoadAltar(AltarID,Grade);
		}

	}
	return true;
}
//删除数据库中的公会
void CConsortiaMgr::DelConsortiaFormDB(ConsortiaElem *pConsrotiaInfo)
{
	char szSQL[512];
	memset( szSQL,0L,sizeof(szSQL) );

	dbtool::IDatabase* DB = m_pWorld->GetDBAddr( DB_NONE );
	if ( !DB )
	{
		ASSERT(0);
		return ;
	}

	sprintf( szSQL, SQL_DELETE_CONSORTIA_INFO ,pConsrotiaInfo->uiID);
	DB->ExecuteAsyncSQL( szSQL, NULL, &CConsortiaMgr::DealWith_DelConsortia );
}

//删除公会
bool CConsortiaMgr::DismissConsortia( ConsortiaElem *pConsortia )
{
	ASSERT(pConsortia);

	stdext::hash_map< ConsortiaElem*, Member >::iterator itor = m_ConsortiaMemberMap.find( pConsortia );
	if ( itor != m_ConsortiaMemberMap.end() )
	{
		m_ConsortiaMemberMap.erase( itor );
	}

	stdext::hash_map< std::string, ConsortiaElem >::iterator iter=  m_ConsortiaInfoMap.find( pConsortia->acName);
	if ( iter != m_ConsortiaInfoMap.end() )
	{
		DelConsortiaFormDB(pConsortia); //数据库中删除公会
		m_ConsortiaInfoMap.erase( iter );
		return true;
	}

	return false;

}

//玩家删除角色，处理工会相关数据
bool CConsortiaMgr::DelRole(const std::string &Name)
{
	stdext::hash_map< std::string, ConsortiaElem >::iterator itor_consortia = m_ConsortiaInfoMap.begin();
	for ( ; itor_consortia != m_ConsortiaInfoMap.end(); itor_consortia++ )
	{
		stdext::hash_map< ConsortiaElem*, Member >::iterator itor_consortia_member = m_ConsortiaMemberMap.find( &itor_consortia->second );
		if ( itor_consortia_member != m_ConsortiaMemberMap.end() )
		{
			Member::iterator itor = itor_consortia_member->second.find( Name );
			if ( itor != itor_consortia_member->second.end() )
			{
				if ( itor->second.m_Data._Ttype.Job == CONSORTIA_CHAIRMAN)
				{
					return false;
				}
				itor_consortia_member->second.erase(itor);
				return true;
			}
		}

	}

	return true;
}