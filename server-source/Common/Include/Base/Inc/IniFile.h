// **********************************************************************
//
// Copyright (c) 2004
// All Rights Reserved
//
// Author: zeng cs
// Created:	2004/11/03   0:19
//
// **********************************************************************

#pragma once


namespace sbase
{
	const int  _MAX_INISIZE	= 10240;			// INI�ļ��У�������󳤶�
	
	class CIniFile
	{
	public:
		CIniFile(const char * pFilename, const char * pSection);
		~CIniFile();

	public:
		bool	GetString	(const char * pKey, char * szBuf, int nBufSize);
		int		GetInt		(const char * pKey);
		
	private:
		char m_szFilename[_MAX_INISIZE];
		char m_szSection[_MAX_INISIZE];
	};

}


/*	INI�ļ���ʽ�淶

[SECTION]

KEY1=STRING
KEY2=INT

; COMMENT


	˵����

	SECTION�У�	����Ϊ����ʶ�����������ִ�Сд��([)���붥�У�([])�м䲻���пհס�(])������Դ�ע�͡�

	KEY�У�		KEY���붥�У�����Ϊ����ʶ�����������ִ�Сд��
				(=)ǰ�����пհף���������
				STRING������(")��ס������ᱻ�����ַ�����һ���֡�β�ջᱻɾ����
				INTΪʮ��������
				STRING��INT������ע�͡�

	COMMENT��	(;)���붥�У��������������塣
	���У�		�����塣

*/