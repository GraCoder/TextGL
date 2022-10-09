#ifndef _TC_FONTDB_INC_
#define _TC_FONTDB_INC_

#include <string>

namespace TC_TEXT{

class TC_Font;

class TC_FontDB{
public:
  TC_FontDB();
  ~TC_FontDB();

  std::string file_path(const TC_Font &ft);
};

}

#endif