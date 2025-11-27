#pragma once

#include "core/ImageF.hpp"
enum class GlowBlurMode {
  Gaussian,
};

struct GlowParams {
  bool enabled = false;
  float threshold = 0.8f;
  float radius = 8;
  float intensity = 1.0f;
  GlowBlurMode mode = GlowBlurMode::Gaussian;
};

void apply_glow(const ImageF &src, ImageF &dst, const GlowParams &params);
