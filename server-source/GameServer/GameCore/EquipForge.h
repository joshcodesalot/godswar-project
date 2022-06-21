#ifndef _EQUIPFORGE_H_
#define _EQUIPFORGE_H_

#include "Item.h"
#include "..\ErrorMessage.h"

struct EquipForgeBase
{
	EquipForgeBase():Probability(-1),NextID(-1),MaterialType(-1){}
	int	NextID;
	int BadID;
	int Probability;            
	int MaterialType;
	std::vector<int> BaseProyAdd;
	std::vector<int> AppendProyAdd;
	int MaterialProyAdd;

	std::vector<int> Amoney;
	std::vector<int> Bmoney;
	std::vector<int> Cmoney;
};

struct EquipForgeAppend 
{
	EquipForgeAppend():Probability(-1),MaterialCharacter(-1){}
	int Probability;
	int MaterialCharacter;
};

class EquipForge
{
public:

	EquipForge();
	~EquipForge();

	void Clear();

	eError Forge(int type);
	eError SetEquip(Item* equip , int index , int num,int type);
	eError SetMaterial(Item* material , int index , int num,int type);
	eError ClearEquip();
	eError ClearMaterial();
	eError ClearForge();

	void Init(CPlayer* player);
private:
	int GetForgeProbability();

	void ForgeBase();
	void ForgeBaseData();
	void ForgeAppend();
	void ForgeEquip();
	void ForgeMaterial();

	Item* m_Equip[6];		
	Item* m_Material[6];	

	CPlayer* m_Player;
};

#endif
