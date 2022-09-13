#include "TC_Font.h"

#include "texture-atlas.h"
#include "texture-font.h"
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

	//auto mgr = ftgl::font_manager_new(TEXTURE_SIZE, TEXTURE_SIZE, 1);
	//ftgl::font_manager_get_from_description(mgr, "system", 14, 0, 0);

	//auto atlas = ftgl::texture_atlas_new(1024, 1024, 1);
	//auto ft = ftgl::texture_font_new_from_file(atlas, 18, "C:\\Windows\\Fonts\\simsun.ttc");
	//ftgl::texture_font_load_glyphs(ft, "abcdefg");
	//texture_font_delete(ft);
	//return 0;

}

TC_Font::~TC_Font()
{
}

std::string TC_Font::file_path()
{
	return font_manager_match_description(_family, _size,_bold, _italic);
}
