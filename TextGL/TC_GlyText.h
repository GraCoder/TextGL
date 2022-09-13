#ifndef _TC_GLY_TEXT_INC_
#define _TC_GLY_TEXT_INC_

#include <string>

class TC_GlyText {
public:
	TC_GlyText();
	~TC_GlyText();

	void set_text(const std::string &);

	void set_size(int);

private:

	int		_size;
		
	std::string _text;
};


#endif