#pragma once

#include "ColorScheme.hpp"
#include "Dither.hpp"
#include "Glow.hpp"
#include "Quantize.hpp"

struct Params {
  ColorSchemeParams colorScheme;
  GlowParams glow;
  QuantizeParams quantize;
  DitherParams dither;
  float gammaIn = 2.2f;
  float gammaOut = 2.2f;
};
