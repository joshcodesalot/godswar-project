#ifndef _ENCRYPTOR_H_
#define _ENCRYPTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include "../Base/inc/SvrBase.h"

namespace cnet
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
	};

	// <a1,  b1,   c1,   fst1, a2,   b2,   c2,   fst2>
	// 0xEE, 0x17, 0x33, 0x50, 0x82, 0x23, 0x61, 0x33

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// TEncryptClient
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template<unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1, 
		unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2>
	class TEncryptClient : public IEncryptor
	{
	public:
		TEncryptClient()	{ this->Init(); }
		//TEncryptClient(TEncryptClient & cEncrypt);

		// Interface
	public:
		virtual void			ChangeCode	(DWORD dwData);
		virtual void			ChangeCode	(const char* pszKey);

		virtual void			Encrypt		(unsigned char* bufMsg, int nLen);
		virtual void			Decrypt		(unsigned char* buf, int nLen);
		virtual void			Rencrypt	(unsigned char* buf, int nLen)		{ return; }

		virtual IEncryptor*		Duplicate	(void)								{ return new TEncryptClient; }
		virtual unsigned long	Release		(void)								{ delete this; return 0; }

	protected:
		void			Init		(void);
		int				m_nPos1;
		int				m_nPos2;

		int             m_nPos3;
		int             m_nPos4;

		// EncryptCode
		unsigned char m_bufEncrypt1[256];
		unsigned char m_bufEncrypt2[256];

		// heap manage
	public:
		//	MYHEAP_DECLARATION(s_heap)
	};


	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Implemention
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//template<unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1, 
	//	unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2>
	//	sbase::CHeap TEncryptClient<a1, b1, c1, fst1, a2, b2, c2, fst2>::s_heap;

	//template <unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1, 
	//			unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2>
	//TEncryptClient<a1, b1, c1, fst1, a2, b2, c2, fst2>::CEncryptCode	TEncryptClient<a1, b1, c1, fst1, a2, b2, c2, fst2>::m_cGlobalEncrypt;


	// Init
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1, 
		unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2>
		inline void 
		TEncryptClient<a1, b1, c1, fst1, a2, b2, c2, fst2>::Init(void)
	{
		m_nPos1 = m_nPos2 = 0;
		m_nPos3 = m_nPos4 = 0;

		try{
			unsigned char	nCode = fst1;
			for(int i = 0; i < 256; i++)
			{
				m_bufEncrypt1[i] = nCode;
				//printf("%02X ", nCode);
				nCode = (a1*nCode*nCode + b1*nCode + c1) % 256;
			}
			//printf("[%02X]\n", nCode);
			assert(nCode == fst1);

			nCode = fst2;
			for(int i = 0; i < 256; i++)
			{
				m_bufEncrypt2[i] = nCode;
				nCode = (a2*nCode*nCode + b2*nCode + c2) % 256;
			}
			assert(nCode == fst2);
		}
		catch(...)
		{
			printf("Encryptor Init fail."); 
		}  ;
	}


	// Encrypt
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1, 
		unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2>
		inline void 
		TEncryptClient<a1, b1, c1, fst1, a2, b2, c2, fst2>::Encrypt(unsigned char * bufMsg, int nLen)
	{
		bool bMove = true;
		try{
			int		nOldPos1 = m_nPos1;
			int		nOldPos2 = m_nPos2;
			for(int i = 0; i < nLen; i++)
			{
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1];
				bufMsg[i] ^= m_bufEncrypt2[m_nPos2];

				if(++m_nPos1 >= 256)
					m_nPos1 = 0;

				if(++m_nPos2 >= 256)
					m_nPos2 = 0;

				assert(m_nPos1 >=0 && m_nPos1 < 256);
				assert(m_nPos2 >=0 && m_nPos2 < 256);

				// 				// CQ AccountServer 特有的
				// 				int a = (bufMsg[i]&0x0f)*0x10;
				// 				int b = (bufMsg[i]&0xf0)/0x10;
				// 				bufMsg[i] = (a + b) ^ 0xab;
			}

			if(!bMove)
			{
				// 恢复指针
				m_nPos1 = nOldPos1;
				m_nPos2 = nOldPos2;
			}
		}catch(...){ printf("Encryptor Encrypt fail."); }
	}

	// Decrypt
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template <unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1, 
		unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2>
		inline void 
		TEncryptClient<a1, b1, c1, fst1, a2, b2, c2, fst2>::Decrypt(unsigned char * bufMsg, int nLen)
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

				assert(m_nPos3 >=0 && m_nPos4 < 256);
				assert(m_nPos3 >=0 && m_nPos4 < 256);

				// 				// CQ AccountServer 特有的
				// 				int a = (bufMsg[i]&0x0f)*0x10;
				// 				int b = (bufMsg[i]&0xf0)/0x10;
				// 				bufMsg[i] = (a + b) ^ 0xab;
			}

			if(!bMove)
			{
				// 恢复指针
				m_nPos3 = nOldPos1;
				m_nPos4 = nOldPos2;
			}
		}catch(...){ printf("Encryptor Encrypt fail."); }
	}


	// ChangeCode
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template<unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1, 
		unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2>
		inline void 
		TEncryptClient<a1, b1, c1, fst1, a2, b2, c2, fst2>::ChangeCode(DWORD dwData)
	{
		try{
			DWORD	dwData2 = dwData*dwData;
			for(int i = 0; i < 256; i += 4)
			{
				*(DWORD*)(&m_bufEncrypt1[i]) ^= dwData;
				*(DWORD*)(&m_bufEncrypt2[i]) ^= dwData2;
			}
		}catch(...){ printf("Encryptor ChangeCode fail."); }
	}

	// ChangeCode
	////////////////////////////////////////////////////////////////////////////////////////////////////
	template<unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1, 
		unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2>
		inline void 
		TEncryptClient<a1, b1, c1, fst1, a2, b2, c2, fst2>::ChangeCode(const char* pszKey)
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
		}catch(...){printf("Encryptor ChangeCode fail."); }
	}

}

#endif