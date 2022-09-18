#ifndef _TC_FONT_TEXTURE_INC_
#define _TC_FONT_TEXTURE_INC_

#include "TC_Font.h"

class TC_TextGL_EXPORT TC_FontTexture {
	friend class TC_TextureCache;

public:
	TC_FontTexture();

	~TC_FontTexture();

	inline bool isvalid() { return _width && _height && _data; }

	inline bool isdirty() { return _dirty; }

	inline void set_dirty() { _dirty = true; }

	inline std::pair<uint32_t, uint32_t> get_size() { return std::make_pair(_width, _height); };

	inline const uint8_t* const_data() const { return _data; } 

private:

	TC_FontTexture(TC_FontTexture &);

	TC_FontTexture& operator=(TC_FontTexture &);

private:

	bool _dirty = true;

	uint32_t _width = 0, _height = 0;

	uint8_t*_data = nullptr;
};


#endif