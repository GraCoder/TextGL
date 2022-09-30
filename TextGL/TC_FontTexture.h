#ifndef _TC_FONT_TEXTURE_INC_
#define _TC_FONT_TEXTURE_INC_

#include <memory>
#include <vector>
#include <map>

#include "TC_Font.h"
#include "TC_GlyChar.h"
#include "tvec.h"


namespace TC_TEXT{

class TC_GlyChar;

class TC_TextGL_EXPORT TC_FontTexture : public std::enable_shared_from_this<TC_FontTexture> {
  friend class TC_TextureCache;

public:
  TC_FontTexture(int w = 1024, int h = 1024);

  ~TC_FontTexture();

  inline bool isvalid() { return _width && _height && _data; }

  inline bool isdirty() { return _dirty; }

  inline void set_dirty(bool d = true) { _dirty = d; }

  inline std::pair<uint32_t, uint32_t> get_size() { return std::make_pair(_width, _height); };

  inline uint8_t* data() { return _data; }
  inline const uint8_t* const_data() const { return _data; }

  tg::vec2i getspace(int w, int h);

  void addchar(const TC_GlyChar* glychar) { _glys[glychar->code()] = *glychar; };

public:

  const TC_GlyChar* getchar(uint32_t code);

  int space_fit(int idx, int w, int h);

private:
  TC_FontTexture(TC_FontTexture&);

  TC_FontTexture& operator=(TC_FontTexture&);

private:
  bool _dirty = true;

  uint32_t _width = 0, _height = 0;

  uint8_t* _data = nullptr;

  std::map<uint32_t, TC_GlyChar> _glys;

  std::vector<tg::vec4i> _nodes;
};

}

#endif