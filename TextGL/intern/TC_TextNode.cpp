#include "TC_TextNode.h"
#include "TC_GlyText.h"

#pragma comment(lib, "Opengl32.lib")

namespace TC_TEXT{

TC_TextNode::TC_TextNode()
{
  //setUseVertexArrayObject(false);
  initArraysAndBuffers();

  setCullingActive(false);
}

TC_TextNode::~TC_TextNode() {}

void TC_TextNode::drawImplementation(osg::RenderInfo& renderInfo) const
{
  auto& state = *renderInfo.getState();
  osg::Matrix previous_modelview = state.getModelViewMatrix();

  osg::Matrix modelview;
  bool needToApplyMatrix = false;
  // needToApplyMatrix = computeMatrix(modelview, &state);

  if (needToApplyMatrix) {
    // ** mult previous by the modelview for this context
    modelview.postMult(previous_modelview);

    // ** apply this new modelview matrix
    state.applyModelViewMatrix(modelview);

    // workaround for GL3/GL2
    if (state.getUseModelViewAndProjectionUniforms()) state.applyModelViewAndProjectionUniformsIfRequired();

    // OSG_NOTICE<<"New state.applyModelViewMatrix() "<<modelview<<std::endl;
  } else {
    // OSG_NOTICE<<"No need to apply matrix "<<std::endl;
  }

  // state.Normal(_normal.x(), _normal.y(), _normal.z());

  osg::VertexArrayState* vas = state.getCurrentVertexArrayState();
  bool usingVertexBufferObjects = state.useVertexBufferObject(_supportsVertexBufferObjects && _useVertexBufferObjects);
  bool usingVertexArrayObjects = usingVertexBufferObjects && state.useVertexArrayObject(_useVertexArrayObject);
  bool requiresSetArrays = !usingVertexBufferObjects || !usingVertexArrayObjects || vas->getRequiresSetArrays();
  //vas->setVertexBufferObjectSupported(usingVertexBufferObjects);

  //vas->lazyDisablingOfVertexAttributes();
  vas->setVertexArray(state, _vertexs.get());
  vas->setTexCoordArray(state, 0, _uvs.get());
  vas->applyDisablingOfVertexAttributes(state);

  glDepthMask(GL_FALSE);

  osg::Vec4f colorMultiplier(1., 1., 1., 1.);
  drawImplementationSinglePass(state, colorMultiplier);

  // if (_enableDepthWrites) {
  //   glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  //   glDepthMask(GL_TRUE);

  //  drawImplementationSinglePass(state, colorMultiplier);

  //  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  //  state.haveAppliedAttribute(osg::StateAttribute::COLORMASK);
  //}

  state.haveAppliedAttribute(osg::StateAttribute::DEPTH);

  if (usingVertexBufferObjects && !usingVertexArrayObjects) {
    // unbind the VBO's if any are used.
    vas->unbindVertexBufferObject();
    vas->unbindElementBufferObject();
  }

  if (needToApplyMatrix) {
    // apply this new modelview matrix
    state.applyModelViewMatrix(previous_modelview);

    // workaround for GL3/GL2
    if (state.getUseModelViewAndProjectionUniforms()) state.applyModelViewAndProjectionUniformsIfRequired();
  }
}

void TC_TextNode::accept(osg::Drawable::ConstAttributeFunctor& af) const {}

void TC_TextNode::accept(osg::PrimitiveFunctor& pf) const {}

void TC_TextNode::setText(TC_GlyText& text) 
{ auto &ft = text.font();
  auto sz = ft.font_size();
  
  auto chars = text.get_chars();
  for(int i = 0; i < chars.size(); i++){
    _vertexs->push_back(osg::Vec3f(sz * i, sz, 0));
    _vertexs->push_back(osg::Vec3f(sz * i, 0, 0));
    _vertexs->push_back(osg::Vec3f(sz * (i + 1), 0, 0));
    _vertexs->push_back(osg::Vec3f(sz * (i + 1), sz, 0));

    _uvs->push_back(osg::Vec2f(0, 0));
    _uvs->push_back(osg::Vec2f(0, 0));
    _uvs->push_back(osg::Vec2f(0, 0));
    _uvs->push_back(osg::Vec2f(0, 0));
  }

  dirtyGLObjects();
  fillVertexAttribute();
}

void TC_TextNode::initArraysAndBuffers() 
{
  _vbo = new osg::VertexBufferObject;
  _ebo = new osg::ElementBufferObject;

  _vertexs = new osg::Vec3Array(osg::Array::BIND_PER_VERTEX);
  _normals = new osg::Vec3Array(osg::Array::BIND_PER_VERTEX);
  _uvs = new osg::Vec2Array(osg::Array::BIND_PER_VERTEX);

  _vertexs->setBufferObject(_vbo.get());
  //_normals->setBufferObject(_vbo.get());
  _uvs->setBufferObject(_vbo.get());
}

void TC_TextNode::fillVertexAttribute() {

}

void TC_TextNode::drawImplementationSinglePass(osg::State& state, const osg::Vec4& colorMultiplier) const
{
  if (colorMultiplier.a() == 0.0f /*|| _color.a() == 0.0f*/) return;

  osg::VertexArrayState* vas = state.getCurrentVertexArrayState();
  bool usingVertexBufferObjects = state.useVertexBufferObject(_supportsVertexBufferObjects && _useVertexBufferObjects);
  bool usingVertexArrayObjects = usingVertexBufferObjects && state.useVertexArrayObject(_useVertexArrayObject);
  bool requiresSetArrays = !usingVertexBufferObjects || !usingVertexArrayObjects || vas->getRequiresSetArrays();

}

void TC_TextNode::drawVertexArraysImplementation(osg::RenderInfo& renderInfo) const {}

osg::VertexArrayState* TC_TextNode::createVertexArrayStateImplementation(osg::RenderInfo& renderInfo) const
{
  auto& state = *renderInfo.getState();

  auto* vas = new osg::VertexArrayState(&state);

  // OSG_NOTICE<<"Creating new osg::VertexArrayState "<< vas<<std::endl;

  if (_vertexs.valid())
    vas->assignVertexArrayDispatcher();
  if (_normals.valid())
    vas->assignNormalArrayDispatcher();

  if (_uvs.valid())
    vas->assignTexCoordArrayDispatcher(1);

  if (state.useVertexArrayObject(_useVertexArrayObject)) {
    OSG_INFO << "TextBase::createVertexArrayState() Setup VertexArrayState to use VAO " << vas << std::endl;

    vas->generateVertexArrayObject();
  } else {
    OSG_INFO << "TextBase::createVertexArrayState() Setup VertexArrayState to without using VAO " << vas << std::endl;
  }

  return vas;
}

void TC_TextNode::compileGLObjects(osg::RenderInfo& renderInfo) const
{
  using namespace osg;
  State& state = *renderInfo.getState();
  if (renderInfo.getState()->useVertexBufferObject(_supportsVertexBufferObjects && _useVertexBufferObjects)) {
    unsigned int contextID = state.getContextID();
    GLExtensions* extensions = state.get<GLExtensions>();
    if (state.useVertexArrayObject(_useVertexArrayObject)) {
      VertexArrayState* vas = 0;

      _vertexArrayStateList[contextID] = vas = createVertexArrayState(renderInfo);

      State::SetCurrentVertexArrayStateProxy setVASProxy(state, vas);

      state.bindVertexArrayObject(vas);

      drawImplementation(renderInfo);

      state.unbindVertexArrayObject();
    } else {
      drawImplementation(renderInfo);
    }

    // unbind the BufferObjects
    extensions->glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
    extensions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
  }
}

void TC_TextNode::resizeGLObjectBuffers(unsigned int maxSize) {
  if (_vertexs.valid())
    _vertexs->resizeGLObjectBuffers(maxSize);
  if (_uvs.valid())
    _uvs->resizeGLObjectBuffers(maxSize);
}

void TC_TextNode::releaseGLObjects(osg::State* state) const
{
  if (_vertexs.valid())
    _vertexs->releaseGLObjects(state);
  if (_uvs.valid())
    _uvs->releaseGLObjects(state);
}

void TC_TextNode::dirtyGLObjects()
{
  Base::dirtyGLObjects();

  _vertexs->dirty();
  _uvs->dirty();
}

}  // namespace TC_TEXT
