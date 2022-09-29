#ifndef _TC_TEXTURE_CACHE_INC_
#define _TC_TEXTURE_CACHE_INC_

#include <memory>
#include <string>
#include <map>
#include <vector>

#include <freetype/freetype.h>

namespace TC_TEXT{
	class TC_Font;
	class TC_GlyChar;
	class TC_FontTexture;
}

struct texture_font_t;
struct texture_glyph_t;

using TC_TEXT::TC_Font;
using TC_TEXT::TC_GlyChar;
using TC_TEXT::TC_FontTexture;

typedef std::vector<std::shared_ptr<TC_FontTexture>> TC_TextureSet;

class TC_TextureCache {
public:
  static TC_TextureCache *instance();

  void build(TC_GlyChar *, const TC_Font *);

private:
  void construct(TC_GlyChar *, texture_font_t *, texture_glyph_t *);

  std::shared_ptr<TC_FontTexture> make_texture(texture_font_t *);

  void dirty_texture(texture_font_t *);

  void load_gly(TC_GlyChar *, const TC_Font *);

  FT_Face	load_face(const std::string &, int idx = 0);
  FT_Error	set_size(FT_Face f, float size);

  const TC_GlyChar *find_glyph(const TC_Font *, uint32_t code);

private:
  TC_TextureCache();
  ~TC_TextureCache();

  std::map<TC_Font, TC_TextureSet> _cache;
};

#define TexCache ::TC_TextureCache::instance()

#endif