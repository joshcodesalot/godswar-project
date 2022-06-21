#include "stdafx.h"
#include "Trade.h"
#include "ItemBackup.h"
#include <direct.h>
#include<time.h>
#include "../../Common/Include/Base/xml/XMLBase.h"
FILE* g_File = NULL;

#define _BACKUP_ITEM
#ifdef _BACKUP_ITEM

void BackupTrade(__int64 DB , Item** item , int money , int bijou , __int64 ID)
{
	time_t t = time(0); 

	struct tm* timenow = localtime(&t);

	char Tbuf[128];
	XMLBase xml;

	sprintf(Tbuf ,"./Item/Backup/Trade_%d-%d-%d.xml" ,timenow->tm_year + 1900,timenow->tm_mon,timenow->tm_mday); 
	bool bopen=xml.Load(Tbuf);

	//文件不存在新建
	if (!bopen)
	{
		fopen(Tbuf , "wb+");

		xml.Load(Tbuf);

		TiXmlDeclaration* decl = new TiXmlDeclaration("1.0" , "GB2312" , "");
		TiXmlComment* comment = new TiXmlComment("this is BackupTrade");
		TiXmlElement* root = new TiXmlElement("BackupTrade");

		xml.GetDocument()->LinkEndChild(decl);
		xml.GetDocument()->LinkEndChild(comment);
		xml.GetDocument()->LinkEndChild(root);
	}

	static int TcountA=0;
	sprintf(Tbuf ,"Trade%d",TcountA+1);
	TcountA++;

	TiXmlElement *element = new TiXmlElement(Tbuf);
	xml.Root()->LinkEndChild(element);

	element->SetAttribute("ID",ID);
	element->SetAttribute("DB",DB);
	element->SetAttribute("Money",money);
	element->SetAttribute("Bijou",bijou);

	int c1=0,c2=0;
	for (int i = 0 ; i < MAX_TRADE_ITEMS ; i++)
	{
		if (!item[i])
			continue;

		sprintf(Tbuf ,"Base%d",c1);
		element->SetAttribute(Tbuf,item[i]->GetItemBaseAttribute()->ID);

		for (int j = 0 ; j < MAX_EQUIPAPPEND_COUNT ; j++)
		{
			if (item[i]->GetItemAppendAttribute(j))
			{
				sprintf(Tbuf ,"Append%d",c2);
				c2++;
				element->SetAttribute(Tbuf,item[i]->GetItemAppendAttribute(j)->ID);
			}
		}

		sprintf(Tbuf ,"Level%d",c1);
		element->SetAttribute(Tbuf,item[i]->BaseLevel);

		sprintf(Tbuf ,"Overlap%d",c1);
		element->SetAttribute(Tbuf,item[i]->m_Overlap);

		sprintf(Tbuf ,"CdKey%d",c1);
		element->SetAttribute(Tbuf,item[i]->ICdKey);
		
		c1++;
	}

	xml.Save();
}

void BackupStall(__int64 DB , __int64 DBBuy , Item* item , int money , int bijou , __int64 ID)
{
	//摆摊交易
	if (item->IsClear())
		return;

	time_t t = time(0); 

	struct tm* timenow = localtime(&t);

	char Tbuf[128];
	XMLBase xml;

	sprintf(Tbuf ,"./Item/Backup/Stall_%d-%d-%d.xml" ,timenow->tm_year + 1900,timenow->tm_mon,timenow->tm_mday); 
	bool bopen=xml.Load(Tbuf);

	//文件不存在新建
	if (!bopen)
	{
		fopen(Tbuf , "wb+");

		xml.Load(Tbuf);

		TiXmlDeclaration* decl = new TiXmlDeclaration("1.0" , "GB2312" , "");
		TiXmlComment* comment = new TiXmlComment("this is BackupStall");
		TiXmlElement* root = new TiXmlElement("BackupStall");

		xml.GetDocument()->LinkEndChild(decl);
		xml.GetDocument()->LinkEndChild(comment);
		xml.GetDocument()->LinkEndChild(root);
	}

	static int TcountB=0;
	sprintf(Tbuf ,"Stall%d",TcountB+1);
	TcountB++;

	TiXmlElement *element = new TiXmlElement(Tbuf);
	xml.Root()->LinkEndChild(element);

	element->SetAttribute("Hour",timenow->tm_hour);
	element->SetAttribute("Min",timenow->tm_min);
	element->SetAttribute("Sec",timenow->tm_sec);
	element->SetAttribute("ID",ID);
	element->SetAttribute("DB",DB);
	element->SetAttribute("DBBuy",DBBuy);
	element->SetAttribute("Base",item->GetItemBaseAttribute()->ID);

	int c2=0;
	for (int i = 0 ; i < MAX_EQUIPAPPEND_COUNT ; i++)
	{
		if (item->GetItemAppendAttribute(i))
		{
			sprintf(Tbuf ,"Append%d",c2);
			element->SetAttribute(Tbuf,item->GetItemAppendAttribute(i)->ID);
		}
	}

	element->SetAttribute("Level",item->BaseLevel);
	element->SetAttribute("Overlap",item->BaseLevel);
	element->SetAttribute("Money",money);
	element->SetAttribute("Bijou",bijou);
	element->SetAttribute("CdKey",item->ICdKey);

	xml.Save();
}

/*
void BackupTrade(__int64 DB , Item** item , int money , int bijou , __int64 ID)
{
	time_t t = time(0); 

	struct tm* timenow = localtime(&t);

	char Tbuf[128];
	XMLBase xml;

	sprintf(Tbuf ,"Trade%d-%d-%d-%d" ,i+1); 
	xml.Load("./Item/Backup/TT.xml");

	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0" , "GB2312" , "");
	TiXmlComment* comment = new TiXmlComment("this is SkillConfig");
	TiXmlElement* root = new TiXmlElement("SkillConfig");

	xml.GetDocument()->LinkEndChild(decl);
	xml.GetDocument()->LinkEndChild(comment);
	xml.GetDocument()->LinkEndChild(root);

	string name = "player";
	//name += player->GetName();

	TiXmlElement* element = xml.Root()->FirstChildElement(name.c_str());

	if (element)//已存在段
	{
		//sprintf(Tbuf ,"skill%d" ,i+1); 
		//element->SetAttribute("edition" , 1002);
	}
	else//新建
	{
		element = new TiXmlElement(name.c_str());
		xml.Root()->LinkEndChild(element);

		element->SetAttribute("edition" , 1002);
	}

	xml.Save();

	char buffer[32];

	string s = "./Item/Backup/";
	s += itoa(timenow->tm_year + 1900, buffer , 10);
	s += "-";
	s += itoa(timenow->tm_mon , buffer , 10);
	s += "-";
	s += itoa(timenow->tm_mday , buffer , 10);
	s += "-Trade.ibk";

	if (!g_File)
	{
		g_File = fopen(s.c_str() , "at");

		if (!g_File)
			g_File = fopen(s.c_str() , "wb+");
	}

	if (!g_File)
	{
		mkdir("./Item/Backup/");
		g_File = fopen(s.c_str() , "wb+");
	}

	string str;
	str.reserve(128);

	str = "Time: ";
	str += itoa(timenow->tm_hour , buffer , 10);
	str += ":";
	str += itoa(timenow->tm_min , buffer , 10);
	str += ":";
	str += itoa(timenow->tm_sec , buffer , 10);
	str += " ";

	str += "ID: "; str += _i64toa(ID , buffer , 10);
	str += " ";

	str += "DB: "; str += _i64toa(DB , buffer , 10);
	str += " ";

	str += "Money: "; str += itoa(money , buffer , 10);
	str += " ";

	str += "Bijou: "; str += itoa(bijou , buffer , 10);
	str += " ";

	for (int i = 0 ; i < MAX_TRADE_ITEMS ; i++)
	{
		if (!item[i])
			continue;

		str += "base: "; str += itoa(item[i]->GetItemBaseAttribute()->ID , buffer , 10);
		str += " ";

		for (int j = 0 ; j < MAX_EQUIPAPPEND_COUNT ; j++)
		{
			if (item[i]->GetItemAppendAttribute(j))
			{
				str += " append: "; str += itoa(item[i]->GetItemAppendAttribute(j)->ID , buffer , 10);
				//str += " value: "; str += itoa(item[i]->m_Append[j] , buffer , 10);
				str += " ";
			}
		}

		str += "level: "; str += itoa(item[i]->BaseLevel , buffer , 10);
		str += " ";

		str += "overlap: "; str += itoa(item[i]->m_Overlap , buffer , 10);
		//str += " lock: "; str += itoa(item[i]->m_Lock , buffer , 10);
		//str += " binding: "; str += itoa(item[i]->m_Binding , buffer , 10);
		str += " ";
	}

	str += "\n";

	fwrite(str.c_str() , 1 , str.size() , g_File);

	fclose(g_File); g_File = NULL;
}

void BackupStall(__int64 DB , __int64 DBBuy , Item* item , int money , int bijou , __int64 ID)
{
	if (item->IsClear())
		return;

	time_t t = time(0); 

	struct tm* timenow = localtime(&t);

	char buffer[32];

	string s = "./Item/Backup/";
	s += itoa(timenow->tm_year + 1900, buffer , 10);
	s += "-";
	s += itoa(timenow->tm_mon , buffer , 10);
	s += "-";
	s += itoa(timenow->tm_mday , buffer , 10);
	s += "-Stall.ibk";

	if (!g_File)
	{
		g_File = fopen(s.c_str() , "at");

		if (!g_File)
			g_File = fopen(s.c_str() , "wb+");
	}

	if (!g_File)
	{
		mkdir("./Item/Backup/");
		g_File = fopen(s.c_str() , "wb+");
	}

	string str;
	str.reserve(128);

	str = "Time: ";
	str += itoa(timenow->tm_hour , buffer , 10);
	str += ":";
	str += itoa(timenow->tm_min , buffer , 10);
	str += ":";
	str += itoa(timenow->tm_sec , buffer , 10);
	str += " ";

	str += "ID: "; str += _i64toa(ID , buffer , 10);
	str += " ";

	str += "DB: "; str += _i64toa(DB , buffer , 10);
	str += " ";

	str += "DBBuy: "; str += _i64toa(DBBuy , buffer , 10);
	str += " ";

	str += "base: "; str += itoa(item->GetItemBaseAttribute()->ID , buffer , 10);
	str += " ";

	for (int i = 0 ; i < MAX_EQUIPAPPEND_COUNT ; i++)
	{
		if (item->GetItemAppendAttribute(i))
		{
			str += "append:"; str += itoa(item->GetItemAppendAttribute(i)->ID , buffer , 10);
			//str += "value:"; str += itoa(item->m_Append[i] , buffer , 10);
			str += " ";
		}
	}

	str += "level: "; str += itoa(item->BaseLevel , buffer , 10);
	str += " ";

	str += "overlap:"; str += itoa(item->m_Overlap , buffer , 10);
	//str += "lock:"; str += itoa(item->m_Lock , buffer , 10);
	//str += "binding:"; str += itoa(item->m_Binding , buffer , 10);

	str += " ";
	str += "money: "; str += itoa(money , buffer , 10);

	str += " ";
	str += "bijou: "; str += itoa(bijou , buffer , 10);

	str += "\n";

	fwrite(str.c_str() , 1 , str.size() , g_File);

	fclose(g_File); g_File = NULL;
}
*/
#else

void BackupTrade(__int64 DB , Item** item , int money , int bijou , _i64toa ID)
{
	//NULL
}

void BackupStall(__int64 DB , __int64 DBBuy , Item* item , int money , int bijou , _i64toa ID)
{
	//NULL
}

#endif

