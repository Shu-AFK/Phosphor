#pragma once

#include "ImageF.hpp"

struct ScanlineParams {
  int spacing = 3;
  int thickness = 15;
  float gapDarkness = 0.5f;
  bool enabled = false;
};

void apply_scanlines(const ImageF &src, ImageF &dst,
                     const ScanlineParams &params);
