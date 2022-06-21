//========================================================
//
//    Copyright (c) 2006,欢乐连线工作室
//    All rights reserved.
//
//    文件名称 ： Monster.cpp
//    摘    要 ： 怪物功能模块
//    
//
//    修    改 ： 张文欣，添加了怪物技能
//    完成日期 ： 2008-03-26
//
//========================================================
#pragma once

#include "GameObject.h"
#include "Item.h"
#include "./DropItemMgr.h"

// 主动怪/被动怪 标记
#define AI_TYPE_INITIATIVE			1
#define AI_TYPE_PASSIVITY			2
#define AI_MONSTER_WALKTIME			500				// 怪物活动时间周期
#define MAX_OWNER 5
#define MAX_SKILLS 4

enum {
	MONSTER_STATE_LIVE = 0,					// 存活
	MONSTER_STATE_DEAD,						// 死亡
	MONSTER_STATE_LOST,						// 消失
	MONSTER_STATE_LEAVEFIGHT,				// 脱离战斗

	MONSTER_ACTIVITY_IDLE,
	MONSTER_ACTIVITY_WALK,
	MONSTER_ACTIVITY_ESCAPE,
};

enum {
	MONSTER_MODE_PASSIVITY = 1,
	MONSTER_MODE_INITIATIVE,
};

enum {
	MONSTER_MODE_CRAVEN = 0,
	MONSTER_MODE_VALOROUS,
	MONSTER_MODE_DEEP,

};

typedef struct  
{
	float           Exp;
	float           HP;
	float           HPResume;
	float           PhyDefend;
	float           MagicDefend;
	float           Hit;
	float           Miss;
	float           FrenzyHit;
	float           FrenzyMiss;
    float           PhyDamage;
	float           MagicDamage;
	float           DamageSorb;
	float           StatusHit;
	float           statusMiss;
} PARAM ;

struct SkillInfo 
{
	int		SkillID;	//技能ID
	int		SkillPro;	//技能施放机力
	float	SkillCoe;	//技能伤害系数
	int		Attack;		//
	DWORD	TimePos;	//技能释放开始时间
};

class CMonster : public CGameObject
{

public:
	long				m_lState;						// 怪物的行为状态
	long				m_lActivity;					// 智能活动状态
	long				m_lMode;						// 智能模型
	float				m_fIniX, m_fIniY, m_fIniZ;		// 配置表，怪物起始坐标
	long				m_lRefreshTime;					// 刷新时间
	int					m_Level;						// 等级
	int					m_MonsterEquiplv;				// 掉落等级
	int					m_Quality;						// 掉落品质
	float				m_fActivityRange;				// 活动范围
	float				m_fEyeshot;						// 主动攻击的视野
	float               m_fAngle;                       // 初始角度

public:
	CMonster(void);
	~CMonster(void);

public:

	bool				IsLeaveFight(){return m_lState == MONSTER_STATE_LEAVEFIGHT;}

	UINT                CalculateAttack() const;
	UINT                CalculateDefend() const;
	void				Activate( void );
	void				AI( void );												// 智能逻辑
	void				Run( void );											// 逻辑工作

	long				GetState( void ) { return m_lState; }
	void				SetMode( long lMode ) { m_lMode = lMode; }
	long				GetMode( void ) { return m_lMode; }

	void				SetRefreshTime( long value ) { m_lRefreshTime = value; }
	void				Refresh( void );										// 刷新属性并广播
	void				ResetTimer( void );										// 复位刷新时间
	bool				IsRefresh( void );										// 返回是否刷新

	void				ResetCorpseTimer( void );								// 复位尸体状态时间
	bool				IsCorpseTimer( void );									// 是否还在尸体时间

	void				LeaveFight( void );										// 脱离战斗状态，刷新属性

	bool				WalkTo( float x, float y, float z );					// 移动
	bool				FollowTo( float x, float y, float z , BYTE distance = 2);					// 追踪
	bool				StopWalk( void );										// 停止移动

	void				SetDropConfig(const DropConfig& drop);
	DropConfig&			GetDropConfig();

	Item*				GetFreeItem();

	float				GetDistWithTarget();									// 获取自己与目标的距离
	float				GetDist( float x, float z );							// 获取自己与参数的距离

	CGameObject*		GetNearestPartner();

	virtual	void		AddEnmity( CGameObject* pObject, long lValue );
	virtual void		RemoveEnmity(CGameObject* pObject);

	virtual bool		IsOwner(const char* name);

	bool				IsSkillCooling( int index );
	void				Cooldown( int index );
	/*----------------------------------------------------------
	功能：怪物释放技能
	作者：张文欣
	日期：2008-3-26
	----------------------------------------------------------*/
	void				UseSkill( int skillID );

	void				Dead( CGameObject  *pObj );

	void				SetFightPosition();

    void                SynGameData( bool );
	virtual void		EndFight();

public:

	int					m_QuestItem[MAX_DROPS];
	int					m_QuestID;

	ItemData			m_DropItem[MAX_DROPS];

	SkillInfo			m_SkillInfos[MAX_SKILLS];

	CGameObject*		m_ItemOwner[MAX_OWNER];
	string				m_ItemOwnerName[MAX_OWNER];

	DropConfig			m_DropConfig;
	/*----------------------------------------------------------
	功能：配置掉落，怪物先进行自己的配置掉落，再进行相性掉落
	作者：李锋军
	日期：2008-3-12
	----------------------------------------------------------*/
	ConfirmDrop         m_ConfirmConfig;              //配置掉落
	/*----------------------------------------------------------
	功能：怪物配置掉落表(一只怪对应一张表)
	作者：李锋军
	日期：2008-3-12
	----------------------------------------------------------*/
	CDropItemMgr        m_DropItemMgr;                   

	int					m_Drops;
	Item				m_DropItems[MAX_DROPS];

	int					m_WalkTime;

	long				m_lAIType;
	long				m_Kidney;						//性格,

	bool				m_Escaped;						//逃跑过
	bool				m_Escaping;						//逃跑中

	sbase::CTimer		m_Timer;						// 刷新
	sbase::CTimer		m_CorpseTimer;					// 尸体消失
	sbase::CTimer		m_timerAI;						// 智能AI

	unsigned int		m_nRunTick;
	int					m_nUnTick;
	float				m_fDestinationX, m_fDestinationY, m_fDestinationZ;	                    // 目的地坐标

	bool				DoSomething();
	void                SetRandMagicAttack( UINT MagicAttack );

	//计算攻击力
	virtual             UINT                CalculateAttack();
	//物理攻击的时候计算各个技能熟练度
	bool CalculatePassivePracticeDegree( PhysiacalAttack Type, ATTACK_TYPE eAttackType, bool IsDead = false );
	//魔法攻击时候计算各个技能熟练度
	void CalculatePassivePracticeDegree(  int MagicID, ATTACK_TYPE eAttackType, bool IsDead = false );
	//改变技能熟练度
	virtual             bool                ChanegeDegree( int ucSkillID, int Buffer);
	//判断技能升级
	virtual             bool                PassiveSkillUpgrade(UINT  SkillID, int *Rank = NULL ) ;
	//添加被动技能等级或者改变已有技能登记
	virtual             void                AddPassiveSkill( int ucSkillID, int iRank = 1, int EXP = 0) ;
	//计算物理攻击伤害
	virtual             INT                CalculateAttackDamage(  CGameObject& obj , float iPower = 0) ;
	//计算魔法攻击伤害
	virtual             INT                CalculateAttackDamage( int MagicID ) ;
	//将其他对象信息同步给自己
	virtual void		AynObjToObj( CCell *pOldCell, CCell *pNewCell );
	//将自己的信息同步给其他玩家
	virtual void		AynMeToOther( CCell *pOldCell, CCell *pNewCell );

	ULONG				GetEXP();
	virtual UINT		GetRank(){ return m_Level; };
	int					GetMoney();

	//命中
	ULONG CMonster::	GetHit();
	//躲避
	ULONG CMonster::	GetDodge();
	static PARAM				Param;
	static              ATTACK_PARAM        AttackParam;

	bool                InitObjData(long ID);


private :


	int					RandomSkill();

	int					m_SkillCount;
	int					m_SkillRandom;

	float				m_fFightX;
	float				m_fFightZ;
};
