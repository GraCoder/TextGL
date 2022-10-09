#include "TC_Library.h"

#ifndef __THREAD
#if defined(__GNUC__) || defined(__clang__)
#define __THREAD __thread
#elif defined(_MSC_VER)
#define __THREAD __declspec(thread)
#else
#define __THREAD
#endif
#endif

TC_Library* TC_Library::instance() 
{
	__THREAD static TC_Library lib;
	return &lib; 
}

TC_Library::TC_Library() 
{
	FT_Init_FreeType(&_lib); }

TC_Library::~TC_Library() { FT_Done_FreeType(_lib); }
