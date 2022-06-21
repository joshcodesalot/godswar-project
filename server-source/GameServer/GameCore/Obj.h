#ifndef _FILE_OBJ_H 
#define _FILE_OBJ_H

#include "GMCrl.h"

class  IObj
{
public:
	IObj();
	virtual ~IObj();
protected:
	GMCrl *m_GMctr;
private:
	
};
#endif 