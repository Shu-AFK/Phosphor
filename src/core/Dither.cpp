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

uint8_t quantize_dither_channel(int val, int level, int x, int y,
                                float ditherStrength) {
  float v = val / 255.0f;

  float t = bayer_threshold_4x4(x, y); // 0..1
  float offset = t - 0.5f;             // -0.5..0.5

  float nv = v + offset * (ditherStrength / level);
  nv = std::clamp(nv, 0.0f, 1.0f);

  float scaled = nv * (level - 1);
  int r = static_cast<int>(std::round(scaled));

  float out = r * (255.0f / (level - 1));
  return static_cast<uint8_t>(
      std::clamp(static_cast<int>(std::round(out)), 0, 255));
}
} // namespace

void quantize_ordered_dither(const Image &src, Image &dst, int levelsR,
                             int levelsG, int levelsB, float ditherStrength) {
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

      destination_pointer[0] = quantize_dither_channel(
          source_pointer[0], levelsR, x, y, ditherStrength);
      destination_pointer[1] = quantize_dither_channel(
          source_pointer[1], levelsG, x, y, ditherStrength);
      destination_pointer[2] = quantize_dither_channel(
          source_pointer[2], levelsB, x, y, ditherStrength);

      if (ch == 4) {
        destination_pointer[3] = source_pointer[3];
      }
    }
  }
}
