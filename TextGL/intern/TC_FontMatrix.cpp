#include "TC_FontMatrix.h"
#include "TC_Library.h"

#include <freetype/freetype.h>
#include <freetype/ftsizes.h>

namespace TC_TEXT{

TC_FontMatrix::TC_FontMatrix(const TC_Font &ft):_font(ft) { init(); }

TC_FontMatrix::~TC_FontMatrix() {}

void TC_FontMatrix::init()
{
  auto path = _font.file_path();
  FT_Face face;
  if (FT_New_Face(TCLIB->ftlib(), path.c_str(), 0, &face)) return;
  // FT_Size size;
  // if (FT_New_Size(face, &size)) return;
  //_font.set_point(36);
  //if (FT_Set_Char_Size(face, _font.size() * f26, 0, dpi, dpi)) return;

  //face->ascender;
  //face->descender;
  //face->height;
  //face->max_advance_width;
  //face->max_advance_height;
  //face->underline_position;
  //face->underline_thickness;

  float unit = face->units_per_EM;
  _height = face->height / unit; 
  _width = face->max_advance_width / unit;
  _ascender = face->ascender / unit;
  _descender = face->descender / unit;

  FT_Done_Face(face);
}


}

