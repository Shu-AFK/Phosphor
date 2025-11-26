#pragma once

#include "Filter.hpp"

enum class DitherMode {
  None,
  Ordered4x4,
};

struct Params {
  // Filter
  FilterMode filterMode = FilterMode::None;
  float channelIntensity = 0.45f;

  // Quantization
  int levelsR = 4;
  int levelsG = 4;
  int levelsB = 4;

  // Dithering
  DitherMode ditherMode = DitherMode::Ordered4x4;
  float ditherStrength = 1.0f;
};
