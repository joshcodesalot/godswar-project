#include "stdafx.h"
#include "Dial.h"
#include "..\Common\MsgTypes.h"
#include "./GameCore/ItemManager.h"
extern MSG_USEOREQUIP_ITEM msg_useorequip_item;
CDial* CDial::m_This = NULL;
CDial::CDial()
{
	pLocalPlayer = NULL;
}

CDial::~CDial()
{

}



void CDial::Init()
{
	char path_dial[256] ={};
	char buf[256] = {};
	DialItemData temp_dial_item_data;
	DialDefaultItemData temp_default_item_data;

	sprintf( path_dial, "./Drop/dial.ini");
	sbase::CIni iniDial( path_dial, false );

	int FileCount = iniDial.GetData("FileCount", "Count" );
	for(int i=0;i<FileCount;i++)
	{
		sprintf( buf, "ID%d",i);
		int base_id = iniDial.GetData("ID" , buf );

		sprintf(path_dial,"./Drop/dial%d.ini",base_id);
		sbase::CIni iniTempDial( path_dial, false );

		int BoxID = iniTempDial.GetData("ItemCount", "BoxID");

		//��ʼ��DefaultItemDataMap
		temp_default_item_data.DialID = BoxID;
		temp_default_item_data.AppendLV = iniTempDial.GetData("DefaultItem" , "AppLV" );
		temp_default_item_data.BaseLV = iniTempDial.GetData("DefaultItem" , "BaseLV" );
		temp_default_item_data.Overlap = iniTempDial.GetData("DefaultItem" , "OverLap" );
		temp_default_item_data.Base = iniTempDial.GetData("DefaultItem" , "BaseID" );
		temp_default_item_data.Binding = iniTempDial.GetData("DefaultItem" , "Binding" );
		temp_default_item_data.ItemID = iniTempDial.GetData("DefaultItem" , "ItemID" );

		const char *temp_str = iniTempDial.GetString("DefaultItem" , "Append" );
		if(temp_str)
		{
			string Append = temp_str;
			if(!Append.empty())
			{
				int ValuesIndex = (int)Append.find_first_of(',');
				int m_AppendCount = 0;
				while ( ValuesIndex > 0  )
				{
					std::string ValuesTempstr;
					ValuesTempstr.assign( Append, 0, ValuesIndex );
					temp_default_item_data.Append[m_AppendCount] = atoi( ValuesTempstr.c_str() );
					Append.erase( 0, ValuesIndex+1 );
					ValuesIndex = (int)Append.find_first_of(',');
					m_AppendCount++;
				}

				//del warning
				if ( Append.length() != 0 )
				{
					temp_default_item_data.Append[m_AppendCount] = atoi( Append.c_str() );
				}				
// 				if ( Append.c_str() != "" )
// 				{
// 					temp_default_item_data.Append[m_AppendCount] = atoi( Append.c_str() );
// 				}
			}
		}
		
		this->Instance()->m_DefaultItemDataMap[base_id]  = temp_default_item_data;

		//��ʼ��ItemDataMap NonsuchItemDataMap
		int count = iniTempDial.GetData( "ItemCount", "Count" );
		for (int i = 0 ;i<count;i++)
		{
			memset(&temp_dial_item_data,0,sizeof(DialItemData));
			memset(temp_dial_item_data.item_data.Append,-1,sizeof(temp_dial_item_data.item_data.Append));
			sprintf( buf, "DialItem%d", i );

			//		const char* pObjectName = iniMonster.GetString(buf,"Object");
			temp_dial_item_data.DialID = BoxID;
			temp_dial_item_data.ItemID = iniTempDial.GetData(buf , "ItemID" );
			temp_dial_item_data.Nonsuch = iniTempDial.GetData(buf , "Nonsuch" );
			temp_dial_item_data.RandInDial = iniTempDial.GetData(buf , "RandInDial" );
			temp_dial_item_data.Probability = iniTempDial.GetData(buf , "Probability" );
			temp_dial_item_data.item_data.Base = iniTempDial.GetData(buf , "BaseID" );
			temp_dial_item_data.item_data.Binding = iniTempDial.GetData(buf , "Binding" );
			temp_dial_item_data.item_data.AppendLV = iniTempDial.GetData(buf , "AppLV" );
			temp_dial_item_data.item_data.BaseLV = iniTempDial.GetData(buf , "BaseLV" );
			temp_dial_item_data.item_data.Overlap = iniTempDial.GetData(buf , "OverLap" );
			temp_dial_item_data.LV = -1;

			const char *temp_str = iniTempDial.GetString(buf , "Append" );
			if(temp_str)
			{
				string Append = temp_str;
				if(!Append.empty())
				{
					int ValuesIndex = (int)Append.find_first_of(',');
					int m_AppendCount = 0;
					while ( ValuesIndex > 0  )
					{
						std::string ValuesTempstr;
						ValuesTempstr.assign( Append, 0, ValuesIndex );
						temp_dial_item_data.item_data.Append[m_AppendCount] = atoi( ValuesTempstr.c_str() );
						Append.erase( 0, ValuesIndex+1 );
						ValuesIndex = (int)Append.find_first_of(',');
						m_AppendCount++;
					}

					//del warning
					if ( Append.length() != 0 )
					{
						temp_dial_item_data.item_data.Append[m_AppendCount] = atoi( Append.c_str() );
					}

// 					if ( Append.c_str() != "" )
// 					{
// 						temp_dial_item_data.item_data.Append[m_AppendCount] = atoi( Append.c_str() );
// 					}
				}
			}

			if(temp_dial_item_data.Nonsuch)//��Ʒô��
			{
				m_NonsuchItemDataMap[base_id][temp_dial_item_data.ItemID] = temp_dial_item_data;
			}
			else
			{
				m_ItemDataMap[base_id][temp_dial_item_data.ItemID] = temp_dial_item_data;
			}
		}
	}

	//��ʼ��SumOfRandInDial
	std::map<UINT,std::map<UINT,DialItemData>>::iterator itor,itor_end;
	itor_end = this->Instance()->m_ItemDataMap.end();
	for(itor = this->Instance()->m_ItemDataMap.begin();itor != itor_end; itor++)
	{
		std::map<UINT,DialItemData>::iterator iter  , iter_end = (*itor).second.end();
		int sum_rand(0);
		for(iter =  (*itor).second.begin();iter != iter_end;iter++)
		{
			sum_rand += (*iter).second.RandInDial;
		}

		this->Instance()->m_SumOfRandInDial[(*itor).first] = sum_rand;
	}

	//��ʼ��SumOfNonsuchInDial
	itor_end = this->Instance()->m_NonsuchItemDataMap.end();
	for(itor = this->Instance()->m_NonsuchItemDataMap.begin();itor != itor_end; itor++)
	{
		std::map<UINT,DialItemData>::iterator iter  , iter_end = (*itor).second.end();
		int sum_rand(0);
		for(iter =  (*itor).second.begin();iter != iter_end;iter++)
		{
			sum_rand += (*iter).second.RandInDial;
		}

		this->Instance()->m_SumOfNonsuchInDial[(*itor).first] = sum_rand;
	}


}
CDial* CDial::Instance()
{
	if (!m_This)
		m_This = new CDial();

	return m_This;
}

void CDial::RandomItemInDial(UINT base_id)			//������Χ����
{

//	CPlayer* pPlayer = GetPlayerFromSocket( pPortMsg->GetTo() );
	int sum_rand(0),temp_lv= pLocalPlayer->GetRank();
	bool is_continue(false);			//�ȼ��ж���ͨ��������
	DialItemData dial_item_data[DIALITEMCOUNT]={};
	for(int i=0 ;i<DIALITEMCOUNT;i++) 
	{
		int rand(0);
		std::map<UINT,DialItemData>::iterator iter,iter_end ;
		if(i<DIALITEMCOUNT-3)
		{
			rand = sbase::RandGet(this->m_SumOfRandInDial[base_id],true);
			iter = m_ItemDataMap[base_id].begin(), iter_end = m_ItemDataMap[base_id].end();
		}
		else
		{
			rand = sbase::RandGet(this->m_SumOfNonsuchInDial[base_id],true);
			iter = m_NonsuchItemDataMap[base_id].begin(), iter_end = m_NonsuchItemDataMap[base_id].end();
		}
		int temp_rand(0);
		for(;iter != iter_end;iter++)
		{
			temp_rand += (*iter).second.RandInDial;
			if(temp_rand>= rand)
			{
				int num = sizeof(DialItemData);

				if((*iter).second.LV>=0)
				{
					if((temp_lv>((*iter).second.LV+15))||
						(temp_lv<((*iter).second.LV-15)))
						
					{
						is_continue = true;
						break;
					}
				}

				memcpy(&(m_DialItemData[i]),&((*iter).second),num);
				is_continue = false;
				break;
			}
		}
		if(is_continue)		//�ȼ���ƥ�� ��ѡ
		{
			i--;
			continue;
		}
//		assert(&(m_DialItemData[i]));
		sum_rand += m_DialItemData[i].Probability;				//��ȡ��Χ��Ʒ�������
//		m_InDialItemID[i] = m_DialItemData[i].ItemID;
		pLocalPlayer->SetInDialItemID(i,m_DialItemData[i].ItemID);
	}
	pLocalPlayer->SetSumRand(sum_rand);
}

CDial::DialDefaultItemData& CDial::GetDefaultItemData(UINT base_id)			//��ȡĬ�ϵ���ID
{
	return CDial::Instance()->m_DefaultItemDataMap[base_id];
}

UINT* CDial::GetInDialItemID()								//��ȡ��Χ����ID
{
	return pLocalPlayer->GetInDialItemID();
}

int CDial::RandomItem(UINT base_id)							//�������������Ʒ
{
	int rand = sbase::RandGet(pLocalPlayer->GetSumRand());
	UINT* temp_item_id = pLocalPlayer->GetInDialItemID();


	UINT key_id = m_DefaultItemDataMap[base_id].DialID;
//	if(!pLocalPlayer->ExpendGoldBoxId(base_id,key_id)){return -1;}			//����Կ�׺ͱ���

	int temp_rand(0);
	for(int i=0 ;i<DIALITEMCOUNT;i++)
	{
		if(i<DIALITEMCOUNT-3)
		{
			temp_rand += m_ItemDataMap[base_id][temp_item_id[i]].Probability;
		}
		else
		{
			temp_rand += m_NonsuchItemDataMap[base_id][temp_item_id[i]].Probability;
		}
		if(temp_rand>= rand)
		{

			Item item;
			if(i<DIALITEMCOUNT-3)
			{
				ItemManager::Instance()->CreateItem(&(m_ItemDataMap[base_id][temp_item_id[i]].item_data), &item);
			}
			else
			{
				ItemManager::Instance()->CreateItem(&(m_NonsuchItemDataMap[base_id][temp_item_id[i]].item_data), &item);
			}

			pLocalPlayer->SetCurBoxItem(item);
//��ʱ			return i;
			return m_DialItemData[i].ItemID;
//			if(pLocalPlayer->AddItem(&item)){return i;}
		}
	}
	return -1;
}

UINT CDial::DefaultItem(UINT base_id)							//����Ĭ����Ʒ
{
	UINT* temp_item_id = pLocalPlayer->GetInDialItemID();
	UINT key_id = m_DefaultItemDataMap[base_id].DialID;
	int free = pLocalPlayer->GetFreeItemCount();

	if (!free)
		return 0;
//	if(!pLocalPlayer->ExpendGoldBoxId(base_id,key_id)){return 0;}			//����Կ�׺ͱ���
	Item item;
	ItemManager::Instance()->CreateItem(&(m_DefaultItemDataMap[base_id]), &item);
	if(pLocalPlayer->AddItem(&item))
	{
		return m_DefaultItemDataMap[base_id].ItemID;
	}

	return 0;
}
UINT CDial::SecondItem(UINT base_id)					//��ö������Ʒ
{
	int rand(0),sum(0);//sbase::RandGet(pLocalPlayer->GetNonsuchSumRand());
	std::map<UINT,DialItemData>::iterator iter = m_NonsuchItemDataMap[base_id].begin(), iter_end = m_NonsuchItemDataMap[base_id].end();
	for(;iter != iter_end;iter++)
	{
		sum += (*iter).second.Probability;
	}

	rand = sbase::RandGet(sum);
	int temp_rand(0);
	Item item;
	for(iter = m_NonsuchItemDataMap[base_id].begin();iter != iter_end;iter++)
	{

		temp_rand += (*iter).second.Probability;
//		m_NonsuchItemDataMap[GoldBoxID][temp_item_id[i]].Probability;
		if(temp_rand>= rand)
		{
			ItemManager::Instance()->CreateItem(&((*iter).second.item_data), &item);
			pLocalPlayer->AddItem(&item);

			return (*iter).second.ItemID;
		}
	}
	return 0;
}
eError CDial::isGoldBoxIDExist(BYTE Iter,UINT base_id)					//�Ƿ���ڸñ���
{
	if(m_NonsuchItemDataMap.find(base_id) == m_NonsuchItemDataMap.end() ||
		m_ItemDataMap.find(base_id) == m_ItemDataMap.end() ||
		m_DefaultItemDataMap.find(base_id) == m_DefaultItemDataMap.end())
	{
		pLocalPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_GOLD_BOX] Iter%d GoldBoxId%d", pLocalPlayer->GetAccounts(), pLocalPlayer->GetName(),
			Iter,base_id);
		return MSG_ERRO_041D;
	}
	else {return NO_MSG_ERRO;}
}

bool  CDial::isMsgWrong(MSG_GOLD_BOX* pGoldBoxMsg)				//��֤��Ϣ�ĺϷ���
{
	if(pGoldBoxMsg->Type != 8 ||  //��ʱ�Ķ���8
		pGoldBoxMsg->Iter<0 ||
		pGoldBoxMsg->Iter>=MAX_BAG_GRID*MAX_BAGS ||
		pGoldBoxMsg->GoldBoxId>10000)
	{
		pLocalPlayer->AddDanger();
		sbase::LogException("Message type Exception ,Accounts : %s, Role: %s,[_MSG_GOLD_BOX] Type:%d Iter%d GoldBoxId%d", pLocalPlayer->GetAccounts(), pLocalPlayer->GetName(),
			pGoldBoxMsg->Type,pGoldBoxMsg->Iter,pGoldBoxMsg->GoldBoxId);
		return true;
	}
	return false;
}
