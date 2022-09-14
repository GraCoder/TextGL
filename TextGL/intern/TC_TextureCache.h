#ifndef _TC_TEXTURE_CACHE_INC_
#define _TC_TEXTURE_CACHE_INC_

#include <memory>
#include <string>
#include <map>
#include <vector>

class TC_Font;
class TC_GlyChar;
class TC_FontTexture;

namespace ftgl {
	struct texture_atlas_t;
	struct texture_font_t;
}

class TC_TextureSet {
public:
	TC_TextureSet()
	{
	}
	
	std::vector<ftgl::texture_font_t *> _textures;
};

class TC_TextureCache {
public:

	static TC_TextureCache* instance();

	void build(TC_GlyChar *, const TC_Font *);

private:

	std::shared_ptr<TC_FontTexture> make_texture(ftgl::texture_font_t *);

private:
	TC_TextureCache();
	~TC_TextureCache();

	std::map<TC_Font, std::unique_ptr<TC_TextureSet> > _cache;
};


#define TexCache TC_TextureCache::instance()

#endif