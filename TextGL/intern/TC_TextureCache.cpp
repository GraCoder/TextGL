#include "TC_TextureCache.h"

#include "TC_Font.h"
#include "TC_FontTexture.h"
#include "TC_GlyChar.h"
#include "TC_FontMatrix.h"
#include "TC_Library.h"

#include "texture-atlas.h"
#include "texture-font.h"

#include <freetype/freetype.h>
#include <freetype/ftsizes.h>
#include <freetype/ftbitmap.h>
#include <freetype/ftglyph.h>
#include <freetype/ftbbox.h>

using namespace TC_TEXT;

TC_TextureCache *TC_TextureCache::instance()
{
  static TC_TextureCache ca;
  return &ca;
}

void TC_TextureCache::build(TC_GlyChar *glychar, const TC_Font *ft)
{
  const TC_GlyChar *cached;
  if(cached = find_glyph(ft, glychar->code())) {
    *glychar = *cached; 
    return;
  }

  if (!load_gly(glychar, ft))
    return;
    
  auto &set = _cache[*ft];
  for(auto iter : set){
    if(iter->getspace(glychar->width(), glychar->height())) {
    }
  }

  auto tex = std::make_shared<TC_FontTexture>();
  tex->addchar(glychar);
  set.push_back(tex);
}

void TC_TextureCache::construct(TC_GlyChar *gly_char, texture_font_t *tex, texture_glyph_t *gly) 
{ 
    gly_char->_padding = tex->padding;
    gly_char->_advance = gly->advance_x / 64.0;
    gly_char->_offsetx = gly->offset_x;
    gly_char->_offsety = gly->offset_y;
    gly_char->_width = gly->width;
    gly_char->_height = gly->height;

    gly_char->_ltu = gly->s0; gly_char->_ltv = gly->t0;
    gly_char->_rbu = gly->s1; gly_char->_rbv = gly->t1;

    gly_char->_texture = make_texture(tex); 
}

std::shared_ptr<TC_FontTexture> TC_TextureCache::make_texture(texture_font_t *tf)
{
  return 0;
}

void TC_TextureCache::dirty_texture(texture_font_t *tex)
{
  //auto iter = _font_textures.find(tex);
  //if (iter == _font_textures.end()) {
  //  // error
  //}
  //iter->second->set_dirty();
}

bool TC_TextureCache::load_gly(TC_GlyChar *glychar, const TC_Font *ft) 
{ 
    auto face = load_face(ft->file_path()); 
    if (face == nullptr) return false;
    FT_Error err = 0;
    FT_Size sz;

    err = FT_New_Size(face, &sz);
    err = FT_Activate_Size(sz);
    err = set_size(face, ft->point_size());

    
}

FT_Face TC_TextureCache::load_face(const std::string &file, int idx) 
{
  FT_Face face;
  FT_New_Face(TCLIB->ftlib(), file.c_str(), idx, &face);
  return face;
}

FT_Error TC_TextureCache::set_size(FT_Face face, float size) 
{
  FT_Error err = 0;
  FT_Matrix matrix = {(int)((1.0 / f26) * 0x10000L), (int)((0.0) * 0x10000L), (int)((0.0) * 0x10000L), (int)((1.0) * 0x10000L)};
  err = FT_Set_Char_Size(face, size * f26, 0, dpi, dpi);
  FT_Set_Transform(face, &matrix, NULL);
  return err;
}

const TC_GlyChar *TC_TextureCache::find_glyph(const TC_Font *font, uint32_t code)
{ 
  auto set = _cache[*font];
  for(auto iter : set){
    auto ch = iter->getchar(code);
    if (ch)
      return ch;
  }
  return nullptr;
}

TC_TextureCache::TC_TextureCache() {}

TC_TextureCache::~TC_TextureCache() {}
