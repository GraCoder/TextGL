#ifndef _TC_GLY_TEXT_INC_
#define _TC_GLY_TEXT_INC_

#include <string>
#include <vector>

#include "TC_Font.h"
#include "TC_GlyChar.h"

class TC_TextGL_EXPORT TC_GlyText {
	public:
	TC_GlyText();
	~TC_GlyText();

	void set_font(const TC_Font &ft) { _font = ft; }

	void set_text(const std::string &text) { _text = text; }

	std::vector<TC_GlyChar> get_chars();

private:
	TC_Font		_font;
		
	std::string		_text;
};


#endif