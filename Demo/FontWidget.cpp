#include "FontWidget.h"

#include <QDockWidget>
#include <QFontComboBox>
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QOpenGLWidget>
#include <QPlainTextEdit>
#include <QStatusBar>
#include <QToolbar>

#include "GLWidget.h"
#include "TC_Font.h"
#include "TC_FontTexture.h"
#include "TC_GlyChar.h"
#include "TC_GlyText.h"
#include "TC_TextNode.h"

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
  // ftBox->setWritingSystem(QFontDatabase::Latin);

  connect(ftBox, &QFontComboBox::currentFontChanged, this, &FontWidget::resetFont);

  // setStatusBar(new QStatusBar);

  {
    _log = new QPlainTextEdit;
    auto logwgt = new QDockWidget();
    logwgt->setWidget(_log);
    addDockWidget(Qt::BottomDockWidgetArea, logwgt);
  }

  {
    auto wgt = new QWidget;
    auto txt = new QPlainTextEdit;
    auto lay = new QHBoxLayout(wgt);
    _gl_widget = new GLWidget;
    lay->addWidget(_gl_widget, 3);
    lay->addWidget(txt, 1);
    setCentralWidget(wgt);
    _edit = txt;

    connect(txt, &QPlainTextEdit::textChanged, this, &FontWidget::update_text);
  }
}

FontWidget::~FontWidget() {}

void FontWidget::resetFont(const QFont &font)
{
  auto f = font.family().toLocal8Bit().toStdString();
  auto iter = fontMap.find(f);
  TC_TEXT::TC_Font ft(iter == fontMap.end() ? f.c_str() : iter->second.c_str());
  auto file_path = ft.file_path();
  _edit->appendPlainText(QString::fromLocal8Bit(file_path.c_str()));

  _ft = ft;
}

void FontWidget::update_text()
{
  std::vector<uint32_t> codes;
  auto str = _edit->toPlainText();
  if (str.isEmpty())
    str = "ABC";

  for (int i = 0; i < str.size(); i++) {
    auto ch = str[i];
    codes.push_back(ch.unicode());
  }

  TC_TEXT::TC_GlyText tex;
  tex.set_font(_ft);
  tex.set_text(codes);
  auto chars = tex.get_chars();

  // if(!chars.empty()){
  //   auto &c = chars.front();
  //   auto tx = c.get_texture();
  //   auto sz = tx->get_size();
  // QImage(tx->const_data(), sz.first, sz.second,
  // QImage::Format_Grayscale8).save(QString("test{}.png"));
  // }

  auto root = _gl_widget->getRoot();
  if (root) {
    root->removeChildren(0, root->getNumChildren());
    auto node = new TC_TEXT::TC_TextNode;
    node->setText(tex);
    root->addChild(node);
  }
}
