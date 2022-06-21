#include "XMLBase.h"

XMLBase::XMLBase()
: m_bLoad(false)
{}
XMLBase::~XMLBase(){}

bool XMLBase::Load(const char* path)
{
	m_bLoad = m_TiXmlDocument.LoadFile(path);

	m_path = path;

	return m_bLoad;
}
void XMLBase::Save()
{
	m_TiXmlDocument.SaveFile(m_path.c_str());
}
bool XMLBase::IsLoad()
{
	return m_bLoad;
}

TiXmlElement* XMLBase::Root()
{
	return m_TiXmlDocument.RootElement();
}

TiXmlDocument* XMLBase::GetDocument()
{
	return &m_TiXmlDocument;
}

const char* XMLBase::GetVersion()
{
	if (!m_bLoad)
		return NULL;

	TiXmlDeclaration* declaration = m_TiXmlDocument.FirstChild()->ToDeclaration();

	return declaration == NULL ? NULL : declaration->Version();
}

const char* XMLBase::GetEncoding()
{
	if (!m_bLoad)
		return NULL;

	TiXmlDeclaration* declaration = m_TiXmlDocument.FirstChild()->ToDeclaration();

	return declaration == NULL ? NULL : declaration->Encoding();
}


