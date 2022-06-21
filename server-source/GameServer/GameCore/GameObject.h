//========================================================
//
//    Copyright (c) 2006,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� GameObject.h
//    ժ    Ҫ �� ��̬������ģ��
//    
//    ��ǰ�汾 �� 1.00
//    ��    �� �� �ֵ»�
//    ������� �� 2007-01-17
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
	ATTACK_UBAEMED,     //����
	ATTACK_HAMMER,      //��
	ATTACK_CHOP,        //ն
	ATTACK_STICK,       //��
	ATTACK_SHOT,        //��
	ATTACK_STAFF,       //��

};

enum Magic_Attack
{
	MAGIC_PHY,
	MAGIC_MAGIC,        //������
	MAGIC_IATRIC,       //ҽ��
	MAGIC_FLY,
};


enum  TIME_STYLE{
	TIME_STYLE_CAST  ,               //����
	TIME_STYLE_COOL  ,               //��ȴ
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
//���֣��������
//��������Ϸ�ж������
//���ڣ�2008-4-8
//****************************************************
class CGameObject
{	
	typedef  void (CGameObject::*pFAddr)( float, bool,int ) ;
    /*-------------------------------------------*/
    /*               ��Ա������               */
    /*-------------------------------------------*/

public:
	CGameObject(void);
	virtual ~CGameObject(void);

	/*--------------------------------------------------------
	  ���ܣ������߼��ڴ�λ���Ƿ����
	  ��������
	  ���أ�bool������ö�����÷���true�����𷵻�false
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual bool IsActive( void ) { return m_bActive; }
	/*--------------------------------------------------------
	  ���ܣ����ö����߼��ڴ��־
	  ����:  ��
	  ���أ��� 
	  ���ߣ�����
	  ��ע����������
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual void Release( void );
	/*--------------------------------------------------------
	  ���ܣ�����AI
	  ��������
	  ���أ���
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual void AI( void );
	/*--------------------------------------------------------
	  ���ܣ���øö���ĵ�ǰĿ���
	  ��������
	  ���أ�CGageObject*:Ŀ������ָ��
	----------------------------------------------------------*/	
	virtual CGameObject* GetTarget( void ) { return m_targetObj; }
	virtual CGameObject* GetSkillTarget( void ) { return m_skillTargetObj; }
	/*--------------------------------------------------------
	  ���ܣ����øö����Ŀ��
	  ������pObject����Ҫ�����õ�Ŀ��
	  ���أ���
	----------------------------------------------------------*/
	virtual void  SetTarget( CGameObject* pObject ) { m_targetObj = pObject; }
	virtual void  SetSkillTarget( CGameObject* pObject ) { m_skillTargetObj = pObject; }

	//�����Ǯ
	virtual void CalculateMoney( CGameObject* pObj );
	/*--------------------------------------------------------
	  ���ܣ���ù����뾶
	  ������
	  ���أ�
	----------------------------------------------------------*/
	virtual AttackRadius GetAttackRadius();
	/*--------------------------------------------------------
	  ���ܣ��õ������߼�λ��
	  ������ID��������ڴ�λ��
	  ���أ�
	----------------------------------------------------------*/
	virtual  long GetID( void ) const { return m_lID; }
	/*--------------------------------------------------------
	  ���ܣ����ö����߼��ڴ�λ��
	  ������
	  ���أ�
	  ----------------------------------------------------------*/
	void SetID( long lID ) { m_lID = lID; }
	/*--------------------------------------------------------
	  ���ܣ����õ�ǰ�����
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/	
	void SetCurrentCell( CCell *pCell ){ m_pCurrentCell = pCell; }
	/*--------------------------------------------------------
	  ���ܣ��õ���ǰ�����
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	CCell *GetCurrentCell() const { return m_pCurrentCell; }
	/*--------------------------------------------------------
	  ���ܣ����ö������ñ�־
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	void Activate( void );
	/*--------------------------------------------------------
	  ���ܣ������Լ��ĵ�ǰ����
	  ������
	  ���أ�
	----------------------------------------------------------*/
	void SetRegion( CRegion* pRegion ) { m_pRegion = pRegion; m_ObjectData.m_lRegionID = (USHORT)m_pRegion->GetLogicID(); };
	/*--------------------------------------------------------
	  ���ܣ��õ���ǰ����
	  ������
	  ���أ�
	----------------------------------------------------------*/
	CRegion* GetRegion( void ) { return m_pRegion; }
    /*--------------------------------------------------------
      ���ܣ��õ�X��������
      ������
      ���أ�
    ----------------------------------------------------------*/
	float GetPosX() { return m_ObjectData.m_fX; }
	/*--------------------------------------------------------
	  ���ܣ��õ�Y��������
	  ������
	  ���أ�
	  ���ߣ�
	  ��ע����Ϸ��Y����ʼ��Ϊ0
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	float GetPosY() { return m_ObjectData.m_fY; }
	/*--------------------------------------------------------
	  ���ܣ��õ�Z��������
	  ������
	  ���أ�
	----------------------------------------------------------*/
	float GetPosZ() { return m_ObjectData.m_fZ; }
	/*--------------------------------------------------------
	  ���ܣ�ȡ�øö���ĳ���
	  ������
	  ���أ�
	----------------------------------------------------------*/
	float GetAtan2() { return m_fatan2; }
	/*--------------------------------------------------------
	  ���ܣ�ȡ��X�����ƫ��
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	float GetOffsetX( void ) { return m_fOffsetX; }
	/*--------------------------------------------------------
	  ���ܣ�ȡ��Z�����ƫ��
	  ������
	  ���أ�
	----------------------------------------------------------*/
	float GetOffsetZ( void ) { return m_fOffsetZ; }
	/*--------------------------------------------------------
	  ���ܣ����õ�ǰ����
	  ������
	  ���أ�
	----------------------------------------------------------*/
	CCell* SetPos( float x, float y, float z , bool IsFirst = false );
	/*--------------------------------------------------------
	  ���ܣ����ÿ�ʼս���ı�־
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual void StartFight( CGameObject* pObject );
	/*--------------------------------------------------------
	  ���ܣ����ý���ս���ı�־
	  ������
	  ���أ�
	----------------------------------------------------------*/
	virtual void EndFight();
	/*--------------------------------------------------------
	  ���ܣ�����ս��״̬
	  ������
	  ���أ�
	----------------------------------------------------------*/
	void SetFight( bool value ) { m_bIsFight = value; }
	/*--------------------------------------------------------
	  ���ܣ��õ�ս��״̬
	  ������
	  ���أ�
	----------------------------------------------------------*/
	bool GetFight( void ) { return m_bIsFight; }
	/*--------------------------------------------------------
	  ���ܣ��뿪ս��
	  ������
	  ���أ�
	----------------------------------------------------------*/
	void LeaveFight( void ) {};
	/*--------------------------------------------------------
	  ���ܣ��Ƿ�ɽ��й�����ȷ���������
	  ������
	  ���أ�
	----------------------------------------------------------*/
	bool IsAttack( void );
	/*--------------------------------------------------------
	  ���ܣ����ö��󼤻�״̬
	  ������
	  ���أ�
	----------------------------------------------------------*/
	long SetActive( bool active );
	/*--------------------------------------------------------
	  ���ܣ���ö�����Ϸ״̬
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	eObjectType	GetType( void ) { return m_eType; }
	/*--------------------------------------------------------
	  ���ܣ�������·ͬ������
	  ������
	  ���أ�
	----------------------------------------------------------*/
	void ClearPath( void );
	/*--------------------------------------------------------
	  ���ܣ��õ��ű�����
	  ������
	  ���أ�
	----------------------------------------------------------*/
	void GetScriptObject(ScriptObject* obj);
	/*--------------------------------------------------------
	  ���ܣ����ýű�����
	  ������
	  ���أ�
	----------------------------------------------------------*/
	void SetScriptObject(ScriptObject* obj);
	/*--------------------------------------------------------
	  ���ܣ����װ������������
	  ������
	  ���أ�
	----------------------------------------------------------*/
	virtual HandType GetHandType();
	 /*--------------------------------------------------------
	   ���ܣ���ɫ����
	   ������
	   ���أ�
	 ----------------------------------------------------------*/
	virtual void Dead( CGameObject  *pObj );
	/*--------------------------------------------------------
	  ���ܣ���ɫ�Ƿ�����
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual bool IsDead();

	/*--------------------------------------------------------
	  ���ܣ��жϽ�ɫ�Ƿ�Ϊ���������ӵ��
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual bool IsOwner(const char* )
	{
		return false;
	}

	/*--------------------------------------------------------
	  ���ܣ��õ�װ������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	EquipData* GetEquipData()
	{
		return &m_EquipData;
	}
	/*--------------------------------------------------------
	���ܣ��õ���̳����
	������
	���أ�
	���ߣ�����
	��ע��
	���ڣ�2008-4-8
	----------------------------------------------------------*/
	AltarData* GetEquiAltarpData()
	{
		return &m_AltarData;
	}
    /*--------------------------------------------------------
      ���ܣ��õ�״̬����
      ������
      ���أ�
      ���ߣ�����
      ��ע��
      ���ڣ�2008-4-8
    ----------------------------------------------------------*/
	StatusData* GetStatusData()
	{
		return &m_StatusData;
	}

	/*--------------------------------------------------------
	  ���ܣ��жϹ�������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע����Ҫָħ������ʽ��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	ATTACK_TYPE JudgeAttackStyle( CGameObject* pTarget );
	/*--------------------------------------------------------
	  ���ܣ��ж�����,����
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע����Ҫָ��ͨ������
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	ATTACK_TYPE JudgeAppendStatusStyle(  CGameObject* pTarget, const MagicData* pMagicData );
	/*--------------------------------------------------------
	  ���ܣ���ʼ����������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual bool InitObjData(long  ID) = 0 ;
	/*--------------------------------------------------------
	  ���ܣ�������������Ϣͬ�����Լ�
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual void AynObjToObj( CCell *pOldCell, CCell *pNewCell );
	/*--------------------------------------------------------
	  ���ܣ����Լ�����Ϣͬ�����������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual void AynMeToOther( CCell *pOldCell, CCell *pNewCell );
	/*--------------------------------------------------------
	  ���ܣ���������ʱ������������������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual bool CalculatePassivePracticeDegree(  PhysiacalAttack Type, ATTACK_TYPE eAttackType, bool IsDead = false ) = 0;
	/*--------------------------------------------------------
	  ���ܣ�ħ������ʱ������������������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	
	virtual void CalculatePassivePracticeDegree( int MagicID, ATTACK_TYPE eAttackType, bool IsDead = false ) = 0;
	/*--------------------------------------------------------
	  ���ܣ�ħ������ʱ������������������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	
	virtual void BePhysicalAttackedPassivePracticeDegree( PhysiacalAttack , ATTACK_TYPE  ){};
	/*--------------------------------------------------------
	  ���ܣ����㱻ħ������ʱ��ļ���������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	
	virtual void BeMagicAttackPassivePracticeDegree( UINT ,  ATTACK_TYPE ){};
	/*--------------------------------------------------------
	  ���ܣ����㾭��ֵ
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	
	virtual bool CalculateExp( bool TarIsDead , int exp = 1 );
	/*--------------------------------------------------------
	  ���ܣ��ı似��������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	
	virtual bool ChanegeDegree( int ucSkillID, int Buffer)= 0;
	/*--------------------------------------------------------
	  ���ܣ��жϼ�������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual bool PassiveSkillUpgrade(UINT  SkillID, int *Rank = NULL ) =0 ;
	/*--------------------------------------------------------
	  ���ܣ���ӱ������ܵȼ����߸ı����м��ܵǼ�
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual void AddPassiveSkill( int ucSkillID, int iRank = 1, int EXP = 0 ) = 0;
	/*--------------------------------------------------------
	  ���ܣ�ħ���Ƿ�����ȴ״̬
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual	bool IsActiveSkillCooling( int ucSkillID );
	/*--------------------------------------------------------
	  ���ܣ����������ʼ�ۼƵ���Ϣ
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual	bool IsGodSkillTimeOut( int MagicID );
	/*--------------------------------------------------------
	  ���ܣ����ü��ܿ�ʼʱ��
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual	bool SetActiveSkillStartTime( int ucSkillID);
	/*--------------------------------------------------------
	  ���ܣ����õ�ǰ����ID
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual	void SetCurActiveSkillID( int ucActiveSkillID);
	/*--------------------------------------------------------
	  ���ܣ����ü���ʱ��
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual	void ResetTimer( float  IntervalTime ,TIME_STYLE timeStyle);
	/*--------------------------------------------------------
	  ���ܣ��ı���������״̬
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual	void ChangeActiveSkillStatus( int ucSkillID, TIME_STYLE status );
	/*--------------------------------------------------------
	  ���ܣ��жϼ���ˢ��ʱ��
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual	bool IsRefresh( TIME_STYLE timeStyle );
	/*--------------------------------------------------------
	  ���ܣ������������˺�
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual INT CalculateAttackDamage( CGameObject& pObj, float iPower1 = 0, int iPower2 = 0 ) ;
	/*--------------------------------------------------------
	  ���ܣ����㾭��͵ȼ�
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual void CalculateExpAndLevel( CGameObject& pTarObj );
	/*--------------------------------------------------------
	  ���ܣ�����ħ�������˺�
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual INT	CalculateAttackDamage( int MagicID , CGameObject &pObj) ;
	/*--------------------------------------------------------
	  ���ܣ���õȼ�
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual UINT  GetRank()=0;
	/*--------------------------------------------------------
	  ���ܣ�ɾ������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual bool DelPassiveSkill( int ucSkillID );
	/*--------------------------------------------------------
	  ���ܣ����ұ�������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual bool FindPassiveSkill( int ucSkillID,UINT *Degree = NULL  );
	/*--------------------------------------------------------
	  ���ܣ�������������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual bool FindActiveSkill( int ucSkillID ) ;
	/*--------------------------------------------------------
	  ���ܣ������������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual bool AddActiveSkill( int ucSkillID ) ;
	/*--------------------------------------------------------
	  ���ܣ�ɾ����������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual void DelActiveSkill( int ucSkillID ) ;
	/*--------------------------------------------------------
	  ���ܣ�ɾ��״̬
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	bool DelStatus( CStatus *pStatus );
	/*--------------------------------------------------------
	  ���ܣ����״̬
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	bool AddStatus( CStatus *pStatus );
	/*--------------------------------------------------------
	  ���ܣ��ж��ܷ�ɹ����״̬
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע������״̬�����ͺ����޼�
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	bool CanAddStatus( CStatus *pStatus );
	/*--------------------------------------------------------
	  ���ܣ�״̬�Ĵ���
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	void ResolveStatus( );
	/*--------------------------------------------------------
	  ���ܣ��õ��̶�״̬
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	size_t GetStatus( UINT StatusArr[] );
	/*--------------------------------------------------------
	  ���ܣ��Ƿ�װ���ؼ�
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual bool IsHeavyLoricae()  { return false; };
	/*--------------------------------------------------------
	  ���ܣ��Ƿ�װ������
	  ������
	  ���أ�
	----------------------------------------------------------*/
	virtual bool IsFabircLoricae() { return false; };
	/*--------------------------------------------------------
	  ���ܣ���õ�ǰ���ܵ�ID
	  ������
	  ���أ�
	----------------------------------------------------------*/
	virtual	int GetCurActiveSkillID();
	/*--------------------------------------------------------
	  ���ܣ����ӳ�ޣ���������������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual	void AddEnmity( CGameObject* pObject, long lValue );	
	/*--------------------------------------------------------
	  ���ܣ����ٳ��
	  ������
	  ���أ�
	----------------------------------------------------------*/
	virtual	void DecEnmity( CGameObject* pObject, long lValue );		
	/*--------------------------------------------------------
	  ���ܣ�������
	  ������
	  ���أ�
	----------------------------------------------------------*/
	virtual	void RemoveEnmity( CGameObject* pObject );	
	/*--------------------------------------------------------
	  ���ܣ���ճ��
	  ������
	  ���أ�
	----------------------------------------------------------*/
	virtual	void ClearEnmity();
	virtual	void ChainEnmityList(CGameObject* obj , int value);
	virtual void CoagentEnmityList(CGameObject* obj , int value);
	virtual bool IsClearEnmity();
	/*--------------------------------------------------------
	  ���ܣ�����������
	  ������
	  ���أ�
	----------------------------------------------------------*/
	virtual	CGameObject* GetMaxEnmity();
	/*--------------------------------------------------------
	  ���ܣ���þ���
	  ������
	  ���أ�
	----------------------------------------------------------*/
	virtual ULONG GetEXP(){ return 0;};
	/*--------------------------------------------------------
	  ���ܣ�����HP
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	void			AddHP( const int hp );
	/*--------------------------------------------------------
	  ���ܣ�����MP
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	void			AddMP( const int mp );
	/*--------------------------------------------------------
	  ���ܣ��������
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	//virtual ULONG GetHit() = 0;
	/*--------------------------------------------------------
	  ���ܣ���ö��
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual ULONG GetDodge() = 0;
	/*--------------------------------------------------------
	  ���ܣ���ù�����
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual UINT  CalculateAttack();
	/*--------------------------------------------------------
	  ���ܣ���÷�����
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	virtual UINT  CalculateDefend() const ;
	/*--------------------------------------------------------
	  ���ܣ�������HP
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	int	GetMaxHP( void ) const;
	/*--------------------------------------------------------
	  ���ܣ�������MP
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	int	GetMaxMP( void ) const;
    /*--------------------------------------------------------
      ���ܣ���ü��ܵ�
      ������
      ���أ�
      ���ߣ�����
      ��ע��
      ���ڣ�2008-4-8
    ----------------------------------------------------------*/
	UINT GetSkillPoint(){ return m_ConvertPoint; };
	/*--------------------------------------------------------
	  ���ܣ���ü��ܾ���
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	UINT GetSkillExp(){ return m_WinExp; };

	/*--------------------------------------------------------
	  ���ܣ����ü��ܵ�
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	void SetSkillPoint( UINT uiPoint ){  m_ConvertPoint = uiPoint ; };
	/*--------------------------------------------------------
	  ���ܣ����ü��ܾ���
	  ������
	  ���أ�
	  ���ߣ�����
	  ��ע��
	  ���ڣ�2008-4-8
	----------------------------------------------------------*/
	void SetSkillExp( UINT uiExp ){  m_WinExp = uiExp; };
	/*--------------------------------------------------------
	  ���ܣ��ͻ���UI����ͬ��
	  ������
	  ���أ�
	----------------------------------------------------------*/
	virtual void  SynGameData( bool ){};
    /*--------------------------------------------------------
      ���ܣ�Ⱥ�˻���˺���Ϣ
      ������
      ���أ�
    ----------------------------------------------------------*/
	DAMAGE_INFO		GetDamageInfo( void ) const;
	/*--------------------------------------------------------
	  ���ܣ�����Ⱥ���˺���Ϣ
	  ������
	  ���أ�
	----------------------------------------------------------*/
	void	SetDamageInfo( const DAMAGE_INFO dmgInfo );
	/*--------------------------------------------------------
	  ���ܣ�����˺���Ϣ
	  ��������
	  ���أ�
	----------------------------------------------------------*/
	void	ClearDamageInfo( void );
	/*--------------------------------------------------------
	���ܣ�������вֵ
	��������
	���أ�
	----------------------------------------------------------*/
	void AddDanger( int iDander = 1 ) { m_lDanger += iDander; }
	/*--------------------------------------------------------
	���ܣ������вֵ
	��������
	���أ�
	----------------------------------------------------------*/
	long GetDanger( void ) { return m_lDanger; }
	/*--------------------------------------------------------
	���ܣ���ö���״̬ 
	��������
	���أ�
	----------------------------------------------------------*/
	eObjectState GetState( void ) { return m_eState; }
	/*--------------------------------------------------------
	���ܣ���ö���״̬ 
	��������
	���أ�
	----------------------------------------------------------*/
	void SetState( eObjectState  eState ) {  m_eState = eState ; }
    /*--------------------------------------------------------
      ���ܣ���ö����������
      ������
      ���أ�
      ���ߣ�����
      ��ע��
      ���ڣ�2008-4-8
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
	���ܣ�״̬Ч��������
	������
	���أ�
	���ߣ�����
	��ע��
	���ڣ�2008-4-8
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
	void Handler_ChangeIntonateTime( float value, bool isresume, int StatusID ); //��ʱ����   
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
	���ܣ�����Ч��������
	������
	���أ�
	���ߣ�����
	��ע��
	���ڣ�2008-4-8
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
	���ܣ������Զ���Ѫʱ��
	������
	���أ�
	���ߣ�����
	��ע��
	���ڣ�2008-4-8
	----------------------------------------------------------*/
   void ResetResumeTimer(){ m_Resume_Timer.TimeOver(); m_Resume_Timer.Startup( RESUME_TIME_INTERVAL ) ;};
    void ResetRelationTimer(){ m_Relation_Timer.TimeOver(); m_Relation_Timer.Startup( RELATION_TIME_INTERVAL ) ;};

private:

		/*-------------------------------------------*/
		/*               ��Ա������               */
		/*-------------------------------------------*/

public:

	EnmityList         	m_listEnmity;	    // ����б�
	AttackRadius       	m_AttackRadius;
	long               	m_lID;
	volatile  bool      m_bActive;
	float              	m_fOffsetX, m_fOffsetY, m_fOffsetZ;
	float              	m_fDestinationX, m_fDestinationY, m_fDestinationZ;
	float              	m_fDist;
	float              	m_fDest;				    // ����
	float              	m_fatan2;
	float              	m_fX, m_fY, m_fZ;		    // ����
	bool               	m_bIsFight;				    // ս��״̬���
	eObjectType        	m_eType;			        // ����
	eObjectState       	m_eState;			        // ����״̬
	sbase::CTimer      	m_timeFight;		        // ս������
	sbase::CTimerMS	   	m_timeAttack;	            // ս������Ƶ��
	CGameObject*       	m_targetObj;                // Ŀ�����
	CGameObject*       	m_skillTargetObj;           // ����Ŀ�����
	list<tarPath*>     	m_queuePath;
	list<tarWalk*>		m_queueWalk;
	short int          	m_nAvatar[MAX_AVATAR];
	CBuff*             	m_pBuff;				    // ��������
	CBuff*             	m_pDeBuff;				    // ��������
	CRegion*           	m_pRegion;				    // ���ڳ���

	

private:
	DAMAGE_INFO	        m_DamageInfo;
protected:
	
	ObjectData	                m_ObjectData;		//�Լ���ɫ����
	BaseData                    m_BaseData;         //��ɫ��������
	SkillData                   m_Specialty;        //ר������
	EquipData                	m_EquipData;        //װ������
    StatusData                  m_StatusData;       //״̬����
	AltarData                   m_AltarData;        //��̳����


	map< UINT , PASSIVEINFO >   m_PassiveSkill;     //���������б�$$$
	map< UINT , ACTIVEINFO >    m_ActiveSkill;      //���������б�$$$
	map< UINT , UINT >          m_Altar;      //��̳����
	map< UINT, TTimeElement<CStatus*,time_t> > m_StatusMap;    //״̬
	map< UINT, StatusData > m_StatusSelfData; 

	short                       m_ucCurActiveSkill;                    //��ǰʹ��ħ��
	UINT                        m_uiSkillEffect;
	BYTE                        m_lRefreshTime;	 
	BYTE                        m_ucActiveSkillCastTime;    //ħ������ʱ��
	USHORT                      m_usActiveSkillCoolTime;    //ħ����ȴʱ��
	sbase::CTimerMS             m_ActiveSkillCast_Timer;    //ħ��������ʱ��,MillSec
	sbase::CTimer               m_ActiveSkillCool_Timer;    //ħ����ȴ��ʱ��
	sbase::CTimer               m_Resume_Timer;             //HP,MP�ָ���ʱ��
	sbase::CTimer               m_Relation_Timer;           //��ϵˢ�¼�ʱ��
	sbase::CTimer               m_Team_Timer;				//������Ϣ���¼�ʱ��
	time_t                      m_GodLastTime;
	CCell*                      m_pCurrentCell;
	UINT                        m_WinExp;                   //��ǰ��õľ���(�������ܾ���)
	UINT                        m_ConvertPoint;             //ת����ĵ���(�������ܵ���)
	DWORD						dTimeStart;					//����CD����ʱ���

private:
	/*----------------------------------------------------------
	  ���ܣ������вֵ�����ڷ���ң�
	  ���ߣ�
	  ���ڣ�
	----------------------------------------------------------*/
	long m_lDanger;
public:

	bool IsPublicCDOK()	{ return( timeGetTime() - dTimeStart >= 1000 ); }	//����CD
	void StartPublicCD(){ dTimeStart = timeGetTime(); }						//����CD����ʱ���
	
public:

	static CWorld*	s_World ;


};




#endif
