#include "Filter.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>

namespace {

inline void apply_filter_mode_pixel(Vec4f &dst, const Vec4f &src, int c,
                                    FilterMode mode, float intensity) {
  dst = src;
  switch (mode) {
  case FilterMode::None:
    break;

  case FilterMode::Grayscale: {
    // luminance weights
    float lum = src.r * 0.299f + src.g * 0.587f + src.b * 0.114f;
    dst.r = dst.g = dst.b = lum;
    break;
  }

  case FilterMode::RedChannel: {
    float v = src.r * intensity;
    dst.r = v;
    dst.g = 0.0f;
    dst.b = 0.0f;
    break;
  }

  case FilterMode::GreenChannel: {
    float v = src.g * intensity;
    dst.r = 0.0f;
    dst.g = v;
    dst.b = 0.0f;
    break;
  }

  case FilterMode::BlueChannel: {
    float v = src.b * intensity;
    dst.r = 0.0f;
    dst.g = 0.0f;
    dst.b = v;
    break;
  }
  }
}

} // namespace

void apply_channel_filter(const ImageF &src, ImageF &dst, FilterMode mode,
                          float intensity) {
  if (mode == FilterMode::None) {
    return;
  }

  int w = src.width();
  int h = src.height();
  int c = src.channels();

  assert(dst.width() == w);
  assert(dst.height() == h);
  assert(dst.channels() == c);
  assert(c == 3 || c == 4);

  intensity = std::clamp(intensity, 0.1f, 1.0f);

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const Vec4f &source_pointer = src.at(x, y);
      Vec4f &destination_pointer = dst.at(x, y);

      apply_filter_mode_pixel(destination_pointer, source_pointer, c, mode,
                              intensity);
    }
  }
}
