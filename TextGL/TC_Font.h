#ifndef _TC_FONT_INC_
#define _TC_FONT_INC_

#include <string>

#ifdef TextGL_EXPORTS
#if defined _WIN32 || defined WINDOWS
#define TC_TextGL_EXPORT __declspec(dllexport)
#endif
#endif
#ifndef TC_TextGL_EXPORT
#define TC_TextGL_EXPORT
#endif

namespace TC_TEXT{

enum RenderMode { 
    RENDER_NORMAL, 
    RENDER_OUTLINE_EDGE, 
    RENDER_OUTLINE_POSITIVE, 
    RENDER_OUTLINE_NEGATIVE, 
    RENDER_SIGNED_DISTANCE_FIELD 
}; 

class TC_TextGL_EXPORT TC_Font {
public:
  TC_Font(const char *ptr = "simsun");
  ~TC_Font();

  //pixel size
  uint32_t size() const;
  
  uint32_t point_size() const; 

  void set_font_pt_size(uint32_t);

  void set_font_pix_size(uint32_t);

  std::string file_path() const;

  inline bool operator<(const TC_Font &ft) const
  {
    auto ret = strcmp(_family, ft._family);
    if (ret < 0)
      return true;
    else if (ret > 0)
      return false;

    if (_italic < ft._italic)
      return true;
    else if (_italic > ft._italic)
      return false;

    return _bold < ft._bold;
  }

  RenderMode& rendermode() { return _render_mode; }
  const RenderMode& rendermode() const { return _render_mode; }

  int& hinting() { return _hinting; }
  const int& hinting() const { return _hinting; }

  int& padding() { return _padding; }
  const int& padding() const { return _padding; }

private:
  bool _italic = false;
  bool _bold = false;
  uint32_t _size = 12;

  int _hinting = 0, _padding = 0;
  RenderMode _render_mode;

  char _family[32];
};

}

#endif