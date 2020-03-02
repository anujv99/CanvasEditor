
#ifndef __LOG_H__
#define __LOG_H__

#include <cstdio>

#ifdef __LOGGING__
	#define LOG(...) printf(__VA_ARGS__); printf("\n")
#else
	#define LOG(...)
#endif //__LOGGING__

#endif //__LOG_H__
