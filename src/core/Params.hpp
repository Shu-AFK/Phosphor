#pragma once

#include "ColorScheme.hpp"
#include "Dither.hpp"
#include "Glow.hpp"
#include "Quantize.hpp"
#include "Scanlines.hpp"

struct Params {
  ColorSchemeParams colorScheme;
  GlowParams glow;
  QuantizeParams quantize;
  DitherParams dither;
  ScanlineParams scanlines;
  float gammaIn = 2.2f;
  float gammaOut = 2.2f;
};
