#include "TC_TextureCache.h"

#include "TC_Font.h"
#include "TC_GlyChar.h"
#include "TC_FontTexture.h"

#include "texture-atlas.h"
#include "texture-font.h"

#define TEXTURE_SIZE 1024

TC_TextureCache* TC_TextureCache::instance()
{
	static TC_TextureCache ca;
	return &ca;
}

void TC_TextureCache::build(TC_GlyChar*gly_char, const TC_Font *ft)
{
	ftgl::texture_font_t *tex = nullptr;
	ftgl::texture_glyph_t *gly = nullptr;
	auto iter = _cache.find(*ft);	
	if (iter != _cache.end()) {
		for (auto tx : iter->second->_textures) {
			tex = tx;
			gly = ftgl::texture_font_find_glyph_gi(tx, gly_char->code());
			if (gly) break;
		}
	}

	auto fun = [gly_char, this](ftgl::texture_font_t *tex, ftgl::texture_glyph_t *gly) {
		gly_char->_texture = make_texture(tex);
	};

	if (gly && tex) {
		fun(tex, gly);
		return;
	}

	auto atlas = ftgl::texture_atlas_new(TEXTURE_SIZE, TEXTURE_SIZE, 1);
	tex = ftgl::texture_font_new_from_file( atlas, ft->font_size(), ft->file_path().c_str());
	if (ftgl::texture_font_load_glyph(tex, gly_char->code()))
		gly = ftgl::texture_font_find_glyph(tex, gly_char->code());

	if (gly && tex) fun(tex, gly);
}

std::shared_ptr<TC_FontTexture> TC_TextureCache::make_texture(ftgl::texture_font_t *tf)
{
	auto tex = std::make_shared<TC_FontTexture>();
	tex->_width = tf->atlas->width;
	tex->_height = tf->atlas->height;
	tex->_data = tf->atlas->data;

	return tex;
}

TC_TextureCache::TC_TextureCache()
{
}

TC_TextureCache::~TC_TextureCache()
{
}
