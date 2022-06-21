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
	// //��ȡ�ؼ���
	int ReadFile(char* FileName);
protected:
	int KeyWordsNum;				//�ؼ��ָ���
	list<wstring> lKeyWordsList;			//�ؼ�������
	vector<wstring> vKeyWordsList;			//�ؼ�������
	vector<wchar_t> sshort;				//��������
public:
	bool FindKeyWord(char* pchr);			//����
public:
	bool FindBlank(char * pchr);
};
