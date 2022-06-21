#ifndef _ERRORINFO_H_
#define _ERRORINFO_H_

#include "stdafx.h"

enum eError
{
	NO_MSG_ERRO	= 0,

    //_MSG_CREATE_ROLE error  
	MSG_ERRO_0001	= 0x0001,    		//�ɹ�
	MSG_ERRO_0002	,    		        //�ַ�����������
	MSG_ERRO_0003	,    		        //�����Ƿ��ַ�
	MSG_ERRO_0004	,    		        //��ɫ������������
	MSG_ERRO_0005	,    		        //�ظ�ע��
	MSG_ERRO_0006	,					//���ǻ᳤��Ϊ����Ļ�Ա���� ����ɾ���������ɾ����ɫ
	MSG_ERRO_0007	,					//ɾ����ɫʧ��
	MSG_ERRO_0008	,					//ɾ����ɫ�ɹ�

    //_MSG_DELETE_ROLE error
	MSG_ERRO_0011	= 0x0011,

    //_MSG_WALK_BEGIN error
	MSG_ERRO_0020	= 0x0020,          //��̯�����ƶ�

	//_MSG_WALK_END error
	MSG_ERRO_002F	= 0x002F,    

	//_MSG_AVATAR_INFO error
	MSG_ERRO_003E	= 0x003E,

	//_MSG_FIGHT error
	MSG_ERRO_004D	= 0x004D, 

	//_MSG_ATTACK error
	MSG_ERRO_005C = 0x005C,        //Ŀ�겻����

	//_MSG_SKILL
	MSG_ERRO_006A	= 0x006A,		         //��̯��
	MSG_ERRO_006B	= 0x006B,		 //����̫Զ
	MSG_ERRO_006C	,		         //ħ������
	MSG_ERRO_006D	,		         //��ȴ��
	MSG_ERRO_006E	,		         //װ������
	MSG_ERRO_006F	,		         //�޴˼���
	

	//_MSG_LEARN
	MSG_ERRO_007A	= 0x007A,		

	//_MSG_SKILLBACKUP
	MSG_ERRO_0089	= 0x0089,

	//_MSG_SKILL_UPGRADE
	MSG_ERRO_0098	= 0x0098,       //��������������

	//_MSG_SKILL_INTERRUPT
	MSG_ERRO_00A7	= 0x00A7,

	//_MSG_EFFECT
	MSG_ERRO_00B6	= 0x00B6,

	//_MSG_TALK
	MSG_ERRO_00C5	= 0x00C5,

	//_MSG_PICKUPDROPS
	MSG_ERRO_00D4	= 0x00D4,    

	//_MSG_USEOREQUIP
	MSG_ERRO_00E3	= 0x00E3,      

	//_MSG_MOVEITEM
	MSG_ERRO_00F2	= 0x00F2,

	//_MSG_SELL
	MSG_ERRO_0101	= 0x0101,

    //_MSG_STORAGEITEM
	MSG_ERRO_0110	= 0x0110,

	//_MSG_STALL
	MSG_ERRO_011F	= 0x011F,

	//_MSG_STALLITEM
	MSG_ERRO_012E	= 0x012E,

	//_MSG_STALLADDITEM
	MSG_ERRO_013D	= 0x013D,

	//_MSG_STALLDELITEM
	MSG_ERRO_014C	= 0x014C,

	//_MSG_STALLBUYITEM
	MSG_ERRO_015A	= 0x015A,		//��Ʒ������ goods->item == NULL
	MSG_ERRO_015B	= 0x015B,		//��̯�ɹ����ǲ�������Ϣ
	MSG_ERRO_015C	,				//����Ʒ�Ѿ����ػ������ߣ�����̯����ѯ
	MSG_ERRO_015D	,				//����

	MSG_ERRO_015F	,				//�Ƿ������̯��Ʒ����������

	//_MSG_TALKNPC
	MSG_ERRO_016A	= 0x016A,       //��Ӫ����
	MSG_ERRO_016B	= 0x016B,		//���ͨ��
	MSG_ERRO_016C	= 0x016C,		//���ǻ᳤���޷���
	MSG_ERRO_016D	= 0x016D,		//û�����

	//_MSG_NPCDATA
	MSG_ERRO_0179	= 0x0179,

	//_MSG_NPCSELL
	MSG_ERRO_0188	= 0x0188,		//���ڸ������µ���Ʒ�б�

	//_MSG_MALLSELL
	MSG_ERRO_0197	= 0x0197,		//���ڽ����У����ܹ�����Ʒ
	MSG_ERRO_0198	= 0x0198,		//��û���㹻�Ļ���
	MSG_ERRO_0199	= 0x0199,		//����û���㹻�Ŀռ�
	MSG_ERRO_019A	= 0x019A,

	//_MSG_ASSOCIATIONSELL
	MSG_ERRO_01A6	= 0x01A6,

	//_MSG_MALLITEMDATA
	MSG_ERRO_01B5	= 0x01B5,

	//_MSG_ASSOCIATIONITEMDATA
	MSG_ERRO_01C4	= 0x01C4,

	//_MSG_FINDQUEST
	MSG_ERRO_01D3	= 0x01D3,

	//_MSG_NPCACCEPTQUEST
	MSG_ERRO_01E2	= 0x01E2,

	//_MSG_NPCCANCELQUEST
	MSG_ERRO_01F1	= 0x01F1,

	//_MSG_NPCCOMPLETEQUEST
	MSG_ERRO_0200	= 0x0200,		//û����Ʒ
	MSG_ERRO_0201	,		        //��Ʒ����

	//_MSG_NPCQUESTVIEW
	MSG_ERRO_020F	= 0x020F,

    //_MSG_RELATION_REQUEST, 
	MSG_ERRO_021E	= 0x021E,      //��Ҳ�����
	MSG_ERRO_021F   ,              //��ͬ��Ӫ
	MSG_ERRO_0220,				   //������Ѿ�����ĺ���
	MSG_ERRO_0221,				   //������Ѿ�����ĺ������б���
	MSG_ERRO_0222,				   //�����б�����
	MSG_ERRO_0223,				   //�������б�����
	MSG_ERRO_O224,				   //���Ѿ��Ѹ���Ҽ����������
	MSG_ERRO_0225,				   //������Ѿ�����ĺ����б���
	
    //_MSG_RELATION_RESPONSE,
    MSG_ERRO_022D	= 0x022D,

	//_MSG_RELATION_DELETE,
	MSG_ERRO_023C	= 0x023C,

	//_MSG_TRADE,  
	MSG_ERRO_024B	= 0x024B,		//���æ
	MSG_ERRO_024C	= 0x024C,		//����������,���׽���
	MSG_ERRO_024D	= 0x024D,		//û���㹻�Ľ�Ǯ
	MSG_ERRO_024E	= 0x024E,		//û���㹻�ı�ʯ
	MSG_ERRO_024F	= 0x024F,		//����ȡ��
	

	//_MSG_TRADE_MONEY,             	
	MSG_ERRO_025A	= 0x025A,

	//_MSG_TRADE_ADDITEM,           	
	MSG_ERRO_0269	= 0x0269,

	//_MSG_TRADE_REMOVEITEM,        	
	MSG_ERRO_0278	= 0x0278,

	//_MSG_EQUIPFORGE,              	
	MSG_ERRO_0287	= 0x0287,

	//_MSG_EQUIPFORGE_EQUIP,        	
	MSG_ERRO_0296	= 0x0296,

	//_MSG_EQUIPFORGE_MATERIAL,     	
	MSG_ERRO_02A5	= 0x02A5,

	//_MSG_EQUIPFORGE_EQUIPCANCEL,  	
	MSG_ERRO_02B4	= 0x02B4,

	//_MSG_EQUIPFORGE_MATERIALCANCEL,   
	MSG_ERRO_02C3	= 0x02C3,

	//_MSG_EQUIPFORGE_CANCEL,           
	MSG_ERRO_02D2	= 0x02D2,

	//_MSG_TEAM_INVITE,                 
	MSG_ERRO_02E1	= 0x02E1,		//

	//_MSG_TEAM_REJECT,                 
	MSG_ERRO_02F0	= 0x02F0,

	//_MSG_TEAM_ADD,                    
	MSG_ERRO_02FF	= 0x02FF,

	//_MSG_TEAM_DELETE,                 
	MSG_ERRO_030E	= 0x030E,

	//_MSG_TEAM_REPLACELEADER,          
	MSG_ERRO_031D	= 0x031D,

	//_MSG_TEAM_DISSOLVE,               
	MSG_ERRO_032C	= 0x032C,

	//_MSG_TEAM_LEAVE,                  
	MSG_ERRO_033B	= 0x033B,

	//_MSG_BACKHOME,                    
	MSG_ERRO_034A	= 0x034A,

	//_MSG_CONSORTIA_CREATE,            
	MSG_ERRO_0359	= 0x0359,       //���й���
	MSG_ERRO_035A	,               //��Դ����
	MSG_ERRO_035B	,               //�ȼ�����
	MSG_ERRO_035C	,               //�ù����Ѵ���
	MSG_ERRO_035D	,               //��������Ϊ��
	MSG_ERRO_035E   ,               //û�д�������ĵ��� <������>

	//_MSG_CONSORTIA_INVITE,            
	MSG_ERRO_0368	= 0x0368,       //�����������й���
	MSG_ERRO_0369	,               //��ͬ��Ӫ
	MSG_ERRO_036A	,               //û��Ȩ��
	MSG_ERRO_036B	,               //������Ա
	MSG_ERRO_036C	,               //�Ѿ��ǹ����Ա

	//_MSG_CONSORTIA_DISMISS,           
	MSG_ERRO_0377	= 0x0377,       //�ǻ᳤

	//_MSG_CONSORTIA_RESPONSE,          
	MSG_ERRO_0386	= 0x0386,

	//_MSG_CONSORTIA_EXIT,              
	MSG_ERRO_0395	= 0x0395,       //���Լ�һ�˳�Ա

	//_MSG_CONSORTIA_TEXT, 
	MSG_ERRO_03A4	= 0x03A4,      //�������᳤

	//_MSG_CONSORTIA_DUTY,              
	MSG_ERRO_03B3	= 0x03B3,

	//_MSG_CONSORTIA_MEMBER_DEL,        
	MSG_ERRO_03C2	= 0x03C2,     //��������ְ��û��Ȩ��
    MSG_ERRO_03C3	,             //����ɾ���Լ�ƽ�����߸����Լ������
	MSG_ERRO_O3C4	,			  //�����Ѿ����ȼ�
	MSG_ERRO_03C5	,			  //�㻹û�н�������
	MSG_ERRO_03C6	,			  //���������ɹ�
	MSG_ERRO_03C7	,			  //����������ʽ��ʯ����

	//_MSG_CONSORTIA_LVUP,				
	MSG_ERRO_03D1	= 0x03D1,

	//_MSG_ALTAR_CREATE,				
	MSG_ERRO_03E0	= 0x03E0,

	//_MSG_ALTAR_LVUP,					
	MSG_ERRO_03EF	= 0x03EF,

	//_MSG_ALTAR_OBLATION,				
	MSG_ERRO_03FE	= 0x03FE,

	//_MSG_CRETIT_EXCHANGE,				
	MSG_ERO_040D	= 0x040D,

	//_MSG_GOLD_BOX
	MSG_ERRO_041C	= 0x041C,		//�ظ�����
	MSG_ERRO_041D	,		        //��Ʒ��ȫ
	MSG_ERRO_041E	,		        //�ո���
	MSG_ERRO_041F	,		        //���䲻ƥ��
	MSG_ERRO_0420	,		        //������δ����

	//_MSG_EXPLORER_QUEST
	MSG_ERRO_41	= 0x042B,
};

#endif