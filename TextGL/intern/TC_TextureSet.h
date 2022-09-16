#ifndef _TC_TEXTURE_SET_INC_
#define _TC_TEXTURE_SET_INC_

#include <vector>

struct texture_font_t;

class TC_TextureSet {
public:
	TC_TextureSet()
	{
	}
	
	std::vector<texture_font_t *> _textures;
};



#endif