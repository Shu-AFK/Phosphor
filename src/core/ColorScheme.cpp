#include "ColorScheme.hpp"

#include <omp.h>

void apply_color_scheme(const ImageF &src, ImageF &dst,
                        const ColorSchemeParams &params) {
  int w = src.width();
  int h = src.height();

  Vec3f target = params.useCustom ? params.customColor
                                  : COLOR_PRESETS[params.presetIndex].color;

#pragma omp parallel for
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      const Vec4f &rgba = src.at(x, y);
      float lum = 0.2126f * rgba.r + 0.7152f * rgba.g + 0.0722f * rgba.b;

      dst.at(x, y) = Vec4f{
          .r = target.r * lum,
          .g = target.g * lum,
          .b = target.b * lum,
          .a = rgba.a,
      };
    }
  }
}
