#include "TC_GlyChar.h"
#include "TC_TextureCache.h"

namespace TC_TEXT {

TC_GlyChar::TC_GlyChar(uint32_t code) 
	: _code(code)
	, _texture(nullptr)
	, _offsetx(0)
	, _offsety(0)
	, _width(0)
	, _height(0)
	, _advance(0)
	, _padding(0, 0, 0, 0)
{}

TC_GlyChar::~TC_GlyChar() {}

void TC_GlyChar::build(const TC_Font &ft) { TexCache->build(this, &ft); }

}  // namespace TC_TEXT
