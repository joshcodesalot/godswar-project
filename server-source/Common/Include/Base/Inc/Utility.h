//========================================================
//    Copyright (c) 2007,福州天盟工作室
//    All rights reserved.
//
//    文件名称 ： Utility.h
//    摘    要 ： 单一模式的模版类实现
//    
//    当前版本 ： 1.00
//    修    正 ： 李锋军
//    完成日期 ： 2007-01-07 ——
//    说    明 ： 1, 为单一模式提供模版基类
//
//========================================================


#ifndef _UTILITY_H_FILE
#define _UTILITY_H_FILE



namespace utility
{
	template <typename T>
	struct Singleton
	{
		static T& Instance();
	};
	struct ReferenceCounted
	{
		LONG m_count;
		void Release();
		void AddRef();
		ReferenceCounted();
		virtual ~ReferenceCounted();
	};

	template <typename T>
	struct SmartPointer
	{
		SmartPointer(T* p) : m_p(0) { *this = p; }
		SmartPointer(const SmartPointer& that) : m_p(0) { *this = that.m_p; }
		~SmartPointer() { if (m_p) { m_p->Release(); m_p = 0; }}

		const SmartPointer& operator=(T* p);
		T* Get() { return m_p; }
		T*const Get() const { return m_p; }

		mutable T* m_p; 
	}; 

	class CriticalSection
	{
	public:    

		CriticalSection() throw();
		~CriticalSection();

		void       UnLock();
		const bool AcquireLock();

	private:    

		CRITICAL_SECTION m_h_cs;

	}; 

	struct AutoLock
	{
		~AutoLock();
		AutoLock(CriticalSection&);
		const AutoLock& operator=(const AutoLock&);
		CriticalSection& m_c_lock;
	}; 

	template <typename T>
	T& Singleton<T>::Instance()
	{
		static T t_;
		return t_;
	}

	inline ReferenceCounted::ReferenceCounted() 
		: m_count(0) 
	{
	}

	inline ReferenceCounted::~ReferenceCounted() 
	{
	}

	inline void ReferenceCounted::Release() 
	{ 
		if (0 == ::InterlockedDecrement(&m_count))
		{
			delete this; 
		}            
	}

	inline void ReferenceCounted::AddRef() 
	{ 
		::InterlockedIncrement(&m_count); 
	}

	template <class T>
	const SmartPointer<T>& SmartPointer<T>::operator=(T* p)
	{
		if (p == m_p) return *this;
		if (m_p) m_p->Release();
		m_p = p;
		if (m_p) m_p->AddRef();
		return *this;
	}

	inline CriticalSection::CriticalSection()
	{
		::InitializeCriticalSection(&m_h_cs);
	}

	inline CriticalSection::~CriticalSection()
	{
		::DeleteCriticalSection(&m_h_cs);
	}

	inline const bool CriticalSection::AcquireLock()
	{
#if(_WIN32_WINNT >= 0x0400)
		return 1L & ::TryEnterCriticalSection(&m_h_cs);
#else
		::EnterCriticalSection(&m_h_cs);
		return true;
#endif 
	}

	inline void CriticalSection::UnLock()
	{
		::LeaveCriticalSection(&m_h_cs);
	}


	inline AutoLock::AutoLock(CriticalSection& c_lock) 
		: m_c_lock(c_lock) 
	{ 
		while (false == m_c_lock.AcquireLock()) {}
	}        

	inline AutoLock::~AutoLock() 
	{ 
		m_c_lock.UnLock(); 
	}
} 

#endif