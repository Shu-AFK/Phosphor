#pragma once

#include "ImageF.hpp"
#include "Params.hpp"

void quantize_ordered_dither(const ImageF &src, ImageF &dst, QuantizeMode mode,
                             int levelsR, int levelsG, int levelsB,
                             float ditherStrength);
