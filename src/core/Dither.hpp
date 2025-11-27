#pragma once

#include "ImageF.hpp"
#include "Quantize.hpp"

enum class DitherMode {
  None,
  Ordered4x4,
};

struct DitherParams {
  DitherMode mode = DitherMode::None;
  float strength = 1.0f;
};

void quantize_ordered_dither(const ImageF &src, ImageF &dst, QuantizeMode mode,
                             int levelsR, int levelsG, int levelsB,
                             float ditherStrength);
