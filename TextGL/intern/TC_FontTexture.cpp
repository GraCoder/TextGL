#include "TC_FontTexture.h"

#include "texture-atlas.h"
#include "texture-font.h"

TC_FontTexture::TC_FontTexture()
{
	auto atlas = texture_atlas_new(1024, 1024, 1);
}

TC_FontTexture::TC_FontTexture(TC_FontTexture &)
{
}

TC_FontTexture::~TC_FontTexture()
{
}

TC_FontTexture&
TC_FontTexture::operator=(TC_FontTexture &)
{
	return *this;
}
