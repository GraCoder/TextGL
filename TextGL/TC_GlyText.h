#ifndef _TC_GLY_TEXT_INC_
#define _TC_GLY_TEXT_INC_

#include <string>
#include <vector>

#include "TC_Font.h"
#include "TC_GlyChar.h"

namespace TC_TEXT{

class TC_TextGL_EXPORT TC_GlyText {
public:
  TC_GlyText();
  ~TC_GlyText();

  const TC_Font &font() { return _font; };

  void set_font(const TC_Font &ft) { _font = ft; }

  void set_text(const std::string &text);

  void set_text(const std::vector<uint32_t> &text);

  std::vector<TC_GlyChar> get_chars();

private:
  TC_Font _font;

  std::vector<uint32_t> _text;
};

}

#endif