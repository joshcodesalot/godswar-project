//========================================================
//
//    Copyright (c) 2007,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� SkillManager.h
//    ժ    Ҫ �� ���ܹ���������
//    
//    ��ǰ�汾 �� 1.00
//    ��    �� �� ����
//    ������� �� 2007-03-22
//
//========================================================
#ifndef _FILE_SKILL_H
#define _FILE_SKILL_H
#pragma  once


#include "GameData.h"
//#include "..\stdafx.h"
#include "Player.h"
#include "BaseManager.h"


//class CPlayer;
enum TYPE { TYPE_FLOAT, TYPE_BOOL , }; 
enum AFFECT { DECHP, DECMP, DECSPEED,BACKPEDAL,ISPERFORMMAGIC, ISACTION, DECDEFEND,ISWALK,ISSTONE,ISAUTOACTTCK,ISSLEEP,
DECDAMAGE,REBOUNDDAMAGE,ACCEPTHP,ACCEPTMP,DECCOOLTIME, };

struct STATUS_DATA
{
	char cName[32];
	UINT istatus;
};

class  IManager
{
public:
	inline float  GetValue( bool flag ,sbase::CIni &IniFile,const char* Item, const char * Attribute)
	{
		if ( flag )
		{
			char temp[32];
			memset( temp, 0L, sizeof temp);
			sprintf(temp,"%s_modulus",Attribute);
			return IniFile.GetFloatData( Item,temp );

		}
		else 
			return 0.0f;
	}
	char *CatStr(char * source, int rank);     //�ַ�������
	bool Init() { memset(m_Convert,0L,sizeof(m_Convert)); return true; };
protected:
	char   m_Convert[32];                       //�ַ�����ʽ��
	virtual ~IManager();
private:
};


struct SkillHandle
{
	void  (CGameObject::*Handler)( SKillData *Skill, AttributeType Type );
};

class CSkillManager : public IManager,public CBaseManager
{
	typedef std::map<BYTE, SKillData*>              SKILL_LIST;
	typedef std::map<UINT, MagicData>                MAGIC_LIST;
public:
	CSkillManager();
	CSkillManager(CWorld* pWorld);
	~CSkillManager();
	void Release();
	bool Init();                            //�������ļ�����skill��Magic����
	
	const SKillData* GetSkill(int ucID, int ucRank)const;
	const SKillData* GetSkill(int ucID)const;
	const MagicData* GetMagic(int ucID) const;
	SkillHandle GetSkillEffectHandler( UINT EffectID );
protected:

private:
	int m_ucSkillKindsCount;                //��������
	int m_ucSkillRankCount;                 //���ܵȼ�
	int	m_ucMagicCount;                     //ħ������
	int m_ucPhysicalCount;                  //��ʽ����
	int m_ucGodsSkill;                      //�������
	SKILL_LIST m_SkillAll;                  //���������б�
	MAGIC_LIST m_MagicList;                 //���������б�
private:
	bool LoadSkillConfig();                 //���ر�����������
	bool LoadMagicConfig();                 //����������������

};   

//״̬������Ч��
class  CElementEffect
{
public:
	CElementEffect( BYTE ID , const char* Name );
	~CElementEffect();
	 void (CGameObject::*Handler)( float value, bool isresume,int StatusID );
	 std::string GetString(){return m_Name; };
	 BYTE GetID(){return m_ID;};
protected:
private:
	BYTE m_ID;		
	std::string m_Name; 
};

//״̬
class CStatus
{
	typedef  std::map<CElementEffect*,float>  EFFECT_MAP;
	typedef  std::map<CElementEffect*,time_t> EFFECY_TIME_MAP;
public:
	CStatus( int ID, time_t PersistTime, UINT Style, UINT m_Kind, UINT m_Priority );
	~CStatus();
	void AddEffect( CElementEffect *pEffect, float value, time_t PersistTime );
	time_t GetIntervalTime( CElementEffect *pEffect ){ return m_EffectTimeMap[pEffect]; };
	void DealWith( CGameObject *pObj, const time_t& StartTime, map<int,int> &PassTime, bool IsRepeated );
	void Resume( CGameObject *pObj );
	int GetID()const{ return m_ID;};
	int GetEffectCount(){return (int)m_EffectMap.size();}
	UINT GetStyle(){ return m_Style; };
	UINT GetKind(){ return m_Kind; };
	UINT GetPri(){ return m_Priority;};
	time_t GetPersistTime(){ return m_PersistTime;};
	inline bool operator == (const CStatus &arg ) const
	{
		return m_ID == arg.GetID();

	}	
protected:
private: 
	int m_ID; 
	UINT m_Style;
	UINT m_Kind;
	UINT m_Priority;
	time_t m_PersistTime;
	EFFECT_MAP m_EffectMap;
	EFFECY_TIME_MAP m_EffectTimeMap;

};

class  CStatusManager : public IManager,public CBaseManager
{
public:
	CStatusManager();
	CStatusManager(CWorld* pWorld);
	~CStatusManager();
	bool Init();
	void Release();
	CStatus* GetStatus( UINT iStausID )
	{
		std::map<UINT,CStatus*>::iterator itor = m_Status.find( iStausID );
		if ( itor != m_Status.end())
		{
			return itor->second;
		}

		return NULL;
	}
protected:
private:
	int m_ucStatusCount;                     //״̬����
	std::map<UINT,CStatus*> m_Status;
	CElementEffect *m_Effect[MAX_STATUS_EFFECT];
	bool LoadStatusConfig();                 //����״̬����
};


// ------------------------------------------------------------------------------------------------
// �ַ�������
// ------------------------------------------------------------------------------------------------
inline char * IManager::CatStr(char * source,int rank)
{
	memset(m_Convert,0L,sizeof(m_Convert));
	sprintf(m_Convert ,"%s%d",source,rank);
	return m_Convert;
}


#endif


