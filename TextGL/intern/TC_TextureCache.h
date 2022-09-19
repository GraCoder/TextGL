#ifndef _TC_TEXTURE_CACHE_INC_
#define _TC_TEXTURE_CACHE_INC_

#include <memory>
#include <string>
#include <map>
#include <vector>

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

typedef std::vector<texture_font_t *> TC_TextureSet;

class TC_TextureCache {
public:
  static TC_TextureCache *instance();

  void build(TC_GlyChar *, const TC_Font *);

private:
  void construct(TC_GlyChar *, texture_font_t *, texture_glyph_t *);

  std::shared_ptr<TC_FontTexture> make_texture(texture_font_t *);

  void dirty_texture(texture_font_t *);

private:
  TC_TextureCache();
  ~TC_TextureCache();

  std::map<TC_Font, TC_TextureSet> _cache;

  std::map<texture_font_t *, std::shared_ptr<TC_FontTexture> > _font_textures;
};

#define TexCache ::TC_TextureCache::instance()

#endif