#ifndef _TC_FONT_TEXTURE_INC_
#define _TC_FONT_TEXTURE_INC_

#include "TC_Font.h"

class TC_TextGL_EXPORT TC_FontTexture {
	friend class TC_TextureCache;
public:
	TC_FontTexture();

	TC_FontTexture(TC_FontTexture &);

	~TC_FontTexture();

	TC_FontTexture& operator=(TC_FontTexture &);

	bool isvalid() { return _width && _height && _data; }

	std::pair<uint32_t, uint32_t> get_size() { return std::make_pair(_width, _height); };

	const uint8_t* const_data() const { return _data; } 

private:

	uint32_t _width = 0, _height = 0;

	uint8_t*_data = nullptr;
};


#endif