#pragma once
#include <vector>

using namespace std;
class CKeyWords
{
public:
	static CKeyWords *  Instance();
	void Release(){ delete this ;}
	bool Init();
private:
	static CKeyWords *m_KetWords;
public:
	CKeyWords(void);
public:
	~CKeyWords(void);
public:
	// //读取关键字
	int ReadFile(char* FileName);
protected:
	int KeyWordsNum;				//关键字个数
	list<wstring> lKeyWordsList;			//关键字链表
	vector<wstring> vKeyWordsList;			//关键字数组
	vector<wchar_t> sshort;				//索引数组
public:
	bool FindKeyWord(char* pchr);			//查找
public:
	bool FindBlank(char * pchr);
};
