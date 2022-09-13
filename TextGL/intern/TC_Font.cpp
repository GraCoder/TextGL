#include "TC_Font.h"

#include "font-manager.h"


using namespace ftgl;

#define TEXTURE_SIZE 1024

TC_Font::TC_Font(const char *family) 
	: _italic(false), _bold(false)
{
	int n = strlen(family);
	if (n > 31) n = 31;
	memcpy(_family, family, n);
	_family[n] = 0;	
}

TC_Font::~TC_Font()
{
}

std::string TC_Font::file_path()
{
	return font_manager_match_description(_family, _size,_bold, _italic);
}
