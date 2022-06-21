#pragma  once

const float   PARAMATER1 = 1.000f;
const float   PARAMATER2 = 5.000f;
const float   PARAMATER3 = 0.025f;
const float   PARAMATER4 = 0.250f;


#define   RandCoef2053     2053	
#define   RandCoef13849    13849
#define   MODUL65536       65536

enum USERDATA
{
	USERDATA_CURHP,
	USERDATA_NMAE,
	USERDATA_CURMP,

};

enum SkillFalg
{
	SF_Swords = 0,
	SF_TwoHandsSwords,
	SF_Axes,
	SF_TwoHandsAxes,
	SF_Spears,
	SF_Javelins,
	SF_Bows,
	SF_Wands,
	SF_Staffs,
	SF_Shields,
	SF_Armors,
	SF_Cloths,
	SF_Heads,
	SF_Cuffs,
	SF_Leggins,
	SF_Glove,
	SF_Shoes,
	SF_Girdle,
	SF_Amulet,
	SF_Ring,

	SF_Count
};

enum eObjectState {
	OBJECT_IDLE = 0,
	OBJECT_FIGHT,
	OBJECT_WALK,
	OBJECT_RUN,
	OBJECT_DEAD,
	OBJECT_PICKUP,
	OBJECT_GHOST,
	OBJECT_ATTACK = 100,
	OBJECT_CAST,
	OBJECT_PERFORM,
	OBJECT_BUSYTIME,
};

enum eObjectType {
	OBJECTTYPE_UNKNOW = 0,
	OBJECTTYPE_PLAYER,
	OBJECTTYPE_NPC,
	OBJECTTYPE_MONSTER,
	OBJECTTYPE_BOSS,
	OBJECTTYPE_PET,
	OBJECTTYPE_BUILDING,
	OBJECTTYPE_VIP,
	OBJECTTYPE_GM,
	OBJECTTYPE_GameProgramer,
};

enum eLevel
{
	OCCUPATION_WARRIOR = 0,
	OCCUPATION_BUSINESSMAN,
	OCCUPATION_SCHOLAR,
};

enum eFaction
{
	FACTION_ATHEN = 0,
	FACTION_SPARTA,
};

enum AttackRadius
{
	Melee = 0,
	Range,

	AttackRadiusCount
};

enum AttributeType
{
	TYPE_ADD,
	TYPE_UPDATE,
	TYPE_DELETE,
	TYPE_EQUIP_DEC,
	TYPE_EQUIP_INC
};

#define MAX_EQUIPAPPEND_COUNT 5
#define MAX_DROPS 8

#define MAX_BAGS 4
#define MAX_BAG_GRID 24
#define MAX_STORAGE_GRID 64
#define ACADEMIC_NPC_MEM_ID_ITEMTABS 16
#define ACADEMIC_NPC_MEM_ID_ITEMS 112
#define MAX_INC_STATUS      10
#define MAX_DEC_STATUS      10 
#define MAX_NAME 32
#define MAX_EQUIP_COUNT 12
#define MAX_FRIENDS_COUNT  30
#define MAX_SINGLESEND_KITBAG 12
#define MAX_TEAM_NUMBER 5
#define QUEST_OBJECTIVES_COUNT 4
#define QUEST_EXPLORER_COUNT 4
#define QUEST_SOURCE_ITEM_IDS_COUNT 4
#define QUEST_REWARD_CHOICES_COUNT 6
#define QUEST_REWARDS_COUNT 4
#define QUEST_DEPLINK_COUNT 10
#define QUEST_NPCTAKE_COUNT 20
#define QUEST_FIND_COUNT 20
#define QUEST_PLAYER_COUNT 20
#define QUEST_COUNT 1000
#define PLAYER_MAXLV  200
#define MONSTER_MAXLV 200


#define SKILLBACKUPCOUNT 16	

#define MAX_ITEM_OVERLAY 99

class CGameObject;
struct  SKillData
{
	BYTE            ucID;
	char 		    Name[64];
	UINT            PrefixID;
	UINT            RrefixRank;
	UINT            TotalRank;
	std::vector<UINT>    EquipRequest;
	std::map<UINT, float> Effect;
};

struct MagicData
{
	UINT         	ucID;
	CHAR    	  	cName[64];
	BYTE           ucStyle;
	BYTE           ucEffect;
	USHORT         sMP;
	FLOAT          fPower1;
	FLOAT          fPower2;
	BYTE           ucDistance;
	BYTE           ucRange;
	BYTE           ucRequest_Lv;
	UINT           uiMoney;
	FLOAT	 	  	ucIntonateTime;
	FLOAT   	  	usCoolingTime;
	char			ScriptFun[64];
	UINT			ucTarget;
	UINT			ucAffectObj;
	int			iEffectTurn;
	float			fTimeInterval;
	int			iProperty;
	bool           bIsEquipUse;
	int            iEnmity;
	int            StatusOdds;
	int            Kind;
	BYTE           Priority;
	std::vector<int> m_Status;
	std::vector<UINT> EquipRequest;
};

struct ItemData
{
	int	Base;
	char BaseLV;
	int  Append[MAX_EQUIPAPPEND_COUNT];
	char AppendLV;
	char Binding;
	char Overlap;

	__int64 CdKey;

	ItemData() :
		Base(-1),
		BaseLV(0),
		AppendLV(0),
		Binding(0),
		Overlap(0),
		CdKey(0)
	{
		for (int i = 0; i < MAX_EQUIPAPPEND_COUNT; i++)
			Append[i] = -1;
	}
};

struct PlayerBaseProperty
{
	LONG                    m_lMaxHP;
	LONG                    m_lMaxMP;
	LONG                    m_lResumeHP;
	LONG                    m_lResumeMP;
	LONG					m_lAttack;
	LONG					m_lDefend;
	LONG                    m_lMagicAttack;
	LONG                    m_lMagicDefend;
	LONG					m_lHit;
	LONG					m_lDodge;
	LONG                    m_lCritAppend;
	LONG                    m_lCritDefend;
	FLOAT                   m_fPhyDamageAppend;
	FLOAT                   m_fMagicDamageAppend;
	LONG                    m_lDamageSorb;
	FLOAT                   m_fBeCure;
	FLOAT                   m_fCure;
	LONG                    m_lStatusHit;
	LONG                    m_lStatudDodge;

	PlayerBaseProperty() : m_lMaxHP(0), m_lMaxMP(0), m_lResumeHP(0), m_lResumeMP(0), m_lAttack(0), m_lDefend(0),
		m_lMagicAttack(0), m_lMagicDefend(0), m_lHit(0), m_lDodge(0), m_lCritAppend(0),
		m_lCritDefend(0), m_fPhyDamageAppend(0.0f), m_fMagicDamageAppend(0.0f), m_lDamageSorb(0), m_fBeCure(0.0f),
		m_fCure(0.0f), m_lStatusHit(0), m_lStatudDodge(0)
	{


	}

	void init()
	{
		m_lMaxHP = 0;
		m_lMaxMP = 0;
		m_lResumeHP = 0;
		m_lResumeMP = 0;
		m_lAttack = 0;
		m_lDefend = 0;
		m_lMagicAttack = 0;
		m_lMagicDefend = 0;
		m_lHit = 0;
		m_lDodge = 0;
		m_lCritAppend = 0;
		m_lCritDefend = 0;
		m_fPhyDamageAppend = 0.0f;
		m_fMagicDamageAppend = 0.0f;
		m_lDamageSorb = 0;
		m_fBeCure = 0.0f;
		m_fCure = 0.0f;
		m_lStatusHit = 0;
		m_lStatudDodge = 0;
	}
};



struct NpcItemData
{
	int	Base;
	char AppendLV;
	char BaseLV;
	char Binding;
	char Overlap;
	char Sell;

	char BuyType[3];
	UINT BuyConsume[3];
	char BuyTypeNum;


	NpcItemData()
		:
		Base(-1),
		AppendLV(0),
		BaseLV(0),
		Binding(0),
		Overlap(0),
		Sell(0),
		BuyTypeNum(0)
	{
		for (int j = 0; j < 3; j++)
		{
			BuyType[j] = -1;
			BuyConsume[j] = 0;
		}
	}
};

struct MallItemData
{
	int	Base;
	int	Append[MAX_EQUIPAPPEND_COUNT];
	char AppendLV;
	char BaseLV;
	char Binding;
	char Overlap;
	char Sell;

	char BuyType[3];
	UINT BuyConsume[3];
	char BuyTypeNum;
	int Discount;
	char New;
	char Hot;

	MallItemData()
		:
		Discount(100),
		Base(-1),
		AppendLV(0),
		Binding(0),
		Overlap(0),
		Sell(0),
		BuyTypeNum(0)
	{
		for (int i = 0; i < MAX_EQUIPAPPEND_COUNT; i++)
			Append[i] = -1;
		for (int j = 0; j < 3; j++)
		{
			BuyType[j] = -1;
			BuyConsume[j] = 0;
		}
	}
};

struct DiscountData
{
	char level;
	int  StoneDiscount;
	int  GloryDiscount;
};

struct Consortia_AltarInfo
{
	UINT AltarID;
	UINT Grade;
};

struct EquipData : public PlayerBaseProperty
{
	int         m_AttackDistance;
	int         m_AttackSpeed;

	EquipData() : m_AttackDistance(0), m_AttackSpeed(0)
	{
		m_AttackDistance = 0;
		m_AttackSpeed = 0;
	}
};

struct SkillData : public PlayerBaseProperty
{

};

struct BaseData : public PlayerBaseProperty
{

};

struct AltarData : public PlayerBaseProperty
{

};

struct StatusData : public PlayerBaseProperty
{
	float   m_HaltIntonate;
	float   m_ReboundDamage;
	float   m_NonMoving;
	float   m_NonMagicUsing;
	float   m_NonZSUsing;
	float   m_NonAttackUsing;
	float   m_NonPropUsing;
	float   m_ReturnDamage;
	float   m_ChangeIntonateTime;
	float   m_BeMagicDamage;
	float   m_CreateMagicDamage;
	float   m_BeAttackDamage;
	float   m_CreateAttackDamage;
	float   m_GetEXP;
	float   m_AddHP;
	float   m_DecHP;
	float   m_AddMP;
	float   m_DecMP;

	StatusData() : m_HaltIntonate(0.0f), m_ReboundDamage(0.0f), m_NonMoving(0.0f),m_NonMagicUsing(0.0f),
		m_NonZSUsing(0.0f), m_NonAttackUsing(0.0f), m_NonPropUsing(0.0f),m_ReturnDamage(0.0f), m_ChangeIntonateTime(0.0f), 
		m_BeMagicDamage(0.0f), m_CreateMagicDamage(0.0f),m_BeAttackDamage(0.0f), m_CreateAttackDamage(0.0f),
		m_GetEXP(0.0f), m_AddHP(0.0f), m_DecHP(0.0f), m_AddMP(0.0f), m_DecMP(0.0f)
	{
		m_HaltIntonate= 0.0f;
		m_ReboundDamage= 0.0f;
		m_NonMoving= 0.0f;
		m_NonMagicUsing= 0.0f;
		m_NonZSUsing= 0.0f;
		m_NonAttackUsing= 0.0f;
		m_NonPropUsing= 0.0f;
		m_ReturnDamage= 0.0f;
		m_ChangeIntonateTime= 0.0f;
		m_BeMagicDamage= 0.0f;
		m_CreateMagicDamage= 0.0f;
		m_BeAttackDamage= 0.0f;
		m_CreateAttackDamage= 0.0f;
		m_GetEXP= 0.0f;
		m_AddHP= 0.0f;
		m_DecHP= 0.0f;
		m_AddMP= 0.0f;
		m_DecMP= 0.0f;
	}
};

struct ObjectData
{
	char					m_strName[MAX_NAME];
	BYTE                    m_Gender;
	USHORT					m_lRegionID;
	FLOAT					m_fX, m_fY, m_fZ;
	FLOAT					m_fSpeed;
	USHORT					m_lStyle;
	BYTE					m_lFaction;
	USHORT					m_lPrestige;
	BYTE                    m_cKnight;
	USHORT                  m_cUnion;
	BYTE                    m_cUnionBusiness;
	UINT                    m_uiUionContribute;
	UINT					m_uiStoreNum;
	UINT					m_uiBagNum;
	BYTE			        m_lClass;
	UINT                    m_cExp;
	BYTE                    m_cRank;
	long					m_lHP;
	long					m_lMP;
	USHORT                  m_usAttackDistance;
	USHORT                  m_usAttackSpeed;
	BYTE                    m_cHairStyle;
	BYTE                    m_cPupilColor;
	USHORT                  m_usBelief;
	UINT					m_lMoney;
	UINT                    m_lStone;
};


enum {
	PROPERTY_PHY,
	PROPERTY_MAG,
	PROPERTY_CURE,
	PROPERTY_FLY
};

enum CONSORTIA_JOB_TYPE
{
	CONSORTIA_CHAIRMAN = 0x06,
	CONSORTIA_ASSISTANT_CHAIRMAN = CONSORTIA_CHAIRMAN - 1,
	CONSORTIA_DIRECTOR = CONSORTIA_CHAIRMAN - 2,
	CONSORTIA_CADREMAN = CONSORTIA_CHAIRMAN - 3,
	CONSORTIA_MEMBER = CONSORTIA_CHAIRMAN - 4,
	CONSORTIA_PROBATION_MEMBER = CONSORTIA_CHAIRMAN - 5,
	CONSORTIA_NONE = CONSORTIA_CHAIRMAN - 6,
};


#define  SIZE_OF_REGION 256.0f
#define  SIZE_OF_GRID  32.0f
#define  NUMBER_OF_REGION_GRID 16

struct Consortia_Member
{
	long lPlayerID;
	CONSORTIA_JOB_TYPE acDuty;
	char acMemberName[32];
	BYTE ucBusiness;
	UINT uiContribute;
	BYTE iLevel;
	long lMapID;
	Consortia_Member()
	{
		lPlayerID = -1;
		acDuty = CONSORTIA_NONE;
		::ZeroMemory(acMemberName, sizeof(acMemberName));
		ucBusiness = 0;
		uiContribute = 0;
		iLevel = 0;
		lMapID = -1;
	}
};

struct Consortia_Info
{
	char acName[64];
	char acChairman[32];
	BYTE iLevel;
	long lCredit;
	long lfound;
	long lbijou;
	char acTextInfo[256];
	int			MaxMember;
	UINT		AltarCounts;
	Consortia_AltarInfo		Altar[9];
	Consortia_Info()
	{
		::ZeroMemory(acName, sizeof(acName));
		::ZeroMemory(acChairman, sizeof(acChairman));
		iLevel = 0;
		lCredit = 0;
		lfound = 0;
		lbijou = 0;
		::ZeroMemory(acTextInfo, sizeof(acTextInfo));
		MaxMember = 0;
		AltarCounts = 0;
		::ZeroMemory(Altar, sizeof(Altar));
	}
};

#define CREATE_CONSORTIA_NEED_LEVEL 30
#define CREATE_CONSORTIA_NEED_MONEY 0
#define CREATE_CONSORTIA_NEED_ITEM  4100
#define DUTYCHAIRNAME "会长"
#define DUTYMEMBERNAME "会员"

#define LOGIC_TIMEOUT   			40									  
#define REGION_BACKUP_TIMEOUT 		1000*30	
#define CONSOR_BACKUP_TIMEOUT 		300000
#define GM_TIMEOUT                  3600000 
#define HINT_TIMEOUT                1000
#define QUEUE_TIMEOUT			    10000

#define LOGIC2NET_TIMEOUT   		2000
#define LOGIC_LIMIT_TIMEOUT 		1000	 
#define LOGIC_CYCLE_DELAY_TIMEOUT 	120		 


#define LOGIC_CYCLE_DELAY			20
#define LOGIN_QUEUE_REFRESH			20000
#define SURVEY_ONLIEN_TIME			20*60*1000

#define  MAX_STATUS_EFFECT			32
#define  MAX_SKILL_EFFECT			19

#define  MAX_RELATION_NUM			30
#define  MAX_CONSORTIA_MEMBER_SIZE	500
#define  MAX_ALTAR_NUM				8
#define  CONSORTIA_REFRESH_DAY		86400



