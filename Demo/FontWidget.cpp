#include "FontWidget.h"

#include <QToolbar>

#include <QFontComboBox>
#include <QStatusBar>
#include <QPlainTextEdit>
#include <QDockWidget>
#include <QOpenGLWidget>

#include <QFontDatabase>

#include "TC_Font.h"
#include "TC_GlyText.h"
#include "TC_GlyChar.h"
#include "TC_FontTexture.h"


const std::map<std::string, std::string> fontMap = {{"����", "SimSun"},
													{"����", "SimHei"},
													{"΢���ź�", "Microsoft YaHei"},
													{"΢���ź�Light", "Microsoft YaHei Light"},
													{"΢��������", "Microsoft JhengHei"},
													{"����", "KaiTi"},
													{"������", "NSimSun"},
													{"����", "FangSong"},
													{"��Բ", "YouYuan"},
													{"����", "LiSu"},
													{"����ϸ��", "STXihei"},
													{"���Ŀ���", "STKaiti"},
													{"��������", "STSong"},
													{"���ķ���", "STFangsong"},
													{"��������", "STZhongsong"},
													{"���Ĳ���", "STCaiyun"},
													{"��������", "STHupo"},
													{"������κ", "STXinwei"},
													{"��������", "STLiti"},
													{"�����п�", "STXingkai"},
													{"��������", "FZShuTi"},
													{"����Ҧ��", "FZYaoti"},
													{"˼Դ����", "Source Han Sans CN"},
													{"˼Դ����", "Source Han Serif SC"},
													{"��Ȫ��΢�׺�", "WenQuanYi Micro Hei"},
													{"ƻ��", "PingFang SC"},
													{"���ĺ���", "STHeiti"},
													{"���Ŀ���", "STKaiti"},
													{"��������", "STSong"},
													{"���ķ���", "STFangsong"},
													{"��������", "STZhongsong"},
													{"��������", "STHupo"},
													{"������κ", "STXinwei"},
													{"��������", "STLiti"},
													{"�����п�", "STXingkai"},
													{"��������", "Hiragino Sans GB"},
													{"��ͤ��-��", "Lantinghei SC"},
													{"������-��", "Hanzipen SC"},
													{"������-��", "Hannotate SC"},
													{"����-��", "Songti SC"},
													{"������-��", "Wawati SC"},
													{"κ��-��", "Weibei SC"},
													{"�п�-��", "Xingkai SC"},
													{"��Ʀ-��", "Yapi SC"},
													{"Բ��-��", "Yuanti SC"},
													{"�������", "HYQihei 40S"},
													{"�������", "HYQihei 50S"},
													{"�������", "HYQihei 60S"},
													{"���Ǵ��μ�", "HYDaSongJ"},
													{"���ǿ���", "HYKaiti"},
													{"���Ǽ����", "HYJiaShuJ"},
													{"����PP���", "HYPPTiJ"},
													{"�����������", "HYLeMiaoTi"},
													{"����С����", "HYXiaoMaiTiJ"},
													{"���ǳ�����", "HYChengXingJ"},
													{"���Ǻ���֦", "HYHeiLiZhiTiJ"},
													{"�����ſ��W", "HYYaKuHeiW"},
													{"���Ǵ�ڼ�", "HYDaHeiJ"},
													{"������κ����", "HYShangWeiShouShuW"}};




FontWidget::FontWidget()
{
	auto toolBar = new QToolBar;
	addToolBar(toolBar);

	auto ftBox = new QFontComboBox;
	toolBar->addWidget(ftBox);
	//ftBox->setWritingSystem(QFontDatabase::Latin);

	connect(ftBox, &QFontComboBox::currentFontChanged, this, &FontWidget::resetFont);

	//setStatusBar(new QStatusBar);

	{
		_edit = new QPlainTextEdit;
		auto logwgt = new QDockWidget();
		logwgt->setWidget(_edit);
		addDockWidget(Qt::BottomDockWidgetArea, logwgt);
	}

	setCentralWidget(new QOpenGLWidget);
}

FontWidget::~FontWidget()
{
}

void FontWidget::resetFont(const QFont &font)
{
	auto f = font.family().toLocal8Bit().toStdString();
	auto iter = fontMap.find(f);
	TC_Font ft(iter == fontMap.end() ? f.c_str() : iter->second.c_str());
	auto file_path = ft.file_path();
	_edit->appendPlainText(QString::fromLocal8Bit(file_path.c_str()));

	TC_GlyText tex;
	tex.set_font(ft);
	tex.set_text("n");

	for (auto &c : tex.get_chars()) {
		auto tx = c.get_texture();

		auto sz = tx->get_size();

		QImage(tx->const_data(), sz.first, sz.second, QImage::Format_Grayscale8).save("1.png");
	}
}
