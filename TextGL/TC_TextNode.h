#ifndef _TC_TEXT_NODE_INC_
#define _TC_TEXT_NODE_INC_

#include <osg/Drawable>

#include "TC_GlyText.h"

class TC_TextGL_EXPORT TC_TextNode : public osg::Drawable {
 public:
  TC_TextNode();

  void drawImplementation(osg::RenderInfo& renderInfo) const;

  bool supports(const osg::Drawable::AttributeFunctor&) const { return false;}

  bool supports(const osg::Drawable::ConstAttributeFunctor&) const { return true; }

  void accept(osg::Drawable::ConstAttributeFunctor& af) const;

  bool supports(const osg::PrimitiveFunctor&) const { return true; }

  void accept(osg::PrimitiveFunctor& pf) const;

  void setText(const TC_GlyText& glyText);

private:

  ~TC_TextNode();

  typedef osg::ref_ptr<osg::Vec3Array> Coords;      
  typedef osg::ref_ptr<osg::Vec2Array> TexCoords;
  typedef osg::ref_ptr<osg::Vec4Array> ColorCoords;

  Coords& getCoords() { return _coords; }
  const Coords& getCoords() const { return _coords; }


  void drawImplementationSinglePass(osg::State& state,
                                    const osg::Vec4& colorMultiplier) const;


  osg::ref_ptr<osg::VertexBufferObject> _vbo;
  osg::ref_ptr<osg::ElementBufferObject> _ebo;

  Coords _coords;
  Coords _normals;
  //ColorCoords _colorCoords;
  TexCoords _texcoords;

  TC_GlyText _text;

}; 


#endif