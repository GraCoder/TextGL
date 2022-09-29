#include "TC_Library.h"

#include "texture-font.h"

TC_Library* TC_Library::instance() 
{
	__THREAD static TC_Library lib;
	return &lib; 
}

TC_Library::TC_Library() 
{
	FT_Init_FreeType(&_lib); }

TC_Library::~TC_Library() { FT_Done_FreeType(_lib); }
