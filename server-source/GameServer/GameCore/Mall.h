#pragma once

#include "Item.h"
#include "../../Common/Include/PyScript/ScriptSystem.h"
#include "NPC.h"

class CPlayer;


class CMall
{
	//��������
	enum 
	{
		ITEM_TRADE_PRESTIGE=0,			//����
		ITEM_TRADE_CONSORTIA_CONTRIBUTE,//���ṱ��
		ITEM_TRADE_MONEY,				//����
		ITEM_TRADE_BIJOU,				//��ʯ
	};

public:

	CMall(void);
	~CMall(void);

public:

	static CMall* Instance();
	void Release();
	void Init();
	////////////////////��Ϸ�̳�//////////////////////////
	eError  GetAthensMallData(CPlayer* player);	//��ȡ�ŵ��̳ǵ���Ʒ�б�
	eError  GetSpartaMallData(CPlayer* player);	//��ȡ˹�ʹ��̳ǵ���Ʒ�б�

	eError  AthensMallSell(CPlayer* player , int index , int num , int count , int base);  //�ŵ䷷����Ʒ
	eError  SpartaMallSell(CPlayer* player , int index , int num , int count , int base);  //˹�ʹﷷ����Ʒ

	bool CheckMallConsume(CPlayer* player,Item* pItem);	//�ж�����Ƿ����㹻�Ļ���
	void UpdataMallConsume(CPlayer* player,Item* pItem); //����������ϵĻ���

	////////////�����̳�////////////////
	eError  GetAthensAssociationData(CPlayer* player);	//��ȡ�ŵ乤���̳ǵ���Ʒ�б�
	eError  GetSpartaAssociationData(CPlayer* player);	//��ȡ˹�ʹ﹤���̳ǵ���Ʒ�б�

	eError  AthensAssociatonSell(CPlayer* player , int index , int num , int count , int base);	//�ŵ䷷�������̳���Ʒ
	eError  SpartaAssociatonSell(CPlayer* player , int index , int num , int count , int base);	//˹�ʹﷷ�������̳���Ʒ

	bool CheckAssociaConsume(CPlayer* player,Item* pItem);	//�ж�����Ƿ����㹻�Ļ���
	void UpdataAssociaConsume(CPlayer* player,Item* pItem);

	void Clear();

private:
	static CMall* g_pMall;
	void LoadAthensMallItemList( const char* name );	//��ȡ�ŵ��̳���Ʒ�ű�
	void LoadSpartaMallItemList( const char* name );	//��ȡ˹�ʹ��̳���Ʒ�ű�

	void LoadAthensAssociationItemList( const char* name ); //��ȡ�ŵ乫���̳���Ʒ�ű�
	void LoadSpartaAssociationItemList( const char* name ); //��ȡ˹�ʹ﹫���̳���Ʒ�ű�

	void LoadAthensMallItemListType(const char* name); //��ȡ�ŵ��̳Ǳ�ǩ�ű�
	void LoadSpartaMallItemListType(const char* name); //��ȡ˹�ʹ��̳Ǳ�ǩ�ű�

	void LoadAssociationItemListType(const char* name); //Ŀǰ����ֻ��һ����ǩ��Ԥ���ӿ�
	//��Ϸ�̳�
	vector< vector<NPCItemData > > m_AthensMallItems;
	vector<char> m_AthensMallSell;
	vector< vector<NPCItemData > > m_SpartaMallItems;
	vector<char> m_SpartaMallSell;

	PyObject* m_MallModule;

	//�����̳�
	vector<NPCItemData> m_AthensAssociationItems;
	vector<char> m_AthensAssociationSell;
	vector<NPCItemData> m_SpartaAssociationItems;
	vector<char> m_SpartaAssociationSell;

	PyObject* m_AssociationModule;


};

