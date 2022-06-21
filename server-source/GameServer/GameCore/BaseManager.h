//========================================================
//
//    Copyright (c) 2008,�������˹�����
//    All rights reserved.
//
//    �ļ����� �� BaseManager.h 
//    ժ    Ҫ �� ���������
//    
//    ��ǰ�汾 �� 1.0
//    ��    �� �� ����ǿ
//    ������� �� 2008-6-16
//    ˵    �� ��
// 
//========================================================

#ifndef _BASE_MANAGER_H_
#define _BASE_MANAGER_H_

//*****************************************
//�� �֣�CBaseManager
//�� �棺���������		
//*****************************************
class CWorld;
class CBaseManager
{
public:

	CBaseManager();

	CBaseManager(CWorld* pWorld);

	virtual ~CBaseManager();

protected:

	CWorld* m_pWorld;
	
}; // end class CBaseManager

#endif // _BASE_MANAGER_H_

