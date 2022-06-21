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

	//默认参数
	const double & GetDefaultValue(int nDefaultID);
	//人物最大HP
	const int& GetPlayerMaxHP(int nGrade, int nVocation);
	//人物最大MP
	const int& GetPlayerMaxMP(int nGrade, int nVocation);
	//人物HP回复速度
	const int & GetPlayerRecoverHP(int nGrade, int nVocation);
	//人物MP回复速度
	const int & GetPlayerRecoverMP(int nGrade, int nVocation);
	//人物升级下一级需要经验
	const int & GetPlayerNextGradeExp(int nCurrentGrade);
	//人物被动技能升级所需点数
	const int & GetPlayerSkillGradePoint(int nSkillGrade, int nSkillID);
	//怪物最大MP
	const int & GetMonsterMaxHP(int nMonsterGrade);
	//怪物攻击力
	const int & GetMonsterAttack(int nMonsterGrade);
	//怪物物理防御力
	const int & GetMonsterPhyDefend(int nMonsterGrade);
	//怪物魔法防御力＃＃＃＃＃＃＃
	const int & GetMonsterMagicDefend(int nMonsterGrade);
	//怪物命中
	const int & GetMonsterHit(int nMonsterGrade);
	//怪物闪避
	const int & GetMonsterMiss(int nMonsterGrade);
	//怪物伤害吸收＃＃＃＃＃＃
	const int & GetMonsterDamageSorb(int nMonsterGrade);
	//怪物状态魔法成功率＃＃＃＃＃＃
	const int & GetMonsterStatusHit(int nMonsterGrade);
	//怪物状态魔法抵抗率＃＃＃＃＃＃
	const int & GetMonsterStatusMiss(int nMonsterGrade);
	//怪物经验
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