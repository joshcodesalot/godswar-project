#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料

#include "stdafx.h"
#include <stdlib.h>
#include <list>
#include <shlwapi.h>
#include <comutil.h>
#include "KeyWords.h"
#include "..\Dial.h"

#pragma warning(disable:4996)
using namespace std;
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "kernel32.lib")

CKeyWords * CKeyWords::m_KetWords = NULL;

 CKeyWords *  CKeyWords::Instance()
{
   if ( m_KetWords == NULL )
   {
	   m_KetWords = new CKeyWords;
   }
   return  m_KetWords;
}

 bool CKeyWords::Init()
 {
 	 CDial::Instance()->Init();
	 return !m_KetWords->ReadFile("Filter.bat" );
 }


CKeyWords::CKeyWords(void)
: KeyWordsNum(0)
{
}

CKeyWords::~CKeyWords(void)
{
}
// //读取关键字
int CKeyWords::ReadFile(char* FileName)
{
	FILE *stream;
	wstring temp;
	char mychar[256];
	int i;
	list<wstring>::const_iterator iter;

	if( (stream  = fopen( FileName, "r" )) == NULL )
	{printf( "The file '%s' was not opened\n",FileName );return 1;}

	for(this->KeyWordsNum=0;;)
	{
		if(fscanf( stream, "%s",mychar )==EOF){break;}
		temp = _com_util::ConvertStringToBSTR(mychar);//转换程宽字符

		lKeyWordsList.push_back(temp);
		this->KeyWordsNum++;
	}
	fclose(stream);
	if(this->KeyWordsNum==0){return 1;}//没有关键词
//	lKeyWordsList.sort(mysort());//排序
	lKeyWordsList.sort();//排序


	for(iter = this->lKeyWordsList.begin(),i=0;iter != this->lKeyWordsList.end();iter++,i++)
	{
		this->sshort.push_back((*iter)[0]);
		this->vKeyWordsList.push_back(*iter);
	}
	return 0;
}
bool CKeyWords::FindKeyWord(char* pchr)
{
	if(this->FindBlank(pchr)){return true;}
	int index = 0,isfind =0,ilen=0,keylen=0;
	wchar_t *pwchr_temp = new wchar_t[256],*temp,*wchr;
	char chr[2]="*";
	unsigned int wildcard = 0;

	wstring str,str1,str2;
	memset(pwchr_temp,0,sizeof(wchar_t)*256);
	wchr = _com_util::ConvertStringToBSTR(chr);//转换程宽字符
	temp = _com_util::ConvertStringToBSTR(pchr);//转换程宽字符

	ilen = (int)wcslen(temp);
	for(int i=0;i<ilen;i++)
	{
		isfind=0;
		for(index=0;index<this->KeyWordsNum;index++)
		{
			if(isfind == -1){break;}
			if(temp[i] == this->sshort[index])//索引匹配
			{
				isfind = 1;
				str = this->vKeyWordsList[index];
				keylen = (int)str.length();
				if((i+keylen)<=ilen)//不超长
				{
					memset(pwchr_temp,0,sizeof(wchar_t)*256);
					wcsncpy(pwchr_temp,temp+i,keylen);
					if(wcscmp(pwchr_temp,str.c_str())==0)
					{
						{delete[] pwchr_temp; return true;}
					}
					//屏蔽通配符类关键字 法%功
					wildcard = (int)str.find(L"%",0);
					if(wildcard!=str.npos)
					{
						//分别获取两个子串
						str1 = str;
						str1.erase(wildcard,str.length()-wildcard);
						str2 = str;
						str2.erase(0,wildcard+1);

						memset(pwchr_temp,0,sizeof(wchar_t)*256);
						wcsncpy(pwchr_temp,temp+i,str1.length());
						if(wcscmp(pwchr_temp,str1.c_str())==0)
						{
							wcscpy(pwchr_temp,str2.c_str());
							if(wcsstr(temp+i+wildcard,pwchr_temp))
							{delete[] pwchr_temp; return true;}
						}

					}
				}
			}
			else
			{
				if(isfind == 1){isfind =-1;}
			}
		}
	}
	delete[] pwchr_temp;
	return false;
}
bool CKeyWords::FindBlank(char * pchr)
{
	int  ch = ' ';
	if(strchr(pchr,ch)){return true;}
	return false;
}
