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
  TC_GlyChar(uint32_t code = 0);
  ~TC_GlyChar();

  inline uint32_t code() const { return _code; }

  void build(const TC_Font &);

  std::shared_ptr<TC_FontTexture> get_texture() { return _texture; }

  inline float andvance() { return _advance; }

  inline float width() { return _width; }

  inline float height() { return _height; }

  inline std::pair<float, float> offset() { return std::make_pair(_offsetx, _offsety); }
  
  inline std::pair<float, float> tex_lt_coord() { return std::make_pair(_ltu, _ltv); }

  inline std::pair<float, float> tex_rb_coord() { return std::make_pair(_rbu, _rbv); }

  inline bool valid() { return _texture != nullptr; }

 private:
  uint32_t _code;

  int _padding;

  float _advance;
  
  float _offsetx, _offsety;

  float _width, _height;

  //float _ascender, _descender;

  float _ltu, _ltv, _rbu, _rbv;

  std::shared_ptr<TC_FontTexture> _texture;
};

}

#endif