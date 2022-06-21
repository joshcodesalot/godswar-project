#ifndef _MSG_DEFINE_H_H
#define _MSG_DEFINE_H_H

#include ".\MsgTypes.h"
#include "..\GameServer\GameCore\GameData.h"
#include "..\GameServer\GameCore\Item.h"

#pragma warning(disable:4010)

struct MsgHead
{
	USHORT	usSize;
	USHORT	usType;
};

struct MSG_LOGIN
{
	MsgHead Head;
	char    cName[ACCOUNTS_LENGTH];
	char    cPassWord[ACCOUNTS_LENGTH];
#if GW2
	char     Mac[MAC_LENGTH];
	long     fVersion;
#endif
	MSG_LOGIN() {
		ZeroMemory(this, sizeof(*this));
		Head.usSize = sizeof(*this);
		Head.usType = _MSG_LOGIN;
	}
};

struct MSG_LOGIN_RETURN_INFO
{
	MsgHead  Head;
	BYTE  	 ucInfo;
};

struct MSG_PIPESERVER_INFO
{
	MsgHead Head;
	int   ucInfo;
};

// (GameServer ---- > LoginServer) 
// (LoginServer ---- > Client)
struct MSG_GAMESERVER_INFO
{
	MsgHead  Head;
#if GW2
	char     cIP[32];
	char     ServerName[32];
	UINT	 cRegionID;
	UINT     uiPort;
	BYTE     cID;
	BYTE     cState;
	BYTE     cRecommend;
	BYTE     cFlag;
#else
	char   cIP[MAX_NAME];
	UINT   uiPort;
	BYTE   cID;
	char   ServerName[MAX_NAME];
	BYTE   cState;
#endif
};


struct MSG_REQUEST_GAMESERVER
{
#if GW2
	MsgHead 	 Head;
	char         szName[ACCOUNTS_LENGTH];
	char         szHardWare[MAX_HARDWAREINFO];
	USHORT		 cRegionID;
	BYTE         cGameServerID;
	BYTE		 LineID;
#else
	MsgHead  Head;
	char     szName[ACCOUNTS_LENGTH];
	BYTE     cGameServerID;
#endif

	MSG_REQUEST_GAMESERVER() {
		ZeroMemory(this, sizeof(*this));
		Head.usType = _MSG_REQUEST_GAMESERVER;
		Head.usSize = sizeof(MSG_REQUEST_GAMESERVER);
	}
};

struct MSG_VALIDATE_GAMESERVER
{
	MsgHead  Head;
	char     Accounts[ACCOUNTS_LENGTH];
	char  	 cCheckOutText[VALIDATECODE_LENTH];
};

struct  MSG_LOGIN_GAMESERVER
{
	MsgHead 	 Head;
	char     Accounts[MAX_NAME];
	char  	 cCheckOutText[VALIDATECODE_LENTH];
};

struct MSG_LOGIN_QUEUE
{
	MsgHead 	 Head;
	int     iSurplusNum;
};

struct MSG_SERVER_NOTE
{
	MsgHead 	 Head;
	char     cNote[256];
};

struct MSG_RESPONSE_GAMESERVER
{
	MsgHead 	 Head;
	BYTE     cGameServerID;
	char     cIP[MAX_NAME];
	UINT     uiPort;
	BYTE     cLoginError;
	char  	 cCheckOutText[VALIDATECODE_LENTH];
};



struct MSG_GAMESERVER_READY
{
	MsgHead Head;
	BYTE   cServerID;
	BYTE   cLoginError;
};

struct MSG_ENTER_GAME
{
	MsgHead Head;
	char   cName[ROLENAME_LENGTH];
	char   cCheckOutText[VALIDATECODE_LENTH];
};

struct MSG_CREATE_ROLE
{
	MsgHead Head;
	char   Name[ROLENAME_LENGTH];
	BYTE   cGender;
	BYTE   cCamp;
	BYTE   ucProfession;
	BYTE   ucBelief;
	BYTE   ucHairStyle;
	BYTE   ucFaceShap;

	MSG_CREATE_ROLE() {
		ZeroMemory(this, sizeof(*this));
		Head.usType = _MSG_CREATE_ROLE;
		Head.usSize = sizeof(MSG_CREATE_ROLE);
	}
};

struct MSG_DELETE_ROLE
{
	MsgHead Head;
	char   Accounts[ROLENAME_LENGTH];
	char   Name[ROLENAME_LENGTH];

};

struct MSG_ROLE_INFO
{
	MsgHead Head;
	BYTE   ucNum;

	char   Name[ROLENAME_LENGTH];
	BYTE   cCamp;
	BYTE   ucProfession;
	BYTE   ucLV;
	BYTE   ucGender;
	BYTE   ucHairStyle;
	BYTE   ucFaceShap;
	INT    iEquip[MAX_EQUIP_COUNT];
	
	char   Name1[ROLENAME_LENGTH];
	BYTE   cCamp1;
	BYTE   ucProfession1;
	BYTE   ucLV1;
	BYTE   ucGender1;
	BYTE   ucHairStyle1;
	BYTE   ucFaceShap1;
	INT    iEquip1[MAX_EQUIP_COUNT];

	char   Name2[ROLENAME_LENGTH];
	BYTE   cCamp2;
	BYTE   ucProfession2;
	BYTE   ucLV2;
	BYTE   ucGender2;
	BYTE   ucHairStyle2;
	BYTE   ucFaceShap2;
	INT    iEquip2[MAX_EQUIP_COUNT];

	char   Name3[ROLENAME_LENGTH];
	BYTE   cCamp3;
	BYTE   ucProfession3;
	BYTE   ucLV3;
	BYTE   ucGender3;
	BYTE   ucHairStyle3;
	BYTE   ucFaceShap3;
	INT    iEquip3[MAX_EQUIP_COUNT];


};


struct MSG_CLIENT_READY
{
	MsgHead Head;
	USHORT  usUserID;
	char   cCheckOutText[VALIDATECODE_LENTH];
};


struct MSG_MANAGE_RETURN
{
	MsgHead 	 Head;
	USHORT    ucErrorType;
	int     ucDisc;

};

struct Equip
{
	int	Base;
	char BaseLV;
	int	Append[MAX_EQUIPAPPEND_COUNT];
	char AppendLV;

};

struct KitBag
{
	int Base;
	char BaseLV;
	int Append[MAX_EQUIPAPPEND_COUNT];
	char AppendLV;
	char Binding;
	char Overlap;
};

struct MSG_SELFINFO
{
	MsgHead 				Head;
	LONG					lDB_id;
	char	        cName[MAX_NAME];
	BYTE          ucGender;
	BYTE          ucCamp;
	BYTE          ucBelief;
	BYTE          ucProfession;
	BYTE          ucHairStyle;
	BYTE					ucFaceShap;
	USHORT 				usMapID;
	USHORT         cEquipPos;
	UINT  				uiUserID;
	FLOAT  				X, Y, Z;
	UINT          uiMaxHP;
	UINT          uiMaxMP;
	UINT          uiCurHP;
	UINT          uiCurMP;
	UINT          uiExp;
	UINT          uiMaxExp;
	UINT          uiSkillPoint;
	UINT          uiSkillExp;
	Equip          SEquip[MAX_EQUIP_COUNT];

};

struct MSG_SELFPROPERTY
{
	MsgHead 				Head;
	char					m_strName[32];
	BYTE          m_Gender;
	USHORT					m_lRegionID;
	FLOAT					m_fX, m_fY, m_fZ;
	FLOAT					m_fSpeed;
	USHORT					m_lStyle;
	BYTE					m_lFaction;
	USHORT					m_lPrestige;
	BYTE          m_cKnight;
	USHORT         m_cUnion;
	BYTE          m_cUnionBusiness;
	UINT          m_uiUionContribute;
	UINT					m_uiStoreNum;
	UINT					m_uiBagNum;
	BYTE					m_lClass;
	UINT          m_cExp;
	BYTE          m_cRank;
	long					m_lHP;
	long					m_lMP;
	USHORT         m_usAttackDistance;
	USHORT         m_usAttackSpeed;
	BYTE          m_cHairStyle;
	BYTE          m_cPupilColor;
	USHORT         m_usBelief;
	UINT					m_lMoney;
	UINT          m_lStone;
};


struct MSG_PARAMATER
{
	MsgHead Head;
	int   Figher_HP_A;
	float  Figher_HP_B;
	float  Figher_HP_C;
	float	Figher_HP_D;
	int   Figher_MP_A;
	float  Figher_MP_B;
	float  Figher_MP_C;
	int   Figher_Attack_A;
	float	Figher_Attack_B;
	float	Figher_Attack_C;
	float	Figher_Attack_D;
	int	  Figher_Defend_A;
	float	Figher_Defend_B;
	float	Figher_Defend_C;
	float	Figher_Defend_D;
	int		Figher_MagicExempt_A;
	int		Figher_MagicExempt_B;
	float	Figher_MagicExempt_C;
	int	  Figher_MagicResist_A;
	float	Figher_MagicResist_B;

	int   Schloar_HP_A;
	float  Schloar_HP_B;
	float  Schloar_HP_C;
	float	Schloar_HP_D;
	int   Schloar_MP_A;
	float  Schloar_MP_B;
	float  Schloar_MP_C;
	int   Schloar_Attack_A;
	float	Schloar_Attack_B;
	float	Schloar_Attack_C;
	float	Schloar_Attack_D;
	int	  Schloar_Defend_A;
	float	Schloar_Defend_B;
	float	Schloar_Defend_C;
	float	Schloar_Defend_D;
	int		Schloar_MagicExempt_A;
	int		Schloar_MagicExempt_B;
	float	Schloar_MagicExempt_C;
	int	  Schloar_MagicResist_A;
	float	Schloar_MagicResist_B;

	int   Defalult_Hit;
	int	  Defalult_Dodge;
	int	  Defalult_MagicExempt;
	float	Default_Fastness_Wind;
	float	Default_Fastness_Fire;
	float	Default_Fastness_Terra;
	float	Default_Fastness_Water;
	float	Default_Resume_HP;
	float  Default_Resume_MP;

	float  Grade_A;
	float  Grade_B;
	float  Grade_C;

	float  Skill_A;
	float  Skill_B;
	float  Skill_C;
};

struct MSG_KITBAG
{
	MsgHead 				 Head;
	UINT  				 uiUserID;
	UINT					 uiBagNum;
	UINT					 uiUpdataNum;
	char           ctab;
	char           cStartPos;
	UINT           cKitBagFlag;
	KitBag          SKitBagInfo[24];
};

struct MSG_STORAGE
{
	MsgHead 				 Head;
	int						 Money;
	UINT  				 uiUserID;
	char           cActiveNum;
	char           cLine;
	USHORT          cKitBagFlag;
	KitBag          SKitBagInfo[12];
};

struct Friend
{
	char  Name[18];
	bool   IsOnline;
	USHORT  usCamp;
	USHORT  usBusiness;
	USHORT  usCity;
	USHORT  uc_LV;

};

struct MSG_RELATIONALL
{
	MsgHead 				 Head;
	UINT  				 uiUserID;
	BYTE           ucType;
	BYTE           cNum;
	Friend          sFriend[MAX_FRIENDS_COUNT];
};


struct MSG_OBJECTINFO
{
	MsgHead Head;
	long   DB_id;
	UINT   uiID;
	UINT	 uiType;
	USHORT  usMapID;
	float  X, Y, Z;
	float	 fAtan2;
	bool	 Stall;
	wchar_t	 StallText[MAX_NAME];
	char	 cName[MAX_NAME];
	UINT   uiMaxHP;
	UINT   uiHP;
	BYTE   ucGender;
	BYTE   ucCamp;
	BYTE   ucBelief;
	BYTE   ucProfession;
	BYTE   ucHairStyle;
	BYTE	 ucFaceShap;
	UINT   ucLV;
	UINT   iStatusCount;
	UINT   iStautsDis[MAX_INC_STATUS + MAX_INC_STATUS];
	USHORT  cEquipPos;
	Equip  SEquip[MAX_EQUIP_COUNT];
};


struct MSG_TALK
{
	MsgHead		Head;
	INT			uiUserID;
	INT			ucTextSize;
	BYTE		ucType;
	BYTE		ucResPlayerNameSize;
	BYTE		ucAimPlayerNameSize;
	BYTE		ucChatBubbleSytle;
	WCHAR		wcContent[256];
};

struct MSG_TALKCHANNEL
{
	MsgHead		Head;
	BYTE		ucChannel;
};

struct MSG_WALK
{
	MsgHead  Head;
	UINT   uiID;
	USHORT  usMapID;
	float   x, z, fatan;
	long	 lState;
};

struct MSG_WALK_BEGIN
{
	MsgHead  Head;
	UINT   uiID;
	USHORT  usMapID;

	UINT	 uiTick;
	float   X, Y, Z;
	float   OffsetX, OffsetY, OffsetZ;
};

struct MSG_WALK_END
{
	MsgHead  Head;
	UINT   uiID;
	USHORT  usMapID;

	UINT	 uiTick;
	float   X, Y, Z;
	float	 fAtan2;
	USHORT  usSkillFlag;
};



struct MSG_FIGHT
{
	MsgHead	Head;

	UINT uiID;
	UINT uiTargetID;

	bool Fight;
};

struct MSG_ATTACK
{
	MsgHead  Head;
	UINT   uiID;
	float   X, Y, Z;
	UINT	 uiObjectID;
	long	 uiDamage;
	BYTE   ucWeaponType;
	BYTE   ucAttackType;
};

struct MSG_DEAD
{
	MsgHead  Head;
	UINT   uiID;
	INT 	 playerID[MAX_TEAM_NUMBER];
	UINT	 playerMoney[MAX_TEAM_NUMBER];
	UINT	 Exp[MAX_TEAM_NUMBER];
	UINT	 SkillExp[MAX_TEAM_NUMBER];
	UINT	 SkillPoint[MAX_TEAM_NUMBER];
	UINT   KillerID;
	UINT   KillerPre;
};

struct MSG_BACKHOME
{
	MsgHead Head;
	UINT	uiID;
	int		ReviveType;
};

struct MSG_RESUNE
{
	MsgHead Head;
	ULONG  ulHP;
	ULONG  ulMP;
};

struct MSG_DROPS
{
	MsgHead  Head;
	UINT   uiID;

	int	   DropNum;
	char	 Drops[256];
};

struct MSG_SKILL
{
	MsgHead  Head;
	UINT   uiID;
	int    cType;
	BYTE   cStep;
	int	   uiObjectID;
	BYTE   ucMP;
	float	 x;
	float	 z;
	float	 x1;
	float	 z1;
};


struct MSG_MAGIC_DAMAGE
{
	MsgHead  Head;
	UINT   uiID;
	INT	   uiObjectID;
	BYTE	 Attack_Type;
	BYTE   Status_Type;
	BYTE	 bType;
	int		 iPower;
	int    ucMagicID;
	float	 x1;
	float	 z1;
};

struct MSG_MAGIC_PERFORM
{
	MsgHead		Head;
	UINT		uiID;
	INT		uiObjectID;
	UINT		ucMagicID;
	float		x;
	float		z;
};

struct MAGIC_DAMAGE {
	UINT		uiObjectID;
	BYTE		Attack_Type;
	BYTE		ucType;
	int			iPower;
};
#pragma pack(push)
#pragma pack(1)
struct MSG_MAGIC_CLUSTER_DAMAGE
{
	MsgHead			Head;
	UINT			uiID;
	int				iCount;
	UINT		  ucMagicID;
	char      bHitStatus;
	MAGIC_DAMAGE	magicDamage[100];
};
#pragma pack(pop)

struct MSG_LEARN
{
	MsgHead  Head;
	UINT   uiID;
	BYTE   cType;
	BYTE   cKinds;
	BOOL   bIsSucced;
};



struct MAGIC_INFO
{
	int    cType;
	USHORT  usSurplusCoolTime;
};
struct MSG_ACTIVESKILL_INFO
{
	MsgHead  Head;
	UINT    uiID;
	USHORT   MagicCounts;
	MAGIC_INFO Magic[150];

};


struct SKILL_INFO
{
	int    cType;
	USHORT  Rank;
	USHORT  EquipLV;
	UINT   NextGradePoint;
};
struct MSG_PASSIVESKILL_INFO
{
	MsgHead  Head;
	UINT    uiID;
	USHORT   SkillCounts;
	SKILL_INFO Skill[20];
};

struct MSG_MAGIC_PARAMETER
{
	MsgHead  	Head;
	UINT   	uiID;
	BYTE   	cType;
	char   	cName[64];
	BYTE   	ucRank;
	BYTE   	ucPractice;
	USHORT    lMaxHP;
	USHORT    lMaxMP;
	USHORT    lDodge;
	USHORT    lHit;
	USHORT   	lMagicExempt;
	float   	lTerra_Protect;
	float   	lWater_Protect;
	float   	lFire_Protect;
	float    lWind_Protect;
	USHORT    lPhysics_Protect;
	float    fTerra_Max;
	float    fWater_Max;
	float    fFire_Max;
	float    fWind_Max;
};



struct MSG_UPGRADE
{
	MsgHead  Head;
	UINT   ID;
	UINT   Rank;
	UINT   MaxEXP;
	UINT   CurEXP;
	UINT   MAXHP;
	UINT   CURHP;
	UINT   MAXMP;
	UINT   CURMP;
};



struct MSG_EFFECT
{
	MsgHead  Head;
	UINT   uiID;

	float   fColor1R;
	float   fColor1G;
	float   fColor1B;

	float   fColor2R;
	float   fColor2G;
	float   fColor2B;

	float   fColor3R;
	float   fColor3G;
	float   fColor3B;
};

struct MSG_SKILL_INTERRUPT
{
	MsgHead  Head;
	UINT   uiID;
};


struct MSG_LEAVE
{
	MsgHead  Head;
	UINT   uiID;
};


struct MSG_COLONY_LEAVE
{
	MsgHead  Head;
	USHORT  usNum;
	UINT   uiID[800];
};


struct MSG_SCENE_CHANGE
{
	MsgHead  Head;
	UINT   uiUserID;
	float   X, Y, Z;
	USHORT  usCurMapID;
	USHORT  usNextMapID;
};



struct MSG_PICKUP_DROPS
{
	MsgHead	Head;
	UINT	uiID;
	UINT	uiMonsterID;
	int		PickupIndex;
};


struct MSG_USEOREQUIP_ITEM
{
	MsgHead	Head;

	UINT uiID;

	unsigned short Index;
	unsigned short Num;

	int ChangeF;
	int Base;
	int Append[MAX_EQUIPAPPEND_COUNT];

	char AppendLevel;
	char BaseLevel;

	bool False;
};

struct MSG_MOVE_ITEM
{
	MsgHead	Head;

	UINT uiID;

	short	Index;
	short	Num;

	short	IndexAim;
	short	NumAim;

	int Base;
	int Append[MAX_EQUIPAPPEND_COUNT];
	char AppendLevel;
	char BaseLevel;
};

struct MSG_BREAK_ITEM
{
	MsgHead	Head;

	short Flags;
	short Index;
	short Num;
	short IndexAim;
	short NumAim;

	short Overlap;
};

struct MSG_STORAGE_ITEM
{
	MsgHead Head;

	short	Index;

	short	IndexAim;
	short	NumAim;

	int		Money;

	bool	Storage;
};

struct MSG_SELL
{
	MsgHead			Head;

	unsigned short	Index;
	unsigned short	Num;
};

struct MSG_STALL
{
	MsgHead		Head;

	UINT		uiID;
	bool		State;

	wchar_t		Text[MAX_NAME];
	char		SellerName[MAX_NAME];

};

struct MSG_STALLADDITEM
{
	MsgHead		Head;

	int			Index;
	int			Pocket;

	int			BagIndex;
	int			BagNum;

	int			Money;
	int			Bijou;

	__int64		StallID;
};

struct MSG_STALLDELITEM
{
	MsgHead		Head;

	int			Index;
	int			Pocket;
};

struct MSG_STALLITEM
{
	MsgHead		Head;

	UINT		uiID;
	char		SellerName[MAX_NAME];

	__int64		StallID;

	int			Pocket;
	int			Index;

	int			BaseID;
	int			Append[MAX_EQUIPAPPEND_COUNT];

	int			BaseLevel;
	int			AppLevel;
	int			Overlap;

	int			Money;
	int			Bijou;
};

struct MSG_STALLBUYITEM
{
	MsgHead		Head;

	UINT		uiSellID;
	UINT		uiBuyID;

	char		SellerName[MAX_NAME];
	char		CustomName[MAX_NAME];

	__int64		StallID;

	int			Pocket;
	int			Index;
};


struct MSG_TALK_NPC
{
	MsgHead	Head;

	UINT NPCID;

	int NPCFlag;

	int SysFlag;

	char Talk[MAX_NAME];
};

struct MSG_NPC_DATA
{
	MsgHead Head;

	UINT	NPCID;
};

struct MSG_NPC_ITEMDATA
{
	MsgHead Head;

	UINT	NPCID;
	char TabIndex;
	char MaxTabIndex;
	char ItemCount;
	NpcItemData Itemdata[32];
};

struct MSG_MALL_ITEMDATA
{
	MsgHead Head;
	char TabIndex;
	char MaxTabIndex;
	char ItemCount;
	MallItemData Itemdata[16];
};
struct MSG_GETITEMDATA
{
	MsgHead Head;
	UINT	userID;
};

struct MSG_NPC_STORAGEDATA
{
	MsgHead Head;

	UINT	NPCID;

	int		Storage;
};

struct MSG_NPC_SELL
{
	MsgHead Head;
	UINT NPCID;
	int Index;
	int Num;
	int Count;
	int Base;
};

struct MSG_MALL_SELL
{
	MsgHead Head;
	int Index;
	int Num;
	int Count;
	int Base;
};
struct MSG_ASSOCIATIONDISCOUNT
{
	MsgHead Head;
	char  Num;
	DiscountData Data[30];
};
struct NPCQuest
{
	int		QuestID;
	bool	Available;
};

struct PlayerQuest
{
	int		QuestID;

	int		NPCID;
	int		RewardNPCID;
	int		QuestSort;

	int		Money;
	int		Bijou;
	int		RewardMoney;
	int		RewardBijou;

	short	Item[QUEST_OBJECTIVES_COUNT];
	short	Creature[QUEST_OBJECTIVES_COUNT];
	short	ItemCount[QUEST_OBJECTIVES_COUNT];
	short	CreatureCount[QUEST_OBJECTIVES_COUNT];
	bool	ExplorerState[QUEST_EXPLORER_COUNT];

	char	SpecialFlags;
	char	Status;
	char	Completed;
	short	ItemCompletedCount[QUEST_OBJECTIVES_COUNT];
	short	CreatureCompletedCount[QUEST_OBJECTIVES_COUNT];

	ItemData RewardItem[QUEST_REWARDS_COUNT];
};

struct MSG_ExplorerQuest
{
	MsgHead Head;
	int		QuestID;
};

struct MSG_FINDQUEST
{
	MsgHead		Head;
	int			Nothing;
};

struct MSG_FINDQUESTRESULT
{
	MsgHead		Head;

	int			Count;

	short		Quest[QUEST_FIND_COUNT];
};

struct MSG_NPC_QUEST
{
	MsgHead		Head;

	UINT		NPCID;

	NPCQuest	Quest;
};

struct MSG_NPC_NEXTQUEST
{
	MsgHead	Head;

	UINT	NPCID;
	int		QuestID;
	int		QuestSort;

	int		SpecialFlags;

	short	Item[QUEST_OBJECTIVES_COUNT];
	short	Creature[QUEST_OBJECTIVES_COUNT];
	short	ItemCount[QUEST_OBJECTIVES_COUNT];
	short	CreatureCount[QUEST_OBJECTIVES_COUNT];

	int		Money;
	int		Bijou;
	int		RewardMoney;
	int		RewardBijou;

	ItemData RewardItem[QUEST_REWARDS_COUNT];
};

struct MSG_NPC_QUESTS
{
	MsgHead	Head;

	UINT	NPCID;

	int		Count;

	NPCQuest Quest[QUEST_NPCTAKE_COUNT];
};

struct MSG_NPC_QUESTSAVAILABLE
{
	MsgHead Head;

	int		Count;
	UINT	NPCID[QUEST_PLAYER_COUNT * 4];
	int		QuestID[QUEST_PLAYER_COUNT * 4];
};

struct MSG_NPC_QUESTSUNAVAILABLE
{
	MsgHead Head;

	int		Count;
	UINT	NPCID[QUEST_PLAYER_COUNT * 4];
	int		QuestID[QUEST_PLAYER_COUNT * 4];
};

struct MSG_NPC_QUESTREWARD
{
	MsgHead Head;

	UINT	NPCID;

	int		Count;

	int		QuestID[QUEST_NPCTAKE_COUNT];
};


struct MSG_NPC_QUESTVIEW
{
	MsgHead Head;

	UINT	NPCID;
	int		QuestID;
	int		QuestSort;

	int		SpecialFlags;

	short	Item[QUEST_OBJECTIVES_COUNT];
	short	Creature[QUEST_OBJECTIVES_COUNT];
	short	ItemCount[QUEST_OBJECTIVES_COUNT];
	short	CreatureCount[QUEST_OBJECTIVES_COUNT];

	int		Money;
	int		Bijou;
	int		RewardMoney;
	int		RewardBijou;

	ItemData RewardItem[QUEST_REWARDS_COUNT];
};

struct MSG_PLAYER_ACCEPTQUESTS
{
	MsgHead		Head;

	int			Count;

	PlayerQuest Quests[3];
};

struct MSG_NPC_ACCEPTQUEST
{
	MsgHead Head;

	int		NPCID;
	int		RewardNPCID;

	int		QuestID;


	int		QuestSort;

	int		SpecialFlags;

	short	Item[QUEST_OBJECTIVES_COUNT];
	short	Creature[QUEST_OBJECTIVES_COUNT];
	short	ItemCount[QUEST_OBJECTIVES_COUNT];
	short	CreatureCount[QUEST_OBJECTIVES_COUNT];

	int		Money;
	int		Bijou;
	int		RewardMoney;
	int		RewardBijou;

	ItemData RewardItem[QUEST_REWARDS_COUNT];
};

struct MSG_NPC_CANCELQUEST
{
	MsgHead Head;

	int		NPCID;
	int		RewardNPCID;

	int		QuestID;

	bool	Available;
};

struct MSG_NPC_COMPLETEQUEST
{
	MsgHead	Head;

	int		RewardNPCID;

	int		QuestID;
	int		Choice;
};

struct MSG_NPC_QUESTFAILD
{
	MsgHead	Head;

	int		RewardNPCID;

	int		QuestID;
};

struct MSG_NPC_REWARDQUEST
{
	MsgHead	Head;

	int		NPCID;
	int		RewardNPCID;

	int		QuestID;
	int		Choice;

	int		Credit;
	int		Contribute;
	int		EXP;
	int		SkillPoint;

	int		SpecialFlags;
	int		SpecialCount;

	ItemData Item;
};

struct MSG_NPC_QUESTKILLORCAST
{
	MsgHead Head;

	int		QuestID;
	int		NPCID;

	bool	Kill;
	int		KillOrCastID;
};

struct MSG_NPC_QUESTEXPLORERRESULT
{
	MsgHead Head;

	int		QuestID;
	int		NPCID;
	int		Index;
};

struct MSG_RELATION
{
	MsgHead		Head;
	char  	cName[ROLENAME_LENGTH];
	BYTE  	bUnion;
	BYTE    ucCity;
	BYTE    Business;
	BYTE    LV;
	bool  	IsOnline;
	BYTE    Style;
};



struct MSG_RELATION_REQUEST
{
	MsgHead		Head;
	char  	cName_Request[ROLENAME_LENGTH];
	char    cName_Response[ROLENAME_LENGTH];
	char    cType;
	char    Error;
};

struct MSG_RELATION_RESPONSE
{
	MsgHead		Head;
	char  	cName_Request[ROLENAME_LENGTH];
	bool    IsAgree;
};

struct MSG_RELATION_DELETE
{
	MsgHead		Head;
	char  	cName_Request[ROLENAME_LENGTH];
	BYTE    Style;
};

struct MSG_TRADE
{
	MsgHead		Head;
	UINT		uiID;
	UINT		uiObjectID;
	int			State;
	int			TraderState;
	char		chName[64];
	char		chObjectName[64];
};

struct MSG_TRADE_MONEY
{
	MsgHead		Head;
	UINT		uiID;
	int			Money;
	int			Bijou;
	char		chName[64];
};

struct MSG_TRADE_ITEM
{
	MsgHead		Head;
	UINT		uiID;
	int			Index;
	int			BaseID;
	int			Append[MAX_EQUIPAPPEND_COUNT];
	char		AppendLevel;
	char		BaseLevel;
	int			Overlap;
	char		chName[64];
};

struct MSG_TRADE_ADDITEM
{
	MsgHead		Head;
	UINT		uiID;
	int			Index;
	int			Bag;
	int			Grid;
	char		chName[64];
};

struct MSG_TRADE_REMOVEITEM
{
	MsgHead		Head;
	UINT		uiID;
	int			Index;
	char		chName[64];
};

struct MSG_EQUIPFORGE_EQUIP
{
	MsgHead		Head;

	int			Index;
	int			Num;
	int			Type;
	int			ForgeTab;

	long		ItemId;
	int			ItemBaseLv;
	int			ItemAppdLv;
	int			ItemOverlap;
	int			ItemBinding;
	int			ItemAppData[MAX_EQUIPAPPEND_COUNT];
};

struct MSG_EQUIPFORGE_MATERIAL
{
	MsgHead		Head;

	int			Index;
	int			Num;
	int			Type;
};

struct MSG_EUQIPFORGE
{
	MsgHead		Head;
	bool		Success;
	int			Type;
	int			BaseLv;
	int			AppLv;
	int			AppId[5];
};


							    
struct MSG_EQUIPFORGE_EQUIPCANCEL
{
	MsgHead		Head;
};

struct MSG_EQUIPFORGE_MATERIALCANCEL
{
	MsgHead		Head;
};

struct MSG_EQUIPFORGE_CANCEL
{
	MsgHead		Head;
};

struct MSG_TEAM
{
	MsgHead		Head;
	UINT		uiResPlayer;
	char		chResPlayer[64];
	char		chAimPlayer[64];
};

struct MSG_TEAMINFO
{
	MsgHead		Head;
	char		chAimName[64];
	bool		bIsAimInTeam;
	bool		bIsSelfInTeam;
};

struct MSG_TEAM_ADD
{
	MsgHead		Head;
	UINT		uiPlayerID;
	int			uiTeamID[5];
};

struct MSG_TEAM_REFLASH
{
	MsgHead		Head;

	long		lTeamLeaderID;
	UINT		uiTeamLeader_HP;
	UINT		uiTeamLeader_MaxHP;
	UINT		uiTeamLeader_Lv;
	BYTE		ucTeamLeader_Class;
	char		chTeamLeader_Name[64];
	SHORT		usTeamLeader_MapID;
	float		ufTeamLeader_X;
	float		ufTeamLeader_Y;
	float		ufTeamLeader_Z;

	long		lTeammate0_ID;
	UINT		uiTeammate0_HP;
	UINT		uiTeammate0_MaxHP;
	UINT		uiTeammate0_Lv;
	char		chTeammate0_Name[64];
	BYTE		ucTeammate0_Class;
	SHORT		usTeammate0_MapID;
	float		ufTeammate0_X;
	float		ufTeammate0_Y;
	float		ufTeammate0_Z;

	long		lTeammate1_ID;
	UINT		uiTeammate1_HP;
	UINT		uiTeammate1_MaxHP;
	UINT		uiTeammate1_Lv;
	char		chTeammate1_Name[64];
	BYTE		ucTeammate1_Class;
	SHORT		usTeammate1_MapID;
	float		ufTeammate1_X;
	float		ufTeammate1_Y;
	float		ufTeammate1_Z;

	long		lTeammate2_ID;
	UINT		uiTeammate2_HP;
	UINT		uiTeammate2_MaxHP;
	UINT		uiTeammate2_Lv;
	char		chTeammate2_Name[64];
	BYTE		ucTeammate2_Class;
	SHORT		usTeammate2_MapID;
	float		ufTeammate2_X;
	float		ufTeammate2_Y;
	float		ufTeammate2_Z;

	long		lTeammate3_ID;
	UINT		uiTeammate3_HP;
	UINT		uiTeammate3_MaxHP;
	UINT		uiTeammate3_Lv;
	char		chTeammate3_Name[64];
	BYTE		ucTeammate3_Class;
	SHORT		usTeammate3_MapID;
	float		ufTeammate3_X;
	float		ufTeammate3_Y;
	float		ufTeammate3_Z;

	bool		bIsDateUpdate;

};

struct MSG_TEAM_DESTROY
{
	MsgHead		Head;
};

struct MSG_TIP
{
	MsgHead		Head;
	WCHAR		chName[64];
	WCHAR		tip[128];
};



struct MSG_CONSORTIA_CREATE
{
	MsgHead   Head;
	UINT    uiPlayerID;
	Consortia_Info ci;
	Consortia_Member cm;
	MSG_CONSORTIA_CREATE()
	{
		::ZeroMemory(&ci, sizeof(ci));
	}
};

#pragma pack(push)
#pragma pack(1)
struct MSG_CONSORTIA_CREATE_RESPONSE
{
	MsgHead     Head;
	UINT       uiPlayerID;
	Consortia_Info  ci;
	Consortia_Member cm;
	BYTE       Style;
};
#pragma pack(pop)

struct MSG_CONSORTIA_BASE_INFO
{
	MsgHead   Head;
	char		acName[64];
	BYTE		iLevel;
	long    lfound;
	long    lbijou;
	long		lCredit;
	char	  acTextInfo[256];
	int			MaxMember;
	UINT		AltarCounts;
	Consortia_AltarInfo		Altar[9];
};

struct CONSORTIA_MEMBER1
{
	char acMemberName[32];
	char lMapID;
	BYTE acDuty;
	BYTE iLevel;
	BYTE ucBusiness;
	UINT uiContribute;

	template<typename T>
	CONSORTIA_MEMBER1& operator = (const T& Element)
	{
		strcpy(acMemberName, Element.strName.c_str());
		lMapID = (BYTE)Element.lMapID;
		iLevel = (BYTE)Element.LV;
		acDuty = (BYTE)Element.Job;
		ucBusiness = (BYTE)Element.ucBusiness;
		uiContribute = (UINT)Element.uiContribute;

		return *this;
	}

};

struct MSG_CONSORTIA_MEMBER_LIST
{
	MsgHead   Head;
	UINT    uiReceivePlayerID;
	UINT    uiCount;
	CONSORTIA_MEMBER1 Member[24];
	bool    IsAll;
};

struct MSG_CONSORTIA_INVITE
{
	MsgHead   Head;
	UINT    uiInvitePlayerID;
	char    ciInvitedName[32];
	char    ciConsortiaName[64];
};


struct MSG_CONSORTIA_DISMISS
{
	MsgHead   Head;
	char    cPlayerName[32];
};

struct MSG_CONSORTIA_RESPONSE
{
	MsgHead   Head;
	BYTE    Style;
	UINT    uiSendPlayerID;
	char    ciReceiveName[32];
};

struct MSG_CONSORTIA_EXIT
{
	MsgHead   Head;
	char    StrName[32];
	char    NewDutyName[32];
	char    Duty;
};

struct MSG_CONSORTIA_TEXT
{
	MsgHead   Head;
	UINT    uiPlayerID;
	char    NewPlacard[128];
};

struct MSG_CONSORTIA_DUTY
{
	MsgHead   Head;
	char    OldName[32];
	char    OldDuty;
	char    NewName[32];
	char    NewDuty;

};

struct MSG_CONSORTIA_MEMBER_DEL
{
	MsgHead   Head;
	UINT    uiPlayerID;
	char    strName[32];
};

struct MSG_ALTAR_INFO
{
	MsgHead   Head;
	int			iAltarCounts;
	Consortia_AltarInfo Altar[20];
};
struct MSG_SKILL_BACKUP
{
	MsgHead   Head;

	int			ID[SKILLBACKUPCOUNT];
	int			Item[SKILLBACKUPCOUNT];

	int			RID;
	int			RItem;
};

struct MSG_SKILLPOINT_UPGRADE
{
	MsgHead   Head;
	UINT    uiPlayerID;
	UINT    SKillPoint;
};

struct MSG_SKILL_UPGRADE
{
	MsgHead   Head;
	UINT    uiPlayerID;
	UINT    uiSkillID;
	UINT    uiCurrentSkillGrade;
	UINT    uiNextSkillGradePoint;
	UINT    uiSurplusPoint;
};

struct MSG_SYN_GAMEDATA
{
	MsgHead   Head;
	UINT    iUserID;
	ObjectData sGameData;
	PlayerBaseProperty    sBaseData;
	int     iAttackDistance;
	int     iAttackSpeed;
};

struct MSG_STATUS
{
	MsgHead   Head;
	UINT    uiID;
	UINT    uiCount;
	UINT    uiStatusID[MAX_INC_STATUS + MAX_DEC_STATUS];
	USHORT   uiSurplusTime[MAX_INC_STATUS + MAX_DEC_STATUS];
	StatusData Data;
};

struct MSG_ERROR
{
	MsgHead		Head;
	char		Error[MAX_NAME];
};

struct MSG_UPDATE_MP {
	MsgHead		Head;
	UINT		playerID;
	int			mp;
};

struct MSG_CONSORTIA_LVUP
{
	MsgHead		Head;
	UINT    iUserID;
};

struct MSG_ALTAR_CREATE
{
	MsgHead		Head;
	UINT    iUserID;
	BYTE    iAltarID;
};

struct MSG_ALTAR_LVUP
{
	MsgHead		Head;
	UINT    iUserID;
	BYTE    iAltarID;
};

struct MSG_ALTAR_OBLATION
{
	MsgHead		Head;
	UINT    iUserID;
	BYTE    iAltarID;
};

struct MSG_CRETIT_EXCHANGE
{
	MsgHead Head;
	UINT					iUserID;
	USHORT					iPrestige;
	UINT          iExp;
	UINT					iMoney;
	int						iMaterialType;
	unsigned short			Num;
};

struct MSG_GOLD_BOX
{
	MsgHead Head;
	BYTE	Type;
	UINT	GoldBoxId;
	BYTE	Iter;
};


struct MSG_GOLD_BOX_RETURN
{
	MsgHead		Head;
	BYTE		ucInfo;
	UINT		DialItem[DIALITEMCOUNT + 1];
};

struct MSG_SYS_ADD_ITEM
{
	MsgHead		Head;
	ItemData	itemData;
};
struct MSG_SYS_DEL_ITEM
{
	MsgHead		Head;
	int		  index;
	int     num;
	ItemData	itemData;
};

struct MSG_TARGETINFO
{
	MsgHead		Head;
	UINT		resPlayerID;
	char		AimName[64];
};

struct MSG_DELAY_EXIT
{
	MsgHead		Head;
	int			type;
};

struct MSG_SYS_NPC_DATA
{
	MsgHead		Head;
	UINT	  NPCID;
	int			NpcType;
	int     State[8];
};

struct MSG_SYS_FUN_USE
{
	MsgHead		Head;
	UINT	  NPCID;
	int			NpcType;
	USHORT   Data[20];
	bool    NextWin;
};

struct MSG_KEY_RETURN
{
	MsgHead		Head;
	char     Account[32];
};

struct MSG_PLAYER_BAN
{
	MsgHead		Head;
	char    Str[32];
	char    Reason[32];
	char    GM[32];
	char    Style;
};

struct MSG_MAGIC_APPEND
{
	MsgHead Head;

	int		Money;
	int		EXP;
	int		SkillPoint;
};

struct MSG_CONSORTIA_LIST
{
	MsgHead   Head;
	UINT    uiReceivePlayerID;
	UINT    uiCount;
	Consortia_Member cm[10];
	Consortia_Info  ci;
	MSG_CONSORTIA_LIST()
	{
		::ZeroMemory(cm, sizeof(MSG_CONSORTIA_LIST) * 10);
	}
};

struct MSG_CHECK_INVITER_ID
{
	MsgHead  Head;
	UINT	 HttpID;
};

struct MSG_CHECK_INVITER_RETURN
{
	MsgHead  Head;
	BYTE	 count;
	char	 cName1[ROLENAME_LENGTH];
	char	 cName2[ROLENAME_LENGTH];
	char	 cName3[ROLENAME_LENGTH];
	char	 cName4[ROLENAME_LENGTH];
};

struct MSG_INVITER_TIP 
{
	MsgHead		Head;
	int			Type;
	UINT		Value;
	char		Name[MAX_NAME];
};

#endif 