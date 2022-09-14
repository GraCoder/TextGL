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


const std::map<std::string, std::string> fontMap = {{"宋体", "SimSun"},
													{"黑体", "SimHei"},
													{"微软雅黑", "Microsoft YaHei"},
													{"微软雅黑Light", "Microsoft YaHei Light"},
													{"微软正黑体", "Microsoft JhengHei"},
													{"楷体", "KaiTi"},
													{"新宋体", "NSimSun"},
													{"仿宋", "FangSong"},
													{"幼圆", "YouYuan"},
													{"隶书", "LiSu"},
													{"华文细黑", "STXihei"},
													{"华文楷体", "STKaiti"},
													{"华文宋体", "STSong"},
													{"华文仿宋", "STFangsong"},
													{"华文中宋", "STZhongsong"},
													{"华文彩云", "STCaiyun"},
													{"华文琥珀", "STHupo"},
													{"华文新魏", "STXinwei"},
													{"华文隶书", "STLiti"},
													{"华文行楷", "STXingkai"},
													{"方正舒体", "FZShuTi"},
													{"方正姚体", "FZYaoti"},
													{"思源黑体", "Source Han Sans CN"},
													{"思源宋体", "Source Han Serif SC"},
													{"文泉驿微米黑", "WenQuanYi Micro Hei"},
													{"苹方", "PingFang SC"},
													{"华文黑体", "STHeiti"},
													{"华文楷体", "STKaiti"},
													{"华文宋体", "STSong"},
													{"华文仿宋", "STFangsong"},
													{"华文中宋", "STZhongsong"},
													{"华文琥珀", "STHupo"},
													{"华文新魏", "STXinwei"},
													{"华文隶书", "STLiti"},
													{"华文行楷", "STXingkai"},
													{"冬青黑体简", "Hiragino Sans GB"},
													{"兰亭黑-简", "Lantinghei SC"},
													{"翩翩体-简", "Hanzipen SC"},
													{"手札体-简", "Hannotate SC"},
													{"宋体-简", "Songti SC"},
													{"娃娃体-简", "Wawati SC"},
													{"魏碑-简", "Weibei SC"},
													{"行楷-简", "Xingkai SC"},
													{"雅痞-简", "Yapi SC"},
													{"圆体-简", "Yuanti SC"},
													{"汉仪旗黑", "HYQihei 40S"},
													{"汉仪旗黑", "HYQihei 50S"},
													{"汉仪旗黑", "HYQihei 60S"},
													{"汉仪大宋简", "HYDaSongJ"},
													{"汉仪楷体", "HYKaiti"},
													{"汉仪家书简", "HYJiaShuJ"},
													{"汉仪PP体简", "HYPPTiJ"},
													{"汉仪乐喵体简", "HYLeMiaoTi"},
													{"汉仪小麦体", "HYXiaoMaiTiJ"},
													{"汉仪程行体", "HYChengXingJ"},
													{"汉仪黑荔枝", "HYHeiLiZhiTiJ"},
													{"汉仪雅酷黑W", "HYYaKuHeiW"},
													{"汉仪大黑简", "HYDaHeiJ"},
													{"汉仪尚魏手书", "HYShangWeiShouShuW"}};




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
