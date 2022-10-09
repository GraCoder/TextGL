#ifndef _TC_LIBRARY_INC_
#define _TC_LIBRARY_INC_

#include <freetype/freetype.h>

constexpr float minsz = 14;

class TC_Library{
public:
  static TC_Library* instance();

  inline operator FT_Library(){ return _lib; }
  FT_Library ftlib() { return _lib; }


private:
  TC_Library();
  ~TC_Library();

  FT_Library _lib;
};

#define TCLIB TC_Library::instance()

#endif