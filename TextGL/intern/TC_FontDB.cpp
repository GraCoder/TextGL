#include "TC_FontDB.h"
#include "TC_Font.h"

#include <fontconfig/fontconfig.h>
#include <fontconfig/fcfreetype.h>

namespace TC_TEXT{

TC_FontDB::TC_FontDB() {}

TC_FontDB::~TC_FontDB() {}

std::string TC_FontDB::file_path(const TC_Font& ft)
{
  char *filename = 0;
  int weight = FC_WEIGHT_REGULAR;
  int slant = FC_SLANT_ROMAN;
  if (ft.bold()) 
    weight = FC_WEIGHT_BOLD;

  if (ft.italic())
    slant = FC_SLANT_ITALIC;

  FcInit();
  FcPattern *pattern = FcPatternCreate();
  FcPatternAddDouble(pattern, FC_SIZE, ft.point());
  FcPatternAddInteger(pattern, FC_WEIGHT, weight);
  FcPatternAddInteger(pattern, FC_SLANT, slant);
  FcPatternAddString(pattern, FC_FAMILY, (FcChar8 *)ft.family());
  FcConfigSubstitute(0, pattern, FcMatchPattern);
  FcDefaultSubstitute(pattern);
  FcResult result;
  FcPattern *match = FcFontMatch(0, pattern, &result);
  FcPatternDestroy(pattern);

  if (!match) {
    return 0;
  } else {
    FcValue value;
    FcResult result = FcPatternGet(match, FC_FILE, 0, &value);
    if (!result) {
      filename = strdup((char *)(value.u.s));
    }
    result = FcPatternGet(match, FC_INDEX, 0, &value);
    if (!result) {
      int idx = value.u.i;
      printf("");
    }
  }
  FcPatternDestroy(match);
  return filename;
}


}
