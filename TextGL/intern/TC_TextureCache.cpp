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

void TC_TextureCache::build(TC_GlyChar *gly_char, const TC_Font *ft)
{
#if 0

  texture_font_t *tex = nullptr;
  texture_glyph_t *gly = nullptr;
  auto iter = _cache.find(*ft);
  if (iter != _cache.end()) {
    for (auto tx : iter->second) {
      tex = tx;
      gly = texture_font_find_glyph(tx, gly_char->code());
      if (gly) break;
    }
  } else {
    // auto set = std::make_unique<TC_TextureSet>();
    iter = _cache.insert(std::make_pair(*ft, TC_TextureSet())).first;
  }

  if (gly && tex) {
    construct(gly_char, tex, gly);
    return;
  }

  auto &texs = iter->second;

  bool create_tex = false;
  if (texs.empty())
    create_tex = true;
  else {
    auto tex = texs.front();
    auto ret = texture_font_load_glyph(tex, gly_char->code());
    if (ret) {
      int h = ret & 0xFF, w = ret >> 16;
      if (w > 0) {
        for (int i = 1; i < texs.size(); i++) {
          tex = texs[i];
          if (!texture_atlas_has_space(tex->atlas, w, h)) {
            texture_font_load_glyph(tex, gly_char->code());
            gly = texture_font_find_glyph(tex, gly_char->code());
            construct(gly_char, tex, gly);
            dirty_texture(tex);
            return;
          }
        }
        create_tex = true;
      } else
        return;
    } else {
      gly = texture_font_find_glyph(tex, gly_char->code());
      construct(gly_char, tex, gly);
      dirty_texture(tex);
    }
  }

  if (create_tex) {
    auto atlas = texture_atlas_new(DEF_TEXTURE_SIZE, DEF_TEXTURE_SIZE, 1);
    tex = texture_font_new_from_file(atlas, ft->point_size(), ft->file_path().c_str());
    texs.push_back(tex);
    if (!texture_font_load_glyph(tex, gly_char->code())) {
      gly = texture_font_find_glyph(tex, gly_char->code());
      construct(gly_char, tex, gly);
      dirty_texture(tex);
    }
  }
  #endif
  load_gly(gly_char, ft);
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
  //auto iter = _font_textures.find(tf);
  //if (iter != _font_textures.end())
  //  return iter->second;
  //else {
  //  auto tex = std::make_shared<TC_FontTexture>();
  //  tex->_width = tf->atlas->width;
  //  tex->_height = tf->atlas->height;
  //  tex->_data = tf->atlas->data;
  //  _font_textures[tf] = tex;
  //  return tex;
  //}
  //return 0;
}

void TC_TextureCache::dirty_texture(texture_font_t *tex)
{
  //auto iter = _font_textures.find(tex);
  //if (iter == _font_textures.end()) {
  //  // error
  //}
  //iter->second->set_dirty();
}

void TC_TextureCache::load_gly(TC_GlyChar *glychar, const TC_Font *ft) 
{ 
    auto face = load_face(ft->file_path()); 
    if (face == nullptr) return;
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
}

const TC_GlyChar *TC_TextureCache::find_glyph(const TC_Font *font, uint32_t code)
{
    auto &texs = _cache.find(font)
}

TC_TextureCache::TC_TextureCache() {}

TC_TextureCache::~TC_TextureCache() {}
