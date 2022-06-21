// **********************************************************************
//
// Copyright (c) 2004
// All Rights Reserved
//
// Author: zeng cs
// Created:	2006/01/18
//
// **********************************************************************

#pragma once

#include <string>


namespace sbase
{
	// 这里要求后续使用sstring格式化后的字符串长度不能超过4096-1
	class sstring : public std::string  
	{
	public:
		sstring();
		sstring(const std::string& str)	{this->assign(str.c_str());}
		sstring(const sstring& str)		{this->assign(str.c_str());}
		sstring(int i)					{format("%d", i); }
		sstring(unsigned int u)			{format("%u", u); }
		sstring(float f)				{format("%f", f); }
		sstring(double d)				{format("%f", d); }

		//sstring(const char* pstr)		{ if (pstr) this->assign(pstr); }
		sstring(const char* fmt, ...);
		virtual ~sstring();
		
		sstring& operator = (const std::string& str){ this->assign(str.c_str()); return *this; }
		sstring& operator = (const sstring& str)	{ this->assign(str.c_str()); return *this; }
		sstring& operator = (const char* pstr)		{ if (pstr) this->assign(pstr); return *this; }
		sstring& operator = (int i)					{ format("%d", i); return *this; }
		sstring& operator = (unsigned int u)		{ format("%u", u); return *this; }
		sstring& operator = (float f)				{ format("%f", f); return *this; }
		sstring& operator = (double d)				{ format("%f", d); return *this; }
		
		const char operator [] (int nPos)			{ return this->at(nPos); }
		
		bool operator == (const std::string& str)	{ return (0 == this->compare(str)); }
		bool operator != (const std::string& str)	{ return (0 != this->compare(str)); }
		
		bool operator == (const char* pstr)			
		{ 
			if (pstr) 
				return (0 == this->compare(pstr)); 
			else
				return false;
		}
		bool operator != (const char* pstr)			
		{ 
			return !(operator == (pstr));
		}
		
		operator const char*() const				{ return this->c_str(); }
		
		operator char	() const					{ return (char)atoi(this->c_str()); }
		operator unsigned char	() const			{ return (unsigned char)atoi(this->c_str()); }
		
		operator int	() const					{ return atoi(this->c_str()); }
		operator unsigned int	() const			{ return atoi(this->c_str()); }
		
		operator long	() const					{ return atol(this->c_str()); }
		operator unsigned long	() const			{ return atol(this->c_str()); }
		
		operator double	() const					{ return atof(this->c_str()); }
		operator float	() const					{ return (float)atof(this->c_str()); }
		
		enum { MAX_STRING = 4096 };
		void format(const char* fmt, ...);
	};
}

