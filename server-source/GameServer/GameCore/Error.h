#ifndef _ERROR_H_
#define _ERROR_H_

class CPlayer;

#include <string>


enum ServerErrorID
{
	PlayerIsBusy,
	PlayerIsDead,
	ItemFull,
	ItemLock,
	ItemUnLock,
	NoItem,
	MoneyNotEnough,
	BijouNotEnough,
	ChangeProFaild,

	ErrorCount
};

static std::string ServerErrorStr [] = 
{
	"PlayerIsBusy",
	"PlayerIsDead",
	"ItemFull",
	"ItemLock",
	"ItemUnLock",
	"NoItem",
	"MoneyNotEnough",
	"BijouNotEnough",
	"ChangeProFaild",

	"ErrorCount"
};

void SendServerError(ServerErrorID ID , CPlayer* player);

#endif

