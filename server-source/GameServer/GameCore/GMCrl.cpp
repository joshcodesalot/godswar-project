#include "stdafx.h"

#include "GMCrl.h"

GMCrl::GMCrl()
{

}

GMCrl::~GMCrl()
{

}

bool GMCrl::HaveActionCtr( GM_MSG_TYPE eType )
{
	std::vector<ActionElem>::iterator itor= this->Begin();
	for ( ; itor != this->End(); itor++ )
	{
		ActionElem Elem = (*itor);
		if (  eType ==  Elem.Head.usType  )
		{
			return true;
		}
	}
   return false;
}
bool GMCrl::DelActionCtr( GM_MSG_TYPE eType)
{
	std::vector<ActionElem>::iterator itor= this->Begin();
	while ( itor != this->End() )
	{
		ActionElem Elem = (*itor);
		if (  eType ==  Elem.Head.usType )
		{
			itor = this->Remove(itor);
		}
		else
		{
			itor++;
		}
	}
	return true;
}
void GMCrl::CrtAI()
{
	std::vector<ActionElem>::iterator itor= this->Begin();
	while ( itor != this->End() )
	{
		ActionElem Elem = (*itor);
		if (  Elem.callback( *itor ) )
		{
			itor = this->Remove(itor);
		}
		else
		{
			itor++;
		}
	}

}

void GMCrl::PushCrl( ActionElem &Elem )
{
   Push(Elem);
}



