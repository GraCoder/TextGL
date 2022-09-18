#pragma once

#include <QWidget>

namespace osg {
	class Group;
}

namespace osgViewer {
	class Viewer;
}

class GLWidget : public QWidget {
  Q_OBJECT
 public:
  GLWidget();
  ~GLWidget();

  osg::Group *getRoot();

 private:
  void render();

  void resizeEvent(QResizeEvent *ev);

  void createViewer();

 private:
  typedef QWidget Base;

  osgViewer::Viewer *_viewer;
};