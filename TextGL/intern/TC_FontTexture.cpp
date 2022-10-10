#include "TC_FontTexture.h"

//#include "texture-atlas.h"
//#include "texture-font.h"

#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace TC_TEXT{

TC_FontTexture::TC_FontTexture(int w, int h) 
    : _width(w)
    , _height(h)
{
  _nodes.push_back(tg::vec4i{1, 1, w - 2, h - 2});
  _data = new unsigned char[w * h];
  memset(_data, 0, w * h);
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

  space_merge();
  return reg;
}

void TC_FontTexture::addchar(TC_GlyChar *glychar, const tg::vec2i &v)
{
  float of = 0;
  glychar->_ltu = float(v.x() + of) / _width;
  glychar->_ltv = float(v.y() + of) / _height;
  glychar->_rbu = float(v.x() + glychar->width() - of) / _width;
  glychar->_rbv = float(v.y() + glychar->height() - of) / _height;
  glychar->_texture = shared_from_this();
  _glys[glychar->code()] = *glychar;
}

const TC_GlyChar *TC_FontTexture::getchar(uint32_t code)
{
  auto iter = _glys.find(code);
  if (iter != _glys.end())
    return &iter->second;
  return nullptr;
}

void TC_FontTexture::filldata(const tg::vec4i &rect, std::vector<uint8_t> &data) 
{
  _dirty = true;

  for (int i = 0; i < rect.w(); i++) {
    auto dst = _data + (rect.y() + i) * _width + rect.x();
    memcpy(dst, data.data() + i * rect.z(), rect.z());
  }

  //stbi_write_png("test1.png", rect.z(), rect.w(), 1, data.data(), 0);
  //stbi_write_png("test.png", _width, _height, 1, _data, 0);
}

int TC_FontTexture::space_fit(int idx, int w, int h)
{
  auto &node = _nodes[idx];
  int x = node.x(), y = node.y();
  if (x + w > _width - 1)
    return -1;
  int wleft = w;
  while(wleft>0){
    auto &node = _nodes[idx];
    if (node.y() > y) y = node.y();

    if (y + h > _height - 1) return -1;

    wleft -= node.z();
    idx++;
  }
  return y;
}

void TC_FontTexture::space_merge() 
{
  int i = 1;
  while(i < _nodes.size()) {
    auto &node = _nodes[i - 1];
    auto &next = _nodes[i];
    if(node.y() == next.y()) {
      node.z() += next.z();
      _nodes.erase(_nodes.begin() + i);
      continue;
    }
    i++;
  }
}

TC_FontTexture::TC_FontTexture(TC_FontTexture &other)
{
  this->operator=(other);
}

TC_FontTexture::~TC_FontTexture() 
{
  delete []_data;
}

TC_FontTexture &TC_FontTexture::operator=(TC_FontTexture &other)
{
  _width = other._width;
  _height = other._height;
  _data = other._data;

  return *this;
}

}
