//========================================================
//
//    Copyright (c) 2006,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� RoleRelation.h
//    ժ    Ҫ �� ��ɫ��ϵͷ�ļ������ѣ������������ᣩ
//    
//
//    ��    �� �� ����
//    ������� �� 2007-05-09
//
//========================================================
#pragma  once
#pragma warning(disable:4201)
#pragma warning(disable:4503)
#include <string>
#include <algorithm>
#include <hash_map>
#include "..\..\Common/Include/DB\Inc\Database.h"
#include "..\ServiceLoader.h"
#include "..\ErrorMessage.h"
#include ".\Altar.h"
#include ".\ObjMgr.h"
#include "..\..\Common/Include/PyScript\ScriptSystem.h"
#include "..\GameCore\GameData.h"
#include "../../Common/Include/Base/xml/XMLBase.h"
#include "../GSMaster.h"

#include "BaseManager.h"




enum RELATION_TYPE
{
	TYPE_FRIEND = 0x0000,             //����
	TYPE_BLACKLIST ,                  //������
};

enum  RELATION_ERRO
{
	RELATION_NONE   = 0x00,
	RELATION_MEMEBER_FULL ,          //��Ա
	RELATION_MEMEBER_EXIST,          //��Ա����
	RELATION_REGISTERED,             //�ù����Ѿ�ע��
	RELATION_CONSORTIA_NOT_EXIST,     //���᲻����
};

class CPlayer;
typedef struct RelationRole 
{
	string           strName;   		 //����
	RELATION_TYPE    Type;      		 //���ͣ����ѣ�
	USHORT           Business;           //ְҵ
	USHORT           Camp;               //��Ӫ
	USHORT           City;               //����
	USHORT           LV;                 //�ȼ�
	RelationRole & operator = (const RelationRole &Role)
	{
		Type = Role.Type;
		Business = Role. Business;
		Camp = Role.Camp ;  
		City = Role.City ;  
		LV = Role.LV ;     

		return *this;
	}
}RELATION,*PRELATION;

class  CRelationManager : public CBaseManager
{
	friend class CPlayer;
public:
	CRelationManager();

	CRelationManager(CWorld* pWorld);

	~CRelationManager();
	bool  AddRelation(  RELATION & relation );
	bool  DeleteRelation( const string* StrName);
	bool  UpdateRelation( const RELATION &relation);
	bool  FindRole( const string * Name);
	bool  IsOverFlow( RELATION_TYPE eType );
	bool  IsInBlackList(const string &Name);								
	string PackRelationInfo( RELATION_TYPE Type );
	void  GetPlayerList(std::vector<string> &NameVec, RELATION_TYPE TYPE );
	void  Clear(){ m_Relation.clear();};
	RELATION * GetRelationRole( const string * Name );
protected:

private:
	std::map< string , RELATION >       m_Relation;             //����,������

};

/////////////////////////////////////////////////////////////////////////////
//��ϵ��Ա����
struct  RelationElem
{
	string           strName;   		 //����
	RELATION_TYPE    Type;      		 //���ͣ����ѣ�
	USHORT           Business;           //ְҵ
	USHORT           Camp;               //��Ӫ
	USHORT           City;               //����
	USHORT           LV;                 //սʿְҵ�ȼ�
};

struct ConsortiaRelationElem  
{
	string           strName;   		 //����
	USHORT           lMapID;             //����
	USHORT           LV;                 //ְҵ�ȼ�
	BYTE             Job;                //ְλ
	BYTE             ucBusiness;         //ְҵ
	UINT             uiContribute;       //����
};

//����ά���������ñ�ṹ��
struct ConsortiaConsume
{
	int  level;						   //����ȼ�
	long lfound;                       //�ʽ�
	long lbijou;                       //��ʯ
};
struct ConsortiaLevelUp 
{
	int  level;						   //����ȼ�
	long lfound;                       //�ʽ�
	long lbijou;                       //��ʯ
	int member;						   //��������
	int altarNum;					   //��̳����
};
//������Ϣ
class   ConsortiaElem
{
	friend class CAltarElem;
    /*-------------------------------------------*/
    /*               ��Ա������               */
    /*-------------------------------------------*/
public:
	ConsortiaElem();
	/*--------------------------------------------------------
	���ܣ�����������ʱ�����ݿ��ж�ȡ���еļ�̳
	������AltarID:��̳ID�� level����̳�ȼ�
	���أ�bool���Ƿ������ɹ�
	���ߣ�����
	��ע��������ӦȨ�޵Ĺ����Ա������̳�ĳ���
	���ڣ�2008-3-20
	----------------------------------------------------------*/
	bool LoadAltar( UINT AltarID,UINT level );

	/*--------------------------------------------------------
	���ܣ�������̳
	������
	���أ�
	���ߣ�����
	��ע������ĳ���Եļ�̳
	���ڣ�2008-3-25
	----------------------------------------------------------*/
	bool CreateAltar(int AltarID);

    /*--------------------------------------------------------
	  ���ܣ�ɾ����̳
	  ������pAltar:Ҫɾ���ļ�̳
	  ���أ�bool��ɾ����̳�Ƿ�ɹ�
	  ���ߣ�����
	  ��ע������Ȩ�޵Ĺ����Աɾ����ǰ����
	  ���ڣ�2008-3-20
    ----------------------------------------------------------*/
    bool DelAltar( int AltarID );

	/*--------------------------------------------------------
	  ���ܣ���̳����
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע�����й���Ȩ������Ա������̳�ȼ�
	  ���ڣ�2008-3-25
	----------------------------------------------------------*/
	bool AltarLevelUp(CPlayer* pPlayer,int ID);

	/*--------------------------------------------------------
	���ܣ�������м�̳ID
	������
	���أ�
	���ߣ�����
	��ע�����ű�����
	���ڣ�2008-3-25
	----------------------------------------------------------*/
	vector<int>& GetHaveAltars();

	/*--------------------------------------------------------
	  ���ܣ���������ȼ�
	  ������pPlaye��Ҫ��������ȼ������
	  ���أ�bool:�����ɹ�����true,���򷵻�false
	  ���ߣ�����
	  ��ע������Ȩ�޵Ĺ����Ա������������ȼ�
	  ���ڣ�2008-3-25
	----------------------------------------------------------*/
	bool LevelUp( CPlayer *pPlayer );
	/*--------------------------------------------------------
	���ܣ�������̳�Ĺ���ȼ�
	������
	���أ�
	���ߣ�����
	��ע�������ù�����������̳�Ĺ���ȼ�
	���ڣ�2008-3-21
	----------------------------------------------------------*/
	bool OblationLevelUp( CPlayer* pPlayer, int ID ,int type);
	
	

protected:
private:


	/*-------------------------------------------*/
	/*               ��Ա������               */
	/*-------------------------------------------*/
public:
	char acName[64];                    //��������
	char acTextInfo[256];               //���ṫ��
	mutable UINT uiID;                   //����ID
	long lfound;                        //�ʽ�
	long lbijou;                        //��ʯ
	int iLevel;                        //����ȼ�
	long lCredit;                       //��������
	/*----------------------------------------------------------
	���ܣ��������̳������
	���ߣ�����
	���ڣ�2008-3-25
	----------------------------------------------------------*/
	map<int,CAltarElem*> m_pAltar;
protected:

private:

    PyObject *m_pModuleAltar;
};


template < typename T >
class Clustering
{
public:	
	union NewType
	{  
		struct  
		{
			T  _Ttype;  
		};
		CPlayer *m_player;
	};
	NewType m_Data;

	Clustering(){};
	Clustering(const T& _Ttype)
	{ 
		m_Data.m_player = NULL;
		memcpy( &m_Data._Ttype, &_Ttype, sizeof(T) );
	};
// 	Clustering(const Clustering<T>& _Ttype)
// 	{
//         memcpy( &m_Data, &_Ttype.m_Data, sizeof(NewType) );
// 	};
	~Clustering(){};
};

static char*  SQL_SELECT_CONSORTIA_MEMBER_INFO   =  "CALL SP_SELECT_CONSORTIA_MEMBER_INFO(\'%s\');";
static char*  SQL_SELECT_CONSORTIA_BASE_INFO     =  "CALL SP_SELECT_CONSORTIA_BASE_INFO( );";

template< typename T, int MAXSIZE >
class  CTRelationMgr
{

	friend class CPlayer;
	typename Clustering<T>  ClusteringElems;

public:
	CTRelationMgr(){};
	virtual ~CTRelationMgr(){};
protected:
	virtual void Init(){ m_MemberNameMap.clear(); };
	virtual RELATION_ERRO Add( T &rElements)
	{ 
		if ( m_MemberNameMap.size() >= MAXSIZE )
		{
			return  RELATION_MEMEBER_FULL;
		}

		if ( m_MemberNameMap.find( rElements.strName ) != m_MemberNameMap.end() )
		{
			return  RELATION_MEMEBER_EXIST;
		}

		m_MemberNameMap[rElements.strName] = rElements;
		return  RELATION_NONE;
	};
	bool Del( std::string &Name ) 
	{
		stdext::hash_map< std::string, Clustering<T> >::iterator itor =  m_MemberNameMap.find( Name );
		if ( itor != m_MemberNameMap.end() )
		{
			m_MemberNameMap.erase( itor );
			return true;
		}

		return false;
	};
protected:

	//member information
	stdext::hash_map< std::string, Clustering<T> > m_MemberNameMap;
};
	
class  CConsortiaMgr : public CTRelationMgr< ConsortiaRelationElem, MAX_CONSORTIA_MEMBER_SIZE > ,public XMLBase,public CBaseManager
{
	friend class CPlayer;
    typedef Clustering<ConsortiaRelationElem>  ClusteringElems;
	typedef stdext::hash_map< std::string, ClusteringElems > Member;

public:
	CConsortiaMgr(){ m_MemberNameMap.clear(); m_ConsortiaMemberMap.clear(); };
	CConsortiaMgr(CWorld* pWorld):CBaseManager(pWorld){ m_MemberNameMap.clear(); m_ConsortiaMemberMap.clear(); };
	~CConsortiaMgr(){};

	void Release(){ delete this; };

	//initialize all consortia information
	virtual void Init();

	///////by cdx//////////////////////////////////////////////////////////
	void UpData();  //����ÿ�ո�������
	inline time_t GetLocalTime()
	{
		return m_TimeZone + time(0);
	}
	bool BackUp_Consortia_info(); //ÿʮ���ӱ���һ�����ݿ�
	void UpDataConConsume();     //ÿ������һЩ������Դ
	static void DealWith_DelConsortia( dbtool::PSQL_RESULT result );
	bool LoadConsortiaInfo(const char* path);  //��ӹ����һЩ���ã�ÿ��ά����������ģ��������������ĵȣ�
	ConsortiaLevelUp * GetConsortiaLevelUp(int level); //��ȡ����������Ҫ����Դ
	int GetConsortiaMaxAltar(int level);				//��ȡ���ᵱǰ�ȼ����Դ����ļ�̳����
	bool CreateAltarFromDB(ConsortiaElem *pConsrotiaInfo,string DBString); //�����ݿ��ȡ���ᴴ���ļ�̳
	void DelConsortiaFormDB(ConsortiaElem *pConsrotiaInfo);		//�����ɢʱɾ�����ݿ��еĹ�����Ϣ
	bool DismissConsortia( ConsortiaElem *pConsortia );     //ɾ������
	bool DelRole(const std::string &Name);					//���ɾ����ɫ���������������
	/////by cdx/////////////////////////////////////////////////////////////

	//the operator of member of one consortia 
	virtual bool Add( ConsortiaElem *pConsortia, ConsortiaRelationElem &rElements )
	{
		stdext::hash_map< ConsortiaElem*, Member >::iterator itor = m_ConsortiaMemberMap.find( pConsortia );
		if ( itor != m_ConsortiaMemberMap.end() )
		{
			itor->second[ rElements.strName ] = rElements;
			return false;
		}

		return true;

	};
 
	bool HaveMember( ConsortiaElem *pConsortia, const std::string &Name )
	{ 
		//ASSERT(pConsortia);
		if (!pConsortia)
			return true;
		stdext::hash_map< ConsortiaElem*, Member >::iterator itor = m_ConsortiaMemberMap.find( pConsortia );
		if ( itor != m_ConsortiaMemberMap.end() )
		{
			Member::iterator iter = itor->second.find( Name );
			if ( iter != itor->second.end() )
			{
				return true;
			}
		}
		return false;
	}

	virtual bool Del( ConsortiaElem *pConsortia, const std::string &Name )
	{ 
		stdext::hash_map< ConsortiaElem*, Member >::iterator itor = m_ConsortiaMemberMap.find( pConsortia );
		if ( itor != m_ConsortiaMemberMap.end() )
		{
			Member::iterator iter = itor->second.find( Name );
			if ( iter != itor->second.end() )
			{
				itor->second.erase(iter);
				return true;
			}	
		}

		return false;
	};

	//get the special consortia information from consortia name
	ConsortiaElem* GetConsortiaInfo( const std::string& Name )
	{ 
		stdext::hash_map< std::string, ConsortiaElem >::iterator itor = m_ConsortiaInfoMap.find( Name );
		if ( itor != m_ConsortiaInfoMap.end() )
		{
			return &(itor->second);
		}
		return NULL; 
	};

	//get the special consortia information from consortia name
	ConsortiaElem* GetConsortiaInfo( UINT ID )
	{
       stdext::hash_map< std::string, ConsortiaElem >::iterator itor = m_ConsortiaInfoMap.begin();
	   for ( ; itor != m_ConsortiaInfoMap.end(); itor++ )
	   {
		   if ( itor->second.uiID  ==  ID )
		   {
			   return &(itor->second);
		   }
	   }
	   return NULL;
	}



	//add a new consortia
	ConsortiaElem* AddConsortia( ConsortiaElem &Info  )
	{ 
		if ( m_ConsortiaInfoMap.find( Info.acName ) == m_ConsortiaInfoMap.end() )
		{   
			Member tempMember;
			m_ConsortiaInfoMap[ Info.acName ] = Info;
            m_ConsortiaMemberMap[ &m_ConsortiaInfoMap[ Info.acName ] ] = tempMember;
			return &m_ConsortiaInfoMap[ Info.acName ];
		}

		return NULL;
	};

	//delete the consortia
	bool DelConsortia( std::string& Name ) 
	{ 
		stdext::hash_map< std::string, ConsortiaElem >::iterator itor = m_ConsortiaInfoMap.find( Name );
		if ( itor != m_ConsortiaInfoMap.end() )
		{
			m_ConsortiaInfoMap.erase( itor );
			return true;
		}
		return false; 
	};

	//change the job of member 
	bool GrantJob( ConsortiaElem *pConsortia, const std::string& OldPlayerName, std::string& NewPlayerName, CONSORTIA_JOB_TYPE Duty )
	{
		ClusteringElems *pOld,*pNew;
		pOld = GetFutureMemberWithJob( pConsortia, OldPlayerName );
		pNew = GetFutureMemberWithJob( pConsortia, NewPlayerName );
		if ( pOld && pNew )
		{
			if ( pOld->m_Data._Ttype.Job >= CONSORTIA_ASSISTANT_CHAIRMAN )
			{
				pNew->m_Data._Ttype.Job = (BYTE)Duty; 
				return true;
			}
			else 
			{
				return false;
			}
		}
		return false;
	};

	//get the last job memeber
	ClusteringElems *GetCurMemberWithJob( ConsortiaElem *pConsortia, CONSORTIA_JOB_TYPE EType, std::string strName = "" )
	{
		stdext::hash_map< ConsortiaElem*, Member >::iterator itor = m_ConsortiaMemberMap.find( pConsortia );
		if ( itor != m_ConsortiaMemberMap.end() )
		{
			Member MemberMap = itor->second;
			Member::iterator MemberItor = MemberMap.begin();
			for( ; MemberItor != MemberMap.end(); MemberItor++  )
			{
				if ( strName == "")
				{
					if ( MemberItor->second.m_Data._Ttype.Job == EType )
					{
						return &m_ConsortiaMemberMap[ pConsortia ][ MemberItor->second.m_Data._Ttype.strName ];
					}
				}
				else
				{
					if ( MemberItor->second.m_Data._Ttype.Job == EType && strName == MemberItor->second.m_Data._Ttype.strName )
					{
						return &m_ConsortiaMemberMap[ pConsortia ][ MemberItor->second.m_Data._Ttype.strName ];
					}
				}

			}
		 }

		return NULL;
	};

	bool IsMemberFull( ConsortiaElem *pConsortia )
	{
		ASSERT(pConsortia);
		int MaxMember = m_ConsortiaLevelUpMap[pConsortia->iLevel].member;
		stdext::hash_map< ConsortiaElem*, Member >::iterator itor = m_ConsortiaMemberMap.find( pConsortia );
		if ( itor != m_ConsortiaMemberMap.end() )
		{
			return itor->second.size() >= MaxMember;
		}
        return true;
	}

	UINT GetConsortiaSize( ConsortiaElem *pConsortia )
	{
		ASSERT(pConsortia);
		stdext::hash_map< ConsortiaElem*, Member >::iterator itor = m_ConsortiaMemberMap.find( pConsortia );
		if ( itor != m_ConsortiaMemberMap.end() )
		{
			return (UINT)itor->second.size();
		}
		return 0;
	}

	int GetConsortiaMaxMember(ConsortiaElem *pConsortia)
	{
		if (!pConsortia)
		{
			return 0;
		}
		return m_ConsortiaLevelUpMap[pConsortia->iLevel].member;
	}

protected:

private:
	//consortia information
	stdext::hash_map< std::string, ConsortiaElem > m_ConsortiaInfoMap;
	stdext::hash_map< ConsortiaElem*, Member > m_ConsortiaMemberMap;
	sbase::CTimer m_Timer;
	long m_TimeZone;	//GMTƫ��
	bool m_IsTimerOut;
	std::map<int , ConsortiaConsume> m_ConsortiaConsumeMap;  //����ά���������ñ�
	std::map<int , ConsortiaLevelUp> m_ConsortiaLevelUpMap;  //���������������ñ�
private:
	
	//get the one that will take a post of the job
	ClusteringElems *GetFutureMemberWithJob( ConsortiaElem *pConsortia, const std::string& PlayerName )
	{
		stdext::hash_map< ConsortiaElem*, Member >::iterator itor = m_ConsortiaMemberMap.find( pConsortia );
		if ( itor != m_ConsortiaMemberMap.end() )
		{
			Member MemberMap = itor->second;
			Member::iterator MemberItor = MemberMap.find( PlayerName );
			if ( MemberItor != MemberMap.end() )
			{
				return &m_ConsortiaMemberMap[ pConsortia ][ MemberItor->second.m_Data._Ttype.strName ];
			}
		}
		return NULL;
	};

};

///////////////////////////////////////////////////////////////////////////////


