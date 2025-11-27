#pragma once

#include "Dither.hpp"
#include "Filter.hpp"
#include "Quantize.hpp"

struct Params {
  FilterParams filter;
  QuantizeParams quantize;
  DitherParams dither;
};
