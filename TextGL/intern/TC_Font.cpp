#include "TC_Font.h"

#include "font-manager.h"

TC_Font::TC_Font(const char *family) 
	: _italic(false), _bold(false)
{
	int n = strlen(family);
	if (n > 31) n = 31;
	memcpy(_family, family, n);
	_family[n] = 0;	

	set_font_size(72);
}

TC_Font::~TC_Font()
{
}

void TC_Font::set_font_size(uint32_t sz)
{
	if (sz > 72)
		_size = 72;
	else if (sz < 5)
		_size = 5;
	else
		_size = sz;
}

std::string TC_Font::file_path() const
{
	return font_manager_match_description(_family, _size,_bold, _italic);
}
