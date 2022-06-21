#ifndef _FILE_OBJCONTROL_H 
#define _FILE_OBJCONTROL_H

#include "ObjMgr.h"
#include "..\..\Common\MsgDefine.h"
#include "..\..\Common\MsgTypes.h"

enum GM_MSG_TYPE { ACTION_NONE, ACTION_KICK_PLAYER, ACTION_BAN_SPEAK, ACTION_SYS_PLACARD, ACTION_SYS_SHUTDOWN, ACTION_PLAYER_ITEM };
#define SERVER_SHOWDOWN_START 900  //服务器离关闭的时间
#define SERVER_NOTE_REFRESH   120  //提示信息的间隔时间

struct ActionElem
{
	MsgHead Head;  
	char  ObjName[32];
	char  PlacardInfo[255];
	UINT  IntervalTime;
	LONGLONG   StartTime;
	LONGLONG   EndTime;
	BYTE  PlacardType;
	BYTE  SendType;
	BYTE  Repeat;
	BYTE  HaveRepeat;
	time_t OriginalTime;
	

	void  *Obj;
    bool  (*callback)( ActionElem& );

	ActionElem(const ActionElem& Elem )
	{
	   Head.usSize = Elem.Head.usSize;
       Head.usType = Elem.Head.usType;
	   strcpy( ObjName, Elem.ObjName );
	   strcpy( PlacardInfo, Elem.PlacardInfo );
	   StartTime = Elem.StartTime;
	   EndTime = Elem.EndTime;
	   IntervalTime = Elem.IntervalTime;
	   PlacardType = Elem.PlacardType;
	   SendType = Elem.SendType;
	   Repeat = Elem.Repeat;
	   HaveRepeat = Elem.HaveRepeat;
	   OriginalTime = Elem.OriginalTime;
	   Obj = Elem.Obj;
	   callback = Elem.callback;

	}
	ActionElem(){ memset( this, 0L, sizeof(ActionElem) );};

	ActionElem& operator = (const ActionElem &Elem)
	{
		if (this == &Elem)
			return *this;

		strcpy( ObjName, Elem.ObjName );
		strcpy( PlacardInfo, Elem.PlacardInfo );
		StartTime = Elem.StartTime;
		EndTime = Elem.EndTime;
		IntervalTime = Elem.IntervalTime;
		PlacardType = Elem.PlacardType;
		SendType = Elem.SendType;
		Repeat = Elem.Repeat;
		HaveRepeat = Elem.HaveRepeat;
		OriginalTime = Elem.OriginalTime;
		Obj = Elem.Obj;
		callback = Elem.callback;

		return *this;
	}
};

class GMCrl : public ObjMgr<ActionElem>
{
public:
	GMCrl();
	~GMCrl();
public:
	//是否有该类型的动作控制
	bool HaveActionCtr( GM_MSG_TYPE eType );
	bool DelActionCtr( GM_MSG_TYPE eType);
	//GM行为AI
	void CrtAI();
	void PushCrl( ActionElem &Elem );
protected:
private:
};


#endif