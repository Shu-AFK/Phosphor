#pragma once

#include "Filter.hpp"

enum class QuantizeMode {
  None,
  UniformPerChannel,
};

enum class DitherMode {
  None,
  Ordered4x4,
};

struct Params {
  // Filter
  FilterMode filterMode = FilterMode::None;
  float channelIntensity = 0.45f;

  // Quantization
  QuantizeMode quantizeMode = QuantizeMode::None;
  int levelsR = 4;
  int levelsG = 4;
  int levelsB = 4;

  // Dithering
  DitherMode ditherMode = DitherMode::None;
  float ditherStrength = 1.0f;
};
