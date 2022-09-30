#include "TC_GlyChar.h"
#include "TC_TextureCache.h"

namespace TC_TEXT {

TC_GlyChar::TC_GlyChar(uint32_t code) 
	: _code(code)
	, _texture(nullptr)
{}

TC_GlyChar::~TC_GlyChar() {}

void TC_GlyChar::build(const TC_Font &ft) { TexCache->build(this, &ft); }

}  // namespace TC_TEXT
