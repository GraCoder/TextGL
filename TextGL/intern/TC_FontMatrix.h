#ifndef _TC_FONT_MATRIX_INC_
#define _TC_FONT_MATRIX_INC_

#include "TC_Font.h"

namespace TC_TEXT{

constexpr uint32_t dpi = 96;
constexpr uint32_t f26 = 64;

class TC_FontMatrix {
public:
  TC_FontMatrix(const TC_Font &);

  ~TC_FontMatrix();

  const TC_Font& font() { return _font; } 

  inline float width() { return _width * _font.size(); }
  inline float height() { return _height * _font.size(); };
  inline float ascend() { return _ascender * _font.size(); } 
  inline float descend() { return _descender * _font.size(); }

private:

  void init();

  TC_Font _font;

  float _ascender;
  float _descender;
  float _width;
  float _height;
};

}

#endif