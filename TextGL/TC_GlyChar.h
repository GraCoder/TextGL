#ifndef _TC_GLY_CHAR_INC_
#define _TC_GLY_CHAR_INC_

#include <memory>

#include "TC_Font.h"

namespace TC_TEXT{

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

  int _padding;

  float _advance;
  
  float _offsetx, _offsety;

  float _height;

  //float _ascender, _descender;

  float _ltu, _ltv, _rbu, _rbv;

  std::shared_ptr<TC_FontTexture> _texture;
};

}

#endif