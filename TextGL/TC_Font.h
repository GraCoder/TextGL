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

class TC_TextGL_EXPORT TC_Font {
public:
	TC_Font(const char *ptr);
	~TC_Font();

	std::string file_path();

private:

	bool		_italic = false;
	bool		_bold = false;
	uint32_t	_size = 12;

	char		_family[32];
};

#endif