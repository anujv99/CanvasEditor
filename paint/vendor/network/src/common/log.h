
#ifndef __LOG_H__
#define __LOG_H__

#include <cstdio>
#include <thread>
#include <sstream>

#ifdef __LOGGING__
	#define LOG(...) { std::stringstream ss; ss << std::this_thread::get_id(); printf("[Thread ID : %s] ", ss.str().c_str()); printf(__VA_ARGS__); printf("\n"); }
#else
	#define LOG(...)
#endif //__LOGGING__

#endif //__LOG_H__
