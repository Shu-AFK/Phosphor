#include "Glow.hpp"
#include "core/ImageF.hpp"
#include <cmath>

#define GAMMA_IN 2.2

namespace {

void exposure_scaling(const ImageF &src, ImageF &dst, float exposure) {
  int w = src.width();
  int h = src.height();
  int c = src.channels();

  dst = ImageF(w, h, c);

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {

      auto rgba = src.at(x, y);

      for (int ch = 0; ch < c; ++ch) {
        float v = rgba[ch] / 255.0f;
        float lin = std::pow(v, 1.0f / GAMMA_IN);
        lin *= exposure;
        rgba[ch] = lin * 255.0f;
      }

      dst.at(x, y) = rgba;
    }
  }
}

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
  exposure_scaling(src, dst, params.exposure);

  ImageF tmp = dst;
  bright_pass_filter(tmp, dst, params.threshold);
}
