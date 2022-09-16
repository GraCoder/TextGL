#include "TC_TextureCache.h"

#include "TC_Font.h"
#include "TC_FontTexture.h"
#include "TC_GlyChar.h"
#include "TC_TextureSet.h"
#include "texture-atlas.h"
#include "texture-font.h"

TC_TextureCache *TC_TextureCache::instance() {
  static TC_TextureCache ca;
  return &ca;
}

void TC_TextureCache::build(TC_GlyChar *gly_char, const TC_Font *ft) {
  texture_font_t *tex = nullptr;
  texture_glyph_t *gly = nullptr;
  auto iter = _cache.find(*ft);
  if (iter != _cache.end()) {
    for (auto tx : iter->second->_textures) {
      tex = tx;
      gly = texture_font_find_glyph(tx, gly_char->code());
      if (gly) break;
    }
  } else {
    auto set = std::make_unique<TC_TextureSet>();
    iter = _cache.insert(std::make_pair(*ft, set)).first;
  }

  if (gly && tex) {
    construct(gly_char, tex, gly);
    return;
  }

  auto &texs = iter->second->_textures;

  bool create_tex = false;
  if (iter == _cache.end()) create_tex = true;
  else {
    auto tex = texs.front();
    auto ret = texture_font_load_glyph(tex, gly_char->code());
    if (ret) {
      int h = ret & 0xFF, w = ret >> 16;
      if (w > 0) {
        for (int i = 1; i < texs.size(); i++) {
          if (!texture_atlas_has_space(texs[i]->atlas, w, h)) {
            texture_font_load_glyph(tex, gly_char->code());
            gly = texture_font_find_glyph(tex, gly_char->code());
            construct(gly_char, tex, gly);
            return;
          }
        }
        create_tex = true;
      } else
        return;
    } else {
      gly = texture_font_find_glyph(tex, gly_char->code());
      construct(gly_char, tex, gly);
    }
  }

  if (create_tex) {
    auto atlas = texture_atlas_new(DEF_TEXTURE_SIZE, DEF_TEXTURE_SIZE, 1);
    if (iter == _cache.end())
      tex = texture_font_new_from_file(atlas, ft->font_size(), ft->file_path().c_str());
    else {
      auto &fst = iter->second->_textures.front();
      tex = texture_font_clone(fst, fst->size);
      tex->atlas = atlas;
    }

    if (!texture_font_load_glyph(tex, gly_char->code())) {
      gly = texture_font_find_glyph(tex, gly_char->code());
      construct(gly_char, tex, gly);
    }
  }
}

void TC_TextureCache::construct(TC_GlyChar *gly_char, texture_font_t *tex, texture_glyph_t *gly) { gly_char->_texture = make_texture(tex); }

std::shared_ptr<TC_FontTexture> TC_TextureCache::make_texture(texture_font_t *tf) {
  auto tex = std::make_shared<TC_FontTexture>();
  tex->_width = tf->atlas->width;
  tex->_height = tf->atlas->height;
  tex->_data = tf->atlas->data;

  return tex;
}

TC_TextureCache::TC_TextureCache() {}

TC_TextureCache::~TC_TextureCache() {}
