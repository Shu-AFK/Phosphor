#include "Scanlines.hpp"

void apply_scanlines(const ImageF &src, ImageF &dst,
                     const ScanlineParams &params) {
  int w = src.width();
  int h = src.height();

  for (int y = 0; y < h; ++y) {
    int band = y / params.thickness;
    int bandInCycle = band % (1 + params.spacing);
    bool isLit = bandInCycle == 0;

    for (int x = 0; x < w; ++x) {
      Vec4f rgba = src.at(x, y);

      if (isLit) {
        dst.at(x, y) = rgba;
      } else {
        dst.at(x, y) = Vec4f{.r = rgba.r * params.gapDarkness,
                             .g = rgba.g * params.gapDarkness,
                             .b = rgba.b * params.gapDarkness,
                             .a = rgba.a};
      }
    }
  }
}
