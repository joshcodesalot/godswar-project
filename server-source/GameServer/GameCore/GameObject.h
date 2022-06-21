//========================================================
//
//    Copyright (c) 2006,欢乐连线工作室
//    All rights reserved.
//
//    文件名称 ： GameObject.h
//    摘    要 ： 动态对象功能模块
//    
//    当前版本 ： 1.00
//    作    者 ： 林德辉
//    完成日期 ： 2007-01-17
//
//========================================================
#ifndef _FILE_GAMEOBJ_H
#define _FILE_GAMEOBJ_H
#pragma once

#pragma warning( disable:4244 )

#include "Obj.h"
#include "GameData.h"
#include "..\..\Common/Include/DB\inc\idb.h"
#include ".\TimeElems.h"
#include ".\Region.h"
#include "Map.h"
#include "..\ErrorMessage.h"
#include <mmsystem.h>

class CStatus;
class CSkillManager;
class CWorld;

//--------------------------------------------------------------------------------------
// Helper macros to build member functions that access member variables with thread safety
//--------------------------------------------------------------------------------------
#define SET_DATA_ACCESSOR( x, y )     inline void SetBase##y( x t )  {  m_BaseData.m_##y = t; };\
                                 inline void SetSpecialty##y( x t )  {  m_Specialty.m_##y = t; };\
                                 inline void SetEquip##y( x t )  {  m_EquipData.m_##y = t; };\
								 inline void SetStatus##y( x t )  {  m_StatusData.m_##y = t; };\
								 inline void SetAltar##y( x t )  {  m_AltarData.m_##y = t; };
#define GET_DATA_ACCESSOR( x, y)      inline x GetBase##y() {  return (x)m_BaseData.m_##y; };\
								 inline x GetSpecialty##y() {  return (x)m_Specialty.m_##y; };\
								 inline x GetEquip##y() {  return (x)m_EquipData.m_##y; };\
								 inline x GetStatus##y() {  return (x)m_StatusData.m_##y; };\
								 inline x GetAltar##y() {  return (x)m_AltarData.m_##y; };\
                                 inline x GetAll##y() {  return (x)(m_BaseData.m_##y + m_Specialty.m_##y + m_EquipData.m_##y + m_StatusData.m_##y + m_AltarData.m_##y );};
#define GET_SET_DATA_ACCESSOR( x, y )   SET_DATA_ACCESSOR( x, y) GET_DATA_ACCESSOR( x, y)

#define SET_ACCESSOR( x, y, z)       inline void Set##y( x t )  {  z.m_##y = t; };
#define GET_ACCESSOR( x, y, z )       inline x Get##y() {  return (x)z.m_##y; };
#define GET_SET_ACCESSOR( x, y, z )   SET_ACCESSOR( x, y, z ) GET_ACCESSOR( x, y, z )

#define RESUME_TIME_INTERVAL 5
#define RELATION_TIME_INTERVAL 30
#define EXP_INTERVAL 100
#define MAX_AVATAR	12


struct tarWalk
{
	float x, z, fatan;
};
typedef list< tarWalk* >::iterator WalkIterator;

struct tarPath
{
	float		x, y, z;
	float		end_x, end_y, end_z;
	float		OffsetX, OffsetY, OffsetZ;
	unsigned int nEndTick;
	unsigned int nTick;
};
typedef list< tarPath* >::iterator PathIterator;


#define MAX_BUFF			20
#define MAX_DEBUFF			20
class CBuff;

enum PhysiacalAttack
{   
	ATTACK_UBAEMED,     //空手
	ATTACK_HAMMER,      //锤
	ATTACK_CHOP,        //斩
	ATTACK_STICK,       //刺
	ATTACK_SHOT,        //射
	ATTACK_STAFF,       //杖

};

enum Magic_Attack
{
	MAGIC_PHY,
	MAGIC_MAGIC,        //地属性
	MAGIC_IATRIC,       //医疗
	MAGIC_FLY,
};


enum  TIME_STYLE{
	TIME_STYLE_CAST  ,               //吟唱
	TIME_STYLE_COOL  ,               //冷却
	TIME_STYLE_NONE
};

enum{	
	STYLE_DECREASE = 0,
	STYLE_INCREASE
};

enum HandType
{
	HTNULL,
	HTOne,
	HTTwo,
	HTDoubule
};

enum ATTACK_TYPE
{
	ATTACK_FORCE,
	ATTACK_HIT,
	ATTACK_MISS,
};


typedef struct  ACTIVE_INFO
{
	
	TIME_STYLE  Time_style;
	time_t      Time_start;
	UINT        Time_interval;

} ACTIVEINFO ;

typedef struct PASSIVE_INFO
{
	//UINT  Degree_Practice; 
	//SKillData *SkillFunAdd;
	UINT  Grade;
	USHORT EquipLV;

} PASSIVEINFO;


struct DAMAGE_INFO
{
	int		Type;				// 0 - ATTACK_FORCE, 1 - ATTACK_HIT, 2 - ATTACK_MISS
	int		HP;
	int		MP;
};


enum eAvatar{
	AVATAR_CAP = 0,
	AVATAR_HAIR,
	AVATAR_HEAD,
	AVATAR_EYE,
	AVATAR_BODY,
	AVATAR_GLOVES,
	AVATAR_BOOTS,
	AVATAR_SLEEVES,
	AVATAR_LEGGINS,
	AVATAR_WEAPONONEHAND,
	AVATAR_WEAPONTWOHAND,
};


static char* SQL_SELECT_USER_BY_ID	    = "SELECT curHP,name,curMP FROM character_base WHERE id = %d LIMIT 1";//"set names \"utf8\" ;
static char* SQL_DELETE_COOLTIME_BY_ID  = "DELETE  FROM user_magic WHERE user_id= %d ";
static char* SQL_INSERT_COOLTIME_BY_ID  = "INSERT INTO user_magic VALUES ( %d,%d,%d)" ;

class CGameObject;
class CCell;

struct tarEnmity
{
	CGameObject*	pObject;
	long			lValue;

	bool operator == (const tarEnmity& right) const
	{
		return pObject == right.pObject && lValue == right.lValue;
	}
};


typedef list< tarEnmity > EnmityList;
typedef list< tarEnmity >::iterator EnmityIterator;
typedef struct
{
	int     HP_A;
	float   HP_B;
	float   HP_C;
	float	HP_D;
	int     MP_A;
	float   MP_B;
	float   MP_C;
	int     Attack_A;
	float	Attack_B;
	float	Attack_C;
	float	Attack_D;
	int	    Defend_A;
	float	Defend_B;
	float	Defend_C;
	float	Defend_D;
	int		Hit_A;
	int		Hit_B;
	float	Hit_C;
	int		Miss_A;
	int		Miss_B;
	float	Miss_C;
	int		MagicExempt_A;
	int		MagicExempt_B;
	float	MagicExempt_C;

	int	    MagicResist_A;
	float	MagicResist_B;

} PARAMETER_LIST;

typedef struct
{
	int     Defalult_Hit ;
	int	    Defalult_Dodge;
	float	Default_Resume_HP;
	float   Default_Resume_MP;

} DEFAULT_LIST;

struct ScriptObject 
{
	ScriptObject()
	:
		ID(-1) ,
		Rank(0)
	{}

	int ID;
	int	Rank;
};

typedef struct
{
  float  PhyAttack_A_1 ;        
  float  PhyAttack_A_2  ;      
  float  PhyAttack_A_3  ;     
  float  PhyAttack_B_1  ;     
  float  PhyAttack_B_2  ;     
  float  PhyAttack_B_3  ;     
  float  PhyAttack_B_4  ;     
  float  PhyAttack_C    ;

  float  MagicAttack_A_1  ;    
  float  MagicAttack_A_2  ;      
  float  MagicAttack_A_3  ;    
  float  MagicAttack_B_1  ;       
  float  MagicAttack_B_2  ;       
  float  MagicAttack_B_3  ;     
  float  MagicAttack_B_4  ;       
  float  MagicAttack_C   ;
} ATTACK_PARAM;


//****************************************************
//名字：对象基类
//描述：游戏中对香基类
//日期：2008-4-8
//****************************************************
class CGameObject
{	
	typedef  void (CGameObject::*pFAddr)( float, bool,int ) ;
    /*-------------------------------------------*/
    /*               成员方法区               */
    /*-------------------------------------------*/

public:
	CGameObject(void);
	virtual ~CGameObject(void);

	/*--------------------------------------------------------
	  功能：对象逻辑内存位置是否可用
	  参数：无
	  返回：bool：如果该对象可用返回true，负责返回false
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual bool IsActive( void ) { return m_bActive; }
	/*--------------------------------------------------------
	  功能：重置对象逻辑内存标志
	  参数:  无
	  返回：无 
	  作者：李锋军
	  备注：对象重置
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual void Release( void );
	/*--------------------------------------------------------
	  功能：对象AI
	  参数：无
	  返回：无
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual void AI( void );
	/*--------------------------------------------------------
	  功能：获得该对象的当前目标点
	  参数：无
	  返回：CGageObject*:目标对象的指针
	----------------------------------------------------------*/	
	virtual CGameObject* GetTarget( void ) { return m_targetObj; }
	virtual CGameObject* GetSkillTarget( void ) { return m_skillTargetObj; }
	/*--------------------------------------------------------
	  功能：设置该对象的目标
	  参数：pObject：将要被设置的目标
	  返回：无
	----------------------------------------------------------*/
	virtual void  SetTarget( CGameObject* pObject ) { m_targetObj = pObject; }
	virtual void  SetSkillTarget( CGameObject* pObject ) { m_skillTargetObj = pObject; }

	//计算金钱
	virtual void CalculateMoney( CGameObject* pObj );
	/*--------------------------------------------------------
	  功能：获得攻击半径
	  参数：
	  返回：
	----------------------------------------------------------*/
	virtual AttackRadius GetAttackRadius();
	/*--------------------------------------------------------
	  功能：得到对象逻辑位置
	  参数：ID：对象的内存位置
	  返回：
	----------------------------------------------------------*/
	virtual  long GetID( void ) const { return m_lID; }
	/*--------------------------------------------------------
	  功能：设置对象逻辑内存位置
	  参数：
	  返回：
	  ----------------------------------------------------------*/
	void SetID( long lID ) { m_lID = lID; }
	/*--------------------------------------------------------
	  功能：设置当前区域格
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/	
	void SetCurrentCell( CCell *pCell ){ m_pCurrentCell = pCell; }
	/*--------------------------------------------------------
	  功能：得到当前区域格
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	CCell *GetCurrentCell() const { return m_pCurrentCell; }
	/*--------------------------------------------------------
	  功能：重置对象重用标志
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	void Activate( void );
	/*--------------------------------------------------------
	  功能：设置自己的当前场景
	  参数：
	  返回：
	----------------------------------------------------------*/
	void SetRegion( CRegion* pRegion ) { m_pRegion = pRegion; m_ObjectData.m_lRegionID = (USHORT)m_pRegion->GetLogicID(); };
	/*--------------------------------------------------------
	  功能：得到当前场景
	  参数：
	  返回：
	----------------------------------------------------------*/
	CRegion* GetRegion( void ) { return m_pRegion; }
    /*--------------------------------------------------------
      功能：得到X方向坐标
      参数：
      返回：
    ----------------------------------------------------------*/
	float GetPosX() { return m_ObjectData.m_fX; }
	/*--------------------------------------------------------
	  功能：得到Y方向坐标
	  参数：
	  返回：
	  作者：
	  备注：游戏中Y方向始终为0
	  日期：2008-4-8
	----------------------------------------------------------*/
	float GetPosY() { return m_ObjectData.m_fY; }
	/*--------------------------------------------------------
	  功能：得到Z方向坐标
	  参数：
	  返回：
	----------------------------------------------------------*/
	float GetPosZ() { return m_ObjectData.m_fZ; }
	/*--------------------------------------------------------
	  功能：取得该对象的朝向
	  参数：
	  返回：
	----------------------------------------------------------*/
	float GetAtan2() { return m_fatan2; }
	/*--------------------------------------------------------
	  功能：取得X方向的偏移
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	float GetOffsetX( void ) { return m_fOffsetX; }
	/*--------------------------------------------------------
	  功能：取得Z方向的偏移
	  参数：
	  返回：
	----------------------------------------------------------*/
	float GetOffsetZ( void ) { return m_fOffsetZ; }
	/*--------------------------------------------------------
	  功能：设置当前坐标
	  参数：
	  返回：
	----------------------------------------------------------*/
	CCell* SetPos( float x, float y, float z , bool IsFirst = false );
	/*--------------------------------------------------------
	  功能：设置开始战斗的标志
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual void StartFight( CGameObject* pObject );
	/*--------------------------------------------------------
	  功能：设置结束战斗的标志
	  参数：
	  返回：
	----------------------------------------------------------*/
	virtual void EndFight();
	/*--------------------------------------------------------
	  功能：设置战斗状态
	  参数：
	  返回：
	----------------------------------------------------------*/
	void SetFight( bool value ) { m_bIsFight = value; }
	/*--------------------------------------------------------
	  功能：得到战斗状态
	  参数：
	  返回：
	----------------------------------------------------------*/
	bool GetFight( void ) { return m_bIsFight; }
	/*--------------------------------------------------------
	  功能：离开战斗
	  参数：
	  返回：
	----------------------------------------------------------*/
	void LeaveFight( void ) {};
	/*--------------------------------------------------------
	  功能：是否可进行攻击，确保攻击间隔
	  参数：
	  返回：
	----------------------------------------------------------*/
	bool IsAttack( void );
	/*--------------------------------------------------------
	  功能：设置对象激活状态
	  参数：
	  返回：
	----------------------------------------------------------*/
	long SetActive( bool active );
	/*--------------------------------------------------------
	  功能：或得对象游戏状态
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	eObjectType	GetType( void ) { return m_eType; }
	/*--------------------------------------------------------
	  功能：清理走路同步数据
	  参数：
	  返回：
	----------------------------------------------------------*/
	void ClearPath( void );
	/*--------------------------------------------------------
	  功能：得到脚本对象
	  参数：
	  返回：
	----------------------------------------------------------*/
	void GetScriptObject(ScriptObject* obj);
	/*--------------------------------------------------------
	  功能：设置脚本对象
	  参数：
	  返回：
	----------------------------------------------------------*/
	void SetScriptObject(ScriptObject* obj);
	/*--------------------------------------------------------
	  功能：或得装备武器的类型
	  参数：
	  返回：
	----------------------------------------------------------*/
	virtual HandType GetHandType();
	 /*--------------------------------------------------------
	   功能：角色死亡
	   参数：
	   返回：
	 ----------------------------------------------------------*/
	virtual void Dead( CGameObject  *pObj );
	/*--------------------------------------------------------
	  功能：角色是否死亡
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual bool IsDead();

	/*--------------------------------------------------------
	  功能：判断角色是否为另外对象所拥有
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual bool IsOwner(const char* )
	{
		return false;
	}

	/*--------------------------------------------------------
	  功能：得到装备数据
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	EquipData* GetEquipData()
	{
		return &m_EquipData;
	}
	/*--------------------------------------------------------
	功能：得到祭坛数据
	参数：
	返回：
	作者：李锋军
	备注：
	日期：2008-4-8
	----------------------------------------------------------*/
	AltarData* GetEquiAltarpData()
	{
		return &m_AltarData;
	}
    /*--------------------------------------------------------
      功能：得到状态数据
      参数：
      返回：
      作者：李锋军
      备注：
      日期：2008-4-8
    ----------------------------------------------------------*/
	StatusData* GetStatusData()
	{
		return &m_StatusData;
	}

	/*--------------------------------------------------------
	  功能：判断攻击命中
	  参数：
	  返回：
	  作者：李锋军
	  备注：主要指魔法和招式类
	  日期：2008-4-8
	----------------------------------------------------------*/
	ATTACK_TYPE JudgeAttackStyle( CGameObject* pTarget );
	/*--------------------------------------------------------
	  功能：判断命中,暴击
	  参数：
	  返回：
	  作者：李锋军
	  备注：主要指普通攻击类
	  日期：2008-4-8
	----------------------------------------------------------*/
	ATTACK_TYPE JudgeAppendStatusStyle(  CGameObject* pTarget, const MagicData* pMagicData );
	/*--------------------------------------------------------
	  功能：初始化对象数据
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual bool InitObjData(long  ID) = 0 ;
	/*--------------------------------------------------------
	  功能：将其他对象信息同步给自己
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual void AynObjToObj( CCell *pOldCell, CCell *pNewCell );
	/*--------------------------------------------------------
	  功能：将自己的信息同步给其他玩家
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual void AynMeToOther( CCell *pOldCell, CCell *pNewCell );
	/*--------------------------------------------------------
	  功能：物理攻击的时候计算各个技能熟练度
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual bool CalculatePassivePracticeDegree(  PhysiacalAttack Type, ATTACK_TYPE eAttackType, bool IsDead = false ) = 0;
	/*--------------------------------------------------------
	  功能：魔法攻击时候计算各个技能熟练度
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	
	virtual void CalculatePassivePracticeDegree( int MagicID, ATTACK_TYPE eAttackType, bool IsDead = false ) = 0;
	/*--------------------------------------------------------
	  功能：魔法攻击时候计算各个技能熟练度
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	
	virtual void BePhysicalAttackedPassivePracticeDegree( PhysiacalAttack , ATTACK_TYPE  ){};
	/*--------------------------------------------------------
	  功能：计算被魔法攻击时候的技能熟练度
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	
	virtual void BeMagicAttackPassivePracticeDegree( UINT ,  ATTACK_TYPE ){};
	/*--------------------------------------------------------
	  功能：计算经验值
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	
	virtual bool CalculateExp( bool TarIsDead , int exp = 1 );
	/*--------------------------------------------------------
	  功能：改变技能熟练度
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	
	virtual bool ChanegeDegree( int ucSkillID, int Buffer)= 0;
	/*--------------------------------------------------------
	  功能：判断技能升级
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual bool PassiveSkillUpgrade(UINT  SkillID, int *Rank = NULL ) =0 ;
	/*--------------------------------------------------------
	  功能：添加被动技能等级或者改变已有技能登记
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual void AddPassiveSkill( int ucSkillID, int iRank = 1, int EXP = 0 ) = 0;
	/*--------------------------------------------------------
	  功能：魔法是否处于冷却状态
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual	bool IsActiveSkillCooling( int ucSkillID );
	/*--------------------------------------------------------
	  功能：发送神恩开始累计的消息
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual	bool IsGodSkillTimeOut( int MagicID );
	/*--------------------------------------------------------
	  功能：设置技能开始时间
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual	bool SetActiveSkillStartTime( int ucSkillID);
	/*--------------------------------------------------------
	  功能：设置当前技能ID
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual	void SetCurActiveSkillID( int ucActiveSkillID);
	/*--------------------------------------------------------
	  功能：重置技能时间
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual	void ResetTimer( float  IntervalTime ,TIME_STYLE timeStyle);
	/*--------------------------------------------------------
	  功能：改变主动技能状态
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual	void ChangeActiveSkillStatus( int ucSkillID, TIME_STYLE status );
	/*--------------------------------------------------------
	  功能：判断技能刷新时间
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual	bool IsRefresh( TIME_STYLE timeStyle );
	/*--------------------------------------------------------
	  功能：计算物理攻击伤害
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual INT CalculateAttackDamage( CGameObject& pObj, float iPower1 = 0, int iPower2 = 0 ) ;
	/*--------------------------------------------------------
	  功能：计算经验和等级
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual void CalculateExpAndLevel( CGameObject& pTarObj );
	/*--------------------------------------------------------
	  功能：计算魔法攻击伤害
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual INT	CalculateAttackDamage( int MagicID , CGameObject &pObj) ;
	/*--------------------------------------------------------
	  功能：获得等级
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual UINT  GetRank()=0;
	/*--------------------------------------------------------
	  功能：删除技能
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual bool DelPassiveSkill( int ucSkillID );
	/*--------------------------------------------------------
	  功能：查找被动技能
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual bool FindPassiveSkill( int ucSkillID,UINT *Degree = NULL  );
	/*--------------------------------------------------------
	  功能：查找主动技能
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual bool FindActiveSkill( int ucSkillID ) ;
	/*--------------------------------------------------------
	  功能：添加主动技能
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual bool AddActiveSkill( int ucSkillID ) ;
	/*--------------------------------------------------------
	  功能：删除主动技能
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual void DelActiveSkill( int ucSkillID ) ;
	/*--------------------------------------------------------
	  功能：删除状态
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	bool DelStatus( CStatus *pStatus );
	/*--------------------------------------------------------
	  功能：添加状态
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	bool AddStatus( CStatus *pStatus );
	/*--------------------------------------------------------
	  功能：判断能否成功添加状态
	  参数：
	  返回：
	  作者：李锋军
	  备注：根据状态的类型和有限级
	  日期：2008-4-8
	----------------------------------------------------------*/
	bool CanAddStatus( CStatus *pStatus );
	/*--------------------------------------------------------
	  功能：状态的处理
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	void ResolveStatus( );
	/*--------------------------------------------------------
	  功能：得到固定状态
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	size_t GetStatus( UINT StatusArr[] );
	/*--------------------------------------------------------
	  功能：是否装备重甲
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual bool IsHeavyLoricae()  { return false; };
	/*--------------------------------------------------------
	  功能：是否装备布甲
	  参数：
	  返回：
	----------------------------------------------------------*/
	virtual bool IsFabircLoricae() { return false; };
	/*--------------------------------------------------------
	  功能：获得当前技能的ID
	  参数：
	  返回：
	----------------------------------------------------------*/
	virtual	int GetCurActiveSkillID();
	/*--------------------------------------------------------
	  功能：增加仇恨，如果不存在则添加
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual	void AddEnmity( CGameObject* pObject, long lValue );	
	/*--------------------------------------------------------
	  功能：减少仇恨
	  参数：
	  返回：
	----------------------------------------------------------*/
	virtual	void DecEnmity( CGameObject* pObject, long lValue );		
	/*--------------------------------------------------------
	  功能：脱离仇恨
	  参数：
	  返回：
	----------------------------------------------------------*/
	virtual	void RemoveEnmity( CGameObject* pObject );	
	/*--------------------------------------------------------
	  功能：清空仇恨
	  参数：
	  返回：
	----------------------------------------------------------*/
	virtual	void ClearEnmity();
	virtual	void ChainEnmityList(CGameObject* obj , int value);
	virtual void CoagentEnmityList(CGameObject* obj , int value);
	virtual bool IsClearEnmity();
	/*--------------------------------------------------------
	  功能：或得最大仇恨者
	  参数：
	  返回：
	----------------------------------------------------------*/
	virtual	CGameObject* GetMaxEnmity();
	/*--------------------------------------------------------
	  功能：获得经验
	  参数：
	  返回：
	----------------------------------------------------------*/
	virtual ULONG GetEXP(){ return 0;};
	/*--------------------------------------------------------
	  功能：增加HP
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	void			AddHP( const int hp );
	/*--------------------------------------------------------
	  功能：增加MP
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	void			AddMP( const int mp );
	/*--------------------------------------------------------
	  功能：获得命中
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	//virtual ULONG GetHit() = 0;
	/*--------------------------------------------------------
	  功能：获得躲避
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual ULONG GetDodge() = 0;
	/*--------------------------------------------------------
	  功能：获得攻击力
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual UINT  CalculateAttack();
	/*--------------------------------------------------------
	  功能：获得防御力
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	virtual UINT  CalculateDefend() const ;
	/*--------------------------------------------------------
	  功能：获得最大HP
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	int	GetMaxHP( void ) const;
	/*--------------------------------------------------------
	  功能：获得最大MP
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	int	GetMaxMP( void ) const;
    /*--------------------------------------------------------
      功能：或得技能点
      参数：
      返回：
      作者：李锋军
      备注：
      日期：2008-4-8
    ----------------------------------------------------------*/
	UINT GetSkillPoint(){ return m_ConvertPoint; };
	/*--------------------------------------------------------
	  功能：获得技能经验
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	UINT GetSkillExp(){ return m_WinExp; };

	/*--------------------------------------------------------
	  功能：设置技能点
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	void SetSkillPoint( UINT uiPoint ){  m_ConvertPoint = uiPoint ; };
	/*--------------------------------------------------------
	  功能：设置技能经验
	  参数：
	  返回：
	  作者：李锋军
	  备注：
	  日期：2008-4-8
	----------------------------------------------------------*/
	void SetSkillExp( UINT uiExp ){  m_WinExp = uiExp; };
	/*--------------------------------------------------------
	  功能：客户端UI数据同步
	  参数：
	  返回：
	----------------------------------------------------------*/
	virtual void  SynGameData( bool ){};
    /*--------------------------------------------------------
      功能：群伤获得伤害信息
      参数：
      返回：
    ----------------------------------------------------------*/
	DAMAGE_INFO		GetDamageInfo( void ) const;
	/*--------------------------------------------------------
	  功能：设置群伤伤害信息
	  参数：
	  返回：
	----------------------------------------------------------*/
	void	SetDamageInfo( const DAMAGE_INFO dmgInfo );
	/*--------------------------------------------------------
	  功能：清楚伤害信息
	  参数：无
	  返回：
	----------------------------------------------------------*/
	void	ClearDamageInfo( void );
	/*--------------------------------------------------------
	功能：增加威胁值
	参数：无
	返回：
	----------------------------------------------------------*/
	void AddDanger( int iDander = 1 ) { m_lDanger += iDander; }
	/*--------------------------------------------------------
	功能：获得威胁值
	参数：无
	返回：
	----------------------------------------------------------*/
	long GetDanger( void ) { return m_lDanger; }
	/*--------------------------------------------------------
	功能：获得对象状态 
	参数：无
	返回：
	----------------------------------------------------------*/
	eObjectState GetState( void ) { return m_eState; }
	/*--------------------------------------------------------
	功能：获得对象状态 
	参数：无
	返回：
	----------------------------------------------------------*/
	void SetState( eObjectState  eState ) {  m_eState = eState ; }
    /*--------------------------------------------------------
      功能：获得对象基本数据
      参数：
      返回：
      作者：李锋军
      备注：
      日期：2008-4-8
    ----------------------------------------------------------*/

	void ResetCastState();

	virtual char* GetName(){ return m_ObjectData.m_strName; }
	virtual void  SetName( const char * RoleName ){ strcpy( m_ObjectData.m_strName, RoleName ); };

// 	virtual char* GetacName(){ return m_ObjectData.m_acName; }
// 	virtual void  SetacName( const char * cName ){ strcpy( m_ObjectData.m_acName, cName ); };
	LONG   GetlHP(){ return m_ObjectData.m_lHP + m_StatusData.m_AddHP - m_StatusData.m_DecHP; };
	LONG   GetlMP(){ return m_ObjectData.m_lMP + m_StatusData.m_AddMP - m_StatusData.m_DecMP; };
	void   SetlHP( LONG HP ){   m_ObjectData.m_lHP = HP; };
	void   SetlMP( LONG MP ){  m_ObjectData.m_lMP  = MP; };
	GET_SET_ACCESSOR( BYTE,   		Gender,      		  	m_ObjectData);
	GET_SET_ACCESSOR( USHORT, 		lRegionID,   		  	m_ObjectData);
	GET_SET_ACCESSOR( FLOAT,  		fX,          		  	m_ObjectData);
	GET_SET_ACCESSOR( FLOAT,  		fY,          		  	m_ObjectData);
	GET_SET_ACCESSOR( FLOAT,  		fZ,          		  	m_ObjectData);
	GET_SET_ACCESSOR( FLOAT,  		fSpeed,           		m_ObjectData);
	GET_SET_ACCESSOR( USHORT, 		lStyle,           		m_ObjectData);
	GET_SET_ACCESSOR( BYTE,   		lFaction,         		m_ObjectData);
	GET_SET_ACCESSOR( USHORT, 		lPrestige,        		m_ObjectData);
	GET_SET_ACCESSOR( BYTE,   		cKnight,          		m_ObjectData);
	GET_SET_ACCESSOR( USHORT, 		cUnion,           		m_ObjectData);
	GET_SET_ACCESSOR( BYTE,   		cUnionBusiness,   		m_ObjectData);
	GET_SET_ACCESSOR( UINT,   		uiUionContribute,       m_ObjectData);
	GET_SET_ACCESSOR( UINT,   		uiStoreNum,				m_ObjectData);
	GET_SET_ACCESSOR( UINT,   		uiBagNum,				m_ObjectData);
	GET_SET_ACCESSOR( BYTE,   		lClass,                 m_ObjectData);
	GET_SET_ACCESSOR( BYTE,   		cRank,            		m_ObjectData);
	GET_SET_ACCESSOR( UINT,   		cExp,             		m_ObjectData);
//	GET_SET_ACCESSOR( long,   		lHP,                	m_ObjectData);
//	GET_SET_ACCESSOR( long,   		lMP,                   	m_ObjectData);
	GET_SET_ACCESSOR( USHORT, 		usAttackDistance, 		m_ObjectData);
	GET_SET_ACCESSOR( USHORT, 		usAttackSpeed,    		m_ObjectData);
	GET_SET_ACCESSOR( BYTE,   		cHairStyle,       		m_ObjectData);
	GET_SET_ACCESSOR( BYTE,   		cPupilColor,      		m_ObjectData);
	GET_SET_ACCESSOR( USHORT, 		usBelief,         		m_ObjectData);
	GET_SET_ACCESSOR( UINT,   		lMoney,               	m_ObjectData);
	GET_SET_ACCESSOR( UINT,   		lStone,           		m_ObjectData);
	GET_SET_ACCESSOR( UINT,   		AttackDistance,         m_EquipData);
	GET_SET_ACCESSOR( UINT,   		AttackSpeed,           	m_EquipData);

	GET_SET_DATA_ACCESSOR( LONG,	  lMaxHP           		);
 	GET_SET_DATA_ACCESSOR( LONG,	  lMaxMP           		); 
 	GET_SET_DATA_ACCESSOR( LONG,	  lResumeHP      		);
 	GET_SET_DATA_ACCESSOR( LONG,	  lResumeMP      		);
 	GET_SET_DATA_ACCESSOR( LONG,	  lAttack        		);
 	GET_SET_DATA_ACCESSOR( LONG,	  lDefend        		);
 	GET_SET_DATA_ACCESSOR( LONG,	  lMagicAttack   		);
 	GET_SET_DATA_ACCESSOR( LONG,	  lMagicDefend   		);
 	GET_SET_DATA_ACCESSOR( LONG,	  lHit           		);
 	GET_SET_DATA_ACCESSOR( LONG,	  lDodge         		);
 	GET_SET_DATA_ACCESSOR( LONG,	  lCritAppend    		);
 	GET_SET_DATA_ACCESSOR( LONG,	  lCritDefend     		);
 	GET_SET_DATA_ACCESSOR( FLOAT,     fPhyDamageAppend   	);
 	GET_SET_DATA_ACCESSOR( FLOAT,     fMagicDamageAppend 	);
 	GET_SET_DATA_ACCESSOR( LONG,	  lDamageSorb        	);
 	GET_SET_DATA_ACCESSOR( FLOAT,     fBeCure            	);
 	GET_SET_DATA_ACCESSOR( FLOAT,     fCure              	);
 	GET_SET_DATA_ACCESSOR( LONG,	  lStatusHit   	        );
 	GET_SET_DATA_ACCESSOR( LONG,	  lStatudDodge   	    );

	GET_SET_ACCESSOR( FLOAT  ,   	HaltIntonate,          			m_StatusData);
	GET_SET_ACCESSOR( FLOAT ,   	ReboundDamage,         			m_StatusData);
	GET_SET_ACCESSOR( FLOAT  ,   	NonMoving,             			m_StatusData);
	GET_SET_ACCESSOR( FLOAT  ,   	NonMagicUsing,         	 		m_StatusData);
	GET_SET_ACCESSOR( FLOAT  ,   	NonZSUsing,            	 		m_StatusData);
	GET_SET_ACCESSOR( FLOAT  ,   	NonAttackUsing,        	 		m_StatusData);
	GET_SET_ACCESSOR( FLOAT  ,   	NonPropUsing,         			m_StatusData);
	GET_SET_ACCESSOR( FLOAT ,   	ReturnDamage,          			m_StatusData);
	GET_SET_ACCESSOR( FLOAT ,   	ChangeIntonateTime,    	 		m_StatusData);
	GET_SET_ACCESSOR( FLOAT ,   	BeMagicDamage,         	 		m_StatusData);
	GET_SET_ACCESSOR( FLOAT ,   	CreateMagicDamage,     			m_StatusData);
	GET_SET_ACCESSOR( FLOAT ,   	BeAttackDamage,        	 		m_StatusData);
	GET_SET_ACCESSOR( FLOAT ,   	CreateAttackDamage,    			m_StatusData);
	GET_SET_ACCESSOR( FLOAT ,   	GetEXP,                	 		m_StatusData);
	GET_SET_ACCESSOR( FLOAT ,   	AddHP,	 		                m_StatusData);
	GET_SET_ACCESSOR( FLOAT ,   	DecHP, 	 		                m_StatusData);
	GET_SET_ACCESSOR( FLOAT ,       AddMP,               	    	m_StatusData);
	GET_SET_ACCESSOR( FLOAT ,       DecMP,                	 		m_StatusData);				 				 
					 				 
	/*--------------------------------------------------------
	功能：状态效果处理函数
	参数：
	返回：
	作者：李锋军
	备注：
	日期：2008-4-8
	----------------------------------------------------------*/
	static void RouterHandler( pFAddr *Addr, int ID );
	void Handler_HaltIntonate( float value, bool isresume, int StatusID );          
	void Handler_ReboundDamage( float value, bool isresume, int StatusID );         
	void Handler_NonMoving( float value, bool isresume, int StatusID );             
	void Handler_NonMagicUsing( float value, bool isresume, int StatusID );         
	void Handler_NonZSUsing( float value, bool isresume, int StatusID );            
	void Handler_NonAttackUsing( float value, bool isresume, int StatusID );        
	void Handler_NonPropUsing( float value, bool isresume, int StatusID );          
	void Handler_ReturnDamage( float value, bool isresume, int StatusID );          
	void Handler_ChangeIntonateTime( float value, bool isresume, int StatusID ); //暂时不做   
	void Handler_BeCure( float value, bool isresume, int StatusID );                
	void Handler_UseCure( float value, bool isresume, int StatusID );               
	void Handler_BeMagicDamage( float value, bool isresume, int StatusID );         
	void Handler_CreateMagicDamage( float value, bool isresume, int StatusID );     
	void Handler_BeAttackDamage( float value, bool isresume, int StatusID );        
	void Handler_CreateAttackDamage( float value, bool isresume, int StatusID );    
	void Handler_GetEXP( float value, bool isresume, int StatusID );                
	void Handler_MPMax( float value, bool isresume, int StatusID );                 
	void Handler_HPMax( float value, bool isresume, int StatusID );                         
	void Handler_Hit( float value, bool isresume, int StatusID );                   
	void Handler_Duck( float value, bool isresume, int StatusID );                  
	void Handler_Defend( float value, bool isresume, int StatusID );                
	void Handler_Attack( float value, bool isresume, int StatusID );  
	void Handler_CirtAdd( float value, bool isresume, int StatusID );
	void Handler_CirtDefend( float value, bool isresume, int StatusID );
	void Handler_MagicAttack( float value, bool isresume, int StatusID );
	void Handler_MagicDefend( float value, bool isresume, int StatusID );
	void Handler_AddHP( float value, bool isresume, int StatusID );                 
	void Handler_DecHP( float value, bool isresume, int StatusID );                 
	void Handler_AddMP( float value, bool isresume, int StatusID );                 
	void Handler_DecMP( float value, bool isresume, int StatusID );                 
	void Handler_ChangeMagicSuccedOdds( float value, bool isresume, int StatusID ); 
	void Handler_ChangeMagicDefendOdds( float value, bool isresume, int StatusID );

	/*--------------------------------------------------------
	功能：技能效果处理函数
	参数：
	返回：
	作者：李锋军
	备注：
	日期：2008-4-8
	----------------------------------------------------------*/
	void Handler_PS_HPMax( SKillData *Skill, AttributeType Type );
	void Handler_PS_MPMax( SKillData *Skill, AttributeType Type );
	void Handler_PS_ResumeHP( SKillData *Skill, AttributeType Type );
	void Handler_PS_ResumeMP( SKillData *Skill, AttributeType Type );
	void Handler_PS_Attack( SKillData *Skill, AttributeType Type );
	void Handler_PS_Defend( SKillData *Skill, AttributeType Type );
	void Handler_PS_MagicAttack( SKillData *Skill, AttributeType Type );
	void Handler_PS_MagicDefend( SKillData *Skill, AttributeType Type );
	void Handler_PS_Hit( SKillData *Skill, AttributeType Type );
	void Handler_PS_Dodge( SKillData *Skill, AttributeType Type );
	void Handler_PS_CritAppend( SKillData *Skill, AttributeType Type );	
	void Handler_PS_DecPhysicalDamage( SKillData *Skill, AttributeType Type );
	void Handler_PS_CritDefen( SKillData *Skill, AttributeType Type );
	void Handler_PS_PhyDamageAppend( SKillData *Skill, AttributeType Type );
	void Handler_PS_MagicDamageAppend( SKillData *Skill, AttributeType Type );
	void Handler_PS_DamageSorb( SKillData *Skill, AttributeType Type );
	void Handler_PS_BeCure( SKillData *Skill, AttributeType Type );
	void Handler_PS_Cure( SKillData *Skill, AttributeType Type );
	void Handler_PS_OneStatusAppend ( SKillData *Skill, AttributeType Type );
	void Handler_PS_OneStatudDefend( SKillData *Skill, AttributeType Type );	
protected:
	/*--------------------------------------------------------
	功能：重置自动回血时间
	参数：
	返回：
	作者：李锋军
	备注：
	日期：2008-4-8
	----------------------------------------------------------*/
   void ResetResumeTimer(){ m_Resume_Timer.TimeOver(); m_Resume_Timer.Startup( RESUME_TIME_INTERVAL ) ;};
    void ResetRelationTimer(){ m_Relation_Timer.TimeOver(); m_Relation_Timer.Startup( RELATION_TIME_INTERVAL ) ;};

private:

		/*-------------------------------------------*/
		/*               成员变量区               */
		/*-------------------------------------------*/

public:

	EnmityList         	m_listEnmity;	    // 仇恨列表
	AttackRadius       	m_AttackRadius;
	long               	m_lID;
	volatile  bool      m_bActive;
	float              	m_fOffsetX, m_fOffsetY, m_fOffsetZ;
	float              	m_fDestinationX, m_fDestinationY, m_fDestinationZ;
	float              	m_fDist;
	float              	m_fDest;				    // 朝向
	float              	m_fatan2;
	float              	m_fX, m_fY, m_fZ;		    // 坐标
	bool               	m_bIsFight;				    // 战斗状态标记
	eObjectType        	m_eType;			        // 类型
	eObjectState       	m_eState;			        // 动作状态
	sbase::CTimer      	m_timeFight;		        // 战斗周期
	sbase::CTimerMS	   	m_timeAttack;	            // 战斗攻击频率
	CGameObject*       	m_targetObj;                // 目标对象
	CGameObject*       	m_skillTargetObj;           // 技能目标对象
	list<tarPath*>     	m_queuePath;
	list<tarWalk*>		m_queueWalk;
	short int          	m_nAvatar[MAX_AVATAR];
	CBuff*             	m_pBuff;				    // 增益属性
	CBuff*             	m_pDeBuff;				    // 减益属性
	CRegion*           	m_pRegion;				    // 所在场景

	

private:
	DAMAGE_INFO	        m_DamageInfo;
protected:
	
	ObjectData	                m_ObjectData;		//自己角色数据
	BaseData                    m_BaseData;         //角色基础数据
	SkillData                   m_Specialty;        //专长数据
	EquipData                	m_EquipData;        //装备数据
    StatusData                  m_StatusData;       //状态数据
	AltarData                   m_AltarData;        //祭坛数据


	map< UINT , PASSIVEINFO >   m_PassiveSkill;     //被动技能列表$$$
	map< UINT , ACTIVEINFO >    m_ActiveSkill;      //主动技能列表$$$
	map< UINT , UINT >          m_Altar;      //祭坛容器
	map< UINT, TTimeElement<CStatus*,time_t> > m_StatusMap;    //状态
	map< UINT, StatusData > m_StatusSelfData; 

	short                       m_ucCurActiveSkill;                    //当前使用魔法
	UINT                        m_uiSkillEffect;
	BYTE                        m_lRefreshTime;	 
	BYTE                        m_ucActiveSkillCastTime;    //魔法吟唱时间
	USHORT                      m_usActiveSkillCoolTime;    //魔法冷却时间
	sbase::CTimerMS             m_ActiveSkillCast_Timer;    //魔法吟唱计时器,MillSec
	sbase::CTimer               m_ActiveSkillCool_Timer;    //魔法冷却计时器
	sbase::CTimer               m_Resume_Timer;             //HP,MP恢复计时器
	sbase::CTimer               m_Relation_Timer;           //关系刷新计时器
	sbase::CTimer               m_Team_Timer;				//队友信息更新计时器
	time_t                      m_GodLastTime;
	CCell*                      m_pCurrentCell;
	UINT                        m_WinExp;                   //当前获得的经验(被动技能经验)
	UINT                        m_ConvertPoint;             //转化后的点数(被动技能点数)
	DWORD						dTimeStart;					//公共CD启动时间点

private:
	/*----------------------------------------------------------
	  功能：玩家威胁值（用于反外挂）
	  作者：
	  日期：
	----------------------------------------------------------*/
	long m_lDanger;
public:

	bool IsPublicCDOK()	{ return( timeGetTime() - dTimeStart >= 1000 ); }	//公共CD
	void StartPublicCD(){ dTimeStart = timeGetTime(); }						//公共CD启动时间点
	
public:

	static CWorld*	s_World ;


};




#endif
