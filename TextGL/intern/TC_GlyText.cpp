#include "TC_GlyText.h"

#include <freetype/freetype.h>

TC_GlyText::TC_GlyText() : _font("simsum")
{
}

TC_GlyText::~TC_GlyText()
{
}

std::vector<TC_GlyChar> TC_GlyText::get_chars()
{
	std::vector<TC_GlyChar> ret;
	for (int i = 0; i < _text.size(); i++) {
		TC_GlyChar gly_char(_text[i]);
		gly_char.build(_font);
		ret.emplace_back(gly_char);
	}
	return ret;
}
