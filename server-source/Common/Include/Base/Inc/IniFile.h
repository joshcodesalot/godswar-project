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
	const int  _MAX_INISIZE	= 10240;			// INI文件中，串的最大长度
	
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


/*	INI文件格式规范

[SECTION]

KEY1=STRING
KEY2=INT

; COMMENT


	说明：

	SECTION行：	必须为“标识符”，不区分大小写。([)必须顶行，([])中间不能有空白。(])后面可以带注释。

	KEY行：		KEY必须顶行，必须为“标识符”，不区分大小写。
				(=)前后不能有空白，否则会出错。
				STRING不能用(")括住，否则会被当着字符串的一部分。尾空会被删掉。
				INT为十进制数。
				STRING或INT后不能有注释。

	COMMENT：	(;)必须顶行，所有内容无意义。
	空行：		无意义。

*/