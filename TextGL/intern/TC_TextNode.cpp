#include "TC_TextNode.h"
#include "TC_GlyText.h"

#pragma comment(lib, "Opengl32.lib")

namespace TC_TEXT{

TC_TextNode::TC_TextNode() { setUseVertexArrayObject(false); }

TC_TextNode::~TC_TextNode() {}

void TC_TextNode::drawImplementation(osg::RenderInfo& renderInfo) const
{
  osg::Vec4f colorMultiplier(1., 1., 1., 1.);

  auto& state = *renderInfo.getState();
  osg::Matrix previous_modelview = state.getModelViewMatrix();

  // set up the new modelview matrix
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

  if (requiresSetArrays) {
    vas->lazyDisablingOfVertexAttributes();
    vas->setVertexArray(state, _coords.get());
    vas->setTexCoordArray(state, 0, _texcoords.get());
    vas->applyDisablingOfVertexAttributes(state);
  }

  glDepthMask(GL_FALSE);

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
  
  if (_coords == nullptr) _coords = new osg::Vec3Array;

  _coords->clear();
  _coords->push_back(osg::Vec3d(0, 0, 0));
  _coords->push_back(osg::Vec3d(0, sz, 0));
  auto chars = text.get_chars();
  for(int i = 0; i < chars.size(); i++){
    _coords->push_back(osg::Vec3d(sz * (i + 1), 0, 0));
    _coords->push_back(osg::Vec3d(sz * (i + 1), sz, 0));
  }
}

void TC_TextNode::drawImplementationSinglePass(osg::State& state, const osg::Vec4& colorMultiplier) const
{
  if (colorMultiplier.a() == 0.0f /*|| _color.a() == 0.0f*/) return;

  osg::VertexArrayState* vas = state.getCurrentVertexArrayState();
  bool usingVertexBufferObjects = state.useVertexBufferObject(_supportsVertexBufferObjects && _useVertexBufferObjects);
  bool usingVertexArrayObjects = usingVertexBufferObjects && state.useVertexArrayObject(_useVertexArrayObject);
  bool requiresSetArrays = !usingVertexBufferObjects || !usingVertexArrayObjects || vas->getRequiresSetArrays();

  //  if ((_drawMode & (~TEXT)) != 0 && !_decorationPrimitives.empty()) {
  //#if defined(OSG_GL_FIXED_FUNCTION_AVAILABLE)
  //    state.applyTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::OFF);
  //#endif
  //    vas->disableColorArray(state);
  //    for (Primitives::const_iterator itr = _decorationPrimitives.begin();
  //         itr != _decorationPrimitives.end(); ++itr) {
  //      if ((*itr)->getMode() == GL_TRIANGLES)
  //        state.Color(colorMultiplier.r() * _textBBColor.r(),
  //                    colorMultiplier.g() * _textBBColor.g(),
  //                    colorMultiplier.b() * _textBBColor.b(),
  //                    colorMultiplier.a() * _textBBColor.a());
  //      else
  //        state.Color(colorMultiplier.r(), colorMultiplier.g(),
  //                    colorMultiplier.b(), colorMultiplier.a());
  //
  //      (*itr)->draw(state, usingVertexBufferObjects);
  //    }
  //#if defined(OSG_GL_FIXED_FUNCTION_AVAILABLE)
  //    state.applyTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::ON);
  //#endif
  //  }
  //
  //  if (_drawMode & TEXT) {
  //    for (TextureGlyphQuadMap::const_iterator titr =
  //             _textureGlyphQuadMap.begin();
  //         titr != _textureGlyphQuadMap.end(); ++titr) {
  //      // need to set the texture here...
  //      state.applyTextureAttribute(0, titr->first.get());
  //
  //      const GlyphQuads& glyphquad = titr->second;
  //
  //      if (_colorGradientMode == SOLID) {
  //        vas->disableColorArray(state);
  //        state.Color(
  //            colorMultiplier.r() * _color.r(), colorMultiplier.g() * _color.g(),
  //            colorMultiplier.b() * _color.b(), colorMultiplier.a() * _color.a());
  //      } else {
  //        if (requiresSetArrays) {
  //          vas->setColorArray(state, _colorCoords.get());
  //        }
  //      }
  //
  //      glyphquad._primitives->draw(state, usingVertexBufferObjects);
  //    }
  //  }
}

}
