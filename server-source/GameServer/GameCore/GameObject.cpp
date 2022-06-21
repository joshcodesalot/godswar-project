//========================================================
//    Copyright (c) 2006,欢乐连线工作室
//    All rights reserved.
//
//    文件名称 ： GameObject.cpp
//    摘    要 ： 动态对象功能模块
//    
//    原始版本 ： 1.00
//    作    者 ： 林德辉
//    完成日期 ： 2007-01-17
//
//    当前版本 ： 1.01
//    作    者 ： 李锋军
//    完成日期 ： 2007-05-17
//
//========================================================

#include "StdAfx.h"
#include "GameObject.h"
#include "Buff.h"
#include "..\DataCore.h"
#include "SkillManager.h"
#include "../GameScript.h"
#include "../World.h"



#include ".\Mat.h"
extern MSG_LEAVE msg_leave;

CWorld* CGameObject::s_World = NULL;

//-------------------------------------------------------
CGameObject::CGameObject(void)
:	m_AttackRadius(Melee)
{
	m_lID = 0;
	m_bActive = false;

	m_pBuff = new CBuff[MAX_BUFF];
	m_pDeBuff = new CBuff[MAX_DEBUFF];
	m_pRegion = NULL;

	m_lDanger = 0;		// 威胁值

	m_timeAttack.Startup( 1 );
	memset( &m_ObjectData, 0, sizeof(ObjectData) );
}

//------------------------------------------------------------------------------------
CGameObject::~CGameObject(void)
{
	SAFE_DELETE_ARRAY( m_pBuff );
	SAFE_DELETE_ARRAY( m_pDeBuff );
	ClearPath();

}

//------------------------------------------------------------------------------------
long CGameObject::SetActive( bool active )
{
	m_bActive = active;
	return 0;
}

//------------------------------------------------------------------------------------
CCell* CGameObject::SetPos( float x, float y, float z, bool IsFirst )
{
	if ( IsFirst )
	{
		m_fX = x;
		m_fY = 0.0f;
		m_fZ = z;

		m_ObjectData.m_fX = x;
		m_ObjectData.m_fY = y;
		m_ObjectData.m_fZ = z;

		CCell  *pOldCell =  GetRegion()->GetMap()->GetCell( m_fX, m_fZ );
		m_pCurrentCell = pOldCell;
		pOldCell->AddObj( this );
		return pOldCell;
	}


	CCell  *pOldCell =  GetRegion()->GetMap()->GetCell( m_fX, m_fZ );
	m_fX = x;
	m_fY = 0.0f;
	m_fZ = z;

	m_ObjectData.m_fX = x;
	m_ObjectData.m_fY = y;
	m_ObjectData.m_fZ = z;

	//::OutputDebugString("Walk Go ON !" );
	//cout<<"Walk Go ON: "<<"X="<<m_fX<<",Z="<<m_fZ<<endl;

	CCell  *pNewCell = GetRegion()->GetMap()->GetCell( x, z );
	if ( pOldCell != pNewCell )
	{
		m_pCurrentCell = pNewCell;
		pOldCell->RemoveObj( this );
		pNewCell->AddObj( this );
		if (  m_eState != OBJECT_DEAD )
		{
			AynObjToObj( pOldCell, pNewCell );
		}
		return pOldCell;
	}

	return NULL;
}

//------------------------------------------------------------------------------------
void CGameObject::Activate( void )
{
	//	memset( &m_ObjectData, 0, sizeof(ObjectData) );

	m_fX = m_fY = m_fZ = 0.0f;
	m_fOffsetX = m_fOffsetY = m_fOffsetZ = 0.0f;
	m_fDestinationX = m_fDestinationY = m_fDestinationZ = 0.0f;
	m_fDest = m_fDist = m_fatan2 = 0.0f;
	m_ObjectData.m_fSpeed = 0.18f;//0.07f;
	m_ObjectData.m_usAttackSpeed = 2500;
	m_eState = OBJECT_IDLE;
	m_bIsFight = false;

	m_lDanger = 0;


	// 清除路径队列
	ClearPath();

	// 清除换装属性表
	for( int i=0; i<MAX_AVATAR; i++ )
		m_nAvatar[i] = 0;

	SetActive( true );
}


void CGameObject::ClearPath( void )
{
	// 清除路径队列
	if( m_queuePath.size() != 0 )	
	{
		for( PathIterator it=m_queuePath.begin(); it!=m_queuePath.end(); it++ )
		{
			tarPath* pPath = *it;
			if( pPath->nEndTick == 0 )
			{
				SetPos( pPath->x, 0.0f, pPath->z );
			}
			SAFE_DELETE( pPath );
		}
		m_queuePath.clear();
	}

	// 新路徑處理
	if( m_queueWalk.size() != 0 )
	{
		for( WalkIterator it=m_queueWalk.begin(); it!=m_queueWalk.end(); it++ )
		{
			tarWalk* pWalk = *it;
// 			SetPos( pWalk->x, 0.0f, pWalk->z );
// 			m_fatan2 = pWalk->fatan;
			SAFE_DELETE( pWalk );
		}
		m_queueWalk.clear();
	}
}

//------------------------------------------------------------------------------------
void CGameObject::Release( void )
{
	SetActive( false );
	m_StatusMap.clear();
	m_StatusSelfData.clear();
}

//------------------------------------------------------------------------------------
// ObjectData *CGameObject::GetObjData()
// {
// 
// 	return &m_ObjectData;	
// }
//------------------------------------------------------------------------------------
void		CGameObject::AddHP( const int hp )
{
	long maxHp = GetMaxHP();

	m_DamageInfo.HP -= hp;

	m_ObjectData.m_lHP += hp; 
	if ( m_ObjectData.m_lHP < 0 ) 
		m_ObjectData.m_lHP = 0;
	else if ( m_ObjectData.m_lHP > maxHp )
		m_ObjectData.m_lHP = maxHp;
}
//------------------------------------------------------------------------------------
void		CGameObject::AddMP( const int mp )	
{ 
	long maxMp = GetMaxMP();

	m_DamageInfo.MP -= mp;

	m_ObjectData.m_lMP += mp; 
	if ( m_ObjectData.m_lMP < 0 ) 
		m_ObjectData.m_lMP = 0;
	else if ( m_ObjectData.m_lMP > maxMp )
		m_ObjectData.m_lMP = maxMp;
}

//------------------------------------------------------------------------------------
// HP上限
// 公式：(等级换算HP + 装备附加 + 装备附加百分比 + 被动技能附加百分比 + 状态加成百分比 )
//------------------------------------------------------------------------------------
int			CGameObject::GetMaxHP( void ) const
{
	return (int)( m_BaseData.m_lMaxHP + m_EquipData.m_lMaxHP + m_Specialty.m_lMaxHP + m_StatusData.m_lMaxHP + m_AltarData.m_lMaxHP );
}

//------------------------------------------------------------------------------------
// MP上限
// 公式: (等级换算MP + 装备附加 1 + 装备附加百分比 + 被动技能附加百分比  + 状态加成百分比 )
//------------------------------------------------------------------------------------
int			CGameObject::GetMaxMP( void ) const
{
	return (int)(  m_BaseData.m_lMaxMP + m_EquipData.m_lMaxMP + m_Specialty.m_lMaxMP  + m_StatusData.m_lMaxMP + m_AltarData.m_lMaxMP );
}

//------------------------------------------------------------------------------------
DAMAGE_INFO	CGameObject::GetDamageInfo( void ) const
{
	return m_DamageInfo;
}
//------------------------------------------------------------------------------------

void		CGameObject::ClearDamageInfo( void )
{
	m_DamageInfo.HP = 0;
	m_DamageInfo.MP = 0;
}
//------------------------------------------------------------------------------------
void		CGameObject::SetDamageInfo( const DAMAGE_INFO dmgInfo )
{
	m_DamageInfo = dmgInfo;
}
//------------------------------------------------------------------------------------

void CGameObject::GetScriptObject(ScriptObject* obj)
{
	obj->ID = int(this);
	obj->Rank = m_ObjectData.m_cRank;
}

void CGameObject::SetScriptObject(ScriptObject* )
{

}

HandType CGameObject::GetHandType()
{
	return HTOne;
}

//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
void CGameObject::StartFight( CGameObject* pObject )
{
	m_timeFight.Startup(FIGHT_TIME);

	SetFight(true);
	SetTarget(pObject);


	AddEnmity(pObject , 0);
}

void CGameObject::EndFight()
{
	SetFight(false);
	ResetResumeTimer();
	ClearEnmity();
}

//------------------------------------------------------------------------------------
bool CGameObject::IsAttack( void )
{
	return m_timeAttack.IsExpire();
}

//------------------------------------------------------------------------------------
void CGameObject::AI( )
{
	//刷新状态
	ResolveStatus();
}

//------------------------------------------------------------------------------------
//计算物理攻击力
//公式:(人物等级换算值+被动技能附加值+装备附加值+状态加成)
//------------------------------------------------------------------------------------
UINT  CGameObject::CalculateAttack()
{
    return  (UINT)( m_BaseData.m_lAttack  +  m_Specialty.m_lAttack + m_EquipData.m_lAttack + m_StatusData.m_lAttack + m_AltarData.m_lMaxMP );

}

void  CGameObject::CalculateMoney( CGameObject* )
{
	return;
}

//------------------------------------------------------------------------------------
//计算物理防御力
//公式:(人物等级换算值+被动技能附加值+装备附加值+状态加成)
//------------------------------------------------------------------------------------
UINT  CGameObject::CalculateDefend() const
{
    return ( m_BaseData.m_lDefend  +  m_Specialty.m_lDefend + m_EquipData.m_lDefend  + m_StatusData.m_lDefend + m_AltarData.m_lDefend );
}

AttackRadius CGameObject::GetAttackRadius()
{
	return m_AttackRadius;
}

//------------------------------------------------------------------------------------
//仇恨列表实现
//------------------------------------------------------------------------------------
void CGameObject::AddEnmity(CGameObject* pObject , long lValue)
{
	if (pObject == this)
		return;

	if(!m_listEnmity.empty())
	{
		for(EnmityIterator it = m_listEnmity.begin() ; it != m_listEnmity.end() ; it++)
		{
			if( it->pObject == pObject )
			{
				it->lValue += lValue;
				return;
			}
		}
	}

	tarEnmity tar;
	tar.pObject = pObject;
	tar.lValue = lValue;
	m_listEnmity.push_back(tar);
}

//------------------------------------------------------------------------------------
void CGameObject::DecEnmity(CGameObject* pObject, long lValue)
{
	if (pObject == this)
		return;

	if(m_listEnmity.size() == 0)
		return;

	for(EnmityIterator it = m_listEnmity.begin() ; it != m_listEnmity.end() ; it++)
	{
		if( it->pObject == pObject )
		{
			it->lValue -= lValue;
			return;
		}
	}

}

bool CGameObject::IsClearEnmity()
{
	return m_listEnmity.empty();
}

//------------------------------------------------------------------------------------
void CGameObject::ClearEnmity()
{
	m_listEnmity.clear();

	m_targetObj = NULL;
}

//------------------------------------------------------------------------------------
void CGameObject::RemoveEnmity(CGameObject* pObject)
{
	if(m_listEnmity.empty())
		return;

	for(EnmityIterator it = m_listEnmity.begin() ; it != m_listEnmity.end() ; it++)
	{
		if( it->pObject == pObject )
		{
			m_listEnmity.erase( it );

			break;
		}
	}

	//換當前目標
	if (pObject == m_targetObj)
		m_targetObj = GetMaxEnmity();
}

//------------------------------------------------------------------------------------
void CGameObject::ChainEnmityList(CGameObject* obj , int value)
{
	if (!obj)
		return;

	for(EnmityIterator iter = m_listEnmity.begin() ; iter != m_listEnmity.end(); iter++)
		obj->m_listEnmity.push_back(*iter);

	obj->m_listEnmity.unique();

	m_listEnmity = obj->m_listEnmity;

	for(EnmityIterator iter = m_listEnmity.begin() ; iter != m_listEnmity.end(); iter++)
		iter->pObject->AddEnmity(this , value);
}

void CGameObject::CoagentEnmityList(CGameObject* obj , int value)
{
	if (!obj)
		return;

	for (EnmityIterator iter = m_listEnmity.begin() ; iter != m_listEnmity.end() ; iter++)
	{
		(*iter).pObject->AddEnmity(obj , value);
		obj->AddEnmity((*iter).pObject , 0);
	}
}

//------------------------------------------------------------------------------------
CGameObject* CGameObject::GetMaxEnmity()
{
	tarEnmity* obj = NULL;

	for(EnmityIterator iter = m_listEnmity.begin() ; iter != m_listEnmity.end(); iter++)
	{
		if (obj)
		{
			obj = iter->lValue > obj->lValue ? &(*iter) : obj;
		}
		else
			obj = &(*iter);
	}

	return obj ? obj->pObject : NULL;
}

//------------------------------------------------------------------------------------
//删除被动技能
//------------------------------------------------------------------------------------
bool  CGameObject::DelPassiveSkill( int ucSkillID )
{
	map< UINT , PASSIVEINFO >::iterator itor= m_PassiveSkill.find( ucSkillID ) ;
	if ( itor != m_PassiveSkill.end() )
	{
		m_PassiveSkill.erase( itor );
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------------
//找到被动技能
//------------------------------------------------------------------------------------
bool  CGameObject::FindPassiveSkill( int ucSkillID,UINT * )
{
	map< UINT , PASSIVEINFO >::const_iterator itor = m_PassiveSkill.find( ucSkillID );
	if ( itor != m_PassiveSkill.end() )
	{
		// 		if( Degree != NULL )
		// 			*Degree = (*itor).second.Degree_Practice;
		return  true;
	}
	return false;

}

//------------------------------------------------------------------------------------
//找到主动技能
//------------------------------------------------------------------------------------
bool   CGameObject::FindActiveSkill( int ucSkillID )
{
	map< UINT ,ACTIVE_INFO> ::iterator itor =  m_ActiveSkill.find( ucSkillID );
	if ( itor != m_ActiveSkill.end() )
	{
		return true;
	}
	return false;

}

//------------------------------------------------------------------------------------
//添加主动技能
//------------------------------------------------------------------------------------
bool   CGameObject::AddActiveSkill( int ucSkillID )
{
	const MagicData*  pActiveSkill, * AddSkill = s_World->g_pSkillManager->GetMagic( ucSkillID );
	if (  NULL == AddSkill )
	{
		ASSERT(0);
	}
	int  SkillID = -1;
	bool  Have = false;

	map< UINT , ACTIVEINFO >::iterator   itor = m_ActiveSkill.begin();
	for ( ; itor != m_ActiveSkill.end(); itor++ )
	{
		pActiveSkill = s_World->g_pSkillManager->GetMagic( itor->first );
		if ( ( pActiveSkill->Kind == AddSkill->Kind ) )
		{
			if ( pActiveSkill->Priority < AddSkill->Priority  )
			{
				Have =  true; 
				SkillID = itor->first;
				break;
			}
			else
			{
				return  false;
			}

		}
	}

	if ( Have )
		DelActiveSkill( SkillID );

	m_ActiveSkill[ucSkillID].Time_style    = TIME_STYLE_NONE;
	m_ActiveSkill[ucSkillID].Time_start    = time(NULL);
	m_ActiveSkill[ucSkillID].Time_interval = 0;//(UINT)AddSkill->usCoolingTime*1000;
	return true;
}

//------------------------------------------------------------------------------------
//删除主动技能
//------------------------------------------------------------------------------------
void   CGameObject::DelActiveSkill( int ucSkillID )
{
	map< UINT,ACTIVE_INFO > ::iterator itor =  m_ActiveSkill.find(  ucSkillID );
	if ( itor != m_ActiveSkill.end() )
	{
		m_ActiveSkill.erase( itor );
	}


}

//------------------------------------------------------------------------------------
//魔法是否处于冷却状态
//------------------------------------------------------------------------------------
bool CGameObject::IsActiveSkillCooling( int ucSkillID )
{
	return (  m_ActiveSkill[ucSkillID].Time_style == TIME_STYLE_COOL )	? true : false;

}

bool CGameObject::IsGodSkillTimeOut( int /*MagicID*/ )
{
	return false;
}

bool   CGameObject::SetActiveSkillStartTime( int ucSkillID)
{
	map<UINT ,ACTIVE_INFO> ::iterator  itor = m_ActiveSkill.find( ucSkillID );
	if ( itor !=  m_ActiveSkill.end() )
	{
		(*itor).second.Time_start = time( NULL);
		return true;
	}
	return false;

}

void    CGameObject::SetCurActiveSkillID( int ucActiveSkillID)
{
	m_ucCurActiveSkill =  ucActiveSkillID;
}

//------------------------------------------------------------------------------------
// 时间刷新
//------------------------------------------------------------------------------------
void CGameObject::ResetTimer( float IntervalTime ,TIME_STYLE timeStyle )
{
	if ( timeStyle == TIME_STYLE_CAST )
	{
		m_ActiveSkillCast_Timer.Startup( IntervalTime * 1000 );  
	}
	else if ( timeStyle == TIME_STYLE_COOL )
	{
		m_ActiveSkillCool_Timer.Startup( IntervalTime );
	}

}

//------------------------------------------------------------------------------------
//改变魔法状态
//------------------------------------------------------------------------------------
void    CGameObject::ChangeActiveSkillStatus( int ucSkillID, TIME_STYLE status )
{
	const MagicData*  pActiveSkill = s_World->g_pSkillManager->GetMagic( ucSkillID );
	if ( pActiveSkill != NULL)
	{
		m_ActiveSkill[ucSkillID].Time_style = status;
		m_ActiveSkill[ucSkillID].Time_interval =(UINT) pActiveSkill->usCoolingTime;
		if ( ucSkillID >= GODSKILL_ID )  //神恩技能冷却完毕时间
		{
			m_GodLastTime = time( NULL );
		}
	}

}

int	CGameObject::GetCurActiveSkillID()
{
	return (int)m_ucCurActiveSkill;
}


bool CGameObject::IsRefresh( TIME_STYLE timeStyle )
{

	if ( timeStyle == TIME_STYLE_CAST)
	{
		return  m_ActiveSkillCast_Timer.IsExpire()? m_ActiveSkillCast_Timer.TimeOver():false;

	}
	else if ( timeStyle == TIME_STYLE_COOL )
	{
		//遍历魔法列表,如果超时,更改状态
		map< UINT,ACTIVE_INFO > ::iterator itor =  m_ActiveSkill.begin();
		for ( ; itor != m_ActiveSkill.end(); itor++ )
		{
			ACTIVE_INFO  active_info = (*itor).second;

			if (m_ActiveSkillCool_Timer.IsExpire( active_info.Time_start,active_info.Time_interval ) )
			{
				ChangeActiveSkillStatus( (*itor).first, TIME_STYLE_NONE );

			}

		}
		return true;
	}
	else if (  timeStyle == TIME_STYLE_NONE )
	{
		map< UINT,ACTIVE_INFO > ::iterator itor =  m_ActiveSkill.begin();
		for ( ; itor != m_ActiveSkill.end(); itor++ )
		{
			//ACTIVE_INFO  active_info = (*itor).second;
			BYTE  MagicID = (*itor).first;

			if ( MagicID == 200 )  //神恩
			{
				//ACTIVE_INFO  active_info = (*itor).second;
				if (m_ActiveSkillCool_Timer.IsExpire( m_GodLastTime,30 ) )
				{
					ChangeActiveSkillStatus( (*itor).first, TIME_STYLE_COOL );

				}
			}

		}
	}

	return false;
}

//-----------------------------------------------------------------------------
//暴击命中的判断
//-----------------------------------------------------------------------------
ATTACK_TYPE CGameObject:: JudgeAttackStyle( CGameObject* pTarget )
{
	ASSERT( pTarget );
	bool  IsForceAttack = false;
	bool  IsHit = false;

	valarray<int> RandSer(2);
	RandSer[0] = AverageRandom(0,CPeizhi::Instance()->GetDefaultValue( HIT_ARG_A ));
	RandSer[1] = AverageRandom(0,CPeizhi::Instance()->GetDefaultValue( HIT_ARG_A ));	

	int Frenzy = GetAlllHit() / CPeizhi::Instance()->GetDefaultValue( HIT_ARG_C ) + GetAlllCritAppend()- pTarget->GetAlllCritDefend();
	if ( Frenzy < CPeizhi::Instance()->GetDefaultValue( HIT_ARG_MIN ) )
        Frenzy = CPeizhi::Instance()->GetDefaultValue( HIT_ARG_MIN );
	else if ( Frenzy > CPeizhi::Instance()->GetDefaultValue( HIT_ARG_MAX ) )
	    Frenzy = CPeizhi::Instance()->GetDefaultValue( HIT_ARG_MAX );

	if ( RandSer[0] < Frenzy )
	{
		IsForceAttack = true;
	}
	else
	{
        int Hit =  CPeizhi::Instance()->GetDefaultValue( HIT_ARG_B ) +  GetAlllHit() - pTarget->GetAlllDodge();
		if ( Hit < CPeizhi::Instance()->GetDefaultValue( HIT_ARG_MIN ) )
			Hit = CPeizhi::Instance()->GetDefaultValue( HIT_ARG_MIN );
		else if ( Hit > CPeizhi::Instance()->GetDefaultValue( HIT_ARG_MAX ) )
			Hit = CPeizhi::Instance()->GetDefaultValue( HIT_ARG_MAX );

		if ( RandSer[1] < Hit )
		{
			IsHit = true;
		}	
	}

	ATTACK_TYPE  attack_result;

	if (  IsForceAttack )
	{
		attack_result = ATTACK_FORCE;
	}
	else if ( IsHit )
	{
		attack_result = ATTACK_HIT;
	}
	else
	{
		attack_result = ATTACK_MISS;
	}

	return  attack_result;

}


//------------------------------------------------------------------------------------
// 状态命中判断
//------------------------------------------------------------------------------------
ATTACK_TYPE CGameObject::JudgeAppendStatusStyle(  CGameObject* pTarget, const MagicData* pMagicData  )
{
	if ( !pTarget )
		return ATTACK_MISS;
	
	bool  IsHit = false;
	int RandSer = AverageRandom(0,CPeizhi::Instance()->GetDefaultValue( HIT_ARG_STATUS_D ));
	int Hit = pMagicData->StatusOdds + GetAlllStatusHit() + GetAlllStatudDodge();

	if ( Hit < CPeizhi::Instance()->GetDefaultValue( HIT_ARG_STATUS_MIN ) )
		Hit = CPeizhi::Instance()->GetDefaultValue( HIT_ARG_STATUS_MIN );
	else if ( Hit > CPeizhi::Instance()->GetDefaultValue( HIT_ARG_STATUS_MAX ) )
		Hit = CPeizhi::Instance()->GetDefaultValue( HIT_ARG_STATUS_MAX );

	if ( RandSer < Hit )
	{
		IsHit = true;
	}  

	ATTACK_TYPE  attack_result;


	if ( IsHit )
	{
		attack_result = ATTACK_HIT;
	}
	else
	{
		attack_result = ATTACK_MISS;
	}

	return  attack_result;
}


//------------------------------------------------------------------------------------
//计算物理攻击的伤害
//------------------------------------------------------------------------------------
int CGameObject::CalculateAttackDamage( CGameObject& pObj , float iPower1, int iPower2 )
 {
	 int FirstDamage =  GetAlllAttack() - pObj.GetAlllDefend() ;
	 int Damage =( 
		 ( FirstDamage < 0 ? 0 : FirstDamage) * 
		 ( 1 + GetAllfPhyDamageAppend() ) * 
		 ( 1 + iPower1 ) + iPower2 - pObj.GetAlllDamageSorb()   
		 ) *
		 (
		 1 + GetCreateAttackDamage() - pObj.GetBeAttackDamage() 
		 );
	 return    Damage <= 0 ? 1 : Damage ;
}

//------------------------------------------------------------------------------------
//计算经验和等级
//------------------------------------------------------------------------------------
void	CGameObject::CalculateExpAndLevel( CGameObject&  )
{

}


//------------------------------------------------------------------------------------
//计算魔法攻击的伤害(A->B)
//计算公式:魔法威力*( 1 + A该系造成的伤害)*( 1 - B该系伤害抗性 )*( 1 + A造成的魔法伤害 )*( 1 + B受到魔法伤害 )
//------------------------------------------------------------------------------------
INT CGameObject::CalculateAttackDamage( int MagicID, CGameObject &pObj )
{
	float Power = 0.0f;	  
	float AKindDamage = 0.0f;
	float BKindDefend = 0.0f;
	float ACreateDamage = 0.0f;
	float BBeDemage = 0.0f;

	//查找该魔法
	const MagicData* pMagic = s_World->g_pSkillManager->GetMagic( MagicID );
	if ( NULL == pMagic  )
	{
		return 0;
	}

	//魔法基本威力
	if ( FindActiveSkill(MagicID) )
		Power =  pMagic->fPower1;

	//吸收
	//double Absorb = 0.0;

	const MagicData* mgc = s_World->g_pSkillManager->GetMagic( MagicID );

	int Damage = 0;

	if ( mgc->iProperty == PROPERTY_MAG )  //魔法
	{ 
		int FirstDamage = GetAlllMagicAttack() - pObj.GetAlllMagicDefend();
		Damage  = ( ( FirstDamage < 0 ? 0 : FirstDamage  ) * ( 1 + GetAllfMagicDamageAppend()) *( 1 + mgc->fPower1 ) + mgc->fPower2 - pObj.GetAlllDamageSorb()  ) * ( 1 + GetCreateMagicDamage() - pObj.GetBeAttackDamage() );
        return Damage <= 0 ? 1 : Damage ;
	}
	else  if (  mgc->iProperty  == PROPERTY_PHY ) //招式
	{ 
		 return   CalculateAttackDamage( pObj, pMagic->fPower1, pMagic->fPower2 );
	}
	else  
	{

		return  0;

	}

	return 0;
}

void CGameObject::Dead( CGameObject  * )
{

}

bool CGameObject::IsDead()
{
	return GetlHP() <= 0;
}

//将其他对象信息同步给自己
void CGameObject::AynObjToObj( CCell *, CCell * )
{

}

//将自己的信息同步给其他玩家
void CGameObject::AynMeToOther( CCell *pOldCell, CCell *pNewCell )
{
	//新区域 
	MSG_OBJECTINFO ObjInfo;
	ObjInfo.Head.usType = _MSG_OBJECTINFO;
	ObjInfo.Head.usSize = sizeof(MSG_OBJECTINFO); 
	ObjInfo.uiID = GetID();
	ObjInfo.Stall = false;
	ObjInfo.uiType = GetType() + 15;
	ObjInfo.usMapID = (USHORT)GetRegion()->GetID();
	ObjInfo.X = GetPosX();
	ObjInfo.Y = GetPosY();
	ObjInfo.Z = GetPosZ();
	ObjInfo.fAtan2 =GetAtan2();
	ObjInfo.ucCamp = GetlFaction();
	ObjInfo.ucLV = GetRank();
	sprintf( ObjInfo.cName, "%s", GetName() );
	ObjInfo.uiMaxHP = GetMaxHP();
	ObjInfo.uiHP = GetlHP();
	
	std::vector<CCell*>  NewCell =  GetRegion()->GetMap()->AreaCollisionTest( pOldCell, pNewCell );
	GetRegion()->SendAreaMsgOneToOther( NewCell, &ObjInfo );
}

//吟唱终止后的复位操作
void CGameObject::ResetCastState()
{
	if ( GetState() != OBJECT_CAST && GetState() != OBJECT_PERFORM )
	 return;

	SetState( OBJECT_IDLE );
	ChangeActiveSkillStatus( GetCurActiveSkillID(), TIME_STYLE_NONE );

	MSG_SKILL_INTERRUPT skillInterrupt;
	skillInterrupt.Head.usSize = sizeof( MSG_SKILL_INTERRUPT );
	skillInterrupt.Head.usType = _MSG_SKILL_INTERRUPT;
	skillInterrupt.uiID = GetID();
	GetRegion()->SendAreaMsgOneToOther( GetCurrentCell(), &skillInterrupt );
}

//--------------------------------------------------------
// 终止别人吟唱
//--------------------------------------------------------
void CGameObject::Handler_HaltIntonate( float value, bool isresume, int StatusID )
{
	if ( !isresume )
	{
		m_StatusData.m_HaltIntonate -= m_StatusSelfData[StatusID].m_HaltIntonate; 
		m_StatusData.m_HaltIntonate += value;
		m_StatusSelfData[StatusID].m_HaltIntonate = value;
	}
	else
	{
		m_StatusData.m_HaltIntonate = 0.0f;
	}
}

//--------------------------------------------------------
// 返还单次伤害
//--------------------------------------------------------
void CGameObject::Handler_ReboundDamage( float value, bool isresume, int StatusID )
{
	if ( !isresume )
	{
		m_StatusData.m_ReboundDamage -= m_StatusSelfData[StatusID].m_ReboundDamage; 
		m_StatusData.m_ReboundDamage += value;
		m_StatusSelfData[StatusID].m_ReboundDamage = value;
	}
	else
	{
		m_StatusData.m_ReboundDamage -=  0.0f;
	}
}

//--------------------------------------------------------
// 不能移动
//--------------------------------------------------------
void CGameObject::Handler_NonMoving( float value , bool isresume, int StatusID )          
{
	m_StatusData.m_HaltIntonate = 0.0f;
	return;

	if ( !isresume )
	{
		m_StatusData.m_NonMoving -= m_StatusSelfData[StatusID].m_NonMoving; 
		m_StatusData.m_NonMoving += value;
		m_StatusSelfData[StatusID].m_NonMoving = value;
	}
	else
	{
		m_StatusData.m_NonMoving -= m_StatusSelfData[StatusID].m_NonMoving;
	}

	if ( m_StatusData.m_NonMoving > 0 )
	{
		m_eState = OBJECT_IDLE;
	}
	
}

//--------------------------------------------------------
// 不能使用魔法
//--------------------------------------------------------
void CGameObject::Handler_NonMagicUsing( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_NonMagicUsing -= m_StatusSelfData[StatusID].m_NonMagicUsing; 
		m_StatusData.m_NonMagicUsing += value;
		m_StatusSelfData[StatusID].m_NonMagicUsing = value;
	}
	else
	{
		m_StatusData.m_NonMagicUsing -= m_StatusSelfData[StatusID].m_NonMagicUsing;
	}
}

//--------------------------------------------------------
// 不能使用招式
//--------------------------------------------------------
void CGameObject::Handler_NonZSUsing( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_NonZSUsing -= m_StatusSelfData[StatusID].m_NonZSUsing; 
		m_StatusData.m_NonZSUsing += value;
		m_StatusSelfData[StatusID].m_NonZSUsing = value;
	}
	else
	{
		m_StatusData.m_NonZSUsing -= m_StatusSelfData[StatusID].m_NonZSUsing;
	}
}

//---------------------------------------------------------
// 不能使用物理攻击
//---------------------------------------------------------
void CGameObject::Handler_NonAttackUsing( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_NonAttackUsing -= m_StatusSelfData[StatusID].m_NonAttackUsing; 
		m_StatusData.m_NonAttackUsing += value;
		m_StatusSelfData[StatusID].m_NonAttackUsing = value;
	}
	else
	{
		m_StatusData.m_NonAttackUsing -= m_StatusSelfData[StatusID].m_NonAttackUsing;
	}
}

//---------------------------------------------------------
// 不能使用道具
//---------------------------------------------------------
void  CGameObject::Handler_NonPropUsing( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_NonPropUsing -= m_StatusSelfData[StatusID].m_NonPropUsing; 
		m_StatusData.m_NonPropUsing += value;
		m_StatusSelfData[StatusID].m_NonPropUsing = value;
	}
	else
	{
		m_StatusData.m_NonPropUsing -= m_StatusSelfData[StatusID].m_NonPropUsing;
	}
}

//---------------------------------------------------------
// 返还伤害
//---------------------------------------------------------
void CGameObject::Handler_ReturnDamage( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_ReturnDamage -= m_StatusSelfData[StatusID].m_ReturnDamage; 
		m_StatusData.m_ReturnDamage += value;
		m_StatusSelfData[StatusID].m_ReturnDamage = value;
	}
	else
	{
		m_StatusData.m_ReturnDamage -= m_StatusSelfData[StatusID].m_ReturnDamage;
	}
}

//---------------------------------------------------------
// 改变吟唱时间
//---------------------------------------------------------
void CGameObject::Handler_ChangeIntonateTime( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_ChangeIntonateTime -= m_StatusSelfData[StatusID].m_ChangeIntonateTime; 
		m_StatusData.m_ChangeIntonateTime += value;
		m_StatusSelfData[StatusID].m_ChangeIntonateTime = value;
	}
	else
	{
		m_StatusData.m_ChangeIntonateTime -= m_StatusSelfData[StatusID].m_ChangeIntonateTime;
	}
}

//---------------------------------------------------------
// 受到医疗
//---------------------------------------------------------
void CGameObject::Handler_BeCure( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_fBeCure -= m_StatusSelfData[StatusID].m_fBeCure; 
		m_StatusData.m_fBeCure += value;
		m_StatusSelfData[StatusID].m_fBeCure = value;
	}
	else
	{
		m_StatusData.m_fBeCure -= m_StatusSelfData[StatusID].m_fBeCure;
	}
	SynGameData( true );
}

//--------------------------------------------------------
// 使用治疗
//--------------------------------------------------------
void CGameObject::Handler_UseCure( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_fCure -= m_StatusSelfData[StatusID].m_fCure; 
		m_StatusData.m_fCure += value;
		m_StatusSelfData[StatusID].m_fCure = value;
	}
	else
	{
		m_StatusData.m_fCure -= m_StatusSelfData[StatusID].m_fCure;
	}
	SynGameData( true );
}

//---------------------------------------------------------
// 受到魔法伤害
//---------------------------------------------------------
void CGameObject::Handler_BeMagicDamage( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_BeMagicDamage -= m_StatusSelfData[StatusID].m_BeMagicDamage; 
		m_StatusData.m_BeMagicDamage += value;
		m_StatusSelfData[StatusID].m_BeMagicDamage = value;
	}
	else
	{
		m_StatusData.m_BeMagicDamage -= m_StatusSelfData[StatusID].m_BeMagicDamage;
	}
}

//----------------------------------------------------------
//造成魔法伤害
//----------------------------------------------------------
void CGameObject::Handler_CreateMagicDamage( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_CreateMagicDamage -= m_StatusSelfData[StatusID].m_CreateMagicDamage; 
		m_StatusData.m_CreateMagicDamage += value;
		m_StatusSelfData[StatusID].m_CreateMagicDamage = value;
	}
	else
	{
		m_StatusData.m_CreateMagicDamage -= m_StatusSelfData[StatusID].m_CreateMagicDamage;
	}
}

//----------------------------------------------------------
// 被物理攻击
//----------------------------------------------------------
void CGameObject::Handler_BeAttackDamage( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_BeAttackDamage -= m_StatusSelfData[StatusID].m_BeAttackDamage; 
		m_StatusData.m_BeAttackDamage += value;
		m_StatusSelfData[StatusID].m_BeAttackDamage = value;
	}
	else
	{
		m_StatusData.m_BeAttackDamage -= m_StatusSelfData[StatusID].m_BeAttackDamage;
	}
}

//----------------------------------------------------------
// 物理攻击
//----------------------------------------------------------
void CGameObject::Handler_CreateAttackDamage( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_CreateAttackDamage -= m_StatusSelfData[StatusID].m_CreateAttackDamage;
		m_StatusData.m_CreateAttackDamage += value;
		m_StatusSelfData[StatusID].m_CreateAttackDamage = value;
	}
	else
	{
		m_StatusData.m_CreateAttackDamage -= m_StatusSelfData[StatusID].m_CreateAttackDamage;
	}
}

//---------------------------------------------------------
// 得到经验值
//---------------------------------------------------------
void CGameObject::Handler_GetEXP( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_GetEXP -= m_StatusSelfData[StatusID].m_GetEXP;
		m_StatusData.m_GetEXP += value;
		m_StatusSelfData[StatusID].m_GetEXP = value;
	}
	else
	{
		m_StatusData.m_GetEXP -= m_StatusSelfData[StatusID].m_GetEXP;
	}
}

//---------------------------------------------------------
// MP上限
//---------------------------------------------------------
void CGameObject::Handler_MPMax( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_lMaxMP -= m_StatusSelfData[StatusID].m_lMaxMP;
		m_StatusData.m_lMaxMP += value;
		m_StatusSelfData[StatusID].m_lMaxMP = value;
	}
	else
	{
		m_StatusData.m_lMaxMP -= m_StatusSelfData[StatusID].m_lMaxMP;
	}
	if ( m_ObjectData.m_lMP > GetMaxMP() )
	{
		m_ObjectData.m_lMP = GetMaxMP();
	}
	SynGameData( true );
}

//---------------------------------------------------------
// HP上限
//---------------------------------------------------------
void CGameObject::Handler_HPMax( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_lMaxHP -= m_StatusSelfData[StatusID].m_lMaxHP;
		m_StatusData.m_lMaxHP += value;
		m_StatusSelfData[StatusID].m_lMaxHP = value;
	}
	else
	{
		m_StatusData.m_lMaxHP -= m_StatusSelfData[StatusID].m_lMaxHP;
	}

	if ( m_ObjectData.m_lHP > GetMaxHP() )
	{
		m_ObjectData.m_lHP = GetMaxHP();
	}

	SynGameData( true );
}


//---------------------------------------------------------
// 命中
//---------------------------------------------------------
void CGameObject::Handler_Hit( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_lHit -= m_StatusSelfData[StatusID].m_lHit;
		m_StatusData.m_lHit += value;
		m_StatusSelfData[StatusID].m_lHit = value;
	}
	else
	{
		m_StatusData.m_lHit -= m_StatusSelfData[StatusID].m_lHit;
	}
	SynGameData( true );
}

//----------------------------------------------------------
// 闪避
//----------------------------------------------------------
void CGameObject::Handler_Duck( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_lDodge -= m_StatusSelfData[StatusID].m_lDodge;
		m_StatusData.m_lDodge += value;
		m_StatusSelfData[StatusID].m_lDodge = value;
	}
	else
	{
		m_StatusData.m_lDodge -= m_StatusSelfData[StatusID].m_lDodge;
	}
	SynGameData( true );
}

//----------------------------------------------------------
// 防御
//----------------------------------------------------------
void CGameObject::Handler_Defend( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_lDefend -= m_StatusSelfData[StatusID].m_lDefend;
		m_StatusData.m_lDefend += value;
		m_StatusSelfData[StatusID].m_lDefend = value;
	}
	else
	{
		m_StatusData.m_lDefend -= m_StatusSelfData[StatusID].m_lDefend;
	}
	SynGameData( true );
}

//----------------------------------------------------------
// 攻击
//----------------------------------------------------------
void CGameObject::Handler_Attack( float value, bool isresume, int StatusID ) 
{
	if ( !isresume )
	{
		m_StatusData.m_lAttack -= m_StatusSelfData[StatusID].m_lAttack;
		m_StatusData.m_lAttack += value;
		m_StatusSelfData[StatusID].m_lAttack = value;
	}
	else
	{
		m_StatusData.m_lAttack -= m_StatusSelfData[StatusID].m_lAttack;
	}
	SynGameData( true );
}

void CGameObject::Handler_CirtAdd( float value, bool isresume, int StatusID )
{
	if ( !isresume )
	{
		m_StatusData.m_lCritAppend -= m_StatusSelfData[StatusID].m_lCritAppend;
		m_StatusData.m_lCritAppend += value;
		m_StatusSelfData[StatusID].m_lCritAppend = value;
	}
	else
	{
		m_StatusData.m_lCritAppend -= m_StatusSelfData[StatusID].m_lCritAppend;
	}
	SynGameData( true );
}
void  CGameObject::Handler_CirtDefend( float value, bool isresume, int StatusID )
{
	if ( !isresume )
	{
		m_StatusData.m_lCritDefend -= m_StatusSelfData[StatusID].m_lCritDefend;
		m_StatusData.m_lCritDefend += value;
		m_StatusSelfData[StatusID].m_lCritDefend = value;
	}
	else
	{
		m_StatusData.m_lCritDefend -= m_StatusSelfData[StatusID].m_lCritDefend;
	}
	SynGameData( true );
}

void  CGameObject::Handler_MagicAttack( float value, bool isresume, int StatusID )
{
	if ( !isresume )
	{
		m_StatusData.m_lMagicAttack -= m_StatusSelfData[StatusID].m_lMagicAttack;
		m_StatusData.m_lMagicAttack += value;
		m_StatusSelfData[StatusID].m_lMagicAttack = value;
	}
	else
	{
		m_StatusData.m_lMagicAttack -= m_StatusSelfData[StatusID].m_lMagicAttack;
	}
	SynGameData( true );
}

void  CGameObject::Handler_MagicDefend( float value, bool isresume, int StatusID )
{
	if ( !isresume )
	{
		m_StatusData.m_lMagicDefend -= m_StatusSelfData[StatusID].m_lMagicDefend;
		m_StatusData.m_lMagicDefend += value;
		m_StatusSelfData[StatusID].m_lMagicDefend = value;
	}
	else
	{
		m_StatusData.m_lMagicDefend -= m_StatusSelfData[StatusID].m_lMagicDefend;
	}
	SynGameData( true );
}

//----------------------------------------------------------
// 增加HP
//----------------------------------------------------------
void CGameObject::Handler_AddHP( float value, bool isresume, int StatusID )
{
	if ( !isresume )
	{
		//没有死才能恢复
		if ( !IsDead() )
		{
			//m_StatusData.m_AddHP -= m_StatusSelfData[StatusID].m_AddHP;
			//m_StatusData.m_AddHP += value;
			m_ObjectData.m_lHP += value -  m_StatusSelfData[StatusID].m_AddHP;
			m_StatusSelfData[StatusID].m_AddHP = value;
		}
	}
	else
	{
		   m_StatusSelfData[StatusID].m_AddHP = 0.0f;
		  //m_StatusData.m_AddHP -= m_StatusSelfData[StatusID].m_AddHP;
	      //m_ObjectData.m_lHP += m_StatusSelfData[StatusID].m_AddHP;
	}

	//if ( GetlHP() >= GetMaxHP() )
	//{
	//	return;
	//}

	SynGameData( true );

}

//----------------------------------------------------------
// 减少HP
//----------------------------------------------------------
void CGameObject::Handler_DecHP( float value, bool isresume, int StatusID )
{
	if ( !isresume )
	{
		m_StatusData.m_DecHP -= m_StatusSelfData[StatusID].m_DecHP; 
		m_StatusData.m_DecHP += value;
		m_StatusSelfData[StatusID].m_DecHP = value;
	}
	else
	{
		m_StatusData.m_DecHP -= m_StatusSelfData[StatusID].m_DecHP;
		m_ObjectData.m_lHP -= m_StatusSelfData[StatusID].m_DecHP; 
	}

	if ( GetlHP() >= GetMaxHP() )
	{
		return;
	}

	if ( IsDead() )
	{
		Dead(NULL);
	}

	SynGameData( true );

}

//----------------------------------------------------------
// 增加MP
//----------------------------------------------------------
void CGameObject::Handler_AddMP( float value, bool isresume, int StatusID )
{
	if ( !isresume )
	{
		m_StatusData.m_AddMP -= m_StatusSelfData[StatusID].m_AddMP; 
		m_StatusData.m_AddMP += value;
		m_StatusSelfData[StatusID].m_AddMP = value;
	}
	else
	{
		m_StatusData.m_AddMP -= m_StatusSelfData[StatusID].m_AddMP;
		m_ObjectData.m_lMP += m_StatusSelfData[StatusID].m_AddMP; 
	}

	if ( GetlMP() >= GetMaxMP() )
	{
		return;
	}

	SynGameData( true );
}

//----------------------------------------------------------
// 减少MP
//----------------------------------------------------------
void CGameObject::Handler_DecMP( float value, bool isresume, int StatusID )
{
	if ( !isresume )
	{
        m_StatusData.m_DecMP -= m_StatusSelfData[StatusID].m_DecMP; 
		m_StatusData.m_DecMP += value;
		m_StatusSelfData[StatusID].m_DecMP = value;
	}
	else
	{
		m_StatusData.m_DecMP -= m_StatusSelfData[StatusID].m_DecMP;
		m_ObjectData.m_lMP -= m_StatusSelfData[StatusID].m_DecMP; 
	}

	if ( GetlMP() >= GetMaxMP() )
	{
		return;
	}

	SynGameData( true );
}
//---------------------------------------------------------
// 状态类魔法成功几率
//---------------------------------------------------------
void  CGameObject::Handler_ChangeMagicSuccedOdds( float value, bool isresume, int StatusID )
{
	if ( !isresume )
	{
        m_StatusData.m_lStatusHit -= m_StatusSelfData[StatusID].m_lStatusHit; 
		m_StatusData.m_lStatusHit += value;
		m_StatusSelfData[StatusID].m_lStatusHit = value;
	}
	else
	{
		m_StatusData.m_lStatusHit -= m_StatusSelfData[StatusID].m_lStatusHit;
	}
	SynGameData( true );
}

//----------------------------------------------------------
// 状态类魔法抵抗率
//----------------------------------------------------------
void  CGameObject::Handler_ChangeMagicDefendOdds( float value, bool isresume, int StatusID )
{
	if ( !isresume )
	{
        m_StatusData.m_lStatudDodge -= m_StatusSelfData[StatusID].m_lStatudDodge; 
		m_StatusData.m_lStatudDodge += value;
		m_StatusSelfData[StatusID].m_lStatudDodge = value;
	}
	else
	{
		m_StatusData.m_lStatudDodge -= m_StatusSelfData[StatusID].m_lStatudDodge;
	}

	SynGameData( true );
}

//---------------------------------------------------------
// 函数指针路由
//---------------------------------------------------------
void CGameObject::RouterHandler( pFAddr *Addr, int ID )
{
	switch( ID )
	{
	case 0:
		*Addr = &Handler_HaltIntonate;
		break;
	case 1:
		*Addr = &Handler_ReboundDamage;
		break;
	case 2:
		*Addr = &Handler_NonMoving;
		break;
	case 3:
		*Addr = &Handler_NonMagicUsing;
		break;
	case 4:
		*Addr = &Handler_NonZSUsing;
		break;
	case 5:
		*Addr = &Handler_NonAttackUsing;
		break;
	case 6:
		*Addr = &Handler_NonPropUsing;
		break;
	case 7:
		*Addr = &Handler_ReturnDamage;
		break;
	case 8:
		*Addr = &Handler_ChangeIntonateTime;
		break;
	case 9:
		*Addr = &Handler_BeCure;
		break;
	case 10:
		*Addr = &Handler_UseCure;
		break;
	case 11:
		*Addr = &Handler_BeMagicDamage;
		break;
	case 12:
		*Addr = &Handler_CreateMagicDamage;
		break;
	case 13:
		*Addr = &Handler_BeAttackDamage;
		break;
	case 14:
		*Addr = &Handler_CreateAttackDamage;
		break;
	case 15:
		*Addr = &Handler_GetEXP;
		break;
	case 16:
		*Addr = &Handler_MPMax;
		break;
	case 17:
		*Addr = &Handler_HPMax;
		break;
	case 18:
		*Addr = &Handler_Hit;
		break;
	case 19:
		*Addr = &Handler_Duck;
		break;
	case 20:
		*Addr = &Handler_Defend;
		break;
	case 21:
		*Addr = &Handler_Attack;
		break;
	case 22:
		*Addr = &Handler_CirtAdd;
		break;
	case 23:
		*Addr = &Handler_CirtDefend;
		break;
	case 24:
		*Addr = &Handler_MagicAttack;
		break;
	case 25:
		*Addr = &Handler_MagicDefend;
		break;
	case 26:
		*Addr = &Handler_AddHP;
		break;
	case 27:
		*Addr = &Handler_DecHP;
		break;
	case 28:
		*Addr = &Handler_AddMP;
		break;
	case 29:
		*Addr = &Handler_DecMP;
		break;
	case 30:
		*Addr = &Handler_ChangeMagicSuccedOdds;
		break;
	case 31:
		*Addr = &Handler_ChangeMagicDefendOdds;
		break;
	default:
		//ASSERT(0);
		break;
	}

}


//-----------------------------------------------------------------
// 删除状态
//-----------------------------------------------------------------
bool CGameObject::DelStatus( CStatus *pStatus )
{
	map< UINT, TTimeElement<CStatus*,time_t> >::iterator  itor = m_StatusMap.find( pStatus->GetID() );
	if ( itor == m_StatusMap.end() )
	{
		return false;
	}

	Handler_HaltIntonate( 0, true, pStatus->GetID()  );          
	Handler_ReboundDamage( 0, true, pStatus->GetID()  );         
	Handler_NonMoving( 0, true, pStatus->GetID()  );             
	Handler_NonMagicUsing( 0, true, pStatus->GetID()  );         
	Handler_NonZSUsing( 0, true, pStatus->GetID()  );            
	Handler_NonAttackUsing( 0, true, pStatus->GetID()  );        
	Handler_NonPropUsing( 0, true, pStatus->GetID()  );          
	Handler_ReturnDamage( 0, true, pStatus->GetID()  );          
	Handler_ChangeIntonateTime( 0, true, pStatus->GetID()  );    
	Handler_BeCure( 0, true, pStatus->GetID()  );                
	Handler_UseCure( 0, true, pStatus->GetID()  );               
	Handler_BeMagicDamage( 0, true, pStatus->GetID()  );         
	Handler_CreateMagicDamage( 0, true, pStatus->GetID()  );     
	Handler_BeAttackDamage( 0, true, pStatus->GetID()  );        
	Handler_CreateAttackDamage( 0, true, pStatus->GetID()  );    
	Handler_GetEXP( 0, true, pStatus->GetID()  );                
	Handler_MPMax( 0, true, pStatus->GetID()  );                 
	Handler_HPMax( 0, true, pStatus->GetID()  );                         
	Handler_Hit( 0, true, pStatus->GetID()  );                   
	Handler_Duck( 0, true, pStatus->GetID()  );                  
	Handler_Defend( 0, true, pStatus->GetID()  );                
	Handler_Attack( 0, true, pStatus->GetID()  );  
	Handler_CirtAdd( 0, true, pStatus->GetID()  );
	Handler_CirtDefend( 0, true, pStatus->GetID()  );
	Handler_MagicAttack( 0, true, pStatus->GetID()  );
	Handler_MagicDefend( 0, true, pStatus->GetID()  );
	Handler_AddHP( 0, true, pStatus->GetID()  );                 
	Handler_DecHP( 0, true, pStatus->GetID()  );                 
	Handler_AddMP( 0, true, pStatus->GetID()  );                 
	Handler_DecMP( 0, true, pStatus->GetID()  );                 
	Handler_ChangeMagicSuccedOdds( 0, true, pStatus->GetID()  ); 
	Handler_ChangeMagicDefendOdds( 0, true, pStatus->GetID()  );
	m_StatusMap.erase( itor );
	m_StatusSelfData.erase( m_StatusSelfData.find( pStatus->GetID() ));

	//同步状态
	MSG_STATUS msg_status;
	msg_status.Head.usSize = sizeof( MSG_STATUS );
	msg_status.Head.usType = _MSG_STATUS;
	msg_status.uiID = GetID();
	msg_status.uiCount = (UINT)m_StatusMap.size();
	memcpy( &msg_status.Data, &m_StatusData, sizeof(StatusData) );
	int a =0 ;
	map< UINT, TTimeElement<CStatus*,time_t> >::iterator iter = m_StatusMap.begin();
	for (  ; iter != m_StatusMap.end(); iter++  )
	{
		TTimeElement<CStatus*,time_t> &TimEle= iter->second;
        CStatus *pStatus = iter->second.Second();
		msg_status.uiStatusID[a] = iter->first;
		msg_status.uiSurplusTime[a] = pStatus->GetPersistTime() - ( time(NULL) - TimEle.m_StartTime );
		a++;
	}

	GetRegion()->SendAreaMsgOneToOther( GetCurrentCell(),&msg_status );
	return true;
}

//-----------------------------------------------------------------
// 添加状态
//-----------------------------------------------------------------
bool CGameObject::AddStatus( CStatus *pStatus )
{
	if ( NULL == pStatus )
		return false;

	TTimeElement<CStatus*,time_t>  tempElems(pStatus,time(NULL));
	if ( CanAddStatus( pStatus ) )
	{
		m_StatusMap.insert( (make_pair( pStatus->GetID(),tempElems ) ) );
		StatusData temp;
		m_StatusSelfData[pStatus->GetID()] = temp;
		ResolveStatus();
		//向周围同步自己的状态
		MSG_STATUS msg_status;
		msg_status.Head.usSize = sizeof( MSG_STATUS );
		msg_status.Head.usType = _MSG_STATUS;
		msg_status.uiID = GetID();
		msg_status.uiCount = (UINT)m_StatusMap.size();
		memcpy( &msg_status.Data, &m_StatusData, sizeof(StatusData) );
		int a =0 ;
		map< UINT, TTimeElement<CStatus*,time_t> >::iterator iter = m_StatusMap.begin();
		for (  ; iter != m_StatusMap.end(); iter++  )
		{
			TTimeElement<CStatus*,time_t> &TimEle= iter->second;
			CStatus *pStatus = iter->second.Second();
			msg_status.uiStatusID[a] = iter->first;
			msg_status.uiSurplusTime[a] = pStatus->GetPersistTime() - ( time(NULL) - TimEle.m_StartTime );
			a++;
		}
		GetRegion()->SendAreaMsgOneToOther( GetCurrentCell(),&msg_status );
		return true;
	}
	else
	{
		return false;
	}
}

size_t CGameObject::GetStatus( UINT StatusArr[] )
{ 
	int Turn = 0;
	map< UINT, TTimeElement<CStatus*,time_t> >::iterator itor = m_StatusMap.begin();
	for( ; itor != m_StatusMap.end(); itor++ )
	{
		StatusArr[ Turn ] = itor->first;
		Turn++;
	}
	return m_StatusMap.size(); 
};

//-----------------------------------------------------------------
// 查找同类且优先级较高的状态
//-----------------------------------------------------------------
bool CGameObject::CanAddStatus( CStatus *pStatus )
{
	static int NumDef[2] = { MAX_DEC_STATUS, MAX_INC_STATUS };
	int Num[2] = {0};
	int MinStatusID = -1;
	int SameKindMinPri = -1;
	bool HaveSameKind = false;

	map< UINT, TTimeElement<CStatus*,time_t> >::iterator TempSameKindItor,TempDiffKindItor, itor = m_StatusMap.begin(); 
	for( ; itor != m_StatusMap.end(); itor++ )
	{
		TTimeElement<CStatus*,time_t> Elems = itor->second;

		//种类优先级最低的状态ID
		MinStatusID =  Elems.Second()->GetKind() <= pStatus->GetKind() ? Elems.Second()->GetID() : -1;
		if ( MinStatusID != -1 )
		{
			TempDiffKindItor = itor;
		}

		//判断增减益
		if ( Elems.Second()->GetStyle() == 1 )
		{
			Num[1]++;
		}
		else
		{
			Num[0]++;
		}

		//找到同类别且级别较低的状态
		if ( Elems.Second()->GetKind() == pStatus->GetKind() )
		{
			if ( Elems.Second()->GetPri() <= pStatus->GetPri() )
			{
				SameKindMinPri = Elems.Second()->GetID();
				TempSameKindItor = itor;
			}
			HaveSameKind = true;
		}

	}

	//替换算法
	if (  Num[ pStatus->GetStyle() ] < NumDef[ pStatus->GetStyle() ] )
	{
		if (  HaveSameKind  )
		{
			if ( SameKindMinPri != -1  )
			{
				m_StatusMap[ SameKindMinPri ].Second()->Resume( this );
				m_StatusMap.erase( TempSameKindItor );
				m_StatusSelfData.erase( m_StatusSelfData.find( SameKindMinPri ) );
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}
	else
	{
		if ( MinStatusID != -1 )
		{
			m_StatusMap[ MinStatusID ].Second()->Resume( this );
			m_StatusMap.erase( TempDiffKindItor );
			m_StatusSelfData.erase( m_StatusSelfData.find(pStatus->GetID() ));
			return true;
		}
		else
		{
			return false;
		}
	}
}

//-----------------------------------------------------------------
// 处理状态
//-----------------------------------------------------------------
void CGameObject::ResolveStatus( )
{
	bool Updata = false;
	
	map< UINT, TTimeElement<CStatus*,time_t> >::iterator itor = m_StatusMap.begin();
	for( ; itor != m_StatusMap.end();  )
	{  
		TTimeElement<CStatus*,time_t> &tempElems = itor->second;
		if ( !tempElems.DealWith( this ) )
		{
			Updata = true;
			m_StatusSelfData.erase( m_StatusSelfData.find( itor->first ));
			m_StatusMap.erase( itor++ );
		}
		else
		{
			itor->second.m_Done = true;  
			itor++;
		}
	}

	if ( Updata )
	{
		//同步状态
		MSG_STATUS msg_status;
		msg_status.Head.usSize = sizeof( MSG_STATUS );
		msg_status.Head.usType = _MSG_STATUS;
		msg_status.uiCount = (UINT)m_StatusMap.size();
		msg_status.uiID = GetID();
		memcpy( &msg_status.Data, &m_StatusData, sizeof(StatusData) );
		int a =0 ;
		map< UINT, TTimeElement<CStatus*,time_t> >::iterator itor = m_StatusMap.begin();
		for (  ; itor != m_StatusMap.end(); itor++  )
		{
			TTimeElement<CStatus*,time_t> &TimEle= itor->second;
			CStatus *pStatus = itor->second.Second();
			msg_status.uiStatusID[a] = itor->first;
			msg_status.uiSurplusTime[a] = pStatus->GetPersistTime() - ( time(NULL) - TimEle.m_StartTime );
			a++;
		}	
		GetRegion()->SendAreaMsgOneToOther( GetCurrentCell(),&msg_status );

		SynGameData( true );
	}
}

//-----------------------------------------------------------------
// 计算经验
//-----------------------------------------------------------------
bool CGameObject::CalculateExp( bool TarIsDead , int exp )
{
	if ( !TarIsDead )
	{
		return false;   
	}

	if ( GetRank() >= PLAYER_MAXLV )
	{
		return false;
	}

	int temp = m_WinExp;
	m_WinExp += exp;

	while ( m_WinExp >= EXP_INTERVAL )
	{
		m_ConvertPoint++;
		m_WinExp = exp - ( EXP_INTERVAL - temp );
		return true;
	}
	return false;
}

//-----------------------------------------------------------------
// 计算被动技能附加
//-----------------------------------------------------------------
void CGameObject::Handler_PS_HPMax(  SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD )
	{
		m_Specialty.m_lMaxHP += int(Skill->Effect[0]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type == TYPE_UPDATE )
	{ 
	    m_Specialty.m_lMaxHP += int(Skill->Effect[0] * m_PassiveSkill[ Skill->ucID ].Grade)- int(Skill->Effect[0] * (m_PassiveSkill[ Skill->ucID ].Grade-1));
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_lMaxHP -= int(Skill->Effect[0]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_lMaxHP += int(Skill->Effect[0]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_lMaxHP -= int(Skill->Effect[0]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}

void CGameObject::Handler_PS_MPMax(  SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD )
	{
		m_Specialty.m_lMaxMP += int( Skill->Effect[1] ) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if (   Type == TYPE_UPDATE  )
	{
		m_Specialty.m_lMaxMP += int(Skill->Effect[1] * m_PassiveSkill[ Skill->ucID ].Grade)- int(Skill->Effect[1] * (m_PassiveSkill[ Skill->ucID ].Grade-1));
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_lMaxMP -= int(Skill->Effect[1]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_lMaxMP += int(Skill->Effect[1]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_lMaxMP -= int(Skill->Effect[1]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}

void CGameObject::Handler_PS_ResumeHP(  SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD )
	{
		m_Specialty.m_lResumeHP += int( Skill->Effect[13] ) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type ==  TYPE_UPDATE )
	{
		m_Specialty.m_lResumeHP += int(Skill->Effect[13] * m_PassiveSkill[ Skill->ucID ].Grade)- int(Skill->Effect[13] * (m_PassiveSkill[ Skill->ucID ].Grade-1));
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_lResumeHP -= int(Skill->Effect[13]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_lResumeHP += int(Skill->Effect[13]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_lResumeHP -= int(Skill->Effect[13]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}
void CGameObject::Handler_PS_ResumeMP(  SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD )
	{
		m_Specialty.m_lResumeMP += Skill->Effect[14] * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if (  Type == TYPE_UPDATE  )
	{
		m_Specialty.m_lResumeMP += int(Skill->Effect[14] * m_PassiveSkill[ Skill->ucID ].Grade)- int(Skill->Effect[14] * (m_PassiveSkill[ Skill->ucID ].Grade-1));
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_lResumeMP -= int(Skill->Effect[14]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_lResumeMP += int(Skill->Effect[14]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_lResumeMP -= int(Skill->Effect[14]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}

void CGameObject::Handler_PS_Attack( SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD )
	{
		m_Specialty.m_lAttack += Skill->Effect[4] *( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type == TYPE_UPDATE )
	{
		m_Specialty.m_lAttack += int(Skill->Effect[4] * m_PassiveSkill[ Skill->ucID ].Grade)- int(Skill->Effect[4] * (m_PassiveSkill[ Skill->ucID ].Grade-1));
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_lAttack -= int(Skill->Effect[4]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_lAttack += int(Skill->Effect[4]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_lAttack -= int(Skill->Effect[4]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}

void CGameObject::Handler_PS_Defend( SKillData *Skill, AttributeType Type )
{
	if ( Type ==  TYPE_ADD )
	{
		m_Specialty.m_lDefend += int( Skill->Effect[5]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if (  Type == TYPE_UPDATE  )
	{
		m_Specialty.m_lDefend += int(Skill->Effect[5] * m_PassiveSkill[ Skill->ucID ].Grade)- int(Skill->Effect[5] * (m_PassiveSkill[ Skill->ucID ].Grade-1));
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_lDefend -= int(Skill->Effect[5]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_lDefend += int(Skill->Effect[5]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_lDefend -= int(Skill->Effect[5]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}

void CGameObject::Handler_PS_MagicAttack (  SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD  )
	{
		m_Specialty.m_lMagicAttack += Skill->Effect[6] *( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type == TYPE_UPDATE )
	{
		m_Specialty.m_lMagicAttack += int(Skill->Effect[6] * m_PassiveSkill[ Skill->ucID ].Grade)- int(Skill->Effect[6] * (m_PassiveSkill[ Skill->ucID ].Grade-1));
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_lMagicAttack -= int(Skill->Effect[6]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_lMagicAttack += int(Skill->Effect[6]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_lMagicAttack -= int(Skill->Effect[6]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}

void CGameObject::Handler_PS_MagicDefend( SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD  )
	{
		m_Specialty.m_lMagicDefend += Skill->Effect[7] *( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type == TYPE_UPDATE )
	{
		m_Specialty.m_lMagicDefend += int(Skill->Effect[7] * m_PassiveSkill[ Skill->ucID ].Grade)- int(Skill->Effect[7] * (m_PassiveSkill[ Skill->ucID ].Grade-1));
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_lMagicDefend -= int(Skill->Effect[7]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_lMagicDefend += int(Skill->Effect[7]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_lMagicDefend -= int(Skill->Effect[7]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}
void CGameObject::Handler_PS_Hit( SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD  )
	{
		m_Specialty.m_lHit += Skill->Effect[2] *( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type == TYPE_UPDATE )
	{
		m_Specialty.m_lHit += int(Skill->Effect[2] * m_PassiveSkill[ Skill->ucID ].Grade)- int(Skill->Effect[2] * (m_PassiveSkill[ Skill->ucID ].Grade-1));
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_lHit -= int(Skill->Effect[2]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_lHit += int(Skill->Effect[2]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_lHit -= int(Skill->Effect[2]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}

void CGameObject::Handler_PS_Dodge(  SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD  )
	{
		m_Specialty.m_lDodge += Skill->Effect[3] *( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type == TYPE_UPDATE )
	{
		m_Specialty.m_lDodge += int(Skill->Effect[3] * m_PassiveSkill[ Skill->ucID ].Grade)- int(Skill->Effect[3] * (m_PassiveSkill[ Skill->ucID ].Grade-1));
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_lDodge -= int(Skill->Effect[3]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_lDodge += int(Skill->Effect[3]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_lDodge -= int(Skill->Effect[3]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}

void CGameObject::Handler_PS_CritAppend(  SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD  )
	{
		m_Specialty.m_lCritAppend += Skill->Effect[8] *( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type == TYPE_UPDATE )
	{
		m_Specialty.m_lCritAppend += int(Skill->Effect[8] * m_PassiveSkill[ Skill->ucID ].Grade)- int(Skill->Effect[8] * (m_PassiveSkill[ Skill->ucID ].Grade-1));
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_lCritAppend -= int(Skill->Effect[8]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_lCritAppend += int(Skill->Effect[8]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_lCritAppend -= int(Skill->Effect[8]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}

void CGameObject::Handler_PS_CritDefen( SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD  )
	{
		m_Specialty.m_lCritDefend += Skill->Effect[9] *( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type == TYPE_UPDATE )
	{
		m_Specialty.m_lCritDefend += int(Skill->Effect[9] * m_PassiveSkill[ Skill->ucID ].Grade)- int(Skill->Effect[9] * (m_PassiveSkill[ Skill->ucID ].Grade-1));
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_lCritDefend -= int(Skill->Effect[9]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_lCritDefend += int(Skill->Effect[9]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_lCritDefend -= int(Skill->Effect[9]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}

void CGameObject::Handler_PS_PhyDamageAppend (  SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD  )
	{
		m_Specialty.m_fPhyDamageAppend += Skill->Effect[11] *( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type == TYPE_UPDATE )
	{
		m_Specialty.m_fPhyDamageAppend += Skill->Effect[11];
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_fPhyDamageAppend -= int(Skill->Effect[11]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_fPhyDamageAppend += int(Skill->Effect[11]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_fPhyDamageAppend -= int(Skill->Effect[11]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}

void CGameObject::Handler_PS_MagicDamageAppend(  SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD  )
	{
		m_Specialty.m_fMagicDamageAppend += Skill->Effect[12] *( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type == TYPE_UPDATE )
	{
		m_Specialty.m_fMagicDamageAppend += Skill->Effect[12];
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_fMagicDamageAppend -= int(Skill->Effect[12]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_fMagicDamageAppend += int(Skill->Effect[12]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_fMagicDamageAppend -= int(Skill->Effect[12]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}



void CGameObject::Handler_PS_DamageSorb( SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD  )
	{
		m_Specialty.m_lDamageSorb += Skill->Effect[10] *( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type == TYPE_UPDATE )
	{
		m_Specialty.m_lDamageSorb += int(Skill->Effect[10] * m_PassiveSkill[ Skill->ucID ].Grade)- int(Skill->Effect[10] * (m_PassiveSkill[ Skill->ucID ].Grade-1));
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_lDamageSorb -= int(Skill->Effect[10]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_lDamageSorb += int(Skill->Effect[10]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_lDamageSorb -= int(Skill->Effect[10]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}
void CGameObject::Handler_PS_BeCure( SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD  )
	{
		m_Specialty.m_fBeCure += Skill->Effect[18] *( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type == TYPE_UPDATE )
	{
		m_Specialty.m_fBeCure += Skill->Effect[18];
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_fBeCure -= int(Skill->Effect[18]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_fBeCure += int(Skill->Effect[18]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_fBeCure -= int(Skill->Effect[18]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}
void CGameObject::Handler_PS_Cure( SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD  )
	{
		m_Specialty.m_fCure += Skill->Effect[17] *( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type == TYPE_UPDATE )
	{
		m_Specialty.m_fCure += Skill->Effect[17];
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_fCure -= int(Skill->Effect[17]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_fCure += int(Skill->Effect[17]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_fCure -= int(Skill->Effect[17]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}

}
void CGameObject::Handler_PS_OneStatusAppend ( SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD  )
	{
		m_Specialty.m_lStatusHit += Skill->Effect[15] *( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type == TYPE_UPDATE )
	{
		m_Specialty.m_lStatusHit += int(Skill->Effect[15] * m_PassiveSkill[ Skill->ucID ].Grade)- int(Skill->Effect[15] * (m_PassiveSkill[ Skill->ucID ].Grade-1));
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_lStatusHit -= int(Skill->Effect[15]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_lStatusHit += int(Skill->Effect[15]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_lStatusHit -= int(Skill->Effect[15]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}
void CGameObject::Handler_PS_OneStatudDefend( SKillData *Skill, AttributeType Type )
{
	if ( Type == TYPE_ADD  )
	{
		m_Specialty.m_lStatudDodge += Skill->Effect[16] *( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if ( Type == TYPE_UPDATE )
	{
		m_Specialty.m_lStatudDodge += int(Skill->Effect[16] * m_PassiveSkill[ Skill->ucID ].Grade)- int(Skill->Effect[16] * (m_PassiveSkill[ Skill->ucID ].Grade-1));
	}
	else if( Type == TYPE_EQUIP_DEC )
	{
		m_Specialty.m_lStatudDodge -= int(Skill->Effect[16]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else if( Type == TYPE_EQUIP_INC )
	{
		m_Specialty.m_lStatudDodge += int(Skill->Effect[16]) * ( m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
	else
	{
		m_Specialty.m_lStatudDodge -= int(Skill->Effect[16]) * ( m_PassiveSkill[ Skill->ucID ].Grade + m_PassiveSkill[ Skill->ucID ].EquipLV );
	}
}

