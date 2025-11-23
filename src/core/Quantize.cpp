#include "Quantize.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>

namespace {
static uint8_t quantize_channel(int val, int level) {
  float v = val / 255.0f;
  float scaled = v * (level - 1);
  int r = static_cast<int>(std::round(scaled));

  int out = r * (255.0f / (level - 1));
  return static_cast<uint8_t>(std::round(out));
}
} // namespace
/*
 * 1. Normalise pixel value
 * 2. Scale to level
 * 3. Round to nearest bucket
 * 4. Map bucket back to 255
 */
void quantize_naive(const Image &src, Image &dst, int levelsR, int levelsG,
                    int levelsB) {
  assert(src.channels() == 3 || src.channels() == 4);

  levelsR = std::clamp(levelsR, 2, 32);
  levelsG = std::clamp(levelsG, 2, 32);
  levelsB = std::clamp(levelsB, 2, 32);

  int w = src.width();
  int h = src.height();
  int ch = src.channels();

  dst = Image(w, h, ch);
  const uint8_t *source_pixels = src.data();
  uint8_t *destination_pixels = dst.data();

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const uint8_t *source_pointer = source_pixels + (y * w + x) * ch;
      uint8_t *destination_pointer = destination_pixels + (y * w + x) * ch;

      destination_pointer[0] = quantize_channel(source_pointer[0], levelsR);
      destination_pointer[1] = quantize_channel(source_pointer[1], levelsG);
      destination_pointer[2] = quantize_channel(source_pointer[2], levelsB);

      if (ch == 4) {
        destination_pointer[3] = source_pointer[3];
      }
    }
  }
}
