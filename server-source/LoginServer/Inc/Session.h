//========================================================
//
//    Copyright (c) 2006,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� Session.h
//    ժ    Ҫ �� ����������ת
//    
//    ��ǰ�汾 �� 1.01
//    ��    �� �� ����
//    ������� �� 2007-05-30
//
//========================================================
#ifndef INC_SESSION_H
#define INC_SESSION_H

#ifdef _WIN32
#pragma once
#endif

#include "stdafx.h"
#include "..\..\Common/Include/Base\Inc\mempool.h"
#include ".\LoginLogic.h"

 namespace login
 {
 	class CLogin;
 }
 
 static char*  SQL_SELECT_ACCOUNT_INFO   =  "CALL SP_SELECT_ACCOUNTS_INFO(\'%s\',\'%s\',\'%s\');"; 
 
 class  CSession
 {
 public:
 	CSession( const char *Name, const char *PassWord, void *Socket, login::CLogin &Logion );
 	~CSession();
 	CSession & operator=( const CSession & ) {}
 
 	void                OnDBRequest() const;
 	void                Release(){ delete this; };
 	const char*         GetAccount(){ return m_Name; };
 	void*               GetSocket(){ return m_Socket; };
 	void                GSCheckFlag(){ m_bGS = true; }

 	bool                GetGSCheckFlag(){ return m_bGS; };

	//add by czq
	void				AccountCheckFlag(){m_bDelAccout = true;}
	bool				GetAccountFlag(){return m_bDelAccout;}
 public:
 	//��¼����
 	static   void       OnDBResponse( dbtool::PSQL_RESULT result );
 protected:
 private:
 	bool                m_bGS;
 	char                m_Name[32];
 	char                m_PassWord[32];
 	void*               m_Socket;
 	login::CLogin       &m_Logion;
	bool				m_bDelAccout;
 	DECLARE_FIXEDSIZE_ALLOCATOR(CSession);
 };


#endif