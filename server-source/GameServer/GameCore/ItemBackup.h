#ifndef _ITEMBACKUP_H_
#define _ITEMBACKUP_H_

#include "Item.h"

void BackupStall(__int64 DB , __int64 DBBuy , Item* item , int money , int bijou , __int64 ID);

void BackupTrade(__int64 DB , Item** item , int money , int bijou , __int64 ID);

#endif

