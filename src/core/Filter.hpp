#pragma once

#include "ImageF.hpp"

enum class FilterMode {
  None,
  Grayscale,
  RedChannel,
  GreenChannel,
  BlueChannel
};

struct FilterParams {
  FilterMode mode = FilterMode::None;
  float channelIntensity = 0.45f;
};

void apply_channel_filter(const ImageF &src, ImageF &dst, FilterMode mode,
                          float intensity);
