#pragma once

#include "ImageF.hpp"
#include "Quantize.hpp"

enum class DitherMode {
  None,
  Ordered4x4,
};

struct DitherParams {
  bool enabled = false;
  DitherMode mode = DitherMode::Ordered4x4;
  float strength = 1.0f;
};

void quantize_ordered_dither(const ImageF &src, ImageF &dst, QuantizeMode mode,
                             int levelsR, int levelsG, int levelsB,
                             float ditherStrength);
