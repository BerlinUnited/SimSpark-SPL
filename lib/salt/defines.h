#ifndef DEFINES_H__
#define DEFINES_H__

#include <assert.h>

#if		defined(_MSC_VER)
	// here we turn of a couple of pesky warnings VC emmits
	#pragma warning (disable : 4786)	// truncation of debug information
#endif

// some compiler-specific defines
#if		defined(_MSC_VER)
	// shared library export stuff
	#define SHARED_LIB_EXPORT	__declspec(dllexport)
	// other symbols
	#define f_inline			__forceinline
#elif	defined(__GNUC__)
	#define SHARED_LIB_EXPORT
	#define f_inline			inline
#endif

// this macro only exports the class in a header, if the shared library version is compiled
#ifdef SHARED_LIB_COMPILE
	#define CLASS_EXPORT	SHARED_LIB_EXPORT
#else
	#define CLASS_EXPORT
#endif

#define Assert(expression, desc)	assert(expression && desc)

#endif //DEFINES_H__
