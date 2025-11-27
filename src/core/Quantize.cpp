#include "Quantize.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>

namespace {
static float quantize_channel(float val, int level) {
  float v = val / 255.0f;
  float scaled = v * (level - 1);
  int r = static_cast<int>(std::round(scaled));

  float out = r * (255.0f / (level - 1));
  return out;
}
} // namespace
/*
 * 1. Normalise pixel value
 * 2. Scale to level
 * 3. Round to nearest bucket
 * 4. Map bucket back to 255
 */
void quantize_naive(const ImageF &src, ImageF &dst, int levelsR, int levelsG,
                    int levelsB) {
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

      destination_pointer.r = quantize_channel(source_pointer.r, levelsR);
      destination_pointer.g = quantize_channel(source_pointer.g, levelsG);
      destination_pointer.b = quantize_channel(source_pointer.b, levelsB);

      if (ch == 4) {
        destination_pointer.a = source_pointer.a;
      }
    }
  }
}
