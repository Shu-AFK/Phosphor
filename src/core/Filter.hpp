#pragma once

#include "Image.hpp"

enum class FilterMode {
  None,
  Grayscale,
  RedChannel,
  GreenChannel,
  BlueChannel
};

void apply_channel_filter(const Image &src, Image &dst, FilterMode mode,
                          float intensity);
