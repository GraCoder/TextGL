#ifndef _TC_TEXT_NODE_INC_
#define _TC_TEXT_NODE_INC_

#include <osg/Drawable>
#include <osg/Texture2D>

#include "TC_Font.h"

namespace TC_TEXT{

class TC_GlyChar;
class TC_GlyText;
class TC_FontMatrix;
class TC_FontTexture;

class TC_TextGL_EXPORT TC_TextNode : public osg::Drawable {
  typedef osg::Drawable Base;
 public:
  TC_TextNode();

  void drawImplementation(osg::RenderInfo& renderInfo) const;

  bool supports(const osg::Drawable::AttributeFunctor&) const { return false; }

  bool supports(const osg::Drawable::ConstAttributeFunctor&) const { return true; }

  void accept(osg::Drawable::ConstAttributeFunctor& af) const;

  bool supports(const osg::PrimitiveFunctor&) const { return true; }

  void accept(osg::PrimitiveFunctor& pf) const;

  void setText(std::shared_ptr<TC_GlyText>& glyText);

private:
  ~TC_TextNode();

  typedef osg::ref_ptr<osg::Vec3Array> V3Array;
  typedef osg::ref_ptr<osg::Vec2Array> V2Array;

  V3Array& getCoords() { return _vertexs; }
  const V3Array& getCoords() const { return _vertexs; }

  void set_style();

  void initArraysAndBuffers();

  void drawImplementationSinglePass(osg::State& state, const osg::Vec4& colorMultiplier) const;

  void drawVertexArraysImplementation(osg::RenderInfo& renderInfo) const;

  osg::VertexArrayState* createVertexArrayStateImplementation(osg::RenderInfo& renderInfo) const override;

  void compileGLObjects(osg::RenderInfo& renderInfo) const override;

  void resizeGLObjectBuffers(unsigned int maxSize) override;

  void releaseGLObjects(osg::State* state = 0) const override;

  void dirtyGLObjects() override;

  bool computeMatrix(osg::Matrix& matrix, osg::State* state = 0) const;

  static osg::ref_ptr<osg::Texture2D> build_tex(std::shared_ptr<TC_FontTexture> tex);

private:

  osg::ref_ptr<osg::VertexBufferObject> _vbo;
  osg::ref_ptr<osg::ElementBufferObject> _ebo;

  V3Array _vertexs;
  V3Array _normals;
  // ColorCoords _colorCoords;
  V2Array _uvs;

  osg::ref_ptr<osg::DrawElements> _elems;

  std::shared_ptr<TC_GlyText>	_gly_text;

  mutable osg::ref_ptr<osg::RefMatrix> _prj_matrix;

  std::vector<std::pair<osg::ref_ptr<osg::Texture2D>, uint32_t>> _char_idxs;
};

}

#endif