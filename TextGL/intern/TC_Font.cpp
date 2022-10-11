#include "TC_Font.h"
#include "TC_FontDB.h"
#include "TC_FontMatrix.h"

#include <cmath>

namespace TC_TEXT{

TC_Font::TC_Font(const char *family) 
    : _hinting(1)
{
  int n = strlen(family);
  if (n > 31) n = 31;
  memcpy(_family, family, n);
  _family[n] = 0;

  set_point(14);
}

TC_Font::~TC_Font() {}

uint32_t TC_Font::pixel() const 
{ 
    float ret = _size / 72.0 * dpi;
    return std::round(ret); 
}

uint32_t TC_Font::point() const { return _size; }

void TC_Font::set_point(uint32_t ptz)
{
  if (ptz > 72)
    ptz = 72;
  else if (ptz < 5)
    ptz = 5;

  _size = ptz;
}

void TC_Font::set_pixel(uint32_t sz) 
{
  _size = std::round(sz * 72.0 / dpi);
}

std::string TC_Font::file_path() const 
{
  TC_FontDB db;
  return db.file_path(*this);
}

}
