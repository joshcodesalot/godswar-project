#pragma once

#pragma warning(disable:4201)
#include "SvrBase.h"
#include "../../Base/Inc\mempool.h"


namespace sbase
{
	//////////////////////////////////////////////////////////////////////
	class IMessage
	{
	public:
		IMessage(void);
		IMessage(const void* buf, int from=-1, void * to=NULL):
		usFrom((USHORT)from), usTo(to) 	
		{	
			ZeroMemory(bufMsg, sizeof(bufMsg));
			if (buf)
			{
				USHORT size = *((USHORT*) buf);
				IF_OK (size <= MAX_MSGSIZE)
					memcpy(bufMsg, buf, size);
			}

		}
		IMessage(int size, int type, const void* buf, int from, void * to); 
		IMessage(const IMessage& rhs); 

		USHORT			Release	(void)	;

		USHORT			GetSize	(void) const	{ return usMsgSize; }
		USHORT			GetType	(void) const	{ return usMsgType; }
		const void*		GetBuf	(void) const	{ return bufMsg; }

		USHORT			GetFrom	(void) const	{ return usFrom; }
		void*			GetTo	(void) const	{ return usTo; }


// 		void*	operator new	(size_t size);
// 		void	operator delete	(void* p);
// 
// 		void*	operator new[]	(size_t size);
// 		void	operator delete[](void* p);

		enum { MAX_MSGSIZE = 1024 };
		USHORT	usFrom;
		void *usTo;
		union 
		{
			char	bufMsg[MAX_MSGSIZE];
			struct 
			{
				unsigned short usMsgSize;
				unsigned short usMsgType;
			};
		};
	private:
		 DECLARE_FIXEDSIZE_ALLOCATOR(IMessage);
	};

}
