#include "TC_FontTexture.h"

#include "texture-atlas.h"
#include "texture-font.h"

namespace TC_TEXT{

TC_FontTexture::TC_FontTexture(int w, int h) 
    : _width(w)
    , _height(h)
{
  _nodes.push_back(tg::vec4i{1, 1, w - 2, h - 2});
}

tg::vec2i TC_FontTexture::getspace(int w, int h)
{
  tg::vec2i reg(-1, -1);
  int idx = -1;
  int best_height = INT_MAX, best_width = INT_MAX;
  for(int i = 0; i < _nodes.size(); i++){
    int y = space_fit(i, w, h);
    if(y >= 0){
      auto &space = _nodes[i];
      if(y + h < best_height || (y + h == best_height && space.z() > 0 && space.z() < best_width)) {
        idx = i;
        best_width = space.z();
        best_height = y + h;
        reg.x() = space.x();
        reg.y() = y;
      }
    }
  }
  if(idx == -1){
    return reg;
  }
  
  tg::vec4i node(reg.x(), reg.y() + h, w, 0);
  _nodes.insert(_nodes.begin() + idx, node);

  idx++;
  while(idx < _nodes.size()){
    auto &node = _nodes[idx];
    auto &prev = _nodes[idx - 1];
    if (node.x() < prev.x() + prev.z()) {
      int shrink = prev.x() + prev.z() - node.x();
      node.x() += shrink;
      node.z() -= shrink;
      if (node.z() <= 0) {
        _nodes.erase(_nodes.begin() + idx);
        continue;
      } else
        break;
    } else
      break;
    idx++;
  }

  return reg;
}

const TC_GlyChar *TC_FontTexture::getchar(uint32_t code)
{
  auto iter = _glys.find(code);
  if (iter != _glys.end())
    return &iter->second;
  return nullptr;
}

int TC_FontTexture::space_fit(int idx, int w, int h)
{
  auto &node = _nodes[idx];
}

TC_FontTexture::TC_FontTexture(TC_FontTexture &other)
{
  this->operator=(other);
}

TC_FontTexture::~TC_FontTexture() {}

TC_FontTexture &TC_FontTexture::operator=(TC_FontTexture &other)
{
  _width = other._width;
  _height = other._height;
  _data = other._data;

  return *this;
}

}
