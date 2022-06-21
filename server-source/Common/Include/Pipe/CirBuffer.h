//========================================================
//
//    Copyright (c) 2007,�������߹�����
//    All rights reserved.
//
//    �ļ����ƣ�CirBuffer.h
//    ժ  Ҫ��  CirBuffer��װ��
//    
//    ��ǰ�汾��1.01
//    ��   �ߣ� ����
//    ������ڣ�2007-12-15
//    ˵   ���� TCPճ������
//
//========================================================
#ifndef _FILE_CIRBUFFER_H
#define _FILE_CIRBUFFER_H

#include <Windows.h>

namespace snet
{
	class CircularBuffer
	{
	public:
		CircularBuffer(size_t size);
		~CircularBuffer();
		bool Write(const char *p,size_t l);
		bool Read(char *dest,size_t l);
		bool SoftRead(char *dest, size_t l);
		bool Remove(size_t l);
		size_t GetLength() { return m_q; }
		char *GetStart() { return buf + m_b; }
		size_t GetL() { return (m_b + m_q > m_max) ? m_max - m_b : m_q; }
		size_t Space() { return m_max - m_q; }
		void Clear();
		unsigned long ByteCounter() { return m_count; }
		void Initnalize();
		void Finalize();

	private:
		CircularBuffer(const CircularBuffer& s)  {}
		CircularBuffer& operator=(const CircularBuffer& ) { return *this; }
		char *buf;
		size_t m_max;
		volatile size_t m_q;
		volatile size_t m_b;
		volatile size_t m_t;
		volatile unsigned long m_count;
		CRITICAL_SECTION   m_CriSec;    
	};
}

#endif                                          
