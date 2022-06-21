#ifndef _SCRIPTSYSTEM_H_
#define _SCRIPTSYSTEM_H_

#include "./python/include/Python.h"
#include "./python/include/structmember.h"
#include "./python/include/object.h"

#include <map>
#include <string>
using namespace std;

class ScriptSystem
{
public:

	static ScriptSystem* Instance();

	void Release();

	bool RunScript(const char* name);

	bool RunScriptFile(const char* name);

	void Init();

	PyObject* GetModule(string name);

	PyObject* ImportModule(const char* name);

	void ReloadAll();

	PyObject* Reload(const char* name);

protected:
private:

	static ScriptSystem* m_This;

	ScriptSystem();
	~ScriptSystem();

	map<string , PyObject*> m_Modules;
};


#endif

