//========================================================
//
//    Copyright (c) 2007,�������߹�����
//    All rights reserved.
//
//    �ļ����� �� TimeElems.h
//    ժ    Ҫ �� ʱ��Ԫ��ģ����
//    
//    ��ǰ�汾 �� 1.00
//    ��    �� �� ���� 
//    ������� �� 2007-08-22
//
//========================================================

#ifndef _FILE_TIMELEMN_H
#define _FILE_TIMELEMN_H

class CGameObject;

template <typename T1, typename TT> 
class TTimeElement 
{ 
}; 

template< typename T1, typename T2 >
class TTimeElement<T1*,T2>
{
	friend class CGameObject;
public:
	TTimeElement():m_Data(NULL),m_StartTime(0), m_Done( false ){ m_PassTime.clear(); };
	TTimeElement( T1* , T2  );
    inline bool operator == (const TTimeElement &Element) const
	{
		return m_StartTime == Element.m_StartTime && m_Data == Element.m_Data;
	}
    TTimeElement& operator=(const TTimeElement &Element ) 
    {
 	   if ( this == &Element )
 	      return *this;
 
 	   this->m_Data = Element.m_Data;
 	   this->m_StartTime = Element.m_StartTime;
	   this->m_Done = Element.m_Done;
       this->m_PassTime = Element.m_PassTime;
 	   return *this;
    } 
	T2 First(){ return m_StartTime; };
	T1* Second(){ return m_Data; };
	void SetDoneFlag(bool flag){ m_Done = flag ;};
	bool GetDoneFlag(){ return m_Done; };
	bool DealWith( CGameObject *pObj )
	{
       if ( time(NULL) - m_StartTime >= m_Data->GetPersistTime() )
       {
		   m_Data->Resume( pObj );
		   return false;
       }
	   else
	   {
		   m_Data->DealWith( pObj, m_StartTime, m_PassTime, m_Done );
		   m_Done = true ;
		   return true;
	   }
	}
 protected:
 private:
    bool m_Done;
 	T2 m_StartTime;
 	T1* m_Data;
	map<int,int> m_PassTime;
};

template< typename T1, typename T2 >
TTimeElement<T1*,T2>::TTimeElement( T1* pData, T2 Start ):m_Data(pData),m_StartTime(Start)
{
   m_Done = false;
   m_PassTime.clear();
}
 

#endif

