#pragma once

#include <QMainWindow>

#include "TC_Font.h"

class QPlainTextEdit;

class GLWidget;

class FontWidget : public QMainWindow {
Q_OBJECT
public:
	FontWidget();
	~FontWidget();

	void resetFont(const QFont &);

	void update_text();

private:
	QPlainTextEdit *_edit = nullptr;

	QPlainTextEdit *_log = nullptr;

	TC_TEXT::TC_Font _ft;

	GLWidget *_gl_widget;
};