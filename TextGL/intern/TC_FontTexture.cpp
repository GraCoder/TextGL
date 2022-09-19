#include "TC_FontTexture.h"

#include "texture-atlas.h"
#include "texture-font.h"

namespace TC_TEXT{

TC_FontTexture::TC_FontTexture() {}

TC_FontTexture::TC_FontTexture(TC_FontTexture &other) { this->operator=(other); }

TC_FontTexture::~TC_FontTexture() {}

TC_FontTexture &TC_FontTexture::operator=(TC_FontTexture &other)
{
  _width = other._width;
  _height = other._height;
  _data = other._data;

  return *this;
}

}
