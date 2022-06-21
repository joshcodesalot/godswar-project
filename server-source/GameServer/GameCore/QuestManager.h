#ifndef _QUESTMANAGER_H_
#define _QUESTMANAGER_H_

#include "QuestDef.h"

class CWorld;
class QuestManager
{
public:

	static QuestManager* Instance();

	void Init(CWorld* pWorld);
	void Release();

	void InsertQuest(int id , QuestInfo* quest);
	void EraseQuest(int id);

	void InsertQuestReward(int id , QuestReward* quest);
	void EraseQuestReward(int id);

	QuestInfo* FindQuest(int id);
	QuestReward* FindQuestReward(int id);

	void UpData();

	inline time_t GetLocalTime()
	{
		return m_TimeZone + time(0);
	}

	void Check();

protected:
private:

	sbase::CTimer			m_Timer;

	long					m_TimeZone;	//GMTÆ«²î

	vector<QuestInfo*>		m_TimeQuests;

	map<int , QuestInfo*>	m_MapQuestInfo;
	map<int , QuestReward*> m_MapQuestReward;

	static QuestManager*	m_This;
	CWorld*					m_pWorld;

	QuestManager();
	~QuestManager();
};


#endif
