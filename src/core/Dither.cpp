#include "Dither.hpp"

#include <algorithm>
#include <assert.h>

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

inline Vec3f apply_ordered_dither_offset(const Vec3f &c, int levelsR,
                                         int levelsG, int levelsB, int x, int y,
                                         float ditherStrength) {
  float t = bayer_threshold_4x4(x, y);        // 0..1
  float offset = (t - 0.5f) * ditherStrength; // -0.5..0.5 scaled by strength

  float rangeR = 1.0f / float(levelsR);
  float rangeG = 1.0f / float(levelsG);
  float rangeB = 1.0f / float(levelsB);

  Vec3f out;
  out.r = std::clamp(c.r + offset * rangeR, 0.0f, 1.0f);
  out.g = std::clamp(c.g + offset * rangeG, 0.0f, 1.0f);
  out.b = std::clamp(c.b + offset * rangeB, 0.0f, 1.0f);

  return out;
}
} // namespace

void quantize_ordered_dither(const ImageF &src, ImageF &dst, QuantizeMode mode,
                             int levelsR, int levelsG, int levelsB,
                             float ditherStrength) {
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

      const Vec4f &s = src.at(x, y);
      Vec4f &d = dst.at(x, y);

      Vec3f rgb{s.r, s.g, s.b};

      Vec3f perturbed = apply_ordered_dither_offset(
          rgb, levelsR, levelsG, levelsB, x, y, ditherStrength);

      Vec3f q =
          mode == QuantizeMode::None
              ? perturbed
              : quantize_color(perturbed, mode, levelsR, levelsG, levelsB);

      d.r = q.r;
      d.g = q.g;
      d.b = q.b;

      if (ch == 4)
        d.a = s.a;
    }
  }
}
