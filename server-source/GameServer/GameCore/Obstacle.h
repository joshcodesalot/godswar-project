#ifndef __OBSTACLE__H__
#define __OBSTACLE__H__

#include <string>
#define MAX_OBSTACLE_FILE_COUNT 40
#define MAX_BLOCK_COUNT 256*256*64

class CObstacle
{
public:
	CObstacle();
	virtual ~CObstacle();

	void Init();                                      //��ʼ������
	bool IsObstacleList(int iMapID,float x,float y);  //����ID�ж��Ƿ����ϰ�,x��yΪ����

protected:
	void LoadIni();                                   //��INI�ļ��ж�ȡ�ϰ��ļ��б�
	void ReadObstacleList();                          //��ȡ�ϰ��б�

private:
	std::string m_strFileList[MAX_OBSTACLE_FILE_COUNT];
	std::map<int,BYTE*> m_mapCbstacleList;
	int m_iFileCount;
};

#endif