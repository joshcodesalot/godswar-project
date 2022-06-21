//========================================================
//
//    Copyright (c) 2006,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� RepRegion.h
//    ժ    Ҫ �� ������������ģ��
//    
//    ��ǰ�汾 �� 1.00
//    ��    �� �� �ֵ»�
//    ������� �� 2007-01-17
//
//========================================================

#pragma once

#define MAX_REP_REGION_NUMBER		60

class CRegion;
class CWorld;
class CRepRegion
{
public:
	CRepRegion();
	~CRepRegion(void);

public:
	long GetID( void ) { return m_lID; }
	void SetID( long lID ) { m_lID = lID; }
	long GetLogicID(){ return m_LogicID;}
	void SetLogicID( long lID ){ m_LogicID = lID; }

public:		// �߼�
	long Init( CWorld* pWorld );
	long AI( void );
	long Release( void ) { delete this; return 0; }

public:		// ���ݲ���
	long GetOnline( void );


public:
	CRegion*		m_pRegion;

	long			m_lID;
	long            m_LogicID;
	string				m_strName;
	string				m_strNameCH;
	string				m_strNumenName;		// Numen/Boss
	CWorld*				m_pWorld;

public:
	void				SetName( const char* pName ) { m_strName = pName; }
	void				SetNameCH( const char* pName ) { m_strNameCH = pName; }
	void				SetNumenName( const char* pName ) { m_strNumenName = pName; }

	string				GetName( void ) { return m_strName; }
	string				GetNameCH( void ) { return m_strNameCH; }
	string				GetNumenName( void ) { return m_strNumenName; }

};
