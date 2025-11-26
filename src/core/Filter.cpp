#include "Filter.hpp"

#include <cassert>
#include <cmath>
#include <cstdint>

namespace {

inline void apply_filter_mode_pixel(uint8_t *dst, const uint8_t *src, int c,
                                    FilterMode mode) {

  switch (mode) {
  case FilterMode::None:
    break;

  case FilterMode::Grayscale: {
    // luminance weights
    float lum = src[0] * 0.299f + src[1] * 0.587f + src[2] * 0.114f;
    uint8_t g = static_cast<uint8_t>(std::round(lum));
    dst[0] = dst[1] = dst[2] = g;
    break;
  }

  case FilterMode::RedChannel:
    dst[0] = src[0];
    dst[1] = 0;
    dst[2] = 0;
    break;

  case FilterMode::GreenChannel:
    dst[0] = 0;
    dst[1] = src[1];
    dst[2] = 0;
    break;

  case FilterMode::BlueChannel:
    dst[0] = 0;
    dst[1] = 0;
    dst[2] = src[2];
    break;
  }

  if (c == 4)
    dst[3] = src[3];
}

} // namespace

void apply_channel_filter(const Image &src, Image &dst, FilterMode mode) {
  if (mode == FilterMode::None) {
    return;
  }

  int w = src.width();
  int h = src.height();
  int c = src.channels();

  assert(dst.width() == w);
  assert(dst.height() == h);
  assert(dst.channels() == c);
  assert(c == 3 || c == 4);

  const uint8_t *src_data = src.data();
  uint8_t *dst_data = dst.data();

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      int idx = (y * w + x) * c;

      const uint8_t *source_pointer = src_data + idx;
      uint8_t *destination_pointer = dst_data + idx;

      apply_filter_mode_pixel(destination_pointer, source_pointer, c, mode);
    }
  }
}
