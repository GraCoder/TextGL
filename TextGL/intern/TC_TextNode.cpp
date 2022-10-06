#include "TC_TextNode.h"

#include <algorithm>
#include <osg/PrimitiveSet>
#include <osg/BlendFunc>
//#include <osgDB/WriteFile>

#include "TC_GlyText.h"
#include "TC_FontTexture.h"
#include "TC_TextShader.h"
#include "TC_FontMatrix.h"

#pragma comment(lib, "Opengl32.lib")

namespace TC_TEXT{

static std::map<std::shared_ptr<TC_FontTexture>, osg::ref_ptr<osg::Texture2D>> _texs;

TC_TextNode::TC_TextNode()
{
  setUseVertexArrayObject(true);
  initArraysAndBuffers();

  setCullingActive(false);

  auto ss = getOrCreateStateSet();
  ss->setAttributeAndModes(new osg::BlendFunc, 1);
  auto prg = new osg::Program;
  prg->addShader(new osg::Shader(osg::Shader::VERTEX, cms_verx));
  prg->addShader(new osg::Shader(osg::Shader::FRAGMENT, cms_frag));
  ss->setAttribute(prg);
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

void TC_TextNode::setText(std::shared_ptr<TC_GlyText>& glyText) {
  _gly_text = glyText;
  auto ss = getOrCreateStateSet();
  auto chars = _gly_text->get_chars();

  auto ele = static_cast<osg::DrawElementsUInt*>(_elems.get());
  _uvs->clear(); _vertexs->clear();
  ele->clear(); _char_idxs.clear();
  if (chars.chars.empty()) {
    dirtyGLObjects();
    return;
  }

  auto vs = _gly_text->vertexs(chars);
  _vertexs->resize(vs.size());
  memcpy(&_vertexs->at(0), vs.data(), vs.size() * sizeof(tg::vec3));

  auto uv = _gly_text->tex_coords(chars);
  _uvs->resize(uv.size());
  memcpy(&_uvs->at(0), uv.data(), uv.size() * sizeof(tg::vec2));

  auto idxs = _gly_text->indexs(chars);
  ele->reserve(chars.chars.size() * 6);
  for (auto& unit : idxs) {
    for (auto idx : unit.second) {
      uint32_t count = idx * 4;
      ele->push_back(count); ele->push_back(count + 1); ele->push_back(count + 2);
      ele->push_back(count); ele->push_back(count + 2); ele->push_back(count + 3);
    }
    auto tex = build_tex(unit.first->shared_from_this());
    _char_idxs.push_back(std::make_pair(tex, unit.second.size()));
  }
 
  auto min = _gly_text->min(), max = _gly_text->max();
  _initialBoundingBox._min.set(min.x(), min.y(), min.z());
  _initialBoundingBox._max.set(max.x(), max.y(), max.z());
  dirtyGLObjects();
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


  _elems = new osg::DrawElementsUInt(GL_TRIANGLES); 
  _elems->setBufferObject(_ebo);
}

void TC_TextNode::drawImplementationSinglePass(osg::State& state, const osg::Vec4& colorMultiplier) const
{
  if (colorMultiplier.a() == 0.0f /*|| _color.a() == 0.0f*/) return;

  osg::VertexArrayState* vas = state.getCurrentVertexArrayState();
  bool usingVertexBufferObjects = state.useVertexBufferObject(_supportsVertexBufferObjects && _useVertexBufferObjects);
  bool usingVertexArrayObjects = usingVertexBufferObjects && state.useVertexArrayObject(_useVertexArrayObject);
  bool requiresSetArrays = !usingVertexBufferObjects || !usingVertexArrayObjects || vas->getRequiresSetArrays();

  auto ele = static_cast<osg::DrawElementsUInt*>(_elems.get());
  auto ebo = ele->getOrCreateGLBufferObject(state.getContextID());
  if (ebo) {
    int count = 0;
    state.getCurrentVertexArrayState()->bindElementBufferObject(ebo);
    //for (auto &tex : _char_idxs) {
    //  tex.first->apply(state);
    //  //auto* ext = state.get<osg::GLExtensions>();
    //  glDrawElements(GL_TRIANGLES, tex.second.size() * 6, GL_UNSIGNED_INT, (const GLvoid *)count);
    //  count += tex.second.size() * 12;
    //}
    for(auto &tex : _char_idxs){
      tex.first->apply(state);
      glDrawElements(GL_TRIANGLES, tex.second * 6, GL_UNSIGNED_INT, (const GLvoid *)count);
      count += tex.second * 24;
    }
  }
}

void TC_TextNode::drawVertexArraysImplementation(osg::RenderInfo& renderInfo) const
{
  using namespace osg;
  State& state = *renderInfo.getState();
  VertexArrayState* vas = state.getCurrentVertexArrayState();
  AttributeDispatchers& attributeDispatchers = state.getAttributeDispatchers();

  attributeDispatchers.reset();
  attributeDispatchers.setUseVertexAttribAlias(state.getUseVertexAttributeAliasing());

  //if (state.useVertexArrayObject(_useVertexArrayObject)) {
  //  if (!vas->getRequiresSetArrays()) return;
  //}

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
  if (iter != _texs.end()) {
    if(tex->isdirty()){
      //tex->set_dirty(false);
      iter->second->dirtyTextureObject();
      //osgDB::writeImageFile(*iter->second->getImage(), "test.jpg");
    } 
    return iter->second;
  }
  auto ret = new osg::Texture2D;
  auto img = new osg::Image;
  auto sz = tex->get_size();
  img->setImage(sz.first, sz.second, 1, GL_RED, GL_RED, GL_UNSIGNED_BYTE, tex->data(), osg::Image::NO_DELETE);
  ret->setImage(img);
  _texs[tex] = ret;
  return ret;
}

}  // namespace TC_TEXT
