#include "texture-atlas.h"
#include "texture-font.h"

int main()
{
	auto atlas = ftgl::texture_atlas_new(1024, 1024, 1);
	auto ft = ftgl::texture_font_new_from_file(atlas, 18, "C:\\Windows\\Fonts\\simsun.ttc");
	ftgl::texture_font_load_glyphs(ft, "abcdefg");
	texture_font_delete(ft);
	return 0;
}