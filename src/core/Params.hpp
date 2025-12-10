#pragma once

#include "Dither.hpp"
#include "Filter.hpp"
#include "Glow.hpp"
#include "Quantize.hpp"

struct Params {
  FilterParams filter;
  GlowParams glow;
  QuantizeParams quantize;
  DitherParams dither;
  float gammaIn = 2.2f;
  float gammaOut = 2.2f;
};
