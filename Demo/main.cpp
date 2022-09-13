
#include "TC_Font.h"
#include "TC_FontTexture.h"
#include "FontWidget.h"

#include <QApplication>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	FontWidget wgt;
	wgt.show();

	app.exec();

	//TC_Font ft("");
}