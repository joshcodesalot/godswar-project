#ifndef _LOG_H_
#define _LOG_H_

class Log
{
public:

	static void __cdecl LogOut(const char * str , ...);

protected:
private:
};

void debug_log(const char * str, ...);

#endif

