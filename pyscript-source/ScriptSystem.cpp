#include "ScriptSystem.h"


	ScriptSystem* ScriptSystem::m_This = NULL;

	ScriptSystem::ScriptSystem()
	{

	}

	ScriptSystem::~ScriptSystem()
	{

	}

	ScriptSystem* ScriptSystem::Instance()
	{
		if (!m_This)
			m_This = new ScriptSystem();

		return m_This;
	}

	void ScriptSystem::Release()
	{	
		Py_Finalize();

		delete m_This;

		m_This = NULL;
	}

	void ScriptSystem::Init()
	{
		Py_Initialize();
	}

	bool ScriptSystem::RunScript(const char* name)
	{
		try
		{
			PyRun_SimpleString(name);
		}
		catch (...)
		{
			PyErr_Print();
			PyErr_Clear();

			return false;
		}

		return true;
	}

	bool ScriptSystem::RunScriptFile(const char* name)
	{
		string str = "execfile(\'";
		str += name;
		str += "\')";

		try
		{
			PyRun_SimpleString(str.c_str());
		}
		catch (...)
		{
			PyErr_Print();
			PyErr_Clear();

			return false;
		}

		return true;
	}

	PyObject* ScriptSystem::ImportModule(const char* name)
	{
		PyObject* Module = PyImport_ImportModule(name);

		if (!Module)
		{
			PyErr_Print();
			PyErr_Clear();

			return NULL;
		}

		m_Modules[name] = Module;

		return Module;
	}

	PyObject* ScriptSystem::GetModule(string name)
	{
		if (m_Modules.find(name) == m_Modules.end())
			return NULL;

		return m_Modules[name];
	}

	PyObject* ScriptSystem::Reload(const char* name)
	{
		map<string , PyObject*>::iterator iter = m_Modules.find(name);

		if (iter == m_Modules.end())
			return NULL;

		PyObject* Module = PyImport_ReloadModule(iter->second);

		iter->second = Module;

		if (!Module)
		{
			PyErr_Print();
			PyErr_Clear();
		}

		return Module;
	}

	void ScriptSystem::ReloadAll()
	{
		for (map<string , PyObject*>::iterator iter = m_Modules.begin() ; iter != m_Modules.end() ; iter++)
		{
			PyObject* Module = PyImport_ReloadModule(iter->second);

			if (!Module)
			{
				PyErr_Print();
				PyErr_Clear();

				continue;
			}

			iter->second = Module;
		}
	}