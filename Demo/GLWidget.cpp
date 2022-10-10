#include "GLWidget.h"

#include <QTimer>

#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgGA/UFOManipulator>
#include <osgGA/TerrainManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/Win32/GraphicsWindowWin32>
#include <osgDB/ReadFile>

#include <Windows.h>

GLWidget::GLWidget() : _viewer(nullptr)
{
  setMinimumSize(600, 480);
  auto timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &GLWidget::render);
  timer->start(30);
}

GLWidget::~GLWidget() {}

osg::Group* GLWidget::getRoot() { return dynamic_cast<osg::Group*>(_viewer->getSceneData()); }

void GLWidget::render()
{
  if (_viewer) _viewer->frame();
}

void GLWidget::resizeEvent(QResizeEvent* ev)
{
  Base::resizeEvent(ev);

  if (!_viewer) {
    createViewer();
  }

  auto sz = size();
  int w = sz.width(), h = sz.height();

  if (w == 0 || h == 0) return;

  if (!_viewer) return;

  auto cam = _viewer->getCamera();
  auto ctx = cam->getGraphicsContext();
  auto handle = dynamic_cast<osgViewer::GraphicsHandleWin32*>(ctx);
  if (handle) MoveWindow(handle->getHWND(), 0, 0, w, h, true);
}

void GLWidget::createViewer()
{
  _viewer = new osgViewer::Viewer;
  _viewer->setThreadingModel(_viewer->SingleThreaded);
  //_viewer->setCameraManipulator(new osgGA::TrackballManipulator());
  //_viewer->setCameraManipulator(new osgGA::UFOManipulator);
  _viewer->setCameraManipulator(new osgGA::TerrainManipulator);
  _viewer->addEventHandler(new osgViewer::StatsHandler);
  _viewer->setLightingMode(_viewer->NO_LIGHT);

  osg::ref_ptr<osg::GraphicsContext> gc;
  osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
  traits->x = 0;
  traits->y = 0;
  traits->width = 600;
  traits->height = 480;
  traits->windowingSystemPreference = "Win32";
  traits->windowDecoration = false;
  traits->doubleBuffer = true;
  traits->sharedContext = 0;
  traits->readDISPLAY();
  traits->setUndefinedScreenDetailsToDefaultScreen();

  gc = osg::GraphicsContext::createGraphicsContext(traits.get());
  gc->realize();

  auto camera = _viewer->getCamera();
  camera->setGraphicsContext(gc.get());
  camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
  GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
  camera->setDrawBuffer(buffer);
  camera->setReadBuffer(buffer);

  _viewer->setCamera(camera);

  auto handle = dynamic_cast<osgViewer::GraphicsHandleWin32*>(gc.get());
  if (handle) {
    SetParent(handle->getHWND(), (HWND)winId());
  }

  auto root = new osg::Group;
  _viewer->setSceneData(root);
  root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
  {
    //auto testNode = osgDB::readNodeFile("D:\\08_Store\\OpenSceneGraph-Data\\cow.osg");
    auto testNode = osgDB::readNodeFile("D:\\dev\\osg\\OpenSceneGraph-Data-3.4.0\\OpenSceneGraph-Data\\cow.osg");
    //root->addChild(testNode);
    //_viewer->getCameraManipulator()->
    _viewer->getCameraManipulator()->setHomePosition({0, 0, 100}, {0, 0, 0}, osg::Y_AXIS);
  }

}
