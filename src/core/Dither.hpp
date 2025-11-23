#pragma once

#include "Image.hpp"

void quantize_ordered_dither(const Image &src, Image &dst, int levelsR,
                             int levelsG, int levelsB, float ditherStrength);
