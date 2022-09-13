#ifndef _TC_GLY_CHAR_INC_
#define _TC_GLY_CHAR_INC_

#include <memory>

class TC_FontTexture;

class TC_GlyChar {
public:
	TC_GlyChar(uint32_t code);
	~TC_GlyChar();

private:
	uint32_t _code;

	union {

	};
	uint32_t x = 0, y = 0, w = 0, h = 0;

	std::shared_ptr<TC_FontTexture> _texture;
};


#endif