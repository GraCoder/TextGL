#include "TC_TextureCache.h"

#include "TC_Font.h"
#include "TC_FontTexture.h"
#include "TC_GlyChar.h"
#include "TC_FontMatrix.h"
#include "TC_Library.h"

#include <freetype/freetype.h>
#include <freetype/ftsizes.h>
#include <freetype/ftbitmap.h>
#include <freetype/ftglyph.h>
#include <freetype/ftbbox.h>
#include <freetype/ftstroke.h>

using namespace TC_TEXT;

TC_TextureCache *TC_TextureCache::instance()
{
  static TC_TextureCache ca;
  return &ca;
}

void TC_TextureCache::build(TC_GlyChar *glychar, const TC_Font *ft)
{
  const TC_GlyChar *cached;
  if (cached = find_glyph(ft, glychar->code())) {
    *glychar = *cached;
    return;
  }

  if (!load_gly(glychar, ft))
    return;

  int w = glychar->box_width();
  int h = glychar->box_height();
  auto &set = _cache[*ft];
  for (auto iter : set) {
    auto sp = iter->getspace(w, h);
    if (sp.x() != -1 && sp.y() != -1) {
      iter->addchar(glychar, sp);
      iter->filldata(tg::vec4i(sp, w, h), _data);
      return;
    }
  }

  auto tex = std::make_shared<TC_FontTexture>();
  auto sp = tex->getspace(w, h);
  tex->addchar(glychar, sp);
  tex->filldata(tg::vec4i(sp, w, h), _data);
  set.push_back(tex);
}

bool TC_TextureCache::load_gly(TC_GlyChar *glychar, const TC_Font *ft)
{
  auto face = load_face(ft->file_path());
  if (face == nullptr)
    return false;
  FT_Error err = 0;
  FT_Size sz;

  err = FT_Select_Charmap(face, FT_ENCODING_UNICODE);
  err = FT_New_Size(face, &sz);
  err = FT_Activate_Size(sz);
  err = set_size(face, ft->point());

  int flags = 0;
  if (ft->rendermode() != RENDER_NORMAL && ft->rendermode() != RENDER_SIGNED_DISTANCE_FIELD) {
    flags |= FT_LOAD_NO_BITMAP;
  } else {
    flags |= FT_LOAD_RENDER;
  }

  if (!ft->hinting()) {
    flags |= FT_LOAD_NO_HINTING | FT_LOAD_NO_AUTOHINT;
  } else {
    flags |= FT_LOAD_FORCE_AUTOHINT;
  }

  //if (self->atlas->depth == 3) {
  //  FT_Library_SetLcdFilter(self->library->library, FT_LCD_FILTER_LIGHT);
  //  flags |= FT_LOAD_TARGET_LCD;

  //  if (self->filtering) {
  //    FT_Library_SetLcdFilterWeights(self->library->library, self->lcd_weights);
  //  }
  //} else if (HRES == 1) {
  //  /* ¡°FT_LOAD_TARGET_LIGHT
  //   *  A lighter hinting algorithm for gray-level modes. Many generated
  //   *  glyphs are fuzzier but better resemble their original shape.
  //   *  This is achieved by snapping glyphs to the pixel grid
  //   *  only vertically (Y-axis), as is done by FreeType's new CFF engine
  //   *  or Microsoft's ClearType font renderer.¡±
  //   * https://www.freetype.org/freetype2/docs/reference/ft2-base_interface.html#ft_load_target_xxx
  //   */
  //  flags |= FT_LOAD_TARGET_LIGHT;
  //}

//  if (depth == 4) {
//#ifdef FT_LOAD_COLOR
//    flags |= FT_LOAD_COLOR;
//#else
//    freetype_gl_error(Load_Color_Not_Available);
//#endif
//  }

  auto char_idx = FT_Get_Char_Index(face, glychar->code());
  err = FT_Load_Glyph(face, char_idx, flags);

  FT_GlyphSlot slot = nullptr;
  FT_Bitmap *bitmap = nullptr;
  if (ft->rendermode() == RENDER_NORMAL || ft->rendermode() == RENDER_SIGNED_DISTANCE_FIELD) {
    slot = face->glyph;
    bitmap = &slot->bitmap;
    auto ft_glyph_top = slot->bitmap_top;
    auto ft_glyph_left = slot->bitmap_left;
  } else {
    //FT_Stroker stroker;
    //FT_BitmapGlyph ft_bitmap_glyph;

    //err = FT_Stroker_New(TCLIB->ftlib(), &stroker);
    //FT_Stroker_Set(stroker, (int)(self->outline_thickness * HRES), FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

    //err = FT_Get_Glyph(self->face->glyph, &ft_glyph);

    //if (ft->rendermode() == RENDER_OUTLINE_EDGE)
    //  err = FT_Glyph_Stroke(&ft_glyph, stroker, 1);
    //else if (ft->rendermode() == RENDER_OUTLINE_POSITIVE)
    //  err = FT_Glyph_StrokeBorder(&ft_glyph, stroker, 0, 1);
    //else if (ft->rendermode() == RENDER_OUTLINE_NEGATIVE)
    //  err = FT_Glyph_StrokeBorder(&ft_glyph, stroker, 1, 1);

    //switch (self->atlas->depth) {
    //  case 1:
    //    err = FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_NORMAL, 0, 1);
    //    break;
    //  case 3:
    //    err = FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_LCD, 0, 1);
    //    break;
    //  case 4:
    //    err = FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_NORMAL, 0, 1);
    //    break;
    //}

    //ft_bitmap_glyph = (FT_BitmapGlyph)ft_glyph;
    //ft_bitmap = ft_bitmap_glyph->bitmap;
    //ft_glyph_top = ft_bitmap_glyph->top;
    //ft_glyph_left = ft_bitmap_glyph->left;

    //FT_Stroker_Done(stroker); 
  }

  tg::vec4i padding = {0, 0, 1, 1};

  if (ft->rendermode() == RENDER_SIGNED_DISTANCE_FIELD) {
    padding.y() = 1;
    padding.x() = 1;
  }

  if (ft->padding() != 0) {
    padding.y() += ft->padding();
    padding.x() += ft->padding();
    padding.z() += ft->padding();
    padding.w() += ft->padding();
  }

  size_t src_w = /*depth == 3 ? ft_bitmap.width / 3 : */bitmap->width;
  size_t src_h = bitmap->rows;

  size_t tgt_w = src_w + padding.x() + padding.z();
  size_t tgt_h = src_h + padding.y() + padding.w();

  glychar->_width = src_w;
  glychar->_height = src_h;
  glychar->_padding = padding;
  //glychar->_padding = 0;
  glychar->_advance = slot->advance.x / 64.0;
  glychar->_offsetx = slot->bitmap_left;
  glychar->_offsety = slot->bitmap_top;

  cache_data(*bitmap, tgt_w, tgt_h, padding.x(), padding.y());

  FT_Done_Face(face);
  return err == 0;
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
  FT_Matrix matrix = {
      (int)((1.0 / f26) * 0x10000L),
      (int)((0.0) * 0x10000L), 
      (int)((0.0) * 0x10000L), 
      (int)((1.0) * 0x10000L)
  };
  err = FT_Set_Char_Size(face, size * f26, 0, dpi * f26, dpi);
  FT_Set_Transform(face, &matrix, NULL);
  return err;
}

void TC_TextureCache::cache_data(FT_Bitmap &bt, int w, int h, int l, int t) 
{
  //_data.resize(w * h, 0);
  _data.assign(w * h, 0);
  auto dst_ptr = &_data[0] + (t * w + l) /** depth*/;
  auto src_ptr = bt.buffer;
  //  if (ft_bitmap.pixel_mode == FT_PIXEL_MODE_BGRA && self->atlas->depth == 4) {
  //    // BGRA in, RGBA out
  //    for (i = 0; i < src_h; i++) {
  //      int j;
  //      for (j = 0; j < ft_bitmap.width; j++) {
  //        uint32_t bgra, rgba;
  //        bgra = ((uint32_t *)src_ptr)[j];
  //#if __BYTE_ORDER == __BIG_ENDIAN
  //        rgba = rol(__builtin_bswap32(bgra), 8);
  //#else
  //        rgba = rol(__builtin_bswap32(bgra), 24);
  //#endif
  //        ((uint32_t *)dst_ptr)[j] = rgba;
  //      }
  //      dst_ptr += tgt_w * self->atlas->depth;
  //      src_ptr += ft_bitmap.pitch;
  //    }
  //  } else if (ft_bitmap.pixel_mode == FT_PIXEL_MODE_BGRA && self->atlas->depth == 1) {
  //    // BGRA in, grey out: Use weighted sum for luminosity, and multiply by alpha
  //    struct src_pixel_t {
  //      uint8_t b;
  //      uint8_t g;
  //      uint8_t r;
  //      uint8_t a;
  //    } *src = (struct src_pixel_t *)ft_bitmap.buffer;
  //    for (int row = 0; row < src_h; row++, dst_ptr += tgt_w * self->atlas->depth) {
  //      for (int col = 0; col < src_w; col++, src++) {
  //        dst_ptr[col] = (0.3 * src->r + 0.59 * src->g + 0.11 * src->b) * (src->a / 255.0);
  //      }
  //    }
  //  } else if (ft_bitmap.pixel_mode == FT_PIXEL_MODE_GRAY && self->atlas->depth == 4) {
  //    // Grey in, RGBA out: Use grey level for alpha channel, with white color
  //    struct dst_pixel_t {
  //      uint8_t r;
  //      uint8_t g;
  //      uint8_t b;
  //      uint8_t a;
  //    } *dst = (struct dst_pixel_t *)dst_ptr;
  //    for (int row = 0; row < src_h; row++, dst += tgt_w) {
  //      for (int col = 0; col < src_w; col++, src_ptr++) {
  //        dst[col] = (struct dst_pixel_t){255, 255, 255, *src_ptr};
  //      }
  //    }
  //  } else {
  for (int i = 0; i < bt.rows; i++) {
    memcpy(dst_ptr, src_ptr, bt.width);
    dst_ptr += w;
    src_ptr += bt.pitch;
  }
  //}
}

const TC_GlyChar *TC_TextureCache::find_glyph(const TC_Font *font, uint32_t code)
{
  auto set = _cache[*font];
  for (auto iter : set) {
    auto ch = iter->getchar(code);
    if (ch)
      return ch;
  }
  return nullptr;
}

TC_TextureCache::TC_TextureCache() {}

TC_TextureCache::~TC_TextureCache() {}
