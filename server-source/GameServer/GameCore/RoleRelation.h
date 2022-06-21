//========================================================
//
//    Copyright (c) 2006,欢乐连线工作室
//    All rights reserved.
//
//    文件名称 ： RoleRelation.h
//    摘    要 ： 角色关系头文件（好友，黑名单，公会）
//    
//
//    修    改 ： 李锋军
//    完成日期 ： 2007-05-09
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
	TYPE_FRIEND = 0x0000,             //好友
	TYPE_BLACKLIST ,                  //黑名单
};

enum  RELATION_ERRO
{
	RELATION_NONE   = 0x00,
	RELATION_MEMEBER_FULL ,          //满员
	RELATION_MEMEBER_EXIST,          //成员存在
	RELATION_REGISTERED,             //该公会已经注册
	RELATION_CONSORTIA_NOT_EXIST,     //公会不存在
};

class CPlayer;
typedef struct RelationRole 
{
	string           strName;   		 //名字
	RELATION_TYPE    Type;      		 //类型（好友）
	USHORT           Business;           //职业
	USHORT           Camp;               //阵营
	USHORT           City;               //城市
	USHORT           LV;                 //等级
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
	std::map< string , RELATION >       m_Relation;             //好友,黑名单

};

/////////////////////////////////////////////////////////////////////////////
//关系成员数据
struct  RelationElem
{
	string           strName;   		 //名字
	RELATION_TYPE    Type;      		 //类型（好友）
	USHORT           Business;           //职业
	USHORT           Camp;               //阵营
	USHORT           City;               //城市
	USHORT           LV;                 //战士职业等级
};

struct ConsortiaRelationElem  
{
	string           strName;   		 //名字
	USHORT           lMapID;             //城市
	USHORT           LV;                 //职业等级
	BYTE             Job;                //职位
	BYTE             ucBusiness;         //职业
	UINT             uiContribute;       //贡献
};

//公会维护费用配置表结构体
struct ConsortiaConsume
{
	int  level;						   //公会等级
	long lfound;                       //资金
	long lbijou;                       //宝石
};
struct ConsortiaLevelUp 
{
	int  level;						   //公会等级
	long lfound;                       //资金
	long lbijou;                       //宝石
	int member;						   //公会人数
	int altarNum;					   //祭坛个数
};
//工会信息
class   ConsortiaElem
{
	friend class CAltarElem;
    /*-------------------------------------------*/
    /*               成员方法区               */
    /*-------------------------------------------*/
public:
	ConsortiaElem();
	/*--------------------------------------------------------
	功能：服务器启动时从数据库中读取所有的祭坛
	参数：AltarID:祭坛ID号 level：祭坛等级
	返回：bool：是否升级成功
	作者：李锋军
	备注：具有相应权限的公会成员升级祭坛的出来
	日期：2008-3-20
	----------------------------------------------------------*/
	bool LoadAltar( UINT AltarID,UINT level );

	/*--------------------------------------------------------
	功能：创建祭坛
	参数：
	返回：
	作者：李锋军
	备注：创建某属性的祭坛
	日期：2008-3-25
	----------------------------------------------------------*/
	bool CreateAltar(int AltarID);

    /*--------------------------------------------------------
	  功能：删除祭坛
	  参数：pAltar:要删除的祭坛
	  返回：bool：删除祭坛是否成功
	  作者：李锋军
	  备注：具有权限的公会成员删除当前公会
	  日期：2008-3-20
    ----------------------------------------------------------*/
    bool DelAltar( int AltarID );

	/*--------------------------------------------------------
	  功能：祭坛升级
	  参数：
	  返回：
	  作者：李锋军
	  备注：具有公会权限所会员提升祭坛等级
	  日期：2008-3-25
	----------------------------------------------------------*/
	bool AltarLevelUp(CPlayer* pPlayer,int ID);

	/*--------------------------------------------------------
	功能：或得已有祭坛ID
	参数：
	返回：
	作者：李锋军
	备注：供脚本调用
	日期：2008-3-25
	----------------------------------------------------------*/
	vector<int>& GetHaveAltars();

	/*--------------------------------------------------------
	  功能：提升公会等级
	  参数：pPlaye：要提升公会等级的玩家
	  返回：bool:提升成功返回true,否则返回false
	  作者：李锋军
	  备注：具有权限的公会成员用来提升公会等级
	  日期：2008-3-25
	----------------------------------------------------------*/
	bool LevelUp( CPlayer *pPlayer );
	/*--------------------------------------------------------
	功能：提升祭坛的供奉等级
	参数：
	返回：
	作者：李锋军
	备注：提升该公会所创建祭坛的供奉等级
	日期：2008-3-21
	----------------------------------------------------------*/
	bool OblationLevelUp( CPlayer* pPlayer, int ID ,int type);
	
	

protected:
private:


	/*-------------------------------------------*/
	/*               成员变量区               */
	/*-------------------------------------------*/
public:
	char acName[64];                    //公会名称
	char acTextInfo[256];               //公会公告
	mutable UINT uiID;                   //工会ID
	long lfound;                        //资金
	long lbijou;                        //宝石
	int iLevel;                        //公会等级
	long lCredit;                       //公会声望
	/*----------------------------------------------------------
	功能：所供奉祭坛的容器
	作者：李锋军
	日期：2008-3-25
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
	void UpData();  //公会每日更新消耗
	inline time_t GetLocalTime()
	{
		return m_TimeZone + time(0);
	}
	bool BackUp_Consortia_info(); //每十分钟备份一次数据库
	void UpDataConConsume();     //每天消耗一些公会资源
	static void DealWith_DelConsortia( dbtool::PSQL_RESULT result );
	bool LoadConsortiaInfo(const char* path);  //添加公会的一些配置（每日维护公会的消耗，公会升级的消耗等）
	ConsortiaLevelUp * GetConsortiaLevelUp(int level); //获取公会升级需要的资源
	int GetConsortiaMaxAltar(int level);				//获取公会当前等级可以创建的祭坛个数
	bool CreateAltarFromDB(ConsortiaElem *pConsrotiaInfo,string DBString); //从数据库读取公会创建的祭坛
	void DelConsortiaFormDB(ConsortiaElem *pConsrotiaInfo);		//公会解散时删除数据库中的公会信息
	bool DismissConsortia( ConsortiaElem *pConsortia );     //删除公会
	bool DelRole(const std::string &Name);					//玩家删除角色，处理工会相关数据
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
	long m_TimeZone;	//GMT偏差
	bool m_IsTimerOut;
	std::map<int , ConsortiaConsume> m_ConsortiaConsumeMap;  //公会维护费用配置表
	std::map<int , ConsortiaLevelUp> m_ConsortiaLevelUpMap;  //公会升级费用配置表
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


