#include "Quantize.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>

// Uniform per-channel quantization in linear 0..1 space:
// 1. Clamp to [0,1]
// 2. Scale to [0, levels-1]
// 3. Round to nearest bucket
// 4. Map back to [0,1]
float quantize_channel_uniform(float val, int levels) {
  float v = std::clamp(val, 0.0f, 1.0f);
  float scaled = v * float(levels - 1);
  int r = static_cast<int>(std::round(scaled));
  float out = float(r) * (1.0f / float(levels - 1));
  return out;
}

Vec3f quantize_color(const Vec3f &src, QuantizeMode mode, int levelsR,
                     int levelsG, int levelsB) {
  switch (mode) {
  case QuantizeMode::UniformPerChannel:
  default:
    return Vec3f{
        quantize_channel_uniform(src.r, levelsR),
        quantize_channel_uniform(src.g, levelsG),
        quantize_channel_uniform(src.b, levelsB),
    };
  }
}

void quantize_naive(const ImageF &src, ImageF &dst, QuantizeMode mode,
                    int levelsR, int levelsG, int levelsB) {
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
      Vec3f q = quantize_color(rgb, mode, levelsR, levelsG, levelsB);

      d.r = q.r;
      d.g = q.g;
      d.b = q.b;

      if (ch == 4) {
        d.a = s.a;
      }
    }
  }
}
