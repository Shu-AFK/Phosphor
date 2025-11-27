#include "Dither.hpp"

#include <algorithm>
#include <assert.h>
#include <cmath>

namespace {
static const int BAYER4[4][4] = {
    {0, 8, 2, 10},
    {12, 4, 14, 6},
    {3, 11, 1, 9},
    {15, 7, 13, 5},
};

inline float bayer_threshold_4x4(int x, int y) {
  // x & 3 == x % 4, but faster since 4 is a power of two
  int tx = x & 3;
  int ty = y & 3;

  return (BAYER4[ty][tx] + 0.5f) / 16.0f;
}

float quantize_dither_channel(float val, int level, int x, int y,
                              float ditherStrength) {
  float v = val / 255.0f;

  float t = bayer_threshold_4x4(x, y); // 0..1
  float offset = t - 0.5f;             // -0.5..0.5

  float nv = v + offset * (ditherStrength / level);
  nv = std::clamp(nv, 0.0f, 1.0f);

  float scaled = nv * (level - 1);
  int r = static_cast<int>(std::round(scaled));

  float out = r * (255.0f / (level - 1));
  return std::clamp(out, 0.0f, 255.0f);
}
} // namespace

void quantize_ordered_dither(const ImageF &src, ImageF &dst, int levelsR,
                             int levelsG, int levelsB, float ditherStrength) {
  assert(src.channels() == 3 || src.channels() == 4);

  levelsR = std::clamp(levelsR, 2, 32);
  levelsG = std::clamp(levelsG, 2, 32);
  levelsB = std::clamp(levelsB, 2, 32);

  int w = src.width();
  int h = src.height();
  int ch = src.channels();

  dst = ImageF(w, h, ch);

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const Vec4f &source_pointer = src.at(x, y);
      Vec4f &destination_pointer = dst.at(x, y);

      destination_pointer.r = quantize_dither_channel(
          source_pointer.r, levelsR, x, y, ditherStrength);
      destination_pointer.g = quantize_dither_channel(
          source_pointer.g, levelsG, x, y, ditherStrength);
      destination_pointer.b = quantize_dither_channel(
          source_pointer.b, levelsB, x, y, ditherStrength);

      if (ch == 4) {
        destination_pointer.a = source_pointer.a;
      }
    }
  }
}
