#include "Pipeline.hpp"
#include "ColorScheme.hpp"
#include "Dither.hpp"
#include "Glow.hpp"
#include "ImageF.hpp"
#include "Quantize.hpp"
#include "Scanlines.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <omp.h>

namespace {

// Convert 8-bit sRGB input into 0..1 linear floating point.
ImageF to_float_image(const Image &src, float gammaIn) {
  int w = src.width();
  int h = src.height();
  int c = src.channels();

  ImageF out(w, h, c);
  const uint8_t *source_pixels = src.data();

#pragma omp parallel for
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      int idx = (y * w + x) * c;
      Vec4f &pixel = out.at(x, y);
      float sr = static_cast<float>(source_pixels[idx]) / 255.0f;
      float sg = static_cast<float>(source_pixels[idx + 1]) / 255.0f;
      float sb = static_cast<float>(source_pixels[idx + 2]) / 255.0f;

      pixel.r = std::pow(sr, gammaIn);
      pixel.g = std::pow(sg, gammaIn);
      pixel.b = std::pow(sb, gammaIn);
      pixel.a =
          c == 4 ? static_cast<float>(source_pixels[idx + 3]) / 255.0f : 1.0f;
    }
  }

  return out;
}

// Convert 0..1 linear floating point into 8-bit sRGB output.
Image to_uint8_image(const ImageF &src, float gammaOut) {
  int w = src.width();
  int h = src.height();
  int c = src.channels();

  Image out(w, h, c);
  uint8_t *destination_pixels = out.data();

#pragma omp parallel for
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      int idx = (y * w + x) * c;
      const Vec4f &pixel = src.at(x, y);
      float lr = std::clamp(pixel.r, 0.0f, 1.0f);
      float lg = std::clamp(pixel.g, 0.0f, 1.0f);
      float lb = std::clamp(pixel.b, 0.0f, 1.0f);

      float sr = std::pow(lr, 1.0f / gammaOut);
      float sg = std::pow(lg, 1.0f / gammaOut);
      float sb = std::pow(lb, 1.0f / gammaOut);

      destination_pixels[idx] =
          static_cast<uint8_t>(std::round(std::clamp(sr, 0.0f, 1.0f) * 255.0f));
      destination_pixels[idx + 1] =
          static_cast<uint8_t>(std::round(std::clamp(sg, 0.0f, 1.0f) * 255.0f));
      destination_pixels[idx + 2] =
          static_cast<uint8_t>(std::round(std::clamp(sb, 0.0f, 1.0f) * 255.0f));
      if (c == 4) {
        destination_pixels[idx + 3] = static_cast<uint8_t>(
            std::round(std::clamp(pixel.a, 0.0f, 1.0f) * 255.0f));
      }
    }
  }

  return out;
}

} // namespace

void run_cpu_pipeline(const Image &src, Image &dst, const Params &params) {
  ImageF work = to_float_image(src, params.gammaIn);
  ImageF scratch(work.width(), work.height(), work.channels());

  ImageF *read = &work;
  ImageF *write = &scratch;

  auto swap_buffers = [&]() { std::swap(read, write); };

  if (params.colorScheme.enabled) {
    apply_color_scheme(*read, *write, params.colorScheme);
    swap_buffers();
  }

  if (params.quantize.enabled) {
    if (params.dither.enabled) {
      quantize_ordered_dither(*read, *write, params.quantize.mode,
                              params.quantize.levelsR, params.quantize.levelsG,
                              params.quantize.levelsB, params.dither.strength);
    } else {
      quantize_naive(*read, *write, params.quantize.mode,
                     params.quantize.levelsR, params.quantize.levelsG,
                     params.quantize.levelsB);
    }
    swap_buffers();
  }

  if (params.scanlines.enabled) {
    apply_scanlines(*read, *write, params.scanlines);
    swap_buffers();
  }

  if (params.glow.enabled) {
    apply_glow(*read, *write, params.glow);
    swap_buffers();
  }

  dst = to_uint8_image(*read, params.gammaOut);
}
