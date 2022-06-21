//========================================================
//
//    Copyright (c) 2006,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� Monster.cpp
//    ժ    Ҫ �� ���﹦��ģ��
//    
//
//    ��    �� �� ������������˹��＼��
//    ������� �� 2008-03-26
//
//========================================================
#pragma once

#include "GameObject.h"
#include "Item.h"
#include "./DropItemMgr.h"

// ������/������ ���
#define AI_TYPE_INITIATIVE			1
#define AI_TYPE_PASSIVITY			2
#define AI_MONSTER_WALKTIME			500				// ����ʱ������
#define MAX_OWNER 5
#define MAX_SKILLS 4

enum {
	MONSTER_STATE_LIVE = 0,					// ���
	MONSTER_STATE_DEAD,						// ����
	MONSTER_STATE_LOST,						// ��ʧ
	MONSTER_STATE_LEAVEFIGHT,				// ����ս��

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
	int		SkillID;	//����ID
	int		SkillPro;	//����ʩ�Ż���
	float	SkillCoe;	//�����˺�ϵ��
	int		Attack;		//
	DWORD	TimePos;	//�����ͷſ�ʼʱ��
};

class CMonster : public CGameObject
{

public:
	long				m_lState;						// �������Ϊ״̬
	long				m_lActivity;					// ���ܻ״̬
	long				m_lMode;						// ����ģ��
	float				m_fIniX, m_fIniY, m_fIniZ;		// ���ñ�������ʼ����
	long				m_lRefreshTime;					// ˢ��ʱ��
	int					m_Level;						// �ȼ�
	int					m_MonsterEquiplv;				// ����ȼ�
	int					m_Quality;						// ����Ʒ��
	float				m_fActivityRange;				// ���Χ
	float				m_fEyeshot;						// ������������Ұ
	float               m_fAngle;                       // ��ʼ�Ƕ�

public:
	CMonster(void);
	~CMonster(void);

public:

	bool				IsLeaveFight(){return m_lState == MONSTER_STATE_LEAVEFIGHT;}

	UINT                CalculateAttack() const;
	UINT                CalculateDefend() const;
	void				Activate( void );
	void				AI( void );												// �����߼�
	void				Run( void );											// �߼�����

	long				GetState( void ) { return m_lState; }
	void				SetMode( long lMode ) { m_lMode = lMode; }
	long				GetMode( void ) { return m_lMode; }

	void				SetRefreshTime( long value ) { m_lRefreshTime = value; }
	void				Refresh( void );										// ˢ�����Բ��㲥
	void				ResetTimer( void );										// ��λˢ��ʱ��
	bool				IsRefresh( void );										// �����Ƿ�ˢ��

	void				ResetCorpseTimer( void );								// ��λʬ��״̬ʱ��
	bool				IsCorpseTimer( void );									// �Ƿ���ʬ��ʱ��

	void				LeaveFight( void );										// ����ս��״̬��ˢ������

	bool				WalkTo( float x, float y, float z );					// �ƶ�
	bool				FollowTo( float x, float y, float z , BYTE distance = 2);					// ׷��
	bool				StopWalk( void );										// ֹͣ�ƶ�

	void				SetDropConfig(const DropConfig& drop);
	DropConfig&			GetDropConfig();

	Item*				GetFreeItem();

	float				GetDistWithTarget();									// ��ȡ�Լ���Ŀ��ľ���
	float				GetDist( float x, float z );							// ��ȡ�Լ�������ľ���

	CGameObject*		GetNearestPartner();

	virtual	void		AddEnmity( CGameObject* pObject, long lValue );
	virtual void		RemoveEnmity(CGameObject* pObject);

	virtual bool		IsOwner(const char* name);

	bool				IsSkillCooling( int index );
	void				Cooldown( int index );
	/*----------------------------------------------------------
	���ܣ������ͷż���
	���ߣ�������
	���ڣ�2008-3-26
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
	���ܣ����õ��䣬�����Ƚ����Լ������õ��䣬�ٽ������Ե���
	���ߣ�����
	���ڣ�2008-3-12
	----------------------------------------------------------*/
	ConfirmDrop         m_ConfirmConfig;              //���õ���
	/*----------------------------------------------------------
	���ܣ��������õ����(һֻ�ֶ�Ӧһ�ű�)
	���ߣ�����
	���ڣ�2008-3-12
	----------------------------------------------------------*/
	CDropItemMgr        m_DropItemMgr;                   

	int					m_Drops;
	Item				m_DropItems[MAX_DROPS];

	int					m_WalkTime;

	long				m_lAIType;
	long				m_Kidney;						//�Ը�,

	bool				m_Escaped;						//���ܹ�
	bool				m_Escaping;						//������

	sbase::CTimer		m_Timer;						// ˢ��
	sbase::CTimer		m_CorpseTimer;					// ʬ����ʧ
	sbase::CTimer		m_timerAI;						// ����AI

	unsigned int		m_nRunTick;
	int					m_nUnTick;
	float				m_fDestinationX, m_fDestinationY, m_fDestinationZ;	                    // Ŀ�ĵ�����

	bool				DoSomething();
	void                SetRandMagicAttack( UINT MagicAttack );

	//���㹥����
	virtual             UINT                CalculateAttack();
	//��������ʱ������������������
	bool CalculatePassivePracticeDegree( PhysiacalAttack Type, ATTACK_TYPE eAttackType, bool IsDead = false );
	//ħ������ʱ������������������
	void CalculatePassivePracticeDegree(  int MagicID, ATTACK_TYPE eAttackType, bool IsDead = false );
	//�ı似��������
	virtual             bool                ChanegeDegree( int ucSkillID, int Buffer);
	//�жϼ�������
	virtual             bool                PassiveSkillUpgrade(UINT  SkillID, int *Rank = NULL ) ;
	//��ӱ������ܵȼ����߸ı����м��ܵǼ�
	virtual             void                AddPassiveSkill( int ucSkillID, int iRank = 1, int EXP = 0) ;
	//�����������˺�
	virtual             INT                CalculateAttackDamage(  CGameObject& obj , float iPower = 0) ;
	//����ħ�������˺�
	virtual             INT                CalculateAttackDamage( int MagicID ) ;
	//������������Ϣͬ�����Լ�
	virtual void		AynObjToObj( CCell *pOldCell, CCell *pNewCell );
	//���Լ�����Ϣͬ�����������
	virtual void		AynMeToOther( CCell *pOldCell, CCell *pNewCell );

	ULONG				GetEXP();
	virtual UINT		GetRank(){ return m_Level; };
	int					GetMoney();

	//����
	ULONG CMonster::	GetHit();
	//���
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
