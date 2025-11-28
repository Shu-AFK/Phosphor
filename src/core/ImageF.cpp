#include "ImageF.hpp"

#include <cassert>
#include <stdexcept>

ImageF::ImageF(int w, int h, int c) : _w(w), _h(h), _c(c) {
  if (w <= 0 || h <= 0 || (c != 3 && c != 4)) {
    throw std::invalid_argument(
        "ImageF dimensions must be positive and channels must be 3 or 4");
  }

  _pixels.resize(static_cast<std::size_t>(w) * h);
}

int ImageF::width() const { return _w; }
int ImageF::height() const { return _h; }
int ImageF::channels() const { return _c; }
std::size_t ImageF::size() const { return _pixels.size(); }

bool ImageF::valid() const {
  return _w > 0 && _h > 0 && (_c == 3 || _c == 4) &&
         _pixels.size() == static_cast<std::size_t>(_w * _h);
}

const Vec4f *ImageF::data() const { return _pixels.data(); }
Vec4f *ImageF::data() { return _pixels.data(); }

Vec4f &ImageF::at(int x, int y) {
  assert(x >= 0 && x < _w);
  assert(y >= 0 && y < _h);
  return _pixels[static_cast<std::size_t>(y) * _w + x];
}

const Vec4f &ImageF::at(int x, int y) const {
  assert(x >= 0 && x < _w);
  assert(y >= 0 && y < _h);
  return _pixels[static_cast<std::size_t>(y) * _w + x];
}

float &Vec3f::operator[](int i) {
  assert(i >= 0 && i < 3);
  switch (i) {
  case 0:
    return r;
  case 1:
    return g;
  default:
    return b;
  }
}

const float &Vec3f::operator[](int i) const {
  assert(i >= 0 && i < 3);
  switch (i) {
  case 0:
    return r;
  case 1:
    return g;
  default:
    return b;
  }
}

Vec3f Vec3f::operator*(float s) const { return Vec3f{r * s, g * s, b * s}; }

float &Vec4f::operator[](int i) {
  assert(i >= 0 && i < 4);
  switch (i) {
  case 0:
    return r;
  case 1:
    return g;
  case 2:
    return b;
  default:
    return a;
  }
}

const float &Vec4f::operator[](int i) const {
  assert(i >= 0 && i < 4);
  switch (i) {
  case 0:
    return r;
  case 1:
    return g;
  case 2:
    return b;
  default:
    return a;
  }
}

Vec4f Vec4f::operator*(float s) const { return Vec4f{r * s, g * s, b * s, a}; }
