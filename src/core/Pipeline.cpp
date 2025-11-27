#include "Pipeline.hpp"
#include "Dither.hpp"
#include "Filter.hpp"
#include "ImageF.hpp"
#include "Quantize.hpp"

#include <algorithm>
#include <cassert>

namespace {

ImageF to_float_image(const Image &src) {
  int w = src.width();
  int h = src.height();
  int c = src.channels();

  ImageF out(w, h, c);
  const uint8_t *source_pixels = src.data();

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      int idx = (y * w + x) * c;
      Vec4f &pixel = out.at(x, y);
      pixel.r = static_cast<float>(source_pixels[idx]);
      pixel.g = static_cast<float>(source_pixels[idx + 1]);
      pixel.b = static_cast<float>(source_pixels[idx + 2]);
      pixel.a = c == 4 ? static_cast<float>(source_pixels[idx + 3]) : 255.0f;
    }
  }

  return out;
}

Image to_uint8_image(const ImageF &src) {
  int w = src.width();
  int h = src.height();
  int c = src.channels();

  Image out(w, h, c);
  uint8_t *destination_pixels = out.data();

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      int idx = (y * w + x) * c;
      const Vec4f &pixel = src.at(x, y);
      destination_pixels[idx] =
          static_cast<uint8_t>(std::clamp(pixel.r, 0.0f, 255.0f));
      destination_pixels[idx + 1] =
          static_cast<uint8_t>(std::clamp(pixel.g, 0.0f, 255.0f));
      destination_pixels[idx + 2] =
          static_cast<uint8_t>(std::clamp(pixel.b, 0.0f, 255.0f));
      if (c == 4) {
        destination_pixels[idx + 3] =
            static_cast<uint8_t>(std::clamp(pixel.a, 0.0f, 255.0f));
      }
    }
  }

  return out;
}

} // namespace

void run_cpu_pipeline(const Image &src, Image &dst, const Params &params) {
  ImageF floatSrc = to_float_image(src);
  ImageF floatDst = floatSrc;

  switch (params.filterMode) {
  case FilterMode::None:
    break;
  case FilterMode::Grayscale:
  case FilterMode::RedChannel:
  case FilterMode::GreenChannel:
  case FilterMode::BlueChannel:
    apply_channel_filter(floatSrc, floatDst, params.filterMode,
                         params.channelIntensity);
    break;
  default:
    assert(false && "Invalid filter mode");
    break;
  }

  ImageF tmp = floatDst;

  if (params.quantizeMode == QuantizeMode::None) {
    /* no-op */
  } else if (params.ditherMode == DitherMode::None) {
    quantize_naive(tmp, floatDst, params.quantizeMode, params.levelsR,
                   params.levelsG, params.levelsB);
  } else {
    quantize_ordered_dither(tmp, floatDst, params.quantizeMode, params.levelsR,
                            params.levelsG, params.levelsB,
                            params.ditherStrength);
  }

  dst = to_uint8_image(floatDst);
}
