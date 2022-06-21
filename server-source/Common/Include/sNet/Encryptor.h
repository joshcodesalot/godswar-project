#ifndef _ENCRYPTOR_H_
#define _ENCRYPTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include "../Base/inc/SvrBase.h"
#include "../Base/inc/Heap.h"


namespace snet
{
	class IEncryptor
	{
	public:
		// 释放此加密器接口
		virtual unsigned long	Release			(void)							= 0;

		// 按指定长度加密数据
		virtual void			Encrypt			(unsigned char* buf, int nLen)	= 0;

		// 按指定长度解密数据	
		virtual void			Decrypt			(unsigned char* buf, int nLen)	= 0;

		// 此方法可以把指定长度的解密数据重新加密，恢复到解密前的状态
		virtual void			Rencrypt		(unsigned char* buf, int nLen)	= 0;

		// 改变加密器编码
		virtual void			ChangeCode		(unsigned long ulCode)			= 0;
		virtual void			ChangeCode		(const char* pszKey)			= 0;

		// 产生一个新的加密器接口
		virtual IEncryptor*		Duplicate		(void)							= 0;

		virtual void            Refresh         ()                              = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// 3个随机数, 用于隐藏服务端INI中的SERVER KEY。不变
	////////////////////////////////////////////////////////////////////////////////////////////////////
	#define		aa	0x7E
	#define		bb	0x33
	#define		cc	0xA1


	//#define ENCRYPT_KEY1				0xa61fce5e	// A = 0x20, B = 0xFD, C = 0x07, first = 0x1F, key = a61fce5e
    //#define ENCRYPT_KEY2				0x443ffc04	// A = 0x7A, B = 0xCF, C = 0xE5, first = 0x3F, key = 443ffc04

	template<unsigned long key1, unsigned long key2>
	class TEncryptServer : public IEncryptor
	{
	public:
		TEncryptServer()		{ this->Init(); }
	public:
		virtual void			ChangeCode	(DWORD dwData);
		virtual void			ChangeCode	(const char* pszKey);

		virtual void			Encrypt		(unsigned char * bufMsg, int nLen);
		virtual void			Decrypt		(unsigned char* buf, int nLen);
		virtual void			Rencrypt	(unsigned char* , int )		{ return; }

		virtual IEncryptor*		Duplicate	(void)								{ return new TEncryptServer; }
		virtual unsigned long	Release		(void)								{ delete this; return 0; }
		virtual void            Refresh     (void)                              { m_nPos1 = m_nPos2 = m_nPos3 = m_nPos4 = 0; }

	protected:
		void			Init		(void);
		int				m_nPos1;
		int				m_nPos2;

		int             m_nPos3;
		int             m_nPos4;

	private:
		unsigned char m_bufEncrypt1[256];
		unsigned char m_bufEncrypt2[256];

	public:
		//MYHEAP_DECLARATION(s_heap)
	};

	//template<unsigned long key1, unsigned long key2>
	//sbase::CHeap TEncryptServer<key1, key2>::s_heap;

	//template <unsigned long key1, unsigned long key2>
	//TEncryptServer<key1, key2>::CEncryptCode	TEncryptServer<key1, key2>::m_cGlobalEncrypt;

	template<unsigned long key1, unsigned long key2>
	inline void 
		TEncryptServer<key1, key2>::Init(void)
	{
		m_nPos1 = m_nPos2 = 0;
        m_nPos3 = m_nPos4 = 0;

		try{
			// 生成 ABC
			int	a1, b1, c1, fst1;
			a1		= ((key1 >> 0) & 0xFF) ^ aa;
			b1		= ((key1 >> 8) & 0xFF) ^ bb;
			c1		= ((key1 >> 24) & 0xFF) ^ cc;
			fst1	= (key1 >> 16) & 0xFF;

			int	a2, b2, c2, fst2;
			a2		= ((key2 >> 0) & 0xFF) ^ aa;
			b2		= ((key2 >> 8) & 0xFF) ^ bb;
			c2		= ((key2 >> 24) & 0xFF) ^ cc;
			fst2	= (key2 >> 16) & 0xFF;

			unsigned char	nCode = (unsigned char)fst1;
			for(int i = 0; i < 256; i++)
			{
				m_bufEncrypt1[i] = nCode;
				//printf("%02X ", nCode);
				nCode = (unsigned char)(a1*nCode*nCode + b1*nCode + c1) % 256;
			}
			//printf("[%02X]\n", nCode);
			ASSERT(nCode == fst1);

			nCode = (unsigned char)fst2;
			for(int  i = 0; i < 256; i++)
			{
				m_bufEncrypt2[i] = nCode;
				nCode = (unsigned char)(a2*nCode*nCode + b2*nCode + c2) % 256;
			}
			ASSERT(nCode == fst2);
		}catch(...){ sbase::LogSave("Net", "Encryptor init fail."); }
	}


	template<unsigned long key1, unsigned long key2>
	inline void 
		TEncryptServer<key1, key2>::Encrypt(unsigned char * bufMsg, int nLen)
	{
		bool bMove = true;
		try{
			int		nOldPos1 = m_nPos1;
			int		nOldPos2 = m_nPos2;

			for(int i = 0; i < nLen; i++)
			{
				// CQ AccountServer 特有的
				//bufMsg[i] ^= 0xab;
				//int a = (bufMsg[i]&0x0f)*0x10;
				//int b = (bufMsg[i]&0xf0)/0x10;
				//bufMsg[i] = (unsigned char)(a + b);

				bufMsg[i] ^= m_bufEncrypt1[m_nPos1];
				bufMsg[i] ^= m_bufEncrypt2[m_nPos2];

				if(++m_nPos1 >= 256)
					m_nPos1 = 0;

				if(++m_nPos2 >= 256)
					m_nPos2 = 0;

				assert(m_nPos1 >=0 && m_nPos1 < 256);
				assert(m_nPos2 >=0 && m_nPos2 < 256);
			}

			if(!bMove)
			{
				// 恢复指针
				m_nPos1 = nOldPos1;
				m_nPos2 = nOldPos2;
			}
		}catch(...){  printf("Encryptor Encrypt fail."); }
	}

	template<unsigned long key1, unsigned long key2>
	inline void TEncryptServer<key1, key2>::Decrypt(unsigned char * bufMsg, int nLen)
	{
		bool bMove = true;
		try{
			int		nOldPos1 = m_nPos3;
			int		nOldPos2 = m_nPos4;
			for(int i = 0; i < nLen; i++)
			{
				bufMsg[i] ^= m_bufEncrypt1[m_nPos3];
				bufMsg[i] ^= m_bufEncrypt2[m_nPos4];
				if(++m_nPos3 >= 256)
					m_nPos3 = 0;

				if(++m_nPos4 >= 256)
					m_nPos4 = 0;

				assert(m_nPos3 >=0 && m_nPos3 < 256);
				assert(m_nPos4 >=0 && m_nPos4 < 256);
			}

			if(!bMove)
			{
				// 恢复指针
				m_nPos3 = nOldPos1;
				m_nPos4 = nOldPos2;
			}
		}catch(...){  printf("Encryptor Encrypt fail."); }
	}

	template<unsigned long key1, unsigned long key2>
	inline void TEncryptServer<key1, key2>::ChangeCode(DWORD dwData)
	{
		try{
			DWORD	dwData2 = dwData*dwData;
			for(int i = 0; i < 256; i += 4)
			{
				*(DWORD*)(&m_bufEncrypt1[i]) ^= dwData;
				*(DWORD*)(&m_bufEncrypt2[i]) ^= dwData2;
			}
		}catch(...){ sbase::LogSave("Net", "Encryptor ChangeCode fail."); }
	}

	template<unsigned long key1, unsigned long key2>
	inline void 
		TEncryptServer<key1, key2>::ChangeCode(const char* pszKey)
	{
		if (!pszKey)
			return;

		try{
			DWORD dwData = (DWORD) atoi(pszKey);

			DWORD	dwData2 = dwData*dwData;
			for(int i = 0; i < 256; i += 4)
			{
				*(DWORD*)(&m_bufEncrypt1[i]) ^= dwData;
				*(DWORD*)(&m_bufEncrypt2[i]) ^= dwData2;
			}
		}catch(...){ sbase::LogSave("Net", "Encryptor ChangeCode fail."); }
	}
}

#endif 

