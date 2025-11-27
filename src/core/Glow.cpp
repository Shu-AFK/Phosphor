#include "Glow.hpp"
#include "core/ImageF.hpp"

namespace {

void bright_pass_filter(const ImageF &src, ImageF &dst, float threshold) {
  int w = src.width();
  int h = src.height();
  int c = src.channels();

  dst = ImageF(w, h, c);

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const Vec4f &rgba = src.at(x, y);

      float lum = 0.2126f * rgba.r + 0.7152f * rgba.g + 0.0722f * rgba.b;
      lum /= 255.0f;

      if (lum > threshold) {
        // for smoothness
        float scale = (lum - threshold) / (1.0f - threshold);
        dst.at(x, y) = rgba * scale;
      } else {
        dst.at(x, y) = Vec4f{0, 0, 0, 0};
      }
    }
  }
}

} // namespace

void apply_glow(const ImageF &src, ImageF &dst, const GlowParams &params) {
  bright_pass_filter(src, dst, params.threshold);
}
