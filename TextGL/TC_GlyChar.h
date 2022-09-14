#ifndef _TC_GLY_CHAR_INC_
#define _TC_GLY_CHAR_INC_

#include <memory>

#include "TC_Font.h"

class TC_Font;
class TC_FontTexture;

class TC_TextGL_EXPORT TC_GlyChar {
	friend class TC_TextureCache;
public:
	TC_GlyChar(uint32_t code);
	~TC_GlyChar();

	inline uint32_t code() { return _code; }

	void build(const TC_Font &);

	std::shared_ptr<TC_FontTexture> get_texture() { return _texture; }

private:


	uint32_t _code;

	union {

	};
	uint32_t x = 0, y = 0, w = 0, h = 0;

	std::shared_ptr<TC_FontTexture> _texture;
};


#endif