#ifndef _FTINYXML_H_
#define _FTINYXML_H_

#include <string>
using namespace std;

#include "tinyxml.h"


class XMLBase
{
public:

	XMLBase();
	virtual ~XMLBase();

	bool Load(const char* path);

	void Save();
	bool IsLoad();

	const char* GetVersion();
	const char* GetEncoding();

	TiXmlElement* Root();
	TiXmlDocument* GetDocument();
protected:

	TiXmlDocument m_TiXmlDocument;

	bool m_bLoad;	
	string m_path;
};


#endif

