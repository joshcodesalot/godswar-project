#ifndef   _MAT_FILE_HEAD
#define  _MAT_FILE_HEAD

#ifdef _WIN32
#pragma once
#endif

#pragma once
#include "../../Common/Include/Base/Inc\mempool.h"
#include <mmsystem.h>
#pragma  comment(lib, "winmm.lib")


enum{
	PLAYER_PHY_ATTACK,
	PLAYER_PHY_DEFEND,
	PLAYER_MAGIC_ATTACK,
	PLAYER_MAGIC_DEFEND,
	PLAYER_HIT,
	PLAYER_MISS,
	PLAYER_RESUME_TIME,
	PLAYER_SKILL_EXP,
	HIT_ARG_A,
	HIT_ARG_B,
	HIT_ARG_C,
	HIT_ARG_STATUS_D,
	HIT_ARG_MAX,
	HIT_ARG_MIN,
	HIT_ARG_STATUS_MAX,
	HIT_ARG_STATUS_MIN,

};


class CPeizhi
{
public:

	//Ĭ�ϲ���
	const double & GetDefaultValue(int nDefaultID);
	//�������HP
	const int& GetPlayerMaxHP(int nGrade, int nVocation);
	//�������MP
	const int& GetPlayerMaxMP(int nGrade, int nVocation);
	//����HP�ظ��ٶ�
	const int & GetPlayerRecoverHP(int nGrade, int nVocation);
	//����MP�ظ��ٶ�
	const int & GetPlayerRecoverMP(int nGrade, int nVocation);
	//����������һ����Ҫ����
	const int & GetPlayerNextGradeExp(int nCurrentGrade);
	//���ﱻ�����������������
	const int & GetPlayerSkillGradePoint(int nSkillGrade, int nSkillID);
	//�������MP
	const int & GetMonsterMaxHP(int nMonsterGrade);
	//���﹥����
	const int & GetMonsterAttack(int nMonsterGrade);
	//�������������
	const int & GetMonsterPhyDefend(int nMonsterGrade);
	//����ħ����������������������
	const int & GetMonsterMagicDefend(int nMonsterGrade);
	//��������
	const int & GetMonsterHit(int nMonsterGrade);
	//��������
	const int & GetMonsterMiss(int nMonsterGrade);
	//�����˺����գ�����������
	const int & GetMonsterDamageSorb(int nMonsterGrade);
	//����״̬ħ���ɹ��ʣ�����������
	const int & GetMonsterStatusHit(int nMonsterGrade);
	//����״̬ħ���ֿ��ʣ�����������
	const int & GetMonsterStatusMiss(int nMonsterGrade);
	//���ﾭ��
	const int & GetMonsterEXP(int nMonsterGrade);

	static CPeizhi* Instance()
	{
		if (!g_pPeizhi)
		{
			g_pPeizhi = new CPeizhi();
		}
		return g_pPeizhi;
	}

public:

	CPeizhi & operator=( const CPeizhi & ) {}

	void Release()
	{
		delete this;

		g_pPeizhi = NULL;
	}


private:
	static CPeizhi * g_pPeizhi;
	const int ErrorReturn_int;
	const double ErrorReturn_double;
	int *out;

	CPeizhi(void):ErrorReturn_int(0),ErrorReturn_double(0)
	{
		out = new int[2];
	}
	~CPeizhi(void)
	{

	}

};

#endif