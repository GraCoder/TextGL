#include "TC_GlyText.h"
#include "TC_Library.h"
#include "TC_FontMatrix.h"

#include <freetype/freetype.h>

#include <boost/locale.hpp>
#include <boost/algorithm/string.hpp>

namespace TC_TEXT {

TC_GlyText::TC_GlyText() 
    : _font("simsum")
    , _ratio(1) 
    , _line_max(80)
    , _axis_align(AxisAlignment::AA_XY_PLANE)
    , _size_mode(CharacterSizeMode::CSM_OBJECT)
{}

TC_GlyText::~TC_GlyText() {}

void TC_GlyText::set_text(const std::string& text)
{
  _texts.clear();
  std::vector<std::string> strs;
  boost::split(strs, text, boost::is_any_of("\n"));
  for (auto& str : strs) {
    auto wstr = boost::locale::conv::utf_to_utf<wchar_t>(str);
    std::vector<uint32_t> tmp;
    for (int i = 0; i < wstr.size(); i++) {
      tmp.push_back(wstr[i]);
    }
    if(tmp.empty()) {
      _texts.push_back(tmp);
      continue;
    }
    int idx = 0, num = 0;
    while((num = tmp.size() - idx) > 0){
      std::vector<uint32_t> line;
      if (num > _line_max)
        num = _line_max;
      line.assign(tmp.begin() + idx, tmp.begin() + idx + num);
      idx += num;
      _texts.push_back(line);
    }
  }
}

void TC_GlyText::set_text(const std::vector<uint32_t>& text)
{
  _texts.clear();
  _texts.push_back(text);
}

void TC_GlyText::set_line_maxpix(int pix) {}

void TC_GlyText::set_line_maxnum(int num) 
{
  _line_max = num;
}

void TC_GlyText::set_axis_alignment(AxisAlignment aa) 
{
  _axis_align = aa;
}

void TC_GlyText::set_charactor_size_mode(CharacterSizeMode csm) 
{
  _size_mode = csm;
}

TC_GlyText::GlyChars TC_GlyText::get_chars()
{
  _ft_matrix = std::make_unique<TC_FontMatrix>(_font);

  GlyChars ret;
  for (int i = _texts.size() - 1; i >= 0; i--) {
    for (auto& ch : _texts[i]) {
      TC_GlyChar gly_char(ch);
      gly_char.build(_font);
      ret.chars.push_back(gly_char);
    }
    ret.nums.push_back(_texts[i].size());
  }
  return ret;
}

std::vector<tg::vec3> TC_GlyText::vertexs(GlyChars& chars)
{
  std::vector<tg::vec3> ret;
  float width = 0, lineh = 0, lines = 0;
  lineh = _ft_matrix->height();
  lines = _ft_matrix->descend();
  if (_size_mode == CharacterSizeMode::CSM_SCREEN) {
    lineh = std::round(lineh);
    lines = std::round(lines);
  }
  for (int i = 0, count = 0; i < chars.nums.size(); i++) {
    float wtmp = 0;
    float htmp = i * lineh - lines;
    for (int j = 0; j < chars.nums[i]; j++) {
      auto& ch = chars.chars[count + j];
      auto oft = ch.offset();
      auto x = wtmp + oft.first;
      float l = x * _ratio;
      float t = (oft.second + htmp) * _ratio;
      float f = (oft.second - ch.height() + htmp) * _ratio;
      float r = (x + ch.width()) * _ratio;
      ret.push_back(tg::vec3(l, t, 0));
      ret.push_back(tg::vec3(l, f, 0));
      ret.push_back(tg::vec3(r, f, 0));
      ret.push_back(tg::vec3(r, t, 0));
      wtmp += ch.andvance();
    }
    count += chars.nums[i];
    if (wtmp > width)
      width = wtmp;
  }
  _min = tg::vec3(0, 0, 0);
  _max = tg::vec3(width, lineh * chars.nums.size(), 0.1);
  return ret;
}

std::vector<tg::vec2> TC_GlyText::tex_coords(GlyChars& chars)
{
  std::vector<tg::vec2> ret;
  for (int i = 0; i < chars.chars.size(); i++) {
    auto& ch = chars.chars[i];
    auto lt = ch.tex_lt_coord();
    auto rb = ch.tex_rb_coord();
    ret.push_back(tg::vec2(lt.first, lt.second));
    ret.push_back(tg::vec2(lt.first, rb.second));
    ret.push_back(tg::vec2(rb.first, rb.second));
    ret.push_back(tg::vec2(rb.first, lt.second));
  }
  return ret;
}

std::map<TC_FontTexture*, std::vector<uint32_t>>
 TC_GlyText::indexs(GlyChars& chars)
{
  //typedef std::pair<std::shared_ptr<TC_FontTexture>, uint32_t> unit;
  //std::vector<unit> tmps;
  //for (int i = 0, idx = 0; i < chars.chars.size(); i++) {
  //    tmps.push_back(std::make_pair(chars.chars[i].get_texture(), idx++)); 
  //}
  //std::make_heap(tmps.begin(), tmps.end(), [](const unit& u1, const unit& u2) { return u1.first < u2.first; });
  std::map<TC_FontTexture*, std::vector<uint32_t>> ret;
  for(int i = 0; i < chars.chars.size(); i++){
    ret[chars.chars[i].get_texture().get()].push_back(i);
  }
  return ret;
}

}  // namespace TC_TEXT
