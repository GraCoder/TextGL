#include "TC_TextNode.h"

#include <algorithm>
#include <osg/PrimitiveSet>

#include "TC_GlyText.h"
#include "TC_FontTexture.h"

#pragma comment(lib, "Opengl32.lib")

namespace TC_TEXT{

static std::map<std::shared_ptr<TC_FontTexture>, osg::ref_ptr<osg::Texture2D>> _texs;

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
  vas->setVertexBufferObjectSupported(usingVertexBufferObjects);

  drawVertexArraysImplementation(renderInfo);

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
{
  auto& ft = text.font();
  auto sz = ft.font_size();

  auto ss = getOrCreateStateSet();
  auto chars = text.get_chars();
  auto ele = static_cast<osg::DrawElementsUShort*>(_elems.get());
  ele->clear();
  float width = 0, height = 0;
  for (int i = 0; i < chars.size(); i++) {
    auto& ch = chars[i];
    if (ch.height() > height) height = ch.height(); 
    width += ch.width();

    auto lt = ch.tex_lt_coord();
    auto rb = ch.tex_rb_coord();
    _uvs->push_back(osg::Vec2f(lt.first, lt.second));
    _uvs->push_back(osg::Vec2f(lt.first, rb.second));
    _uvs->push_back(osg::Vec2f(rb.first, rb.second));
    _uvs->push_back(osg::Vec2f(rb.first, lt.second));

    uint16_t idx = i * 4;
    ele->push_back(idx); ele->push_back(idx + 1); ele->push_back(idx + 2);
    ele->push_back(idx); ele->push_back(idx + 2); ele->push_back(idx + 3);

    auto tex = build_tex(chars[i].get_texture());
    ss->setTextureAttributeAndModes(0, tex.get(), 1);
  }

  std::sort(chars.begin(), chars.end(), [](TC_GlyChar& char1, TC_GlyChar& char2) -> bool { return char1.get_texture() < char2.get_texture(); });

  width = 0;
  height = 0;
  _vertexs->clear();
  for(int i = 0; i < chars.size(); i++){
    auto &ch = chars[i];
    auto oft = ch.offset();
    auto x = width + oft.first;
    _vertexs->push_back(osg::Vec3f(x, oft.second, 0));
    _vertexs->push_back(osg::Vec3f(x, oft.second - ch.height(), 0));
    _vertexs->push_back(osg::Vec3f(x + ch.width(), oft.second - ch.height(), 0));
    _vertexs->push_back(osg::Vec3f(x + ch.width(), oft.second, 0));
    width += ch.andvance();
  }

  dirtyGLObjects();
  fillVertexAttribute();

  _initialBoundingBox._min.set(0, 0, 0);
  _initialBoundingBox._max.set(sz * chars.size(), sz, 0.01);
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


  _elems = new osg::DrawElementsUShort(GL_TRIANGLES); 
  _elems->setBufferObject(_ebo);
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


  _elems->draw(state, true);
}

void TC_TextNode::drawVertexArraysImplementation(osg::RenderInfo& renderInfo) const
{
  using namespace osg;
  State& state = *renderInfo.getState();
  VertexArrayState* vas = state.getCurrentVertexArrayState();
  AttributeDispatchers& attributeDispatchers = state.getAttributeDispatchers();

  attributeDispatchers.reset();
  attributeDispatchers.setUseVertexAttribAlias(state.getUseVertexAttributeAliasing());

  if (state.useVertexArrayObject(_useVertexArrayObject)) {
    if (!vas->getRequiresSetArrays()) return;
  }

  vas->lazyDisablingOfVertexAttributes();
  if (_vertexs.valid()) vas->setVertexArray(state, _vertexs.get());
  if (_uvs.valid()) vas->setTexCoordArray(state, 0, _uvs.get());
  // if (_normalArray.valid() && _normalArray->getBinding() == osg::Array::BIND_PER_VERTEX) vas->setNormalArray(state, _normalArray.get());
  vas->applyDisablingOfVertexAttributes(state);
}

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

  _elems->dirty();
}

osg::ref_ptr<osg::Texture2D> TC_TextNode::build_tex(std::shared_ptr<TC_FontTexture> tex)
{
  auto iter = _texs.find(tex);
  if (iter != _texs.end())
    return iter->second;
  auto ret = new osg::Texture2D;
  auto img = new osg::Image;
  auto sz = tex->get_size();
  img->setImage(sz.first, sz.second, 1, GL_RED, GL_RED, GL_UNSIGNED_BYTE, tex->data(), osg::Image::NO_DELETE);
  ret->setImage(img);
  _texs[tex] = ret;
  return ret;
}

}  // namespace TC_TEXT
