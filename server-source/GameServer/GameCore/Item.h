#ifndef _ITEM_H_
#define _ITEM_H_

#include "GameData.h"
#include <vector>
#include <string>
class CPlayer;
 
enum ItemColor
{
	IC_WHITE = 0,
	IC_GREEN,
	IC_BLUE,
	IC_PURPLE,

	IC_COUNT
};

enum MaterialType
{
	MT_BASE= 0,
	MT_APPEND,

	MT_COUNT
};



struct BaseAttribute
{
	__int64 GreenCard;		

	bool Binding;			
	bool Equip;				
	bool Use;				
	bool Treasure;			
	bool Auto;				

	int Money;				
	int Bijou;				

	int Overlap;			
	int	Color;				

	int Character;			

	BaseAttribute()
		:	GreenCard(0) ,
		Binding(false) , 
		Money(0) , Bijou(0) ,
		Equip(false) , Use(false) , Treasure(false) , Auto(false) ,
		Overlap(1) , 
		Color(IC_WHITE) ,
		Character(0)
	{

	}
};

static std::string HANDS_STRING[] = 
{
	"OneHand",			
	"TwoHand",			
	"MainHand",			
	"AuxiliaryHand",	
	""
};

static std::string ITEM_TYPE_STRING[] = 
{
	"weapon",				
	"shield",				

	"head",					
	"amulet",				
	"glove",				
	"armor",				
	"cloth",				
	"cuff",					
	"girdle",				
	"shoes",				
	"leggins",				
	"ring",					

	"consume item",			

	""
};

static std::string ITEM_APPENDTYPE_STRING[] =
{
	"AttackA" ,
	"DefenceA" ,
	"AddMagicAk",
	"AddMagicRec" ,
	"Hit",
	"Miss",
	"FuryAkAdd",
	"FuryAkRec",
	"PhysicalDamage",
	"MagicDamage" ,
	"InjureImbibe",
	"State",
	"StateImmunity" ,
	"MaxHP" ,
	"MaxMP" ,
	"HPRestore" ,
	"MPRestore",
	"AcceptCure",
	"Cure",

	""
};

struct ItemBaseAttribute : public BaseAttribute
{
	enum ItemType
	{
		Weapon = 0,
		Shield,

		Head,
		Amulet,
		Glove,
		Armor,
		Cloth,
		Cuff,
		Girdle,
		Shoes,
		Leggins,
		Ring,

		ConsumeItem,

		ItemTypeCount
	};

	enum EquipHand
	{
		OneHand = 0,		
		TwoHand,			

		MainHand,			
		AuxiliaryHand,		

		EquipHandCount
	};

	int ID;							
	int Faction;					
	bool SkillBook;					

	vector<int> Class;				
	int	eSkillFlag;					
	int	PrevSkillID;				
	int SkillLearnID;				
	int SkillID;					
	int QuestID;

	int Random;						

	int DistributionMin;			
	int DistributionMax;			

	vector<int>		Attack;			
	vector<float>	fAttackRadius;			
	vector<float>   AttackSpeed;	

	vector<int>     Defence;		

	vector<int>		Hit;			
	vector<int>		Miss;			

	vector<int>		MAXHP;				
	vector<int>		MAXMP;			

	vector<float> AcceptCure;			
	vector<float> Cure;					

	vector<float> PhysicalDamage;		
	vector<float> MagicDamage;		

	float PhysicalDamageAbsorb;		
	float MagicDamageAbsorb;			

	int	MaterialType;				
	int MaterialCharacter;			
	int MaterialProyAdd;            

	std::vector<int> MainAttribute;		

	ItemType EType;					

	std::vector<int>	QuestsID;	

	EquipHand eEquipHand;			

	AttackRadius eAttackRadius;		

	vector<int>	MagicAk;			
	vector<int> MagicRec;			
	vector<int>	InjureImbibe;		
	vector<int>	State;				
	vector<int>	StateImmunity;		
	vector<int>	FuryAddAk;			
	vector<int>	FuryAddRec;			
	int			PlayLv;				
	int			Quest;				
	int			GoldBoxIdx;			
	int			BindType;			
	int			Suit;               
	int 		ArmsEff;			

	int			KEY;				
	int			Stone;				
	int			Glory;				
	int			GLevel;				
	int			Discount;			

	int			Mode;							     
	int			ChangeDateType;					    
	int			LastTime_ms;						
	int			TimeStart_ms;					 
	tm			ExpiredTime;					 
	int			Endurance;										
	float		Param;							
	vector<int>	UseSkillID;						 
	int			UndoSkillID;

	ItemBaseAttribute()
		:	ID(0) , SkillBook(false) ,
		MAXHP(0) , MAXMP(0) ,
		Attack(0) , fAttackRadius(0) , AttackSpeed(0) , 
		Defence(0) , 
		Hit(0) , Miss(0) ,
		PhysicalDamageAbsorb(0) , MagicDamageAbsorb(0) , PhysicalDamage(0) , MagicDamage(0) ,
		State(0) , StateImmunity(0) , AcceptCure(0) , Cure(0) , 
		DistributionMin(-1) , DistributionMax(-1) ,
		EType(ItemTypeCount) , eEquipHand(EquipHandCount) , eAttackRadius(AttackRadiusCount) ,
		Suit(0) , PlayLv(-1),
		PrevSkillID(-1) , SkillLearnID(-1) ,
		SkillID(-1) , QuestID(-1), eSkillFlag(SF_Count) ,
		MaterialType(-1) , MaterialCharacter(-1) ,MaterialProyAdd(-1),
		Faction(-1),KEY(-1),GoldBoxIdx(-1),
		Mode(-1),ChangeDateType(0),LastTime_ms(0),TimeStart_ms(0),Endurance(0),Param(0.0),UndoSkillID(0)
	{
		MAXHP.assign( 5, 0 );
		MAXMP.assign( 5, 0 );
		Attack.assign( 5, 0 );
		Defence.assign( 5, 0 );       
		MagicAk.assign( 5, 0 );      
		MagicRec.assign( 5, 0 ); 
		Hit.assign( 5, 0 );				
		Miss.assign( 5, 0 );	
		FuryAddAk.assign( 5, 0 );      
		FuryAddRec.assign( 5, 0 );    
		PhysicalDamage.assign( 5, 0 );
		MagicDamage.assign( 5, 0 );
		InjureImbibe.assign( 5, 0 );
		AcceptCure.assign( 5, 0 );    
		Cure.assign( 5, 0 );         
		State.assign( 5, 0 );     
		StateImmunity.assign( 5, 0 );
		fAttackRadius.assign( 5,0 );
		AttackSpeed.assign(5,0)	;
		UseSkillID.assign(5,0);
	}
};

struct ItemAppendAttribute
{

	enum ItemAppendType
	{
		AttackA,
		DefenceA,
		AddMagicAk,
		AddMagicRec,
		Hit,
		Miss,
		FuryAkAdd,
		FuryAkRec,
		PhysicalDamage,
		MagicDamage,
		InjureImbibe,
		State,
		StateImmunity,
		MaxHP,
		MaxMP,
		HPRestore,
		MPRestore,
		AcceptCure,
		Cure,
		ItemAppendTypeCount
	};

	int ID;

	int Type;  

	std::vector<float> LvInfo;

	std::vector<int> Distribution;
	int DistributionRandom;
	int Random;

	int Flag;
	ItemAppendAttribute()
		:	ID(0) ,Type(ItemAppendTypeCount) ,
		DistributionRandom(0) , Random(0)
	{

	}
};

struct DropConfig
{
	int DropNum;
	int DropProbability;
	int AppendDropProbability;
	int Distribution;

	DropConfig()
		:	DropNum(0) ,
		DropProbability(0) , AppendDropProbability(0) ,
		Distribution(0)
	{

	}
};

struct  ConfirmDrop  : public DropConfig
{
	char  ItemTable[12];
	ConfirmDrop():DropConfig()
	{
	   strcpy(ItemTable,"NULL");
	}
};

class Item
{
public:

	Item();
	~Item();

	bool HasClass(int n);

	void SetItemBaseAttribute(ItemBaseAttribute* attribute);
	ItemBaseAttribute* GetItemBaseAttribute();

	void SetItemAppendAttribute(ItemAppendAttribute* attribute , int index);
	ItemAppendAttribute* GetItemAppendAttribute(int index);
	void ClearAppendAttribute();

	int InsertAppendAttribute(ItemAppendAttribute* attribute);

	ItemData GetItemData();

	int GetEquipLV(){ return m_BaseAttribute->PlayLv;}

	void Init(int Base , int* Append , int BaseLV, int Appendlevel, int Overlap);

	void Clear();
	bool IsClear();

	bool IsFull();

	int m_Overlap;

	bool m_Binding;
	char m_BuyType[3];

	unsigned int m_BuyConsume[3];
	char m_BuyTypeNum;
	int Discount;
	int StoneDiscount;
	int GloryDiscount;

	int BaseLevel;
	int AppLevel;
	long ICdKey;

	tm			ExpiredDate;					
	bool		IsExpired;						
	bool		IsActivated;					


	bool m_Lock;

	void Increase(EquipData* data);
	void Decrease(EquipData* data);

	bool Push(Item* item);

	SkillFalg GetSkillFalg();

protected:
private:

	ItemAppendAttribute* m_ItemAppendAttributes[MAX_EQUIPAPPEND_COUNT];

	ItemBaseAttribute* m_BaseAttribute;
};

struct MallBase 
{
	int Discount;
	char New;
	char Hot;
	MallBase()
		:
	Discount(100),
		New(0) ,
		Hot(0) 
	{

	}
};
struct AssociationDiscount 
{
	char level;
	int StoneDiscount;
	int GloryDiscount;
};

struct ReviveConsum
{
	int money;
	int stone;
};
#endif

