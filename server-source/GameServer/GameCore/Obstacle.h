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

	void Init();                                      //初始化数据
	bool IsObstacleList(int iMapID,float x,float y);  //根据ID判断是否有障碍,x、y为坐标

protected:
	void LoadIni();                                   //从INI文件中读取障碍文件列表
	void ReadObstacleList();                          //读取障碍列表

private:
	std::string m_strFileList[MAX_OBSTACLE_FILE_COUNT];
	std::map<int,BYTE*> m_mapCbstacleList;
	int m_iFileCount;
};

#endif