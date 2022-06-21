#pragma once

#include ".\GameCore\GameData.h"
#include "..\Common\MsgTypes.h"
#include "./GameCore/player.h"
class CDial
{
public:
	CDial();
	~CDial();

	struct DialItemData
	{

		ItemData item_data;						//ʵ�ʵ���Ʒ����
		UINT DialID;							//�������ֱ���
		BYTE Nonsuch;							//�Ǽ�Ʒô��
		UINT RandInDial;						//��Χ����
		UINT Probability;						//���ɼ���
		UINT ItemID;							//��ƷDNA
		int LV; 						//��Ʒ�ȼ�
	};
	class DialDefaultItemData:public ItemData
	{
	public:
		UINT DialID;							//�������ֱ���
		UINT ItemID;							//��ƷDNA
	};
	void Init();								//��ʼ��
	static  CDial* Instance();						//ʵ��
	void RandomItemInDial(UINT base_id);					//������Χ����
	//del warning
	int RandomItem(UINT base_id);						//�������������Ʒ
	UINT DefaultItem(UINT base_id);						//����Ĭ����Ʒ
	DialDefaultItemData& CDial::GetDefaultItemData(UINT base_id);		//��ȡĬ����Ʒ
	UINT* GetInDialItemID();						//��ȡ��Χ��ƷID
	void SetPlayer(CPlayer* pPlayer){pLocalPlayer = pPlayer;}		//���õ�ǰ���
	UINT SecondItem(UINT base_id);						//��ö������Ʒ
	eError  isGoldBoxIDExist(BYTE Iter,UINT base_id);			//�Ƿ���ڸñ���
	bool  isMsgWrong(MSG_GOLD_BOX* pGoldBoxMsg);				//��֤��Ϣ�ĺϷ���
	std::map<UINT,DialDefaultItemData>m_DefaultItemDataMap;			//����Ĭ����Ʒ

private:
//	UINT m_InDialItemID[DIALITEMCOUNT];					//��Χ��ƷID
	CPlayer* pLocalPlayer;							//��ǰ��ɫ
	std::map<UINT,std::map<UINT,DialItemData>>m_ItemDataMap;		//��Ʒ�ͱ����Ӧ��
	std::map<UINT,std::map<UINT,DialItemData>>m_NonsuchItemDataMap;		//��Ʒ��
	std::map<UINT,UINT> m_SumOfRandInDial;					//��Χ�ܼ���
	std::map<UINT,UINT> m_SumOfNonsuchInDial;				//��Ʒ��Χ�ܼ���
	DialItemData m_DialItemData[DIALITEMCOUNT];				//��Χ��Ʒ
	static CDial* m_This;

};