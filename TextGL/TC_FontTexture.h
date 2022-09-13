#ifndef _TC_FONT_TEXTURE_INC_
#define _TC_FONT_TEXTURE_INC_

#include "TC_Font.h"

class TC_TextGL_EXPORT TC_FontTexture {
public:
	TC_FontTexture();

	TC_FontTexture(TC_FontTexture &);

	~TC_FontTexture();

	TC_FontTexture& operator=(TC_FontTexture &);

private:
};


#endif