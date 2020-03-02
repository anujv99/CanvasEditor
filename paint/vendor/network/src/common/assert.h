
#include <cassert>

#ifndef __ASSERT_H__
#define __ASSERT_H__

#ifdef __ASSERT__
	#define ASSERT(X) assert(X)
#else
	#define ASSERT(X)
#endif //__ASSERT__

#endif //__ASSERT_H__
