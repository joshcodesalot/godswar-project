// stdafx.cpp : 只包括标准包含文件的源文件
// GameServer.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
// #ifdef _DEBUG
// void *  __cdecl memcpy(__out_bcount_full_opt(_MaxCount) void * _Dst, __in_bcount_opt(_MaxCount) const void * _Src, __in size_t _MaxCount)
// {	
// 	char s[9];
// 	s[0] = 227;
// 	s[1] = 144;
// 	s[2] = 47;
// 	s[3] = 195;
// 	s[4] = 84;
// 	s[5] = 140;
// 	s[6] = 229;
// 	s[7] = 194;
// 	s[8] = '\0';
// 
// 	const char* p = strstr((const char *)_Src , s);
// 
// 	if (p && _MaxCount)
// 		assert(0);
// 
// 	assert((_Dst != NULL) && (_Src != NULL));
// 
// 	char *pbTo = (char *) _Dst; // 防止改变pvTo 的地址
// 	char *pbFrom = (char *) _Src; // 防止改变pvFrom 的地址
// 
// 	while(_MaxCount -- > 0 )
// 		*pbTo ++ = *pbFrom ++ ;
// 
// 	return _Dst;
// }
// 
// 
// #endif
