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

  const char* family() const { return _family; }

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

  inline bool& bold() { return _bold; }
  inline const bool& bold() const { return _bold; }
  inline bool& italic() { return _italic; }
  inline const bool italic() const { return _italic; }

  int& hinting() { return _hinting; }
  const int& hinting() const { return _hinting; }

  int& padding() { return _padding; }
  const int& padding() const { return _padding; }

private:
  bool _bold = 0;
  bool _italic = 0;
  uint32_t _size = 12;

  int _hinting = 1, _padding = 0;
  RenderMode _render_mode = RENDER_NORMAL;

  char _family[32];
};

}

#endif