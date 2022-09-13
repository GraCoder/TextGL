#pragma once

#include <QMainWindow>

class QPlainTextEdit;

class FontWidget : public QMainWindow {
Q_OBJECT
public:
	FontWidget();
	~FontWidget();

	void resetFont(const QFont &);

private:
	QPlainTextEdit *_edit = nullptr;
};