#ifndef _TC_GLY_TEXT_INC_
#define _TC_GLY_TEXT_INC_

#include <string>
#include <vector>
#include <map>

#include "TC_Font.h"
#include "TC_GlyChar.h"
#include "tvec.h"

namespace TC_TEXT{

enum class AxisAlignment{
    AA_XY_PLANE,
    AA_SCREEN
};

enum class CharacterSizeMode{
    CSM_OBJECT,
    CSM_SCREEN
};

class TC_FontMatrix;

class TC_TextGL_EXPORT TC_GlyText {
public:
  TC_GlyText();
  ~TC_GlyText();

  const TC_Font &font() { return _font; }

  inline float &ratio() { return _ratio; }

  void set_font(const TC_Font &ft) { _font = ft; }

  //utf-8
  void set_text(const std::string &text);

  //unicode
  void set_text(const std::vector<uint32_t> &text);

  void set_line_maxpix(int pix);

  void set_line_maxnum(int num);

  void set_axis_alignment(AxisAlignment aa);
  AxisAlignment axis_alignment() { return _axis_align; }

  void set_charactor_size_mode(CharacterSizeMode csm);
  CharacterSizeMode character_size_mode() { return _size_mode; };

  struct GlyChars{
    std::vector<TC_GlyChar> chars;
    std::vector<uint32_t> nums;
  };

  GlyChars get_chars();

  std::vector<tg::vec3> vertexs(GlyChars &);

  std::vector<tg::vec2> tex_coords(GlyChars &);

  std::map<TC_FontTexture*, std::vector<uint32_t>> indexs(GlyChars &);

  inline tg::vec3 min() { return _min; }
  inline tg::vec3 max() { return _max; }

private:
  TC_Font _font;

  float _ratio;

  int _line_max;

  AxisAlignment _axis_align;

  CharacterSizeMode _size_mode;

  tg::vec3 _min, _max;

  std::shared_ptr<TC_FontMatrix> _ft_matrix;

  std::vector<std::vector<uint32_t>> _texts;
};

}

#endif