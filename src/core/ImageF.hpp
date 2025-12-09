#pragma once

#include <cassert>
#include <cstddef>
#include <vector>

struct Vec3f {
  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;

  float &operator[](int i);
  const float &operator[](int i) const;

  Vec3f operator*(float s) const;
  Vec3f operator+(const Vec3f &o) const;
  Vec3f &operator+=(const Vec3f &o);
};

struct Vec4f {
  float r = 0.0f;
  float g = 0.0f;
  float b = 0.0f;
  float a = 0.0f;

  float &operator[](int i);
  const float &operator[](int i) const;

  Vec4f operator*(float s) const;
  Vec4f operator+(const Vec4f &o) const;
  Vec4f &operator+=(const Vec4f &o);
};

class ImageF {
public:
  ImageF() = default;

  ImageF(int w, int h, int c);

  int width() const;
  int height() const;
  int channels() const;
  std::size_t size() const;

  bool valid() const;

  const Vec4f *data() const;
  Vec4f *data();

  Vec4f &at(int x, int y);
  const Vec4f &at(int x, int y) const;

private:
  int _w = 0;
  int _h = 0;
  int _c = 0;
  std::vector<Vec4f> _pixels;
};
