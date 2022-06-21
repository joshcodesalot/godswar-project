#ifndef _FILE_OBJCONTROL_H 
#define _FILE_OBJCONTROL_H

#include "ObjMgr.h"
#include "..\..\Common\MsgDefine.h"
#include "..\..\Common\MsgTypes.h"

enum GM_MSG_TYPE { ACTION_NONE, ACTION_KICK_PLAYER, ACTION_BAN_SPEAK, ACTION_SYS_PLACARD, ACTION_SYS_SHUTDOWN, ACTION_PLAYER_ITEM };
#define SERVER_SHOWDOWN_START 900  //��������رյ�ʱ��
#define SERVER_NOTE_REFRESH   120  //��ʾ��Ϣ�ļ��ʱ��

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
	//�Ƿ��и����͵Ķ�������
	bool HaveActionCtr( GM_MSG_TYPE eType );
	bool DelActionCtr( GM_MSG_TYPE eType);
	//GM��ΪAI
	void CrtAI();
	void PushCrl( ActionElem &Elem );
protected:
private:
};


#endif