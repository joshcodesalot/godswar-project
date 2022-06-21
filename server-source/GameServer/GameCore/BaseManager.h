//========================================================
//
//    Copyright (c) 2008,福州天盟工作室
//    All rights reserved.
//
//    文件名称 ： BaseManager.h 
//    摘    要 ： 管理类基类
//    
//    当前版本 ： 1.0
//    作    者 ： 蔡智强
//    完成日期 ： 2008-6-16
//    说    明 ：
// 
//========================================================

#ifndef _BASE_MANAGER_H_
#define _BASE_MANAGER_H_

//*****************************************
//名 字：CBaseManager
//描 绘：管理类基类		
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

