#pragma once

#include "ImageF.hpp"

enum class FilterMode {
  None,
  Grayscale,
  RedChannel,
  GreenChannel,
  BlueChannel
};

void apply_channel_filter(const ImageF &src, ImageF &dst, FilterMode mode,
                          float intensity);
