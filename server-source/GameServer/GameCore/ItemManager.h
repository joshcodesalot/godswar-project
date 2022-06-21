#ifndef _EQUIPITEMMANAGER_H_
#define _EQUIPITEMMANAGER_H_

#include "Item.h"
#include "EquipForge.h"
#include "../../Common/Include/Base/xml/XMLBase.h"

class CMonster;

#define MAXAPPENDS 255 //255 新附加tao

class ItemManager : public XMLBase
{
public:
	
	void Release();

	static ItemManager* Instance();

	bool LoadItemBaseAttribute(const char* path);
	bool LoadItemAppendAttribute(const char* path);
    bool LoadBijouForge(const char* path);
	bool LoadEquipForge(const char* path);
	bool LoadItemShop(const char* path);
	bool LoadRevive( const char* path);

	void Init();

	Item* CreateItem(CMonster* pMonster);
	Item* CreateItemAppend(Item* item,int CreateNum);
	void  CreateItem(ItemData* pData , Item* pItem);
	void  CreateItemKey( Item*pItem );

	/*--------------------------------------------------------
		功能：根据基础ID获得相应的装备指针
		参数：BaseID :基础ID
		返回：Item*  : 参数ID所对应的道具指针
		作者：李锋军
		备注：知道相应基础ID可以用此方法获得该装备
		日期：2008-3-13
	----------------------------------------------------------*/
	Item* CreateConfimItem( CMonster* pMonster );

	//Item CreateItem(int distribution , int level , ItemCharacter character , int seed);

	EquipForgeBase* GetEquipForgeBase(int id);
	EquipForgeAppend* GetEquipForgeAppend(int id);


	MallBase* GetMallItemAppend(int id);
	AssociationDiscount* GetAssociationMallItemDiscount(int Level);
	map<int , AssociationDiscount> GetAssociationDiscount();

	int GetReviveConsum(int Level,int ReviveType);
private:

	long m_TimeZone;

	list<Item*> m_Items;

	vector<ItemBaseAttribute> m_ItemBaseAttribute;

	ItemAppendAttribute m_ItemAppendAttribute[MAXAPPENDS];//ItemAppendAttribute::ItemAppendTypeCount 新附加tao

	map<int , ItemAppendAttribute*> m_ItemDistributionMap;
	map<int , ItemAppendAttribute*> m_ItemAppendAttributeMap;
	map<int , ItemBaseAttribute*> m_ItemBaseAttributeMap; 

	map<int , EquipForgeBase> m_EquipForgeBase;
	map<int , EquipForgeAppend> m_EquipForgeAppend;

	map<int , MallBase> m_MallBase;
	map<int , AssociationDiscount> m_AssociationDiscount;
	map<int ,ReviveConsum > m_ReviveConsume;

	ItemManager();
	virtual ~ItemManager();

	Item m_Cache;

	void AddBaseAttribute(TiXmlElement* root);

	static ItemManager* m_This;
};


#endif

