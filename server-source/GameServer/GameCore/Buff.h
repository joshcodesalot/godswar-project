//========================================================
//
//    Copyright (c) 2006,欢乐连线工作室
//    All rights reserved.
//
//    文件名称 ： buff.h
//    摘    要 ： Buff功能模块
//    
//    当前版本 ： 1.00
//    作    者 ： 林德辉
//    完成日期 ： 2007-01-17
//
//========================================================


#pragma once

class CBuff
{
public:
	CBuff(void);
	~CBuff(void);

public:
	sbase::CTimer m_ts;
	long m_lType;
};
